// average_FFT3.C
//
// Average FFT power spectrum of the raw ACTAF pad traces (fTrace[2500]) for one pad.
//
// Usage (ROOT prompt or shell):
//   root -l 'average_FFT3.C(122, 19.44e6, "/path/to/data.root")'
//   root -l 'average_FFT3.C(122, 19.44e6, "/path/to/data.root", "my_output.root")'
//
// Output: TH1F "h_avg_fft" (mean of P_k = 4 |X_k|^2 / N^2 in ADC^2) saved to
//         fft_result_pad<pad>_<input file name>.root unless outName is given.

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLeaf.h>
#include <TObjArray.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TString.h>
#include <TSystem.h>
#include <TVirtualFFT.h>
#include <iostream>
#include <vector>

void average_FFT(int targetPad = 122,
                  double samplingRateHz = 19.44e6,
                  const char *filename = "/mnt/c/Users/patri/Desktop/nearline_data_4130_20260907_190103.root",
                  const char *outName = "")
{
    const int nSamples = 2500;                         // samples per trace (fTrace[2500])
    const int nFFTBins = nSamples / 2;                 // bins k = 0 ... N/2-1
    const double dfkHz = samplingRateHz / nSamples / 1e3; // frequency step [kHz]

    // ---------------------------------------------------------------- input
    TFile *f1 = TFile::Open(filename, "READ");
    if (!f1 || f1->IsZombie()) {
        std::cout << "Cannot open the file: " << filename << "\n";
        return;
    }

    TTree *tree = (TTree *)f1->Get("evt");
    if (!tree) {
        std::cout << "Tree 'evt' not found.\n";
        f1->Close();
        return;
    }
    Long64_t nEvents = tree->GetEntries();

    TBranch *topBranch = tree->GetBranch("ActafMappedData");
    if (!topBranch) {
        std::cout << "Branch 'ActafMappedData' not found.\n";
        f1->Close();
        return;
    }
    TObjArray *subBranches = topBranch->GetListOfBranches();
    TBranch *padBranch   = (TBranch *)subBranches->FindObject("ActafMappedData.fPad");
    TBranch *traceBranch = (TBranch *)subBranches->FindObject("ActafMappedData.fTrace[2500]");
    if (!padBranch || !traceBranch) {
        std::cout << "Sub-branches fPad / fTrace[2500] not found.\n";
        f1->Close();
        return;
    }
    TLeaf *padLeaf   = (TLeaf *)padBranch->GetListOfLeaves()->At(0);
    TLeaf *traceLeaf = (TLeaf *)traceBranch->GetListOfLeaves()->At(0);
    if (traceLeaf->GetLenStatic() != nSamples) {
        std::cout << "Warning: trace length in the file (" << traceLeaf->GetLenStatic()
                  << ") differs from nSamples = " << nSamples << ".\n";
    }

    // ------------------------------------------------------------ FFT plan
    // Real-to-complex 1D transform, created once and reused for all events.
    Int_t n = nSamples;
    TVirtualFFT *fft = TVirtualFFT::FFT(1, &n, "R2C ES");
    if (!fft) {
        std::cout << "Cannot create FFT plan (is ROOT built with FFTW?).\n";
        f1->Close();
        return;
    }

    // Accumulate in double precision; the histogram (float) is filled at the end.
    std::vector<double> sumPower(nFFTBins, 0.0);
    std::vector<double> trace(nSamples);

    std::cout << ">>> Calculating mean FFT power for pad " << targetPad << "...\n";
    int matchedEvents = 0;
    int skippedEvents = 0;

    // ----------------------------------------------------------- event loop
    for (Long64_t ev = 0; ev < nEvents; ev++) {
        tree->GetEntry(ev);

        int nPads = padLeaf->GetLen();
        int foundIdx = -1;
        for (int i = 0; i < nPads; i++) {
            if ((int)padLeaf->GetValue(i) == targetPad) { foundIdx = i; break; }
        }
        if (foundIdx == -1) continue;

        int offset = foundIdx * nSamples;
        if (traceLeaf->GetLen() < offset + nSamples) { // safety: incomplete trace
            skippedEvents++;
            continue;
        }

        // copy trace and subtract its mean (DC offset)
        double mean = 0.0;
        for (int s = 0; s < nSamples; s++) {
            trace[s] = traceLeaf->GetValue(offset + s);
            mean += trace[s];
        }
        mean /= nSamples;
        for (int s = 0; s < nSamples; s++) trace[s] -= mean;

        // FFT
        fft->SetPoints(trace.data());
        fft->Transform();

        // P_k = 4 |X_k|^2 / N^2  (squared amplitude of the harmonic component, ADC^2)
        for (int k = 0; k < nFFTBins; k++) {
            double re, im;
            fft->GetPointComplex(k, re, im);
            sumPower[k] += 4.0 * (re * re + im * im) / ((double)nSamples * nSamples);
        }

        matchedEvents++;
        if (matchedEvents % 100 == 0) {
            std::cout << "  ... event " << ev << " processed (in total " << matchedEvents << ")\n";
        }
    }

    delete fft;

    if (skippedEvents > 0) {
        std::cout << ">>> Warning: " << skippedEvents << " events skipped (incomplete trace).\n";
    }
    if (matchedEvents == 0) {
        std::cout << ">>> Warning: no event contains pad " << targetPad << ".\n";
        f1->Close();
        return;
    }

    // ------------------------------------------------------------ histogram
    // FFT component k has frequency k * fs / N. Bin k+1 is therefore centred on
    // k * df, i.e. the axis runs from -0.5*df to (nFFTBins - 0.5)*df.
    TH1F *h_avg_fft = new TH1F("h_avg_fft",
        Form("Average FFT power spectrum - Pad %d;Frequency [kHz];Average power [ADC^{2}]", targetPad),
        nFFTBins, -0.5 * dfkHz, (nFFTBins - 0.5) * dfkHz);
    h_avg_fft->SetDirectory(0); // detach from the input file

    for (int k = 0; k < nFFTBins; k++) {
        h_avg_fft->SetBinContent(k + 1, sumPower[k] / matchedEvents);
    }
    h_avg_fft->SetEntries(matchedEvents);
    std::cout << ">>> Done, " << matchedEvents << " events processed.\n";

    // ----------------------------------------------------------------- draw
    TCanvas *c1 = new TCanvas(Form("c_fft_pad%d", targetPad), "Average FFT Power Spectrum", 900, 600);
    c1->SetLogy(1);
    h_avg_fft->SetLineColor(kBlue + 1);
    h_avg_fft->Draw("HIST");
    c1->Update();

    // ----------------------------------------------------------------- save
    TString outFile = outName;
    if (outFile.IsNull()) {
        TString base = gSystem->BaseName(filename);
        base.ReplaceAll(".root", "");
        outFile = Form("fft_result_pad%d_%s.root", targetPad, base.Data());
    }

    TFile *fout = TFile::Open(outFile, "RECREATE");
    if (fout && !fout->IsZombie()) {
        // WriteTObject keeps the histogram alive in memory (Close() would delete
        // it if it were owned by the file, leaving the canvas with a dangling pointer).
        fout->WriteTObject(h_avg_fft);
        fout->Close();
        delete fout;
        std::cout << ">>> Histogram saved to " << outFile << "\n";
    } else {
        std::cout << ">>> Error: cannot create output file " << outFile << "\n";
    }

    f1->Close();
}
