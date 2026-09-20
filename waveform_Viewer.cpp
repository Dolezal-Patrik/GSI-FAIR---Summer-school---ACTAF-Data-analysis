#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TObjArray.h>
#include <TLeaf.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TGraph.h>
#include <TLatex.h>
#include <TString.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// --- Helper functions for Eraw calculation and leaf retrieval ---

TLeaf* GetLeafFromSub(TObjArray* sub, const char* name) {
    if (!sub) return nullptr;
    TBranch* b = (TBranch*)sub->FindObject(name);
    if (!b || !b->GetListOfLeaves()) return nullptr;
    return (TLeaf*)b->GetListOfLeaves()->At(0);
}

template <class Cont>
double ComputeBaselineMean(const Cont& signal, int numBins)
{
    int samplesToUse = std::min(static_cast<int>(numBins * 0.5), static_cast<int>(signal.size()));
    if (samplesToUse <= 0) return 0.0;
    return std::accumulate(signal.begin(), signal.begin() + samplesToUse, 0.0) / samplesToUse;
}

template <class Cont>
int FindMaxPosition(const Cont& signal)
{
    return std::distance(signal.begin(), std::max_element(signal.begin(), signal.end()));
}

// Integration returns the pulse integral and pulse bounds (left, right)
template <class Cont>
double IntegratePulseWithBounds(const Cont& signal, int maxIdx, double baseline, int& leftBin, int& rightBin)
{
    int left = maxIdx;
    int right = maxIdx;
    const int size = static_cast<int>(signal.size());

    // Search for left pulse boundary (drops below baseline)
    while (left > 0 && signal[left] > baseline) --left;

    // Search for right pulse boundary (drops below baseline)
    while (right < size - 1 && signal[right] > baseline) ++right;

    leftBin = left;
    rightBin = right;

    double sum = std::accumulate(signal.begin() + left, signal.begin() + right + 1, 0.0);
    int numBins = (right - left + 1);
    
    return sum - (numBins * baseline);
}

// --- Main Script Function ---

