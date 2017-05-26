////////////////////////////////////////////////////////////////////////////////
//
// Plot components of c_{2}{4}
//
////////////////////////////////////////////////////////////////////////////////
//
// Darren McGlinchey
// 18 Aug 2017
//
////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <numeric>
#include <iterator>
#include <cmath>
#include <TLegend.h>
#include <TLatex.h>
#include <TGraphErrors.h>

#include "TLorentzVector.h"
#include "TFile.h"
#include "TStyle.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TF1.h"

using namespace std;

void plot_components()
{
  gStyle->SetOptStat(0);

  float v2_max = 0.1;
  int x_max = 140;   // PbPb: maximum 6000     pPb: 300
  // int bin_size = 50; // PbPb: maximum 100      pPb: 50
  int rebin = 4;

  const int NRANGE = 3;
  int rlo[] = {10, 40,  90};
  int rhi[] = {20, 50, 100};
  int rColor[] = {kBlack, kRed, kBlue};

  // const char* fname = "rootfiles/cumulants_ampt_dau200_b20_sigparton150_ptfilter_50M_set5.root";
  // const char* lname = "AMPT -- d+Au 200 -- 50M b<20 fm -- #sigma_{parton}=1.50 mb -- p_{T} filter";
  // const char* pname = "ampt_dau200_b20_sigparton150_ptfilter_50M_rebin";

  const char* fname = "rootfiles/cumulants_ampt_ppb5020_b02_sigparton150_ptfilter_10M_set1.root";
  const char* lname = "AMPT -- p+Pb 5020 -- 10M b<02 fm -- #sigma_{parton}=1.50 mb -- p_{T} filter";
  const char* pname = "ampt_ppb5020_b02_sigparton150_ptfilter_10M_rebin";

  TFile *file = TFile::Open(fname);

  TProfile* tp1f_raa6 = (TProfile*) file->Get("raa6_Ncharge");
  TProfile* tp1f_raa4 = (TProfile*) file->Get("raa4_Ncharge");
  TProfile* tp1f_raa2 = (TProfile*) file->Get("raa2_Ncharge");

  tp1f_raa6->SetLineColor(kRed);
  tp1f_raa4->SetLineColor(kRed);
  tp1f_raa2->SetLineColor(kRed);

  tp1f_raa6->Rebin(rebin);
  tp1f_raa4->Rebin(rebin);
  tp1f_raa2->Rebin(rebin);

  TH2D* th2d_raa6 = (TH2D*) file->Get("th2d_raa6_Ncharge");
  TH2D* th2d_raa4 = (TH2D*) file->Get("th2d_raa4_Ncharge");
  TH2D* th2d_raa2 = (TH2D*) file->Get("th2d_raa2_Ncharge");

  // get distributions for given track range
  TH1D* th1d_raa6_range[NRANGE];
  TH1D* th1d_raa4_range[NRANGE];
  TH1D* th1d_raa2_range[NRANGE];
  for (int i = 0; i < NRANGE; i++)
  {
    int bl = th2d_raa6->GetXaxis()->FindBin(rlo[i]);
    int bh = th2d_raa6->GetXaxis()->FindBin(rhi[i]);

    th1d_raa6_range[i] =
      (TH1D*) th2d_raa6->ProjectionY(Form("th2d_raa6_range_%i", i), bl, bh);
    th1d_raa6_range[i]->SetLineColor(rColor[i]);
    th1d_raa6_range[i]->Scale(1./th1d_raa6_range[i]->Integral());

    th1d_raa4_range[i] =
      (TH1D*) th2d_raa4->ProjectionY(Form("th2d_raa4_range_%i", i), bl, bh);
    th1d_raa4_range[i]->SetLineColor(rColor[i]);
    th1d_raa4_range[i]->Scale(1./th1d_raa4_range[i]->Integral());

    th1d_raa2_range[i] =
      (TH1D*) th2d_raa2->ProjectionY(Form("th2d_raa2_range_%i", i), bl, bh);
    th1d_raa2_range[i]->SetLineColor(rColor[i]);
    th1d_raa2_range[i]->Scale(1./th1d_raa2_range[i]->Integral());
  } // i


  TLatex ltitle;
  ltitle.SetNDC();
  ltitle.SetTextAlign(22);

  // plot <<4>>
  TCanvas *c4 = new TCanvas("c4", "<<4>>", 800, 600);

  th2d_raa4->SetTitle(";N_{charge};#LT#LT4#GT#GT");
  th2d_raa4->GetXaxis()->SetRangeUser(0, x_max);
  th2d_raa4->Draw("colz");
  tp1f_raa4->Draw("same");

  ltitle.DrawLatex(0.5, 0.95, lname);


  // plot <<2>>
  TCanvas *c2 = new TCanvas("c2", "<<2>>", 800, 600);

  th2d_raa2->SetTitle(";N_{charge};#LT#LT2#GT#GT");
  th2d_raa2->GetXaxis()->SetRangeUser(0, x_max);
  th2d_raa2->Draw("colz");
  tp1f_raa2->Draw("same");

  ltitle.DrawLatex(0.5, 0.95, lname);

  // slices
  TCanvas *crange = new TCanvas("crange", "range", 800, 600);
  gPad->SetLogy();
  th1d_raa4_range[0]->SetTitle(";#LT#LT4#GT#GT;");
  th1d_raa4_range[0]->GetYaxis()->SetRangeUser(1e-5, 1);
  th1d_raa4_range[0]->Draw();
  for (int i = 1; i < NRANGE; i++)
    th1d_raa4_range[i]->Draw("same");

  TLegend *legrange = new TLegend(0.15, 0.7, 0.40, 0.88);
  legrange->SetFillStyle(0);
  legrange->SetBorderSize(0);
  for (int i = 0; i < NRANGE; i++)
    legrange->AddEntry(th1d_raa4_range[i], Form("%i < N_{charge} < %i", rlo[i], rhi[i]), "L");
  legrange->Draw("same");

  ltitle.DrawLatex(0.5, 0.95, lname);


  // print
  c4->Print(Form("pdfs/four_%s.pdf", pname));
  crange->Print(Form("pdfs/four_slices_%s.pdf", pname));

}