#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TSpectrum.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TROOT.h>
#include <TString.h>
#include <TKey.h>
#include <TList.h>
#include <TMarker.h>
#include <TSystem.h>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <cmath>

// ---------------------------------------------------------------------
// Nastavení (hodnoty x jsou v 10^3 ADC chan)
// ---------------------------------------------------------------------
const double kSearchLow       = 30.0;  // rozsah hledání píků: dolní mez
const double kSearchHigh      = 100.0; // rozsah hledání píků: horní mez
const double kSearchSigma     = 1.5;   // očekávané sigma píku pro TSpectrum
const double kSearchThreshold = 0.5;   // prahová hodnota TSpectrum vůči nejvyššímu píku
const double kMinPeakCounts   = 5.0;   // pík s méně než těmito countu je ignorován
const double kPreFitWindow    = 2.5;   // okno pro pre-fit: nalezený pík +- tato hodnota
const double kSigmaMax        = 8.0;   // maximální povolená hodnota sigmy z pre-fitu
const double kSigmaFallback   = 2.5;   // záložní sigma, pokud pre-fit selže
const double kDisplayLow      = 5.0;   // zobrazený rozsah x
const double kDisplayHigh     = 100.0;

// ---------------------------------------------------------------------
// Konfigurace fitu pro jednotlivé pady
// ---------------------------------------------------------------------
struct PadFitConfig {
    TString formula;       // TF1 recept
    double  lowSigmaMult;  // dolní mez fitu = mean - lowSigmaMult * sigma
    double  highSigmaMult; // horní mez fitu = mean + highSigmaMult * sigma
    int     meanParIdx;    // index parametru "mean" v TF1
    int     sigmaParIdx;   // index parametru "sigma" v TF1
    bool    isCompound;    // true pro pol1 + crystalball
};

PadFitConfig DefaultConfig()
{
    PadFitConfig cfg;
    cfg.formula = "crystalball";
    cfg.lowSigmaMult = 4.5;  // Dostatečně široký rozsah pro zachycení chvostu
    cfg.highSigmaMult = 2.0;
    cfg.meanParIdx = 1;      // V čistem "crystalball" je Mean na indexu 1
    cfg.sigmaParIdx = 2;     // V čistem "crystalball" je Sigma na indexu 2
    cfg.isCompound = false;
    return cfg;
}

// Konfigurace pro pady s výrazným pozadím (např. Pad 112)
PadFitConfig CompoundCBConfig()
{
    PadFitConfig cfg;
    cfg.formula = "pol1(0)+crystalball(2)";
    cfg.lowSigmaMult = 10.0; // Široké okno nalevo pro fitování pozadí
    cfg.highSigmaMult = 2.0;
    cfg.meanParIdx = 3;      // V pol1(0)+crystalball(2) je Mean na indexu 3
    cfg.sigmaParIdx = 4;     // V pol1(0)+crystalball(2) je Sigma na indexu 4
    cfg.isCompound = true;
    return cfg;
}

std::map<int, PadFitConfig> BuildFitConfigTable()
{
    std::map<int, PadFitConfig> table;
    table[112] = CompoundCBConfig(); // Speciální fit s pozadím pro Pad 112
    // Sem lze přidat další problematické pady, např.:
    // table[46] = CompoundCBConfig();
    return table;
}

PadFitConfig GetConfigForPad(const std::map<int, PadFitConfig>& table, int pad)
{
    auto it = table.find(pad);
    return (it != table.end()) ? it->second : DefaultConfig();
}

TString GetScriptDir()
{
    TString scriptPath = __FILE__;
    if (!gSystem->IsAbsoluteFileName(scriptPath))
        scriptPath = gSystem->ConcatFileName(gSystem->WorkingDirectory(), scriptPath.Data());
    return TString(gSystem->DirName(scriptPath.Data()));
}

void SetAlphaStyle()
{
    gROOT->SetStyle("Pub");
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    gStyle->SetFrameLineWidth(2);
}

void formatAxes(TH1* h)
{
    h->GetXaxis()->SetTitle("Integral, 10^{3} ADC chan");
    h->GetYaxis()->SetTitle("Counts");
    h->GetXaxis()->SetLabelSize(0.045);
    h->GetYaxis()->SetLabelSize(0.045);
    h->GetXaxis()->SetTitleSize(0.05);
    h->GetYaxis()->SetTitleSize(0.05);
    h->GetXaxis()->SetTitleOffset(1.0);
    h->GetYaxis()->SetTitleOffset(1.1);
}

