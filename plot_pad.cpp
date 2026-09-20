// plot_pad.C
//
// Processes ONE pad (set below) across all runs:
//   1. loads the histogram for the given pad from each run
//   2. shows all 10 histograms on one canvas (5x2)
//   3. finds the peak (TSpectrum) for each and fits it
//   4. builds the Mean vs. HV dependency across runs from the fits
//
// Run (default pad = 115):
//   root -l -q plot_pad.C
//
// Run for a specific pad, e.g. pad 122:
//   root -l -q 'plot_pad.C(122)'

#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TSpectrum.h>
#include <TString.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TAxis.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TColor.h>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>

// Helper function - draws and saves a "y vs HV" dependency graph in the style
// of the original plots.
void DrawDependencyGraph(int pad, const std::vector<double> &yValues,
                          const std::vector<double> &xHV,
                          const TString &yAxisTitle, const TString &outSuffix) {

    TCanvas *cDep = new TCanvas(Form("cPad_%d_%s", pad, outSuffix.Data()),
                                 Form("Pad %d %s vs HV", pad, outSuffix.Data()), 702, 106, 900, 700);
    cDep->SetFillColor(10);
    cDep->SetBorderMode(0);
    cDep->SetBorderSize(2);
    cDep->SetTickx(1);
    cDep->SetTicky(1);
    cDep->SetLeftMargin(0.15);
    cDep->SetRightMargin(0.05);
    cDep->SetTopMargin(0.06);
    cDep->SetBottomMargin(0.15);
    cDep->SetFrameLineWidth(3);

    TGraph *g = new TGraph((int)yValues.size(), xHV.data(), yValues.data());
    g->SetMarkerStyle(20);
    g->SetMarkerSize(1.8);
    g->SetMarkerColor(TColor::GetColor("#0000ff"));
    g->SetLineColor(TColor::GetColor("#ff0000"));
    g->SetLineWidth(3);

    g->GetXaxis()->SetTitle("HV, % nominal");
    g->GetXaxis()->CenterTitle(true);
    g->GetXaxis()->SetLabelFont(62);
    g->GetXaxis()->SetLabelSize(0.050);
    g->GetXaxis()->SetTitleSize(0.060);
    g->GetXaxis()->SetTitleOffset(1.0);
    g->GetXaxis()->SetTitleFont(62);

    g->GetYaxis()->SetTitle(yAxisTitle);
    g->GetYaxis()->CenterTitle(true);
    g->GetYaxis()->SetLabelFont(62);
    g->GetYaxis()->SetLabelOffset(0.010);
    g->GetYaxis()->SetLabelSize(0.050);
    g->GetYaxis()->SetTitleSize(0.060);
    g->GetYaxis()->SetTitleOffset(1.3);
    g->GetYaxis()->SetTitleFont(62);

    g->Draw("APL");

    TLegend *leg = new TLegend(0.45, 0.65, 0.85, 0.85, nullptr, "brNDC");
    leg->SetBorderSize(0);
    leg->SetTextFont(62);
    leg->SetFillStyle(0);
    TLegendEntry *e = leg->AddEntry(g, Form("Pad %d", pad), "lp");
    e->SetTextFont(62);
    leg->Draw();

    cDep->Modified();
    cDep->Update();
    cDep->SaveAs(Form("Pad_%d_%s_vs_HV.png", pad, outSuffix.Data()));
}

