#ifdef __CLING__
#pragma cling optimize(0)
#endif
void presentation_Fit52()
{
//=========Macro generated from canvas: c_initial_fit/Initial Fit Pad 52
//=========  (Tue Sep  8 11:32:50 2026) by ROOT version 6.36.04
   TCanvas *c_initial_fit = new TCanvas("c_initial_fit", "Initial Fit Pad 52", 888, 210, 800, 600);
   gStyle->SetOptFit(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   TColor::SetPalette(57, nullptr);
   c_initial_fit->Range(22953,-17.6037,73003,108.137);
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
   
   TH1F *h_copy_pad52__1 = new TH1F("h_copy_pad52__1", "ERaw: Pad 52 (Mod 4 Chn 14)", 500, 0, 70000);
   std::vector<Double_t> h_copy_pad52__1_vect0{
      0, 90, 93, 397, 2346, 5544, 5975, 3699, 1525, 535,
      161, 78, 56, 46, 39, 46, 47, 37, 41, 43,
      46, 48, 47, 48, 36, 32, 36, 37, 36, 32,
      39, 50, 55, 33, 29, 38, 31, 31, 33, 39,
      25, 22, 27, 25, 27, 22, 10, 21, 23, 24,
      17, 13, 21, 17, 17, 16, 10, 13, 14, 11,
      11, 13, 12, 18, 10, 14, 12, 9, 12, 10,
      15, 11, 8, 7, 7, 17, 8, 4, 6, 3,
      6, 9, 10, 5, 4, 3, 2, 6, 7, 6,
      7, 7, 5, 6, 5, 7, 9, 9, 4, 5,
      3, 5, 8, 2, 5, 7, 3, 5, 2, 4,
      3, 5, 4, 3, 4, 3, 1, 5, 6, 4,
      2, 2, 2, 1, 3, 3, 3, 1, 2, 4,
      2, 4, 0, 1, 2, 0, 2, 5, 2, 2,
      0, 4, 2, 5, 1, 0, 0, 0, 1, 2,
      2, 3, 0, 0, 3, 1, 4, 2, 3, 2,
      1, 1, 2, 0, 4, 0, 1, 4, 2, 1,
      1, 2, 2, 0, 0, 2, 2, 1, 0, 1,
      3, 1, 3, 1, 0, 1, 0, 0, 0, 1,
      4, 3, 2, 1, 1, 1, 3, 2, 1, 1,
      0, 0, 0, 1, 0, 2, 1, 3, 0, 0,
      2, 0, 0, 1, 0, 0, 1, 3, 1, 1,
      1, 0, 0, 0, 0, 0, 2, 1, 1, 2,
      0, 0, 1, 1, 1, 3, 0, 0, 1, 0,
      1, 2, 0, 1, 1, 0, 1, 1, 0, 0,
      0, 0, 1, 0, 0, 0, 0, 1, 1, 2,
      0, 0, 1, 0, 1, 3, 0, 1, 1, 1,
      2, 2, 0, 1, 1, 1, 0, 2, 1, 3,
      1, 0, 1, 4, 0, 0, 2, 3, 0, 2,
      2, 2, 0, 3, 2, 0, 1, 1, 1, 3,
      2, 2, 1, 0, 2, 2, 0, 1, 0, 2,
      1, 2, 5, 2, 1, 4, 1, 3, 2, 4,
      1, 3, 0, 2, 5, 2, 4, 1, 3, 2,
      2, 1, 5, 5, 3, 3, 3, 1, 0, 4,
      3, 3, 6, 5, 4, 1, 2, 3, 3, 4,
      2, 2, 2, 1, 4, 4, 1, 4, 4, 4,
      1, 5, 2, 1, 7, 5, 3, 3, 4, 3,
      3, 1, 3, 2, 2, 4, 2, 2, 7, 5,
      6, 9, 2, 7, 6, 6, 6, 5, 7, 7,
      2, 2, 2, 6, 6, 2, 5, 7, 5, 4,
      6, 6, 2, 6, 10, 6, 6, 8, 7, 5,
      10, 11, 16, 9, 8, 10, 9, 14, 24, 15,
      21, 19, 23, 21, 24, 19, 27, 28, 31, 44,
      34, 46, 42, 59, 65, 61, 69, 97, 65, 92,
      91, 95, 92, 87, 83, 61, 67, 56, 54, 47,
      30, 27, 20, 24, 15, 12, 6, 3, 5, 5,
      3, 0, 1, 3, 0, 0, 0, 1, 0, 1,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 2, 0,
      0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
      0, 17
   };
   for (Int_t bin = 0; bin < 502; bin++)
      if (h_copy_pad52__1_vect0[bin])
         h_copy_pad52__1->SetBinContent(bin, h_copy_pad52__1_vect0[bin]);
   h_copy_pad52__1->SetEntries(24885);
   h_copy_pad52__1->SetDirectory(nullptr);
   
   TF1 *fit_521 = new TF1("fit_52", "pol1(0) + gaus(2)", 58685.3479741109,64184.70152415881, TF1::EAddToList::kNo);
   fit_521->SetBit(TF1::kNotDraw);
   fit_521->SetFillColor(19);
   fit_521->SetMarkerColor(1);
   fit_521->SetMarkerStyle(1);
   fit_521->SetMarkerSize(1);
   fit_521->SetLineColor(TColor::GetColor("#cc0000"));
   fit_521->SetLineStyle(1);
   fit_521->SetLineWidth(4);
   fit_521->SetChisquare(25.70506605193051);
   fit_521->SetNDF(34);
   fit_521->SetParameter(0, 241.9100656893979);
   fit_521->SetParError(0, 27.43766731567751);
   fit_521->SetParLimits(0, 0, 0);
   fit_521->SetParameter(1, -0.003768991148650399);
   fit_521->SetParError(1, 0.0004407119210538079);
   fit_521->SetParLimits(1, 0, 0);
   fit_521->SetParameter(2, 79.91458199506643);
   fit_521->SetParError(2, 3.076719891115873);
   fit_521->SetParLimits(2, 0, 0);
   fit_521->SetParameter(3, 61622.81494750228);
   fit_521->SetParError(3, 38.78219538022575);
   fit_521->SetParLimits(3, 0, 0);
   fit_521->SetParameter(4, 949.2566797717076);
   fit_521->SetParError(4, 48.20379752552473);
   fit_521->SetParLimits(4, 0, 0);
   fit_521->SetParent(h_copy_pad52__1);
   h_copy_pad52__1->GetListOfFunctions()->Add(fit_521,"");
   h_copy_pad52__1->SetFillColor(31);
   h_copy_pad52__1->SetFillStyle(0);
   h_copy_pad52__1->SetLineColor(TColor::GetColor("#000099"));
   h_copy_pad52__1->SetLineWidth(3);
   h_copy_pad52__1->GetXaxis()->SetTitle("Integral, a.u.");
   h_copy_pad52__1->GetXaxis()->SetRange(215, 500);
   h_copy_pad52__1->GetXaxis()->SetNdivisions(505);     // Redukcia čísel, aby sa neprekrývali
   
   // Font 63 = Arial/Helvetica Bold s veľkosťou v PIXELOCH
   h_copy_pad52__1->GetXaxis()->SetLabelFont(63);
   h_copy_pad52__1->GetXaxis()->SetLabelSize(28);       // Masívne čísla (26px)
   h_copy_pad52__1->GetXaxis()->SetTitleFont(63);
   h_copy_pad52__1->GetXaxis()->SetTitleSize(32);       // Masívny názov osi (30px)
   h_copy_pad52__1->GetXaxis()->SetTitleOffset(1.3);
   
   // ==========================================
   // --- OS Y: EXTRÉMNE VÝRAZNÝ A TUČNÝ ŠTÝL ---
   // ==========================================
   h_copy_pad52__1->GetYaxis()->SetTitle("Counts");
   h_copy_pad52__1->GetYaxis()->SetLabelFont(63);
   h_copy_pad52__1->GetYaxis()->SetLabelSize(28);       // Masívne čísla (26px)
   h_copy_pad52__1->GetYaxis()->SetTitleFont(63);
   h_copy_pad52__1->GetYaxis()->SetTitleSize(32);       // Masívny názov osi (30px)
   h_copy_pad52__1->GetYaxis()->SetTitleOffset(1.2);
   h_copy_pad52__1->Draw("HIST");
   c_initial_fit->Modified();
   c_initial_fit->SetSelected(c_initial_fit);
}
