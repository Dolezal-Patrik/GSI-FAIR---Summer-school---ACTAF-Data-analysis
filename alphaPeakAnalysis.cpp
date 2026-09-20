#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TObjArray.h>
#include <TLeaf.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TString.h>
#include <TSystem.h>

#include <fstream>
#include <vector>
#include <iostream>

// Get leaf from a sub-branch by name
TLeaf* GetLeafFromSubLocal(TObjArray* sub, const char* name)
{
    if (!sub) return nullptr;
    TBranch* b = (TBranch*)sub->FindObject(name);
    if (!b || !b->GetListOfLeaves()) return nullptr;
    return (TLeaf*)b->GetListOfLeaves()->At(0);
}

// Baseline-subtracted integral of one trace, matching the real
// IntegrateTrace() from the analysis chain:
//   - baseline taken from the END of the trace (startBackground..end)
//   - integral taken only up to endSignal
//   - no positive-only filtering
double IntegrateTraceLocal(TLeaf* traceLeaf, int traceIndex, int nSamples,
                            int endSignal, int startBackground)
{
    if (!traceLeaf) return 0.0;
    const int offset = traceIndex * nSamples;
    const int endBackground = nSamples;

    double sumBaseline = 0.0;
    int nBaselineBins = 0;
    for (int i = startBackground; i <= endBackground && i < nSamples; ++i)
    {
        sumBaseline += traceLeaf->GetValue(offset + i);
        ++nBaselineBins;
    }
    double baselineMean = (nBaselineBins > 0) ? sumBaseline / nBaselineBins : 0.0;

    double integral = 0.0;
    for (int i = 0; i <= endSignal && i < nSamples; ++i)
        integral += (traceLeaf->GetValue(offset + i) - baselineMean);

    return integral;
}

// Passes if integral falls within the amplitude window for its
// pad category (Grid or Cathode); "5th ring" pads have no window cut.
bool PassAlphaThreshold(const TString& category, double integral,
                         double gridLow, double gridUp,
                         double cathodeLow, double cathodeUp)
{
    if (category.Contains("grid", TString::kIgnoreCase))
        return integral >= gridLow && integral <= gridUp;
    if (category.Contains("cathode", TString::kIgnoreCase))
        return integral >= cathodeLow && integral <= cathodeUp;
    return true;
}

// e.g. /data/run1234.root -> run1234
TString GetRunName(TString filename)
{
    TString base = gSystem->BaseName(filename);
    if (base.EndsWith(".root")) base.Remove(base.Length() - 5);
    return base;
}

