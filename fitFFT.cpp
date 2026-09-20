#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TSpectrum.h>
#include <TPolyMarker.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

const char* infile = "fft_result_pad106.root";

// Helper function to find the exact peak maximum within a specified frequency window
double FindExactPeakPosition(TH1F* h, double minFreq, double maxFreq) {
    int binMin = h->GetXaxis()->FindBin(minFreq);
    int binMax = h->GetXaxis()->FindBin(maxFreq);
    
    int maxBin = binMin;
    double maxVal = h->GetBinContent(binMin);
    
    for (int b = binMin; b <= binMax; ++b) {
        if (h->GetBinContent(b) > maxVal) {
            maxVal = h->GetBinContent(b);
            maxBin = b;
        }
    }
    return h->GetXaxis()->GetBinCenter(maxBin);
}

void fitFFT() {

    TFile *f = TFile::Open(infile, "READ");
    if (!f || f->IsZombie()) {
        std::cout << "Cannot open file: " << infile << "\n";
        return;
    }

    TH1F *h = (TH1F*)f->Get("h_avg_fft");
    if (!h) {
        std::cout << "Histogram 'h_avg_fft' not found!\n";
        return;
    }
    h->SetDirectory(0);
    f->Close();

    TCanvas *c = new TCanvas("c_fit", "Multi-peak Fit FFT", 1000, 600);
    h->SetTitle("Averaged FFT Power Spectrum with Peak Fits;Frequency [kHz];Average Power [ADC^{2}]");
    h->GetYaxis()->SetRangeUser(0.0001, h->GetMaximum() * 1.5);
    h->GetXaxis()->SetRangeUser(0, 1000);
    h->SetStats(0);
    c->SetLogy(1);
    h->Draw("HIST");

    std::vector<double> detectedPeaks;

    // First pass: search for strong low-frequency peaks (150-400 kHz)
    TSpectrum *spec1 = new TSpectrum(5);
    int n1 = spec1->Search(h, 4, "goff", 0.002);
    double *x1 = spec1->GetPositionX();
    for (int i = 0; i < n1; i++) {
        if (x1[i] >= 150.0 && x1[i] < 400.0) {
            detectedPeaks.push_back(FindExactPeakPosition(h, x1[i] - 15, x1[i] + 15));
        }
    }

    // Second pass: search for higher harmonic peaks (550-850 kHz)
    TSpectrum *spec2 = new TSpectrum(10);
    int n2 = spec2->Search(h, 3.0, "goff", 0.0001);
    double *x2 = spec2->GetPositionX();
    for (int i = 0; i < n2; i++) {
        if (x2[i] >= 550.0 && x2[i] < 850.0) {
            detectedPeaks.push_back(FindExactPeakPosition(h, x2[i] - 10, x2[i] + 10));
        }
    }

    // Third pass: targeted search for intermediate peak around 443 kHz
    std::cout << "--> Searching for intermediate peak...\n";
    double peak3_freq = FindExactPeakPosition(h, 300.0, 700.0);
    detectedPeaks.push_back(peak3_freq);

    // Sort peaks and remove duplicates closer than 15 kHz
    std::sort(detectedPeaks.begin(), detectedPeaks.end());
    auto it = std::unique(detectedPeaks.begin(), detectedPeaks.end(), [](double a, double b) {
        return std::abs(a - b) < 15.0;
    });
    detectedPeaks.erase(it, detectedPeaks.end());

    // Mark detected peak positions
    TPolyMarker *pm = new TPolyMarker(detectedPeaks.size());
    for (size_t i = 0; i < detectedPeaks.size(); i++) {
        double xp = detectedPeaks[i];
        int bin = h->GetXaxis()->FindBin(xp);
        pm->SetPoint(i, xp, h->GetBinContent(bin));
    }
    pm->SetMarkerStyle(23);
    pm->SetMarkerColor(kRed);
    pm->SetMarkerSize(1.3);
   // pm->Draw("SAME");

    // Fit peaks with Gaussian + Linear model and calculate uncertainties
    double binWidth = h->GetXaxis()->GetBinWidth(1);
    double binUncertainty = binWidth / std::sqrt(12.0);

    for (size_t i = 0; i < detectedPeaks.size(); i++) {
        double xPeak = detectedPeaks[i];
        
        double fitWindow = (xPeak > 400.0 && xPeak < 500.0) ? 15.0 : 12.0;
        double fitMin = xPeak - fitWindow;
        double fitMax = xPeak + fitWindow;

        TF1 *fitFunc = new TF1(Form("fit_%zu", i), "gaus(0) + pol1(3)", fitMin, fitMax);
        int binPeak = h->GetXaxis()->FindBin(xPeak);
        double peakHeight = h->GetBinContent(binPeak);
        
        fitFunc->SetParameter(0, peakHeight);
        fitFunc->SetParameter(1, xPeak);
        fitFunc->SetParLimits(1, fitMin, fitMax);
        fitFunc->SetParameter(2, 1.5);
        fitFunc->SetParLimits(2, 0.2, 5.0);

        h->Fit(fitFunc, "R+Q+I");
        fitFunc->SetLineColor(kMagenta + 1);
        fitFunc->SetLineWidth(2);

        double mean = fitFunc->GetParameter(1);
        double fitStatErr = fitFunc->GetParError(1);
        double ampl = fitFunc->GetParameter(0);

        double totalPeakErr = std::sqrt(fitStatErr * fitStatErr + binUncertainty * binUncertainty);

        std::cout << Form("Peak #%-2zu | Max Bin: %7.2f kHz | Fit Mean: %7.2f +/- %4.2f kHz (stat: +/-%.2f, bin: +/-%.2f) | Ampl: %8.2f\n",
                          i + 1, xPeak, mean, totalPeakErr, fitStatErr, binUncertainty, ampl);
    }

    c->Update();
    
    c->SaveAs("fft_fit_spectrum_pad106_run41274_technicalreport.pdf");
    c->SaveAs("fft_fit_spectrum_pad106_run412740_technicalreport.C");

    std::cout << "\n--> Output saved to PDF and C files.\n";
}
