#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TString.h>
#include <TSystem.h>
#include <vector>
#include <iostream>

void openAlphaHistograms(TString filename)
{
    TFile* f = TFile::Open(filename, "READ");
    if (!f || f->IsZombie()) { std::cout << "ERROR: Cannot open " << filename << std::endl; return; }

    std::vector<int> selectedPad =
        { 115, 122, 102, 103, 110, 117, 112, 119, 46, 51, 52, 48, 55 };

    // Alpha-source amplitude window (10^3 ADC units), applied to all pads
    const double xLow = 10;  // 30000 raw ADC
    const double xUp  = 100.0; // 100e3 raw ADC

    TString baseName = filename;
    baseName.ReplaceAll(".root", "");

    // 1D integral spectra
    std::vector<TH1F*> histograms(selectedPad.size(), nullptr);
    TCanvas* c1 = new TCanvas("c_alpha", "Alpha spectra", 1600, 1000);
    c1->Divide(4, 4);

    for (size_t i = 0; i < selectedPad.size(); ++i)
    {
        TString name = Form("h_alpha_pad%d", selectedPad[i]);
        histograms[i] = (TH1F*)f->Get(name);
        if (!histograms[i]) { std::cout << "Missing " << name << std::endl; continue; }

        histograms[i]->GetXaxis()->SetRangeUser(xLow, xUp);

        c1->cd(i + 1);
        histograms[i]->Draw("HIST");
    }

    c1->SaveAs(baseName + "_spectra.png");

    // Integral vs event number
    std::vector<TH2F*> histograms2D(selectedPad.size(), nullptr);
    TCanvas* c2 = new TCanvas("c_alpha_vs_event", "Integral vs event number", 1600, 1000);
    c2->Divide(4, 4);

    for (size_t i = 0; i < selectedPad.size(); ++i)
    {
        TString name2D = Form("h2_alpha_pad%d_vs_event", selectedPad[i]);
        histograms2D[i] = (TH2F*)f->Get(name2D);
        if (!histograms2D[i]) { std::cout << "Missing " << name2D << std::endl; continue; }

        histograms2D[i]->GetYaxis()->SetRangeUser(xLow, xUp);

        c2->cd(i + 1);
        histograms2D[i]->Draw("COLZ");
    }

    c2->SaveAs(baseName + "_vs_event.png");

    std::cout << "Saved: " << baseName + "_spectra.png" << " and " << baseName + "_vs_event.png" << std::endl;
}