void alphaPeakAnalysis(TString filename, long long maxEventsToProcess = 100000)
{
    // Configuration
    const int nSamples = 2500;
    const int endSignal = 1100;
    const int startBackground = 1500;
    const int nBins = 2500;
    const double xMin = 0.0, xMax = 250.0;

    // Binning for the integral-vs-event-number check
    const int nEventBins = 200;

    // Alpha-source amplitude window cuts (raw ADC units, before /1000)
    // applied only to grid and cathode pads (see PassAlphaThreshold)
    const double fAlphaSourceGridUp     = 200e3;
    const double fAlphaSourceGridLow    = 0;
    const double fAlphaSourceCathodeUp  = 200e3;
    const double fAlphaSourceCathodeLow = 0;

    // Selected pads, with category and expected/observed status
    std::vector<int> selectedPad =
        { 115, 122, 102, 103, 110, 117, 112, 119, 46, 51, 52, 48, 55 };

    std::vector<TString> padCategory =
        { "Up grid", "Up grid", "Up cathode", "Up cathode", "Up cathode", "Up cathode",
          "Down cathode", "Down cathode", "Down 5th ring", "Down 5th ring", "Down 5th ring",
          "Down grid", "Down grid" };

    std::vector<TString> padStatus =
        { "Expected & Observed", "Expected & Observed", "Expected", "Expected",
          "Expected & Observed", "Observed", "Expected & Observed", "Expected & Observed",
          "Expected & Observed", "Expected", "Expected & Observed",
          "Expected & Observed", "Expected & Observed" };

    // Open input
    TFile* f = TFile::Open(filename, "READ");
    if (!f || f->IsZombie()) { std::cout << "ERROR: Cannot open " << filename << std::endl; return; }
    std::cout << "Opening " << filename << std::endl;

    // Get tree
    TTree* tree = (TTree*)f->Get("evt");
    if (!tree) { std::cout << "ERROR: Tree 'evt' not found." << std::endl; f->Close(); delete f; return; }

    // Get mapped branch and leaves
    TBranch* mapTopBranch = tree->GetBranch("ActafMappedData");
    if (!mapTopBranch) { std::cout << "ERROR: Branch 'ActafMappedData' not found." << std::endl; f->Close(); delete f; return; }

    TObjArray* mapSub = mapTopBranch->GetListOfBranches();
    TLeaf* mapPadLeaf   = GetLeafFromSubLocal(mapSub, "ActafMappedData.fPad");
    TLeaf* mapTraceLeaf = GetLeafFromSubLocal(mapSub, "ActafMappedData.fTrace[2500]");
    if (!mapPadLeaf || !mapTraceLeaf) { std::cout << "ERROR: Required leaves not found." << std::endl; f->Close(); delete f; return; }

    // Number of events
    long long totalEvents = tree->GetEntries();
    long long eventsToProcess = (maxEventsToProcess > 0 && maxEventsToProcess < totalEvents) ? maxEventsToProcess : totalEvents;
    std::cout << "Total events: " << totalEvents << std::endl;
    std::cout << "Processing: " << eventsToProcess << std::endl;

    // Create histograms: 1D integral spectrum + 2D integral vs event number
    std::vector<TH1F*> histograms(selectedPad.size(), nullptr);
    std::vector<TH2F*> histograms2D(selectedPad.size(), nullptr);
    for (size_t i = 0; i < selectedPad.size(); ++i)
    {
        TString name  = Form("h_alpha_pad%d", selectedPad[i]);
        TString title = Form("%s - Pad %d (%s);Integral [10^{3} ADC chn];Counts",
                              padCategory[i].Data(), selectedPad[i], padStatus[i].Data());
        histograms[i] = new TH1F(name, title, nBins, xMin, xMax);
        histograms[i]->SetDirectory(0);

        TString name2D  = Form("h2_alpha_pad%d_vs_event", selectedPad[i]);
        TString title2D = Form("%s - Pad %d (%s);Event number;Integral [10^{3} ADC chn]",
                                padCategory[i].Data(), selectedPad[i], padStatus[i].Data());
        histograms2D[i] = new TH2F(name2D, title2D, nEventBins, 0, (double)eventsToProcess, nBins, xMin, xMax);
        histograms2D[i]->SetDirectory(0);
    }

    // Processing
    std::cout << std::endl << "Starting trace integration..." << std::endl;
    for (long long ev = 0; ev < eventsToProcess; ++ev)
    {
        tree->GetEntry(ev);
        int nPads = mapPadLeaf->GetLen();

        for (int iPad = 0; iPad < nPads; ++iPad)
        {
            int currentPad = (int)mapPadLeaf->GetValue(iPad);

            for (size_t i = 0; i < selectedPad.size(); ++i)
            {
                if (currentPad != selectedPad[i]) continue;

                double integral   = IntegrateTraceLocal(mapTraceLeaf, iPad, nSamples, endSignal, startBackground);
                double integral_k = integral / 1000.0; // histogram is in units of 10^3 ADC

                bool passesThreshold = PassAlphaThreshold(padCategory[i], integral,
                                           fAlphaSourceGridLow, fAlphaSourceGridUp,
                                           fAlphaSourceCathodeLow, fAlphaSourceCathodeUp);

                if (integral_k > 0.0 && passesThreshold)
                {
                    histograms[i]->Fill(integral_k);
                    histograms2D[i]->Fill((double)ev, integral_k);
                }
            }
        }

        if (ev % 1000 == 0 || ev == eventsToProcess - 1)
            std::cout << "\rProcessed " << ev + 1 << " / " << eventsToProcess << std::flush;
    }
    std::cout << std::endl << "Integration finished." << std::endl;

    f->Close();
    delete f;

    // Output names
    TString runName    = GetRunName(filename);
    TString rootOutput  = Form("alpha_histograms_%s.root", runName.Data());
    TString macroOutput = Form("alpha_histograms_%s.C", runName.Data());

    // Save ROOT file
    TFile* out = TFile::Open(rootOutput, "RECREATE");
    if (!out || out->IsZombie()) { std::cout << "ERROR: Cannot create " << rootOutput << std::endl; return; }

    TNamed info("analysis_info", Form("Alpha histograms; input=%s; events=%lld", filename.Data(), eventsToProcess));
    info.Write();

    for (auto h : histograms) h->Write();
    for (auto h2 : histograms2D) h2->Write();
    out->Close();
    delete out;

    // Create .C macro
    std::ofstream macro(macroOutput.Data(), std::ios::out);
    if (!macro)
    {
        std::cout << "WARNING: Cannot create " << macroOutput << std::endl;
    }
    else
    {
        macro << "#include <TH1F.h>\n#include <TH2F.h>\n#include <TFile.h>\n#include <TCanvas.h>\n"
              << "#include <TROOT.h>\n#include <TStyle.h>\n\n";
        macro << "void " << runName << "_alpha_histograms()\n{\n";
        macro << "    TFile *f = TFile::Open(\"" << rootOutput.Data() << "\", \"READ\");\n";
        macro << "    if (!f || f->IsZombie()) return;\n\n";

        for (size_t i = 0; i < histograms.size(); ++i)
            macro << "    TH1F *h" << selectedPad[i] << " = (TH1F*)f->Get(\"h_alpha_pad"
                  << selectedPad[i] << "\");\n";

        macro << "\n";

        for (size_t i = 0; i < histograms.size(); ++i)
            macro << "    TH2F *h2_" << selectedPad[i] << " = (TH2F*)f->Get(\"h2_alpha_pad"
                  << selectedPad[i] << "_vs_event\");\n";

        macro << "\n    TCanvas *c = new TCanvas(\"c_alpha\", \"Alpha spectra\", 1600, 1000);\n";
        macro << "    c->Divide(4,4);\n\n";

        for (size_t i = 0; i < histograms.size(); ++i)
            macro << "    c->cd(" << i + 1 << ");\n    h" << selectedPad[i] << "->Draw(\"HIST\");\n";

        macro << "\n    TCanvas *c2 = new TCanvas(\"c_alpha_vs_event\", \"Integral vs event number\", 1600, 1000);\n";
        macro << "    c2->Divide(4,4);\n\n";

        for (size_t i = 0; i < histograms.size(); ++i)
            macro << "    c2->cd(" << i + 1 << ");\n    h2_" << selectedPad[i] << "->Draw(\"COLZ\");\n";

        macro << "\n}\n";
        macro.close();
        std::cout << "Created macro: " << macroOutput << std::endl;
    }

    // Print statistics
    std::cout << std::endl << "========================================" << std::endl;
    std::cout << "Histogram creation finished." << std::endl;
    std::cout << "ROOT file:\n  " << rootOutput << std::endl;
    std::cout << "C macro:\n  " << macroOutput << std::endl << std::endl;

    for (size_t i = 0; i < histograms.size(); ++i)
        std::cout << "Pad " << selectedPad[i] << ": " << histograms[i]->GetEntries() << " entries" << std::endl;

    std::cout << "========================================" << std::endl;

    for (auto h : histograms) delete h;
    for (auto h2 : histograms2D) delete h2;
}
