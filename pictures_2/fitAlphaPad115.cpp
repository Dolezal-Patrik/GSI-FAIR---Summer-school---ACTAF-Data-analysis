#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TSpectrum.h>
#include <TString.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TError.h>
#include <TFitResult.h>
#include <algorithm>
#include <vector>
#include <cmath>
#include <iostream>

void fitAlphaPad115(TString filename = "online_data_4128_20260831_191835.root", int pad = 55)
{
    gErrorIgnoreLevel = kError;

    // --- NASTAVENI PUB STYLU ---
    gROOT->SetStyle("Pub");
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    gStyle->SetLineWidth(2); // Tloustka ramecku a os
    // Zarovnani titulku osy X doprava
    gStyle->SetTitleXOffset(1.0);

    // Okraje platna
    gStyle->SetPadLeftMargin(0.14);
    gStyle->SetPadBottomMargin(0.14);
    gStyle->SetPadRightMargin(0.06); // Dostatok mista vpravo pro popisek
    gStyle->SetPadTopMargin(0.05);

    TFile* f = TFile::Open(filename, "READ");
    if (!f || f->IsZombie()) {
        std::cout << "ERROR: Cannot open " << filename << std::endl;
        return;
    }

    TString histName = Form("fh1_Pad_%d_Eraw", pad);
    TH1F* h = (TH1F*)f->Get(histName);

    if (!h) {
        std::cout << "ERROR: Missing " << histName << std::endl;
        f->Close();
        return;
    }

    TH1F* h_copy = (TH1F*)h->Clone(Form("h_copy_pad%d", pad));
    h_copy->SetDirectory(0);
    f->Close();

    // Rebin 4 (pozn.: komentar rika "4", kod dela Rebin(2) - ponechano podle originalu)
    h_copy->Rebin(2);

    TString baseName = filename;
    baseName.ReplaceAll(".root", "");

    // Vzhled histogramu
    h_copy->SetFillStyle(0);
    h_copy->SetLineColor(kBlue + 2);
    h_copy->SetLineWidth(3);
    h_copy->GetXaxis()->SetTitle("Integral, ADC channels");
    h_copy->GetYaxis()->SetTitle("Counts");
    h_copy->GetXaxis()->CenterTitle(kFALSE);   // popisek X osy zarovnany doprava (konec osy)
    h_copy->GetYaxis()->CenterTitle(kFALSE);   // popisek Y osy zarovnany nahoru (konec osy)
    h_copy->GetXaxis()->SetLabelSize(0.04);
    h_copy->GetYaxis()->SetLabelSize(0.04);
    h_copy->GetXaxis()->SetTitleSize(0.05);
    h_copy->GetYaxis()->SetTitleSize(0.05);
    h_copy->GetXaxis()->SetTitleOffset(1.1);
    h_copy->GetYaxis()->SetTitleOffset(1.3);

    // =========================================================================
    // 1. KROK: Vyhledani alfa piku pomoci TSpectrum
    // =========================================================================
    h_copy->GetXaxis()->SetRangeUser(30000, 70000);

    TSpectrum *s = new TSpectrum(5);
    int nfound = s->Search(h_copy, 15, "goff", 0.2);

    if (nfound == 0) {
        std::cout << "Pad " << pad << ": TSpectrum nenalezl zadny pik!" << std::endl;
        delete s;
        return;
    }

    double *xpeaks = s->GetPositionX();
    std::vector<double> rightmostPeaks(xpeaks, xpeaks + nfound);
    std::sort(rightmostPeaks.rbegin(), rightmostPeaks.rend());
    double alphaPeakPos = rightmostPeaks[0];
    delete s;

    h_copy->GetXaxis()->SetRangeUser(30000, 70000);

    // =========================================================================
    // 2. KROK: Predfit (cisty gaus) - jen pro odhad sigma/mean
    // =========================================================================
    double initWindow = 2500.0;
    TF1 *preFit = new TF1(Form("preFit_%d", pad), "gaus", alphaPeakPos - initWindow, alphaPeakPos + initWindow);
    preFit->SetLineColor(kRed + 1);
    preFit->SetLineWidth(4);

    h_copy->Fit(preFit, "RQ0");

    double meanEst  = preFit->GetParameter(1);
    double sigmaEst = std::abs(preFit->GetParameter(2));

    if (sigmaEst <= 0 || sigmaEst > 8000) {
        sigmaEst = 2500;
        meanEst = alphaPeakPos;
    }

    std::cout << "Pre-fit  chi2/ndf = " << preFit->GetChisquare() / preFit->GetNDF() << std::endl;

    // --- CANVAS 1: Initial Fit ---
    TCanvas* c1 = new TCanvas("c_initial_fit", Form("Initial Fit Pad %d", pad), 800, 600);
    h_copy->Draw("HIST");
   // preFit->Draw("SAME");

    TLegend* leg1 = new TLegend(0.15, 0.60, 0.48, 0.88);
    leg1->SetBorderSize(1);
    leg1->SetLineColor(kBlack);
    leg1->SetFillColor(kWhite);
    leg1->SetTextSize(0.035);
    leg1->AddEntry(h_copy, Form("Pad %d Data", pad), "l");
    leg1->AddEntry(preFit, "Pre-Fit", "l");
    leg1->AddEntry((TObject*)0, Form("#mu = %.1f #pm %.1f", preFit->GetParameter(1), preFit->GetParError(1)), "");
    leg1->AddEntry((TObject*)0, Form("#sigma = %.1f #pm %.1f", preFit->GetParameter(2), preFit->GetParError(2)), "");
    leg1->AddEntry((TObject*)0, Form("#chi^{2} / NDF = %.2f", preFit->GetChisquare() / preFit->GetNDF()), "");
   // leg1->Draw();

    c1->SaveAs(Form("%s_pad%d_initial_fit.png", baseName.Data(), pad));

    // =========================================================================
    // 3. KROK: Odhad pozadi ze sidebandu (mimo pik)
    //    - vezmeme pas nalevo a napravo od piku, spocteme prumerny obsah
    //      binu v kazdem pasu a jimi prolozime primku -> lepsi start pro pol1
    // =========================================================================
    double sbWidth = 0.5 * sigmaEst;          // sirka sideband okna
    double sbLeftLo  = meanEst - 3.0 * sigmaEst - sbWidth;
    double sbLeftHi  = meanEst - 3.0 * sigmaEst;
    double sbRightLo = meanEst + 3.0 * sigmaEst;
    double sbRightHi = meanEst + 3.0 * sigmaEst + sbWidth;

    auto avgInRange = [&](double lo, double hi) -> std::pair<double,double> {
        int b1 = h_copy->GetXaxis()->FindBin(lo);
        int b2 = h_copy->GetXaxis()->FindBin(hi);
        if (b2 < b1) std::swap(b1, b2);
        double sumY = 0, sumX = 0; int n = 0;
        for (int b = b1; b <= b2; ++b) {
            sumY += h_copy->GetBinContent(b);
            sumX += h_copy->GetXaxis()->GetBinCenter(b);
            ++n;
        }
        if (n == 0) return {0.0, lo};
        return {sumY / n, sumX / n};
    };

    auto [yLeft, xLeft]   = avgInRange(sbLeftLo, sbLeftHi);
    auto [yRight, xRight] = avgInRange(sbRightLo, sbRightHi);

    double slopeEst = 0.0, offsetEst = 0.0;
    if (xRight != xLeft) {
        slopeEst  = (yRight - yLeft) / (xRight - xLeft);
        offsetEst = yLeft - slopeEst * xLeft;
    } else {
        offsetEst = 0.5 * (yLeft + yRight);
    }

    // =========================================================================
    // 4. KROK: Hlavni fit - asymetricke okno (vice prostoru na low-energy tail)
    //    a start hodnoty pozadi z odhadu vyse
    // =========================================================================
    TCanvas* c2 = new TCanvas("c_final_fit", Form("Final Fit Pad %d", pad), 800, 600);

    double fitLow  = meanEst - 2.5 * sigmaEst;   // vice prostoru na low-energy tail
    double fitHigh = meanEst + 2.5 * sigmaEst;

    TF1 *fitFunc = new TF1(Form("fit_%d", pad), "pol1(0) + gaus(2)", fitLow, fitHigh);
    fitFunc->SetParameters(offsetEst, slopeEst,
                            h_copy->GetBinContent(h_copy->FindBin(meanEst)),
                            meanEst, sigmaEst);
    fitFunc->SetLineColor(kRed + 1);
    fitFunc->SetLineWidth(4);

    // Prvni kolo - klasicky chi2 fit, jen pro dobry start parametru
    TFitResultPtr r1 = h_copy->Fit(fitFunc, "RQ0S");

    // Druhe kolo - log-likelihood fit ("L") + MINUIT improve ("M") + plne chyby ("E")
    // Likelihood fit je pro biny s nizkym poctem countu statisticky korektnejsi
    // nez chi2 zalozeny na sqrt(N) chybach, a chi2/ndf spocitany z nej se chova
    // realistiveji (blize 1), protoze nepredpoklada Gaussovske chyby v kazdem binu.
    TFitResultPtr r2 = h_copy->Fit(fitFunc, "RQ0SMEL");

    double mean  = fitFunc->GetParameter(3);
    double sigma = std::abs(fitFunc->GetParameter(4));

    // Pozn.: po fitu s "L" vraci GetChisquare()/GetNDF() chi2 spocitany post-hoc
    // z vysledneho modelu (ne primo optimalizovanou velicinu), ale je to porovnatelna
    // metrika kvality fitu jako u predchoziho chi2 fitu.
    std::cout << "Final fit chi2/ndf (round 1, chi2)      = "
              << r1->Chi2() / r1->Ndf() << std::endl;
    std::cout << "Final fit chi2/ndf (round 2, likelihood) = "
              << fitFunc->GetChisquare() / fitFunc->GetNDF() << std::endl;

    h_copy->Draw("HIST");
    fitFunc->Draw("SAME");

    TLegend* leg2 = new TLegend(0.15, 0.60, 0.48, 0.88);
    leg2->SetBorderSize(1);
    leg2->SetLineColor(kBlack);
    leg2->SetFillColor(kWhite);
    leg2->SetTextSize(0.035);
    leg2->AddEntry(h_copy, Form("Pad %d Data", pad), "l");
    leg2->AddEntry(fitFunc, "Pol1 + Gauss Fit", "l");
    leg2->AddEntry((TObject*)0, Form("#mu = %.1f #pm %.1f", mean, fitFunc->GetParError(3)), "");
    leg2->AddEntry((TObject*)0, Form("#sigma = %.1f #pm %.1f", sigma, fitFunc->GetParError(4)), "");
    leg2->AddEntry((TObject*)0, Form("#chi^{2} / NDF = %.2f", fitFunc->GetChisquare() / fitFunc->GetNDF()), "");
    leg2->Draw();

    c2->SaveAs(Form("%s_pad%d_final_fit.png", baseName.Data(), pad));

    std::cout << "Ulozeno (sideband-background + asym. okno + MINUIT improve) pro Pad " << pad << std::endl;
}