void waveform_Viewer(int targetPad = 122,
                      double integralThreshold = 100.0,
                      double amplitudeThreshold = 60.0,
                      const char* filename = "/mnt/c/Users/patri/Desktop/nearline_data_4122_20260807_134624.root") {

    TFile *f1 = TFile::Open(filename, "READ");
    if (!f1 || f1->IsZombie()) {
        std::cout << "Failed to open file: " << filename << "\n";
        return;
    }

    TTree *tree = (TTree*)f1->Get("evt");
    if (!tree) {
        std::cout << "Tree 'evt' not found.\n";
        return;
    }

    // --- Load Mapped branches (RAW) ---
    TBranch *mapTopBranch = tree->GetBranch("ActafMappedData");
    if (!mapTopBranch) {
        std::cout << "Branch 'ActafMappedData' not found.\n";
        return;
    }
    TObjArray *mapSub = mapTopBranch->GetListOfBranches();

    TLeaf *mapPadLeaf   = GetLeafFromSub(mapSub, "ActafMappedData.fPad");
    TLeaf *mapTraceLeaf = GetLeafFromSub(mapSub, "ActafMappedData.fTrace[2500]");
    TLeaf *mapELeaf     = GetLeafFromSub(mapSub, "ActafMappedData.fE");

    if (!mapPadLeaf || !mapTraceLeaf || !mapELeaf) {
        std::cout << "Missing one of the Mapped branches!\n";
        return;
    }

    const int nSamples = 2500;
    int numBinsBaseline = 1000;
    long long totalEvents = tree->GetEntries();

    // --- FINDER: Search ALL events for targetPad, INTEGRAL, and AMPLITUDE ---
    std::vector<int> foundEvents;
    std::vector<int> foundMapIdx;
    std::vector<double> traceBuffer(nSamples);

    std::cout << "Searching for events with Pad " << targetPad 
              << " | Integral > " << integralThreshold 
              << " ADC | Amplitude > " << amplitudeThreshold << " ADC...\n";

    for (int ev = 0; ev < totalEvents; ev++) {
        tree->GetEntry(ev);
        int nPads = mapPadLeaf->GetLen();

        for (int i = 0; i < nPads; i++) {
            if ((int)mapPadLeaf->GetValue(i) == targetPad) {
                int offset = i * nSamples;
                
                // Read trace for calculation
                for (int s = 0; s < nSamples; s++) {
                    traceBuffer[s] = mapTraceLeaf->GetValue(offset + s);
                }

                double baseline = ComputeBaselineMean(traceBuffer, numBinsBaseline);
                int maxIdx = FindMaxPosition(traceBuffer);
                double amplitude = traceBuffer[maxIdx] - baseline;
                
                int dummyLeft = 0, dummyRight = 0;
                double calcRawIntegral = IntegratePulseWithBounds(traceBuffer, maxIdx, baseline, dummyLeft, dummyRight);

                if (calcRawIntegral > integralThreshold && amplitude > amplitudeThreshold) {
                    foundEvents.push_back(ev);
                    foundMapIdx.push_back(i);
                }
                break;
            }
        }
    }

    std::cout << "Found " << foundEvents.size() << " out of " << totalEvents 
              << " events with Pad " << targetPad 
              << " (int > " << integralThreshold << " ADC, ampl > " << amplitudeThreshold << " ADC).\n";
    if (foundEvents.empty()) {
        return;
    }

    // --- VIEWER ---
    int current = 0;
    TCanvas *c1 = new TCanvas("c1", "RAW Waveform Integration Viewer", 1100, 650);

    while (true) {
        int ev = foundEvents[current];
        int mapIdx = foundMapIdx[current];

        tree->GetEntry(ev);

        // Load RAW Trace
        int mapOffset = mapIdx * nSamples;
        std::vector<double> rawTrace(nSamples);
        for (int s = 0; s < nSamples; s++) {
            rawTrace[s] = mapTraceLeaf->GetValue(mapOffset + s);
        }

        // --- PRECISE Eraw CALCULATION FOR THIS EVENT ---
        double rawBaseline = ComputeBaselineMean(rawTrace, numBinsBaseline);
        int rawMaxIdx = FindMaxPosition(rawTrace);
        int leftBin = 0, rightBin = 0;
        double calcRawIntegral = IntegratePulseWithBounds(rawTrace, rawMaxIdx, rawBaseline, leftBin, rightBin);
        double amplitude = rawTrace[rawMaxIdx] - rawBaseline;
        double leafE = mapELeaf->GetValue(mapIdx);

        // Histograms
        TH1F *hRaw = new TH1F("hRaw",
            Form("Event %d/%lld (Found %d/%zu), Pad %d;Sample (Time);ADC Channel", 
                 ev, totalEvents, current + 1, foundEvents.size(), targetPad),
            nSamples, 0, nSamples);

        // Integral area (filled polygon)
        int nAreaPoints = (rightBin - leftBin + 1) + 2;
        std::vector<double> gx(nAreaPoints), gy(nAreaPoints);
        
        gx[0] = leftBin;
        gy[0] = rawBaseline;
        for (int b = leftBin, p = 1; b <= rightBin; ++b, ++p) {
            gx[p] = b + 0.5;
            gy[p] = rawTrace[b];
            hRaw->SetBinContent(b + 1, rawTrace[b]);
        }
        gx[nAreaPoints - 1] = rightBin;
        gy[nAreaPoints - 1] = rawBaseline;

        for (int s = 0; s < nSamples; s++) {
            hRaw->SetBinContent(s + 1, rawTrace[s]);
        }

        TGraph *gIntegralArea = new TGraph(nAreaPoints, gx.data(), gy.data());
        gIntegralArea->SetFillColorAlpha(kGreen + 1, 0.35);
        gIntegralArea->SetLineColor(kGreen + 2);
        gIntegralArea->SetLineWidth(2);

        // Styling
        hRaw->SetLineColor(kBlue);
        hRaw->SetLineWidth(1);
        hRaw->SetStats(0);

        double minY = hRaw->GetMinimum();
        double maxY = hRaw->GetMaximum();
        hRaw->GetYaxis()->SetRangeUser(minY - 10, maxY + 40);

        c1->cd();
        hRaw->Draw("HIST");
        gIntegralArea->Draw("F SAME");
        gIntegralArea->Draw("L SAME");

        TLegend *leg = new TLegend(0.55, 0.75, 0.88, 0.88);
        leg->AddEntry(hRaw, "RAW Trace", "l");
        leg->AddEntry(gIntegralArea, Form("Eraw Integration (bins %d to %d)", leftBin, rightBin), "f");
        leg->Draw();

        // On-screen text info
        TLatex txt;
        txt.SetNDC();
        txt.SetTextSize(0.032);
        txt.SetTextColor(kBlack);
        txt.DrawLatex(0.13, 0.83, Form("#bf{Calculated Eraw Integral:} %.1f ADC", calcRawIntegral));
        txt.DrawLatex(0.13, 0.78, Form("Leaf fE from ROOT: %.1f ADC", leafE));
        txt.DrawLatex(0.13, 0.73, Form("Baseline: %.2f | Max Ampl: %.1f ADC", rawBaseline, amplitude));

        c1->Update();

        // Terminal output
        std::cout << "\n==================================================\n";
        std::cout << "EVENT: " << ev << " / " << totalEvents 
                  << " (Found: " << (current + 1) << " of " << foundEvents.size() << ") | PAD: " << targetPad << "\n";
        std::cout << "---------------- RAW INTEGRATION DATA -------------\n";
        std::cout << "--> Value for fh2_ERaw_map [Eraw]: " << calcRawIntegral << " ADC\n";
        std::cout << "--> Value from tree (fE Leaf):      " << leafE << " ADC\n";
        std::cout << "--> Baseline (avg bins 0..500):    " << rawBaseline << " ADC\n";
        std::cout << "--> Max Peak position / Amplitude: Bin " << rawMaxIdx << " / " << amplitude << " ADC above baseline\n";
        std::cout << "--> Pulse integration bounds:     Bin " << leftBin << " to Bin " << rightBin << " (Width: " << (rightBin - leftBin + 1) << " bins)\n";
        std::cout << "==================================================\n";

        std::cout << "Commands: n = next | p = previous | j = jump | s = save RAW image | q = quit: ";
        char cmd;
        std::cin >> cmd;

        // --- EXPORT RAW IMAGE ---
        if (cmd == 's') {
            TCanvas *cRaw = new TCanvas("cRaw", "RAW Trace", 1100, 650);
            cRaw->cd();
            
            TH1F *hRawClone = (TH1F*)hRaw->Clone("hRawClone");
            hRawClone->SetTitle(Form("RAW Trace - Event %d, Pad %d;Sample (Time);ADC Channel", ev, targetPad));
            hRawClone->Draw("HIST");
            gIntegralArea->Draw("F SAME");
            gIntegralArea->Draw("L SAME");
            
            TLegend *legRaw = new TLegend(0.55, 0.75, 0.88, 0.88);
            legRaw->AddEntry(hRawClone, "RAW Trace", "l");
            legRaw->AddEntry(gIntegralArea, Form("Eraw Integration (bins %d to %d)", leftBin, rightBin), "f");
            legRaw->Draw();
            
            txt.DrawLatex(0.13, 0.83, Form("#bf{Calculated Eraw Integral:} %.1f ADC", calcRawIntegral));
            txt.DrawLatex(0.13, 0.78, Form("Baseline: %.2f | Max Ampl: %.1f ADC", rawBaseline, amplitude));

            TString rawFilename = Form("waveform_ev%d_pad%d_RAW.png", ev, targetPad);
            cRaw->SaveAs(rawFilename);
            delete cRaw;

            std::cout << "--> SUCCESSFULLY SAVED: " << rawFilename << "\n";
        }

        delete hRaw;
        delete gIntegralArea;
        delete leg;

        if (cmd == 'q') break;
        else if (cmd == 'n') {
            if (current < (int)foundEvents.size() - 1) current++;
            else std::cout << "Last event.\n";
        } else if (cmd == 'p') {
            if (current > 0) current--;
            else std::cout << "First event.\n";
        } else if (cmd == 'j') {
            int jump;
            std::cout << "Enter index of found event (1.." << foundEvents.size() << "): ";
            std::cin >> jump;
            if (jump >= 1 && jump <= (int)foundEvents.size()) current = jump - 1;
        }
    }

    f1->Close();
}
