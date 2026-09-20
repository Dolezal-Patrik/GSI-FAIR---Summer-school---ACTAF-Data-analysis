#ifdef __CLING__
#pragma cling optimize(0)
#endif
void fft_frequency_dependencypad107_run4129()
{
//=========Macro generated from canvas: c_dep/Zavislost Frekvence na Peaku
//=========  (Fri Aug 28 13:31:06 2026) by ROOT version 6.36.04
   TCanvas *c_dep = new TCanvas("c_dep", "Zavislost Frekvence na Peaku", 632, 233, 800, 600);
   gStyle->SetOptFit(0);
   gStyle->SetOptStat(1111);
   gStyle->SetOptTitle(1);
   TColor::SetPalette(57, nullptr);
   c_dep->Range(-0.2500001,54.77091,7.25,963.7149);
   c_dep->SetFillColor(0);
   c_dep->SetBorderMode(0);
   c_dep->SetBorderSize(2);
   c_dep->SetGridx();
   c_dep->SetGridy();
   c_dep->SetFrameBorderMode(0);
   c_dep->SetFrameBorderMode(0);
   
   std::vector<Double_t> gre_fx_vect4{ 1, 2, 3, 4, 5, 6 };
   std::vector<Double_t> gre_fy_vect5{ 206.2716958521687, 285.0220806947023, 441.6711390091623, 596.887415129979, 698.0477946719959, 812.0808675544207 };
   std::vector<Double_t> gre_fex_vect6{ 0, 0, 0, 0, 0, 0 };
   std::vector<Double_t> gre_fey_vect7{ 0.01011154769381051, 0.01237708983228458, 0.02237726045672161, 0.05073662283416525, 0.2130499827039785, 0.1433985639943103 };
   TGraphErrors *gre = new TGraphErrors(6, gre_fx_vect4.data(), gre_fy_vect5.data(), gre_fex_vect6.data(), gre_fey_vect7.data());
   gre->SetName("Graph");
   gre->SetTitle("Zavislost frekvence peaku");
   gre->SetFillStyle(1000);
   gre->SetMarkerColor(TColor::GetColor("#0000cc"));
   gre->SetMarkerStyle(20);
   gre->SetMarkerSize(1.3);
   
   TH1F *Graph_histogram1 = new TH1F("Graph_histogram1", "Zavislost frekvence peaku", 100, 0.5, 6.5);
   Graph_histogram1->SetMinimum(145.6653161230809);
   Graph_histogram1->SetMaximum(872.820534299809);
   Graph_histogram1->SetDirectory(nullptr);
   Graph_histogram1->SetStats(0);
   Graph_histogram1->SetLineColor(TColor::GetColor("#000099"));
   Graph_histogram1->GetXaxis()->SetTitle("Cislo peaku [#]");
   Graph_histogram1->GetXaxis()->SetLabelFont(42);
   Graph_histogram1->GetXaxis()->SetTitleOffset(1);
   Graph_histogram1->GetXaxis()->SetTitleFont(42);
   Graph_histogram1->GetYaxis()->SetTitle("Frekvence [kHz]");
   Graph_histogram1->GetYaxis()->SetLabelFont(42);
   Graph_histogram1->GetYaxis()->SetTitleFont(42);
   Graph_histogram1->GetZaxis()->SetLabelFont(42);
   Graph_histogram1->GetZaxis()->SetTitleOffset(1);
   Graph_histogram1->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_histogram1);
   
   
   TF1 *linFit8 = new TF1("linFit", "pol1", 0.5,6.5, TF1::EAddToList::kNo);
   linFit8->SetFillColor(19);
   linFit8->SetMarkerColor(1);
   linFit8->SetMarkerStyle(1);
   linFit8->SetMarkerSize(1);
   linFit8->SetLineColor(TColor::GetColor("#ff0000"));
   linFit8->SetLineStyle(2);
   linFit8->SetLineWidth(2);
   linFit8->SetChisquare(7220025);
   linFit8->SetNDF(4);
   linFit8->SetParameter(0, 83.01931);
   linFit8->SetParError(0, 0.01672648);
   linFit8->SetParLimits(0, 0, 0);
   linFit8->SetParameter(1, 113.3408);
   linFit8->SetParError(1, 0.009173683);
   linFit8->SetParLimits(1, 0, 0);
   linFit8->GetXaxis()->SetLabelFont(42);
   linFit8->GetXaxis()->SetTitleOffset(1);
   linFit8->GetXaxis()->SetTitleFont(42);
   linFit8->GetYaxis()->SetLabelFont(42);
   linFit8->GetYaxis()->SetTitleFont(42);
   linFit8->SetParent(gre);
   gre->GetListOfFunctions()->Add(linFit8,"");
   gre->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.264436, 0.934373, 0.735564, 0.995, "blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *pt_text5 = pt->AddText("Zavislost frekvence peaku");
   pt->Draw("blNDC");
   c_dep->Modified();
   c_dep->SetSelected(c_dep);
}