void plot_pad(int pad = 115) {

    // =====================================================================
    // Pad number is now passed in as a function argument (see default above
    // and the run examples at the top of the file) instead of being
    // hardcoded here.
    // =====================================================================

    gROOT->SetStyle("Pub");
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    gStyle->SetOptFit(0);
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);

    // Files (runs), same folder as the script
    std::vector<std::string> fileNames = {
    "online_data_4035_20260909_122322.root",
        "online_data_4122_20260831_170741.root",
        "online_data_4123_20260831_173035.root",
        "online_data_4124_20260831_175201.root",
        "online_data_4125_20260831_181214.root",
        "online_data_4126_20260831_183212.root",
        "online_data_4127_20260831_185950.root",
        "online_data_4128_20260831_191835.root",
        "online_data_4129_20260831_194138.root",
        "online_data_4130_20260831_195939.root",
    };
    std::vector<std::string> runNumbers = {"4035","4122", "4123", "4124", "4125", "4126",
                                            "4127", "4128", "4129", "4130"};
    std::vector<double> HV = {100, 90, 80, 70, 60, 50, 40, 30, 20, 10};

    const int nRuns = (int)fileNames.size();

    // Fitting algorithm for this pad - adjust as needed.
    // Here every pad uses background pol1 + gaus (compound fit).
    bool useCompoundFit = true;

    std::cout << "\n=================================================================================\n";
    std::cout << " Processing Pad " << pad << " across " << nRuns << " runs\n";
    std::cout << "=================================================================================\n";
    std::cout << std::setw(6)  << "Run"
              << std::setw(8)  << "HV"
              << std::setw(12) << "Mean"
              << std::setw(10) << "Sigma"
              << std::setw(12) << "ROI Low"
              << std::setw(12) << "ROI High"
              << std::setw(18) << "Integral (3sig)" << "\n";
    std::cout << "=================================================================================\n";

    TString outFileName = Form("fit_results_pad_%d.txt", pad);
    std::ofstream out(outFileName.Data());
    out << "Run\tHV\tMean\tSigma\tROI_Low\tROI_High\tIntegral_3Sigma\n";

    // Canvas with 10 histograms (this pad, all runs)
    TCanvas *c = new TCanvas(Form("cPad_%d_allruns", pad),
                              Form("Pad %d - all runs", pad), 2000, 1000);
    c->Divide(5, 2);

    std::vector<double> means;
    std::vector<double> integrals;
    std::vector<double> actualHV;

    // Peak position from the last SUCCESSFUL run - used as an "anchor" for
    // picking the right candidate in the next (often weaker) run, because
    // runs go in order of decreasing HV and the peak shifts smoothly, not
    // in a jump.
    double prevPeakPos = -1;
    const double maxJumpFromPrev = 8000; // how many ADC channels the peak may "jump" between adjacent runs

    for (int r = 0; r < nRuns; r++) {

        TVirtualPad *subPad = c->cd(r + 1);
        subPad->SetLogy();

        TFile *f = TFile::Open(fileNames[r].c_str());
        if (!f || f->IsZombie()) {
            std::cerr << "Error opening file: " << fileNames[r] << std::endl;
            continue;
        }

        TH1F *h = (TH1F*)f->Get(Form("fh1_Pad_%d_Eraw", pad));
        if (!h) {
            std::cout << "Run " << runNumbers[r] << ": histogram for Pad " << pad << " not found!" << std::endl;
            f->Close();
            delete f;
            continue;
        }
        h->SetDirectory(nullptr); // so the histogram survives closing the file
        f->Close();
        delete f;

        // 1. Alpha peak search
        h->GetXaxis()->SetRangeUser(30000, 70000);

        TSpectrum *s = new TSpectrum(5);
        int nfound = s->Search(h, 15, "", 0.2);

        if (nfound == 0) {
            h->GetXaxis()->SetRangeUser(0, 70000);
            h->Draw();
            delete s;
            continue;
        }

        double *xpeaks = s->GetPositionX();
        std::vector<double> candidates(xpeaks, xpeaks + nfound);

        double alphaPeakPos;
        if (prevPeakPos > 0) {
            // Take the candidate closest to the position from the previous run
            // (continuity), not simply the rightmost one - that is exactly
            // what, at low HV, was picking the noise bump instead of the
            // actual peak.
            alphaPeakPos = *std::min_element(candidates.begin(), candidates.end(),
                [prevPeakPos](double a, double b) {
                    return std::abs(a - prevPeakPos) < std::abs(b - prevPeakPos);
                });
            // If even the closest candidate is unreasonably far away, better
            // stay at the previous position and let the pre-fit adjust locally.
            if (std::abs(alphaPeakPos - prevPeakPos) > maxJumpFromPrev) {
                alphaPeakPos = prevPeakPos;
            }
        } else {
            // First (strongest) run - original logic: rightmost peak
            std::sort(candidates.rbegin(), candidates.rend());
            alphaPeakPos = candidates[0];
        }

        // 2. Pre-fit to estimate mean/sigma
        double initWindow = 2500;
        TF1 *preFit = new TF1(Form("preFit_%s", runNumbers[r].c_str()),
                               "gaus", alphaPeakPos - initWindow, alphaPeakPos + initWindow);
        h->Fit(preFit, "RQ0");

        double meanEst  = preFit->GetParameter(1);
        double sigmaEst = std::abs(preFit->GetParameter(2));
        delete preFit;

        if (sigmaEst <= 0 || sigmaEst > 8000) {
            sigmaEst = 2500;
            meanEst = alphaPeakPos;
        }

        h->GetXaxis()->SetRangeUser(0, 70000);
        h->Draw();

        // 3. Main fit
        double fitLow, fitHigh;
        TF1 *fitFunc = nullptr;
        int meanIdx, sigmaIdx;

        if (useCompoundFit) {
            fitLow  = meanEst - 1.0 * sigmaEst;
            fitHigh = meanEst + 2.0 * sigmaEst;

            fitFunc = new TF1(Form("fit_%s", runNumbers[r].c_str()), "pol1(0) + gaus(2)", fitLow, fitHigh);
            fitFunc->SetParameters(0, 0, h->GetBinContent(h->FindBin(meanEst)), meanEst, sigmaEst);
            meanIdx = 3;
            sigmaIdx = 4;
        } else {
            fitLow  = meanEst - 2.2 * sigmaEst;
            fitHigh = meanEst + 2.2 * sigmaEst;

            fitFunc = new TF1(Form("fit_%s", runNumbers[r].c_str()), "gaus", fitLow, fitHigh);
            fitFunc->SetParameter(1, meanEst);
            fitFunc->SetParameter(2, sigmaEst);
            meanIdx = 1;
            sigmaIdx = 2;
        }

        h->Fit(fitFunc, "RQ");

        double mean  = fitFunc->GetParameter(meanIdx);
        double sigma = std::abs(fitFunc->GetParameter(sigmaIdx));

        // 4. Integral within a 3-sigma window
        double roiLow  = mean - 3.0 * sigma;
        double roiHigh = mean + 3.0 * sigma;
        int binLow  = h->FindBin(roiLow);
        int binHigh = h->FindBin(roiHigh);
        double integral = h->Integral(binLow, binHigh);

        std::cout << std::setw(6)  << runNumbers[r]
                  << std::setw(8)  << HV[r]
                  << std::setw(12) << std::fixed << std::setprecision(1) << mean
                  << std::setw(10) << std::setprecision(1) << sigma
                  << std::setw(12) << std::setprecision(0) << roiLow
                  << std::setw(12) << std::setprecision(0) << roiHigh
                  << std::setw(18) << std::setprecision(0) << integral << "\n";

        out << runNumbers[r] << "\t" << HV[r] << "\t" << mean << "\t" << sigma << "\t"
            << roiLow << "\t" << roiHigh << "\t" << integral << "\n";

        fitFunc->SetLineColor(kRed);
        fitFunc->SetLineWidth(2);
        fitFunc->Draw("same");

        means.push_back(mean);
        integrals.push_back(integral);
        actualHV.push_back(HV[r]);

        // Update the anchor only when the fit makes sense (lies in a
        // reasonable range and sigma isn't suspiciously large) - otherwise a
        // bad fit would "propagate" into the following, even weaker runs.
        if (mean > 0 && mean < 70000 && sigma > 0 && sigma < 8000) {
            prevPeakPos = mean;
        }

        delete s;
    }

    out.close();

    c->Modified();
    c->Update();
    c->SaveAs(Form("Pad_%d_AllRuns_Fits.png", pad));

    // === Mean vs HV and Integral vs HV dependencies for this pad ===
    if (means.empty()) {
        std::cerr << "Error: no data for Pad " << pad << ", dependency plots skipped.\n";
        return;
    }

    DrawDependencyGraph(pad, means, actualHV, "Mean [ADC channels]", "Mean");
    DrawDependencyGraph(pad, integrals, actualHV, "Integral (3#sigma) [Counts]", "Integral");

    std::cout << "\nDone. Outputs: Pad_" << pad << "_AllRuns_Fits.png, "
              << "Pad_" << pad << "_Mean_vs_HV.png, "
              << "Pad_" << pad << "_Integral_vs_HV.png, " << outFileName << "\n";
}
