#ifdef __CLING__
#pragma cling optimize(0)
#endif
void pad55prefit()
{
//=========Macro generated from canvas: c_initial_fit/Initial Fit Pad 55
//=========  (Sat Sep  5 18:09:29 2026) by ROOT version 6.36.04
   TCanvas *c_initial_fit = new TCanvas("c_initial_fit", "Initial Fit Pad 55", 98, 30, 800, 600);
   gStyle->SetOptFit(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   TColor::SetPalette(57, nullptr);
   c_initial_fit->Range(22953,-5.988889,73003,36.78889);
   c_initial_fit->SetFillColor(10);
   c_initial_fit->SetBorderMode(0);
   c_initial_fit->SetBorderSize(2);
   c_initial_fit->SetTickx(1);
   c_initial_fit->SetTicky(1);
   c_initial_fit->SetLeftMargin(0.14);
   c_initial_fit->SetRightMargin(0.06);
   c_initial_fit->SetTopMargin(0.05);
   c_initial_fit->SetBottomMargin(0.14);
   c_initial_fit->SetFrameLineWidth(3);
   c_initial_fit->SetFrameLineWidth(3);
   
   TH1F *h_copy_pad55__2 = new TH1F("h_copy_pad55__2", "ERaw: Pad 55 (Mod 2 Chn 13)", 500, 0, 70000);
   std::vector<Double_t> h_copy_pad55__2_vect3{
      0, 124, 138, 578, 3439, 7368, 6626, 3501, 1231, 417,
      180, 95, 78, 53, 46, 19, 15, 18, 14, 6,
      13, 15, 9, 9, 8, 11, 5, 6, 5, 7,
      7, 4, 5, 4, 4, 1, 5, 5, 4, 2,
      2, 2, 2, 2, 0, 3, 2, 1, 4, 2,
      1, 0, 0, 1, 5, 3, 0, 2, 0, 1,
      2, 1, 1, 1, 4, 2, 1, 0, 1, 3,
      0, 0, 2, 1, 1, 2, 2, 1, 1, 1,
      1, 1, 0, 1, 2, 2, 1, 1, 0, 0,
      0, 0, 0, 0, 1, 0, 0, 1, 0, 2,
      1, 1, 1, 0, 0, 1, 0, 0, 2, 0,
      0, 0, 0, 1, 0, 2, 0, 1, 0, 0,
      0, 1, 2, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 2, 0, 1, 0, 3, 0, 1, 0,
      0, 0, 2, 0, 1, 0, 0, 1, 0, 0,
      0, 1, 1, 0, 0, 0, 1, 0, 0, 0,
      0, 0, 0, 1, 0, 1, 0, 1, 1, 1,
      0, 1, 0, 1, 1, 0, 0, 0, 0, 0,
      0, 1, 1, 2, 0, 2, 0, 1, 0, 0,
      1, 1, 0, 2, 1, 2, 0, 0, 0, 0,
      1, 0, 0, 1, 1, 0, 0, 0, 1, 0,
      0, 1, 0, 2, 0, 0, 0, 0, 0, 0,
      1, 0, 0, 0, 0, 1, 1, 2, 0, 0,
      1, 1, 0, 0, 1, 2, 0, 1, 0, 0,
      1, 2, 1, 0, 1, 0, 2, 0, 0, 2,
      0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
      2, 0, 0, 0, 0, 1, 0, 1, 1, 2,
      0, 0, 0, 0, 0, 0, 2, 1, 0, 0,
      2, 1, 1, 0, 1, 2, 1, 1, 1, 1,
      0, 3, 0, 2, 3, 0, 0, 1, 1, 0,
      0, 0, 0, 0, 0, 0, 1, 1, 0, 1,
      1, 0, 1, 0, 2, 0, 0, 0, 0, 0,
      0, 1, 2, 1, 0, 3, 1, 1, 0, 1,
      1, 2, 2, 3, 2, 1, 0, 2, 1, 1,
      2, 3, 0, 2, 1, 0, 2, 3, 2, 4,
      1, 5, 2, 1, 3, 4, 8, 2, 4, 4,
      6, 6, 5, 7, 5, 4, 9, 5, 8, 15,
      11, 9, 8, 17, 15, 9, 17, 18, 14, 19,
      18, 29, 17, 26, 18, 19, 24, 19, 33, 21,
      15, 18, 13, 9, 3, 2, 5, 3, 4, 4,
      2, 1, 0, 0, 0, 1, 0, 0, 1, 0,
      1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
      0, 0, 1, 0, 1, 0, 1, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 21
   };
   for (Int_t bin = 0; bin < 502; bin++)
      if (h_copy_pad55__2_vect3[bin])
         h_copy_pad55__2->SetBinContent(bin, h_copy_pad55__2_vect3[bin]);
   h_copy_pad55__2->SetEntries(24884);
   h_copy_pad55__2->SetDirectory(nullptr);
   
   std::vector<Double_t> pmarker_vect4{ 53550 };
   std::vector<Double_t> pmarker_vect5{ 26 };
   TPolyMarker *pmarker = new TPolyMarker(1, pmarker_vect4.data(), pmarker_vect5.data(), "");
   pmarker->SetMarkerColor(TColor::GetColor("#ff0000"));
   pmarker->SetMarkerStyle(23);
   pmarker->SetMarkerSize(1.299999952316284);
   h_copy_pad55__2->GetListOfFunctions()->Add(pmarker,"");
   
   TF1 *fit_554 = new TF1("fit_55", "pol1(0) + gaus(2)", 50406.19380307863,56244.70133722561, TF1::EAddToList::kNo);
   fit_554->SetBit(TF1::kNotDraw);
   fit_554->SetFillColor(19);
   fit_554->SetMarkerColor(1);
   fit_554->SetMarkerStyle(1);
   fit_554->SetMarkerSize(1);
   fit_554->SetLineColor(TColor::GetColor("#cc0000"));
   fit_554->SetLineStyle(1);
   fit_554->SetLineWidth(4);
   fit_554->SetChisquare(44.76172408138238);
   fit_554->SetNDF(37);
   fit_554->SetParameter(0, 69.78884326713892);
   fit_554->SetParError(0, 5.614272351256166);
   fit_554->SetParLimits(0, 0, 0);
   fit_554->SetParameter(1, -0.001264145128466845);
   fit_554->SetParError(1, 0.0001037353325680475);
   fit_554->SetParLimits(1, 0, 0);
   fit_554->SetParameter(2, 20.94731094158586);
   fit_554->SetParError(2, 1.488449026483699);
   fit_554->SetParLimits(2, 0, 0);
   fit_554->SetParameter(3, 53635.16672026783);
   fit_554->SetParError(3, 70.60517346282249);
   fit_554->SetParLimits(3, 0, 0);
   fit_554->SetParameter(4, 1089.460874197526);
   fit_554->SetParError(4, 132.4134074246212);
   fit_554->SetParLimits(4, 0, 0);
   fit_554->SetParent(h_copy_pad55__2);
   h_copy_pad55__2->GetListOfFunctions()->Add(fit_554,"");
   h_copy_pad55__2->SetFillColor(31);
   h_copy_pad55__2->SetFillStyle(0);
   h_copy_pad55__2->SetLineColor(TColor::GetColor("#000099"));
   h_copy_pad55__2->SetLineWidth(3);
   h_copy_pad55__2->GetXaxis()->SetTitle("Integral, a.u.");
 h_copy_pad55__2->GetXaxis()->SetTitle("Integral, a.u.");
   h_copy_pad55__2->GetXaxis()->SetRange(215, 500);
   h_copy_pad55__2->GetXaxis()->SetNdivisions(505);     // Redukcia čísel, aby sa neprekrývali
   
   // Font 63 = Arial/Helvetica Bold s veľkosťou v PIXELOCH
   h_copy_pad55__2->GetXaxis()->SetLabelFont(63);
   h_copy_pad55__2->GetXaxis()->SetLabelSize(28);       // Masívne čísla (26px)
   h_copy_pad55__2->GetXaxis()->SetTitleFont(63);
   h_copy_pad55__2->GetXaxis()->SetTitleSize(32);       // Masívny názov osi (30px)
   h_copy_pad55__2->GetXaxis()->SetTitleOffset(1.3);
   
   // ==========================================
   // --- OS Y: EXTRÉMNE VÝRAZNÝ A TUČNÝ ŠTÝL ---
   // ==========================================
   h_copy_pad55__2->GetYaxis()->SetTitle("Counts");
   h_copy_pad55__2->GetYaxis()->SetLabelFont(63);
   h_copy_pad55__2->GetYaxis()->SetLabelSize(28);       // Masívne čísla (26px)
   h_copy_pad55__2->GetYaxis()->SetTitleFont(63);
   h_copy_pad55__2->GetYaxis()->SetTitleSize(32);       // Masívny názov osi (30px)
   h_copy_pad55__2->GetYaxis()->SetTitleOffset(1.2);
   h_copy_pad55__2->Draw("HIST");
   
   TF1 *preFit_554 = new TF1("preFit_55", "gaus", 51050,56050, TF1::EAddToList::kDefault);
   preFit_554->SetFillColor(19);
   preFit_554->SetMarkerColor(1);
   preFit_554->SetMarkerStyle(1);
   preFit_554->SetMarkerSize(1);
   preFit_554->SetLineColor(TColor::GetColor("#cc0000"));
   preFit_554->SetLineStyle(1);
   preFit_554->SetLineWidth(4);
   preFit_554->SetChisquare(53.18107);
   preFit_554->SetNDF(32);
   preFit_554->SetParameter(0, 21.0501);
   preFit_554->SetParError(0, 1.396226);
   preFit_554->SetParLimits(0, 0, 0);
   preFit_554->SetParameter(1, 53325.45);
   preFit_554->SetParError(1, 72.49957);
   preFit_554->SetParLimits(1, 0, 0);
   preFit_554->SetParameter(2, 1167.702);
   preFit_554->SetParError(2, 67.15801);
   preFit_554->SetParLimits(2, 0, 10879.2);
   preFit_554->GetXaxis()->SetLabelSize(0.05);
   preFit_554->GetXaxis()->SetTitleSize(0.06);
   preFit_554->GetXaxis()->SetTitleOffset(1);
   preFit_554->GetYaxis()->SetLabelOffset(0.01);
   preFit_554->GetYaxis()->SetLabelSize(0.05);
   preFit_554->GetYaxis()->SetTitleSize(0.06);
   preFit_554->GetYaxis()->SetTitleOffset(1.3);
   preFit_554->Draw("SAME");
   
   TLegend *leg = new TLegend(0.15, 0.6, 0.48, 0.88, nullptr, "brNDC");
   leg->SetBorderSize(1);
   leg->SetTextFont(62);
   leg->SetTextSize(0.035);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(2);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *legentry = leg->AddEntry("h_copy_pad55","Pad 55 Data","l");
   legentry->SetLineColor(TColor::GetColor("#000099"));
   legentry->SetLineWidth(3);
   legentry->SetTextFont(62);
   legentry = leg->AddEntry("preFit_55","Pre-Fit","l");
   legentry->SetLineColor(TColor::GetColor("#cc0000"));
   legentry->SetLineWidth(4);
   legentry->SetTextFont(62);
   legentry = leg->AddEntry("NULL","#mu = 53325.4 #pm 72.5","");
   legentry->SetTextFont(62);
   legentry = leg->AddEntry("NULL","#sigma = 1167.7 #pm 67.2","");
   legentry->SetTextFont(62);
   legentry = leg->AddEntry("NULL","#chi^{2} / NDF = 1.66","");
   legentry->SetTextFont(62);
   leg->Draw();
   c_initial_fit->Modified();
   c_initial_fit->SetSelected(c_initial_fit);
}
