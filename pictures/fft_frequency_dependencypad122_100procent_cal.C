#ifdef __CLING__
#pragma cling optimize(0)
#endif
void fft_frequency_dependencypad122_100procent_cal()
{
//=========Macro generated from canvas: c_dep/Zavislost Frekvence na Peaku
//=========  (Fri Sep 11 10:19:26 2026) by ROOT version 6.36.04
   TCanvas *c_dep = new TCanvas("c_dep", "Zavislost Frekvence na Peaku", 307, 50, 800, 600);
   gStyle->SetOptFit(0);
   gStyle->SetOptStat(1111);
   gStyle->SetOptTitle(1);
   TColor::SetPalette(57, nullptr);
   c_dep->Range(0.25,80.44737,4.75,822.3919);
   c_dep->SetFillColor(0);
   c_dep->SetBorderMode(0);
   c_dep->SetBorderSize(2);
   c_dep->SetGridx();
   c_dep->SetGridy();
   c_dep->SetFrameBorderMode(0);
   c_dep->SetFrameBorderMode(0);
   
   std::vector<Double_t> gre_fx_vect4{ 1, 2, 3, 4 };
   std::vector<Double_t> gre_fy_vect5{ 206.3495304951698, 305.4472239496127, 608.3838487993764, 696.4873463910948 };
   std::vector<Double_t> gre_fex_vect6{ 0, 0, 0, 0 };
   std::vector<Double_t> gre_fey_vect7{ 2.244740119706296, 2.244751401617472, 2.246165133707924, 2.247109470349504 };
   TGraphErrors *gre = new TGraphErrors(4, gre_fx_vect4.data(), gre_fy_vect5.data(), gre_fex_vect6.data(), gre_fey_vect7.data());
   gre->SetName("Graph");
   gre->SetTitle("Zavislost frekvence peaku");
   gre->SetFillStyle(1000);
   gre->SetMarkerColor(TColor::GetColor("#0000cc"));
   gre->SetMarkerStyle(20);
   gre->SetMarkerSize(1.3);
   
   TH1F *Graph_histogram1 = new TH1F("Graph_histogram1", "Zavislost frekvence peaku", 100, 0.7, 4.3);
   Graph_histogram1->SetMinimum(154.6418238268654);
   Graph_histogram1->SetMaximum(748.1974224100423);
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
   
   
   TF1 *linFit6 = new TF1("linFit", "pol1", 0.5,4.5, TF1::EAddToList::kNo);
   linFit6->SetFillColor(19);
   linFit6->SetMarkerColor(1);
   linFit6->SetMarkerStyle(1);
   linFit6->SetMarkerSize(1);
   linFit6->SetLineColor(TColor::GetColor("#ff0000"));
   linFit6->SetLineStyle(2);
   linFit6->SetLineWidth(2);
   linFit6->SetChisquare(1744.019);
   linFit6->SetNDF(2);
   linFit6->SetParameter(0, 10.80481);
   linFit6->SetParError(0, 2.74972);
   linFit6->SetParLimits(0, 0, 0);
   linFit6->SetParameter(1, 177.3416);
   linFit6->SetParError(1, 1.004387);
   linFit6->SetParLimits(1, 0, 0);
   linFit6->GetXaxis()->SetLabelFont(42);
   linFit6->GetXaxis()->SetTitleOffset(1);
   linFit6->GetXaxis()->SetTitleFont(42);
   linFit6->GetYaxis()->SetLabelFont(42);
   linFit6->GetYaxis()->SetTitleFont(42);
   linFit6->SetParent(gre);
   gre->GetListOfFunctions()->Add(linFit6,"");
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
