#ifdef __CLING__
#pragma cling optimize(0)
#endif
void pad55_fitting()
{
//=========Macro generated from canvas: c_final_fit/Final Fit Pad 55
//=========  (Sat Sep  5 18:08:46 2026) by ROOT version 6.36.04
   TCanvas *c_final_fit = new TCanvas("c_final_fit", "Final Fit Pad 55", 840, 109, 800, 600);
   gStyle->SetOptFit(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   TColor::SetPalette(57, nullptr);
   c_final_fit->Range(22953,-5.988889,73003,36.78889);
   c_final_fit->SetFillColor(10);
   c_final_fit->SetBorderMode(0);
   c_final_fit->SetBorderSize(2);
   c_final_fit->SetTickx(1);
   c_final_fit->SetTicky(1);
   c_final_fit->SetLeftMargin(0.14);
   c_final_fit->SetRightMargin(0.06);
   c_final_fit->SetTopMargin(0.05);
   c_final_fit->SetBottomMargin(0.14);
   c_final_fit->SetFrameLineWidth(3);
   c_final_fit->SetFrameLineWidth(3);
   
   TH1F *h_copy_pad55__1 = new TH1F("h_copy_pad55__1", "ERaw: Pad 55 (Mod 2 Chn 13)", 500, 0, 70000);
   std::vector<Double_t> h_copy_pad55__1_vect0{
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
      if (h_copy_pad55__1_vect0[bin])
         h_copy_pad55__1->SetBinContent(bin, h_copy_pad55__1_vect0[bin]);
   h_copy_pad55__1->SetEntries(24884);
   h_copy_pad55__1->SetDirectory(nullptr);
   
   std::vector<Double_t> pmarker_vect1{ 53550 };
   std::vector<Double_t> pmarker_vect2{ 26 };
   TPolyMarker *pmarker = new TPolyMarker(1, pmarker_vect1.data(), pmarker_vect2.data(), "");
   pmarker->SetMarkerColor(TColor::GetColor("#ff0000"));
   pmarker->SetMarkerStyle(23);
   pmarker->SetMarkerSize(1.299999952316284);
   h_copy_pad55__1->GetListOfFunctions()->Add(pmarker,"");
   
   TF1 *fit_552 = new TF1("fit_55", "pol1(0) + gaus(2)", 50406.19380307863,56244.70133722561, TF1::EAddToList::kNo);
   fit_552->SetBit(TF1::kNotDraw);
   fit_552->SetFillColor(19);
   fit_552->SetMarkerColor(1);
   fit_552->SetMarkerStyle(1);
   fit_552->SetMarkerSize(1);
   fit_552->SetLineColor(TColor::GetColor("#cc0000"));
   fit_552->SetLineStyle(1);
   fit_552->SetLineWidth(4);
   fit_552->SetChisquare(44.76172408138238);
   fit_552->SetNDF(37);
   fit_552->SetParameter(0, 69.78884326713892);
   fit_552->SetParError(0, 5.614272351256166);
   fit_552->SetParLimits(0, 0, 0);
   fit_552->SetParameter(1, -0.001264145128466845);
   fit_552->SetParError(1, 0.0001037353325680475);
   fit_552->SetParLimits(1, 0, 0);
   fit_552->SetParameter(2, 20.94731094158586);
   fit_552->SetParError(2, 1.488449026483699);
   fit_552->SetParLimits(2, 0, 0);
   fit_552->SetParameter(3, 53635.16672026783);
   fit_552->SetParError(3, 70.60517346282249);
   fit_552->SetParLimits(3, 0, 0);
   fit_552->SetParameter(4, 1089.460874197526);
   fit_552->SetParError(4, 132.4134074246212);
   fit_552->SetParLimits(4, 0, 0);
   fit_552->SetParent(h_copy_pad55__1);
   h_copy_pad55__1->GetListOfFunctions()->Add(fit_552,"");
   h_copy_pad55__1->SetFillColor(31);
   h_copy_pad55__1->SetFillStyle(0);
   h_copy_pad55__1->SetLineColor(TColor::GetColor("#000099"));
   h_copy_pad55__1->SetLineWidth(3);
   h_copy_pad55__1->GetXaxis()->SetTitle("Integral, ADC channels");
   h_copy_pad55__1->GetXaxis()->SetRange(215, 500);
   h_copy_pad55__1->GetXaxis()->SetLabelFont(42);
   h_copy_pad55__1->GetXaxis()->SetLabelSize(0.03999999910593033);
   h_copy_pad55__1->GetXaxis()->SetTitleSize(0.05000000074505806);
   h_copy_pad55__1->GetXaxis()->SetTitleOffset(1.100000023841858);
   h_copy_pad55__1->GetXaxis()->SetTitleFont(42);
   h_copy_pad55__1->GetYaxis()->SetTitle("Counts");
   h_copy_pad55__1->GetYaxis()->SetLabelFont(42);
   h_copy_pad55__1->GetYaxis()->SetLabelSize(0.03999999910593033);
   h_copy_pad55__1->GetYaxis()->SetTitleSize(0.05000000074505806);
   h_copy_pad55__1->GetYaxis()->SetTitleOffset(1.299999952316284);
   h_copy_pad55__1->GetYaxis()->SetTitleFont(42);
   h_copy_pad55__1->GetZaxis()->SetLabelFont(42);
   h_copy_pad55__1->GetZaxis()->SetTitleOffset(1);
   h_copy_pad55__1->GetZaxis()->SetTitleFont(42);
   h_copy_pad55__1->Draw("HIST");
   
   TF1 *fit_552 = new TF1("fit_55", "pol1(0) + gaus(2)", 50406.19,56244.7, TF1::EAddToList::kDefault);
   fit_552->SetFillColor(19);
   fit_552->SetMarkerColor(1);
   fit_552->SetMarkerStyle(1);
   fit_552->SetMarkerSize(1);
   fit_552->SetLineColor(TColor::GetColor("#cc0000"));
   fit_552->SetLineStyle(1);
   fit_552->SetLineWidth(4);
   fit_552->SetChisquare(44.76172);
   fit_552->SetNDF(37);
   fit_552->SetParameter(0, 69.78884);
   fit_552->SetParError(0, 5.614272);
   fit_552->SetParLimits(0, 0, 0);
   fit_552->SetParameter(1, -0.001264145);
   fit_552->SetParError(1, 0.0001037353);
   fit_552->SetParLimits(1, 0, 0);
   fit_552->SetParameter(2, 20.94731);
   fit_552->SetParError(2, 1.488449);
   fit_552->SetParLimits(2, 0, 0);
   fit_552->SetParameter(3, 53635.17);
   fit_552->SetParError(3, 70.60517);
   fit_552->SetParLimits(3, 0, 0);
   fit_552->SetParameter(4, 1089.461);
   fit_552->SetParError(4, 132.4134);
   fit_552->SetParLimits(4, 0, 0);
   fit_552->GetXaxis()->SetLabelSize(0.05);
   fit_552->GetXaxis()->SetTitleSize(0.06);
   fit_552->GetXaxis()->SetTitleOffset(1);
   fit_552->GetYaxis()->SetLabelOffset(0.01);
   fit_552->GetYaxis()->SetLabelSize(0.05);
   fit_552->GetYaxis()->SetTitleSize(0.06);
   fit_552->GetYaxis()->SetTitleOffset(1.3);
   fit_552->Draw("SAME");
   
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
   legentry = leg->AddEntry("fit_55","Pol1 + Gauss Fit","l");
   legentry->SetLineColor(TColor::GetColor("#cc0000"));
   legentry->SetLineWidth(4);
   legentry->SetTextFont(62);
   legentry = leg->AddEntry("NULL","#mu = 53635.2 #pm 70.6","");
   legentry->SetTextFont(62);
   legentry = leg->AddEntry("NULL","#sigma = 1089.5 #pm 132.4","");
   legentry->SetTextFont(62);
   legentry = leg->AddEntry("NULL","#chi^{2} / NDF = 1.21","");
   legentry->SetTextFont(62);
   leg->Draw();
   c_final_fit->Modified();
   c_final_fit->SetSelected(c_final_fit);
}
