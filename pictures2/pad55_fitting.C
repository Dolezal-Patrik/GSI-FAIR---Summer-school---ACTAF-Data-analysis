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
   
   // --- VÝRAZNÉ OKRAJE KANVASU PRE VEĽKÉ FONTY ---
   c_final_fit->SetLeftMargin(0.18);
   c_final_fit->SetRightMargin(0.05);
   c_final_fit->SetTopMargin(0.05);
   c_final_fit->SetBottomMargin(0.18);
   
   // HRUBÝ RÁMČEK A HLBOKÉ TICKS AKO NA FIG. 5
   c_final_fit->SetFrameLineWidth(3);
   c_final_fit->SetTickx(1);
   c_final_fit->SetTicky(1);
   
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
   
   TF1 *fit_552 = new TF1("fit_55", "pol1(0) + gaus(2)", 50406.19, 56244.7, TF1::EAddToList::kDefault);
   fit_552->SetLineColor(TColor::GetColor("#cc0000"));
   fit_552->SetLineWidth(4);
   fit_552->SetParameter(0, 69.78884);
   fit_552->SetParameter(1, -0.001264145);
   fit_552->SetParameter(2, 20.94731);
   fit_552->SetParameter(3, 53635.17);
   fit_552->SetParameter(4, 1089.461);
   
   h_copy_pad55__1->SetLineColor(TColor::GetColor("#000099"));
   h_copy_pad55__1->SetLineWidth(3);
   
   // ==========================================
   // --- OS X: EXTRÉMNE VÝRAZNÝ A TUČNÝ ŠTÝL ---
   // ==========================================
   h_copy_pad55__1->GetXaxis()->SetTitle("E_{raw}, a.u.");
   h_copy_pad55__1->GetXaxis()->SetRange(215, 500);
   h_copy_pad55__1->GetXaxis()->SetNdivisions(505);     // Redukcia čísel, aby sa neprekrývali
   
   // Font 63 = Arial/Helvetica Bold s veľkosťou v PIXELOCH
   h_copy_pad55__1->GetXaxis()->SetLabelFont(63);
   h_copy_pad55__1->GetXaxis()->SetLabelSize(28);       // Masívne čísla (26px)
   h_copy_pad55__1->GetXaxis()->SetTitleFont(63);
   h_copy_pad55__1->GetXaxis()->SetTitleSize(32);       // Masívny názov osi (30px)
   h_copy_pad55__1->GetXaxis()->SetTitleOffset(1.3);
   
   // ==========================================
   // --- OS Y: EXTRÉMNE VÝRAZNÝ A TUČNÝ ŠTÝL ---
   // ==========================================
   h_copy_pad55__1->GetYaxis()->SetTitle("Counts");
   h_copy_pad55__1->GetYaxis()->SetLabelFont(63);
   h_copy_pad55__1->GetYaxis()->SetLabelSize(28);       // Masívne čísla (26px)
   h_copy_pad55__1->GetYaxis()->SetTitleFont(63);
   h_copy_pad55__1->GetYaxis()->SetTitleSize(32);       // Masívny názov osi (30px)
   h_copy_pad55__1->GetYaxis()->SetTitleOffset(1.2);
   
   h_copy_pad55__1->Draw("HIST");
   fit_552->Draw("SAME");
   
   // ==========================================
   // --- LEGENDA (KOMPAKTNÁ A MERNÁ) ---
   // ==========================================
   TLegend *leg = new TLegend(0.20, 0.52, 0.58, 0.88, nullptr, "brNDC");
   leg->SetBorderSize(2);
   leg->SetTextFont(63);
   leg->SetTextSize(20);                                 // Veľkosť textu v px
   leg->SetLineColor(1);
   leg->SetFillColor(0);
   
   TLegendEntry *legentry = leg->AddEntry(h_copy_pad55__1, "Pad 55 Data", "l");
   legentry->SetLineColor(TColor::GetColor("#000099"));
   legentry->SetLineWidth(3);
   
   legentry = leg->AddEntry(fit_552, "Pol1 + Gauss Fit", "l");
   legentry->SetLineColor(TColor::GetColor("#cc0000"));
   legentry->SetLineWidth(4);
   
   leg->AddEntry("NULL", "#mu = 53635.2 #pm 70.6", "");
   leg->AddEntry("NULL", "#sigma = 1089.5 #pm 132.4", "");
   leg->AddEntry("NULL", "#chi^{2} / NDF = 1.21", "");
   
   leg->Draw();
   
   c_final_fit->Modified();
   c_final_fit->Update();
}