std::vector<int> findAllPads(TFile* f)
{
    std::vector<int> pads;
    TList* keys = f->GetListOfKeys();
    if (!keys) return pads;

    const TString prefix = "h_alpha_pad";
    TIter next(keys);
    TKey* key;
    while ((key = (TKey*)next())) {
        TString name = key->GetName();
        if (!name.BeginsWith(prefix)) continue;
        TString number = name(prefix.Length(), name.Length() - prefix.Length());
        if (number.IsDigit()) pads.push_back(number.Atoi());
    }
    std::sort(pads.begin(), pads.end());
    pads.erase(std::unique(pads.begin(), pads.end()), pads.end());
    return pads;
}

bool fitPadAlpha(TH1F* h, int padNumber, const PadFitConfig& cfg, double& mu, double& muErr, double& sigma,
                 double& sigmaErr, double& chi2ndf, TF1*& finalFit, std::vector<double>& peakX,
                 std::vector<double>& peakY)
{
    mu = muErr = sigma = sigmaErr = chi2ndf = 0;
    finalFit = nullptr;
    peakX.clear();
    peakY.clear();
    if (!h) return false;

    TH1F* hFit = (TH1F*)h->Clone(Form("%s_fit", h->GetName()));
    hFit->SetDirectory(nullptr);

    // 1. Vyhledání píků pomocí TSpectrum
    hFit->GetXaxis()->SetRangeUser(kSearchLow, kSearchHigh);
    TSpectrum spectrum(5);
    double sigmaBins = std::max(2.0, kSearchSigma / hFit->GetXaxis()->GetBinWidth(1));
    int nfound = spectrum.Search(hFit, sigmaBins, "goff", kSearchThreshold);
    hFit->GetXaxis()->SetRange(0, 0);

    if (nfound == 0) {
        std::cout << "Pad " << padNumber << " | no peak found" << std::endl;
        delete hFit;
        return false;
    }
    double* posX = spectrum.GetPositionX();
    double* posY = spectrum.GetPositionY();
    int rightmost = 0;
    for (int i = 0; i < nfound; i++) {
        peakX.push_back(posX[i]);
        peakY.push_back(posY[i]);
        if (posX[i] > posX[rightmost]) rightmost = i;
    }
    double alphaPeakPos = posX[rightmost];

    if (posY[rightmost] < kMinPeakCounts) {
        std::cout << "Pad " << padNumber << " | peak too low (" << posY[rightmost] << " counts), skipped" << std::endl;
        peakX.clear();
        peakY.clear();
        delete hFit;
        return false;
    }

    // 2. Pre-fit pro odhad střední hodnoty a sigmy
    TF1* preFit = new TF1(Form("preFit_%s", h->GetName()), "gaus", alphaPeakPos - kPreFitWindow, alphaPeakPos + kPreFitWindow);
    hFit->Fit(preFit, "RQN");
    double meanEst = preFit->GetParameter(1);
    double sigmaEst = std::abs(preFit->GetParameter(2));
    delete preFit;

    if (sigmaEst <= 0 || sigmaEst > kSigmaMax) {
        sigmaEst = kSigmaFallback;
        meanEst = alphaPeakPos;
    }

    // 3. Nastavení rozsahu a fitovací funkce
    double fitLow = meanEst - cfg.lowSigmaMult * sigmaEst;
    double fitHigh = meanEst + cfg.highSigmaMult * sigmaEst;
    std::cout << "Pad " << padNumber << " | " << cfg.formula << " | peak = " << alphaPeakPos
              << " | sigma est. = " << sigmaEst << " | final range = " << fitLow << " - " << fitHigh << std::endl;

    TF1* fitFunc = new TF1(Form("fit_%s", h->GetName()), cfg.formula, fitLow, fitHigh);
    double height = hFit->GetBinContent(hFit->FindBin(meanEst));

    if (cfg.isCompound) {
        // --- pol1(0) + crystalball(2) ---
        // p0, p1 = pol1 background
        // p2 = Constant (Amplituda CB)
        // p3 = Mean (Střední hodnota CB)
        // p4 = Sigma (Šířka CB)
        // p5 = Alpha
        // p6 = n (Power)

        fitFunc->SetParameter(0, 0);         // p0: posun pozadí
        fitFunc->SetParameter(1, 0);         // p1: sklon pozadí

        fitFunc->SetParameter(2, height);    // p2: amplituda
        fitFunc->SetParLimits(2, 0, height * 5.0);

        fitFunc->SetParameter(3, meanEst);   // p3: mean
        fitFunc->SetParLimits(3, fitLow, fitHigh);

        fitFunc->SetParameter(4, sigmaEst);  // p4: sigma
        fitFunc->SetParLimits(4, 0.2, 10.0);

        fitFunc->SetParameter(5, 1.5);       // p5: alpha
        fitFunc->SetParLimits(5, 0.1, 5.0);

        fitFunc->SetParameter(6, 3.0);       // p6: n
        fitFunc->SetParLimits(6, 1.05, 20.0);
    } else {
        // --- Čistý crystalball ---
        // p0 = Constant, p1 = Mean, p2 = Sigma, p3 = Alpha, p4 = n
        fitFunc->SetParameter(0, height);
        fitFunc->SetParLimits(0, 0, height * 5.0);

        fitFunc->SetParameter(1, meanEst);
        fitFunc->SetParLimits(1, fitLow, fitHigh);

        fitFunc->SetParameter(2, sigmaEst);
        fitFunc->SetParLimits(2, 0.2, 10.0);

        fitFunc->SetParameter(3, 1.5);
        fitFunc->SetParLimits(3, 0.1, 5.0);

        fitFunc->SetParameter(4, 3.0);
        fitFunc->SetParLimits(4, 1.05, 20.0);
    }

    fitFunc->SetLineColor(kRed + 1);
    fitFunc->SetLineWidth(3);
    fitFunc->SetNpx(500);

    hFit->Fit(fitFunc, "RQN");

    // 4. Uložení výsledků z odpovídajících indexů
    mu = fitFunc->GetParameter(cfg.meanParIdx);
    muErr = fitFunc->GetParError(cfg.meanParIdx);
    sigma = std::abs(fitFunc->GetParameter(cfg.sigmaParIdx));
    sigmaErr = fitFunc->GetParError(cfg.sigmaParIdx);
    chi2ndf = (fitFunc->GetNDF() > 0) ? fitFunc->GetChisquare() / fitFunc->GetNDF() : 0;
    finalFit = fitFunc;

    delete hFit;
    return true;
}

void fitAllPadsAlpha(TString filename)
{
    gROOT->SetBatch(kFALSE);
    SetAlphaStyle();

    TFile* f = TFile::Open(filename, "READ");
    if (!f || f->IsZombie()) { std::cout << "ERROR: Cannot open file: " << filename << std::endl; return; }

    std::vector<int> pads = findAllPads(f);
    std::cout << "Found " << pads.size() << " pads:" << std::endl;
    for (int pad : pads) std::cout << "  Pad " << pad << std::endl;
    if (pads.empty()) {
        std::cout << "ERROR: No h_alpha_padXXX histograms found." << std::endl;
        f->Close();
        return;
    }

    TString outDir = GetScriptDir() + "/FITDATA";
    if (gSystem->AccessPathName(outDir.Data())) {
        std::cout << "ERROR: output directory " << outDir << " not found." << std::endl;
        f->Close();
        return;
    }

    TString baseName = gSystem->BaseName(filename.Data());
    baseName.ReplaceAll(".root", "");
    baseName = outDir + "/" + baseName;
    TString outputTxt = baseName + "_fit_results.txt";
    std::ofstream out(outputTxt.Data());
    out << "Pad\tEntries\tMu\tMuErr\tSigma\tSigmaErr\tChi2/NDF\n";

    std::map<int, PadFitConfig> fitTable = BuildFitConfigTable();
    const int padsPerPage = 16;
    int nPages = (pads.size() + padsPerPage - 1) / padsPerPage;

    for (int page = 0; page < nPages; page++) {
        TCanvas* c = new TCanvas(Form("c_alpha_page_%d", page), Form("Alpha fits - page %d", page), 1600, 1000);
        c->Divide(4, 4, 0.005, 0.005);

        for (int slot = 0; slot < padsPerPage; slot++) {
            int index = page * padsPerPage + slot;
            if (index >= (int)pads.size()) break;

            int padNumber = pads[index];
            TString histName = Form("h_alpha_pad%d", padNumber);
            TH1F* hOriginal = dynamic_cast<TH1F*>(f->Get(histName));

            c->cd(slot + 1);
            gPad->SetLeftMargin(0.12);
            gPad->SetRightMargin(0.04);
            gPad->SetTopMargin(0.08);
            gPad->SetBottomMargin(0.12);

            if (!hOriginal) { std::cout << "Pad " << padNumber << ": histogram not found." << std::endl; continue; }

            TH1F* h = (TH1F*)hOriginal->Clone(Form("display_pad%d_page%d", padNumber, page));
            h->SetDirectory(nullptr);

            PadFitConfig cfg = GetConfigForPad(fitTable, padNumber);
            double mu = 0, muErr = 0, sigma = 0, sigmaErr = 0, chi2ndf = 0;
            TF1* fitFunc = nullptr;
            std::vector<double> tsPeakX, tsPeakY;

            bool fitOK = fitPadAlpha(h, padNumber, cfg, mu, muErr, sigma, sigmaErr, chi2ndf, fitFunc, tsPeakX, tsPeakY);

            // Vykreslení histogramu
            h->SetLineColor(kBlue + 2);
            h->SetLineWidth(2);
            h->SetFillStyle(0);
            formatAxes(h);
            h->GetXaxis()->SetRangeUser(kDisplayLow, kDisplayHigh);
            h->Draw("HIST");

            // TSpectrum značky
            for (size_t i = 0; i < tsPeakX.size(); i++) {
                if (tsPeakX[i] < kDisplayLow || tsPeakX[i] > kDisplayHigh) continue;
                TMarker* marker = new TMarker(tsPeakX[i], tsPeakY[i], 20);
                marker->SetMarkerColor(kBlack);
                marker->SetMarkerSize(0.8);
                marker->Draw("SAME");
            }

            // Vykreslení fitu
            if (fitOK && fitFunc) fitFunc->Draw("SAME");

            // Popisek padu
            TLatex padLabel;
            padLabel.SetNDC();
            padLabel.SetTextFont(62);
            padLabel.SetTextSize(0.055);
            padLabel.DrawLatex(0.14, 0.88, Form("Pad %d", padNumber));

            // Legenda
            TLegend* leg = new TLegend(0.48, 0.62, 0.95, 0.88);
            leg->SetBorderSize(1);
            leg->SetFillColor(kWhite);
            leg->SetTextSize(0.030);
            leg->AddEntry(h, Form("Entries: %.0f", h->GetEntries()), "l");
            if (fitOK && fitFunc) {
                leg->AddEntry(fitFunc, cfg.isCompound ? "CB + linear bg" : "Crystal Ball", "l");
                leg->AddEntry((TObject*)nullptr, Form("#mu = %.2f #pm %.2f", mu, muErr), "");
                leg->AddEntry((TObject*)nullptr, Form("#sigma = %.2f #pm %.2f", sigma, sigmaErr), "");
                leg->AddEntry((TObject*)nullptr, Form("#chi^{2}/NDF = %.2f", chi2ndf), "");
            }
            leg->Draw();

            if (fitOK) {
                std::cout << "Pad " << padNumber << " | mu = " << mu << " +- " << muErr
                          << " | sigma = " << sigma << " +- " << sigmaErr
                          << " | chi2/ndf = " << chi2ndf << std::endl;
                out << padNumber << "\t" << h->GetEntries() << "\t" << mu << "\t" << muErr << "\t"
                    << sigma << "\t" << sigmaErr << "\t" << chi2ndf << "\n";
            } else {
                std::cout << "Pad " << padNumber << " | FIT FAILED" << std::endl;
            }
        }

        c->Modified();
        c->Update();

        TString pngName = Form("%s_fits_page%d.png", baseName.Data(), page);
        TString pdfName = Form("%s_fits_page%d.pdf", baseName.Data(), page);
        c->SaveAs(pngName);
        c->SaveAs(pdfName);
        std::cout << "Saved: " << pngName << std::endl;
        std::cout << "Saved: " << pdfName << std::endl;
    }

    out.close();
    f->Close();
    std::cout << "\nResults saved to: " << outputTxt << std::endl;
}

void runAlphaFit()
{
    fitAllPadsAlpha("alpha_histograms_nearline_data_4130_20260820_144316.root");
}
