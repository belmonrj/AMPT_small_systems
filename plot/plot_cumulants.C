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

void plot_cumulants()
{
  gStyle->SetOptStat(0);

  float v2_max = 0.1;
  int x_max = 140;   // PbPb: maximum 6000     pPb: 300
  int bin_size = 50; // PbPb: maximum 100      pPb: 50

  // const char* fname = "rootfiles/cumulants_ampt_dau200_b02_sigparton075_ptfilter_50M_set0.root";
  // const char* lname = "AMPT -- d+Au 200 -- b<2 fm -- #sigma_{parton}=0.75 mb -- p_{T} filter";
  // const char* pname = "ampt_dau200_b02_sigparton075_ptfilter";

  // const char* fname = "rootfiles/cumulants_ampt_dau200_b02_sigparton075_nofilter_50M_set1.root";
  // const char* lname = "AMPT -- d+Au 200 -- b<2 fm -- #sigma_{parton}=0.75 mb -- no filter";
  // const char* pname = "ampt_dau200_b02_sigparton075_nofilter";

  // const char* fname = "rootfiles/cumulants_ampt_dau200_b02_sigparton150_nofilter_50M_set2.root";
  // const char* lname = "AMPT -- d+Au 200 -- b<2 fm -- #sigma_{parton}=1.50 mb -- no filter";
  // const char* pname = "ampt_dau200_b02_sigparton150_nofilter";

  // const char* fname = "rootfiles/cumulants_ampt_dau200_b02_sigparton150_ptfilter_50M_set3.root";
  // const char* lname = "AMPT -- d+Au 200 -- b<2 fm -- #sigma_{parton}=1.50 mb -- p_{T} filter";
  // const char* pname = "ampt_dau200_b02_sigparton150_ptfilter";

  // const char* fname = "rootfiles/cumulants_ampt_dau200_b20_sigparton150_ptfilter_50M_set4.root";
  // const char* lname = "AMPT -- d+Au 200 -- b<20 fm -- #sigma_{parton}=1.50 mb -- p_{T} filter";
  // const char* pname = "ampt_dau200_b20_sigparton150_ptfilter";

  const char* fname = "rootfiles/cumulants_ampt_pau200_b02_sigparton150_ptfilter_50M_set0.root";
  const char* lname = "AMPT -- p+Au 200 -- b<2 fm -- #sigma_{parton}=1.50 mb -- p_{T} filter";
  const char* pname = "ampt_pau200_b02_sigparton150_ptfilter";

  TFile *file = TFile::Open(fname); 

  TProfile* tp1f_raa6 = (TProfile*)file->Get("raa6_Ncharge");
  TProfile* tp1f_raa4 = (TProfile*)file->Get("raa4_Ncharge");
  TProfile* tp1f_raa2 = (TProfile*)file->Get("raa2_Ncharge");
  TProfile* tp1f_daa4 = (TProfile*)file->Get("daa4_Ncharge");
  TProfile* tp1f_daa2 = (TProfile*)file->Get("daa2_Ncharge");
  TProfile* tp1f_gapp = (TProfile*)file->Get("comp_Ncharge");


  TH1D* th1d_raa6 = tp1f_raa6->ProjectionX("th1d_raa6");
  TH1D* th1d_raa4 = tp1f_raa4->ProjectionX("th1d_raa4");
  TH1D* th1d_raa2 = tp1f_raa2->ProjectionX("th1d_raa2");
  TH1D* th1d_daa4 = tp1f_daa4->ProjectionX("th1d_daa4");
  TH1D* th1d_daa2 = tp1f_daa2->ProjectionX("th1d_daa2");
  TH1D* th1d_gapp = tp1f_gapp->ProjectionX("th1d_gapp");

  // Calculate c_{2}{4}
  TH1D* th1d_c2 = (TH1D*)th1d_raa2->Clone("th1d_c2");
  th1d_c2->Multiply(th1d_raa2);
  th1d_c2->Scale(2.0);

  TH1D* th1d_c24 = (TH1D*)th1d_raa4->Clone("th1d_c24");
  th1d_c24->Add(th1d_c2, -1.0);

  TH1D* th1d_cv24 = (TH1D*)th1d_c24->Clone("th1d_cv24");

  for (int i = 1; i < bin_size + 1; i++)
  {
    double temp_1 = th1d_cv24->GetBinContent(i);
    double erro_1 = th1d_cv24->GetBinError(i);

    if (temp_1 >= 0) th1d_cv24->SetBinContent(i, -0.09);
    else
    {
      double temp_2 = pow(-temp_1, 0.25);
      double erro_2 = 0.25 * erro_1 * abs(temp_2) / abs(temp_1);
      th1d_cv24->SetBinContent(i, temp_2);
      th1d_cv24->SetBinError(i, erro_2);
    }
  }

  //------------------------------------------------------------------
  // v_{2}{2} with gap
  TH1D* th1d_cv22 = (TH1D*)th1d_gapp->Clone("th1d_cv22");
  for (int i = 1; i < bin_size + 1; i++)
  {
    double temp_1 = th1d_cv22->GetBinContent(i);
    double erro_1 = th1d_cv22->GetBinError(i);

    if (temp_1 <= 0) th1d_cv22->SetBinContent(i, -9999);
    else
    {
      double temp_2 = pow(temp_1, 0.5);
      double erro_2 = 0.5 * erro_1 * abs(temp_2) / abs(temp_1);
      th1d_cv22->SetBinContent(i, temp_2);
      th1d_cv22->SetBinError(i, erro_2);
    }
  }

  TH1D* th1d_cv22_nogap = (TH1D*)th1d_raa2->Clone("th1d_cv22_nogap");
  for (int i = 1; i < bin_size + 1; i++)
  {
    double temp_1 = th1d_cv22_nogap->GetBinContent(i);
    double erro_1 = th1d_cv22_nogap->GetBinError(i);

    if (temp_1 <= 0) th1d_cv22_nogap->SetBinContent(i, -9999);
    else
    {
      double temp_2 = pow(temp_1, 0.5);
      double erro_2 = 0.5 * erro_1 * abs(temp_2) / abs(temp_1);
      th1d_cv22_nogap->SetBinContent(i, temp_2);
      th1d_cv22_nogap->SetBinError(i, erro_2);
    }
  }

  // ------------------------------------------------------------------------------
  TH1D* th1d_v2_mid = (TH1D*)th1d_c24->Clone("th1d_v2_mid");
  TH1D* th1d_v2_sigma = (TH1D*)th1d_c24->Clone("th1d_v2_sigma");
  for (int i = 1; i < bin_size + 1; i++)
  {
    double temp_1 = th1d_cv24->GetBinContent(i);
    double temp_2 = th1d_cv22->GetBinContent(i);

    if (temp_1 <= 0)
    {
      th1d_v2_mid->SetBinContent(i, -9999);
      th1d_v2_sigma->SetBinContent(i, -9999);
    }
    else
    {
      double v2_mid = (temp_1 * temp_1 + temp_2 * temp_2) / 2;
      double v2_sigma = (temp_2 * temp_2 - temp_1 * temp_1) / 2;
      v2_mid = TMath::Sqrt(v2_mid);
      if (v2_sigma > 0) v2_sigma = TMath::Sqrt(v2_sigma) / v2_mid;
      else v2_sigma = -9999;
      th1d_v2_mid->SetBinContent(i, v2_mid);
      th1d_v2_sigma->SetBinContent(i, v2_sigma);
    }
  }



  // -----------------------------------------------------------------------------------
  TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
  // c1->Divide(1, 2, 0, 0);
  c1->SetMargin(0.12, 0.02, 0.12, 0.02);
  c1->SetTicks(1,1);

  c1->cd();

  gPad->SetGridx();
  gPad->SetGridy();

  TH1D *axis1 = new TH1D("axis1", "0", 100, 0, x_max);

  axis1->SetTitle("");
  axis1->GetXaxis()->SetTitle("N_char");
  axis1->GetYaxis()->SetTitle("v_{2}");
  axis1->GetYaxis()->SetRangeUser(0, v2_max);
  axis1->GetXaxis()->SetLabelFont(62);
  axis1->GetXaxis()->SetTitleFont(62);
  axis1->GetYaxis()->SetLabelFont(62);
  axis1->GetYaxis()->SetTitleFont(62);
  axis1->GetYaxis()->SetLabelSize(0.03);
  axis1->GetYaxis()->SetTitleSize(0.03);
  axis1->GetXaxis()->SetLabelSize(0.03);
  axis1->GetXaxis()->SetTitleSize(0.03);
  axis1->Draw();

  //-------------------------------------------

  // ppplane->SetMarkerStyle(33);
  // ppplane->SetMarkerColor(1);
  // ppplane->SetMarkerSize(1.5);

  th1d_v2_mid->SetMarkerStyle(27);
  th1d_v2_mid->SetMarkerColor(1);
  th1d_v2_mid->SetMarkerSize(1.5);

  th1d_cv24->SetMarkerStyle(25);
  th1d_cv24->SetMarkerColor(kBlue);
  th1d_cv24->SetLineColor(kBlue);

  th1d_cv22->SetMarkerStyle(24);
  th1d_cv22->SetMarkerColor(kRed);
  th1d_cv22->SetLineColor(kRed);

  th1d_cv22_nogap->SetMarkerStyle(20);
  th1d_cv22_nogap->SetMarkerColor(kRed);
  th1d_cv22_nogap->SetLineColor(kRed);

  th1d_cv22->Draw("p,same");
  th1d_cv24->Draw("p,same");
  th1d_cv22_nogap->Draw("p,same");

  TLegend *leg1 = new TLegend(0.2, 0.7, 0.8, 0.96);
  leg1->SetFillStyle(0);
  leg1->SetBorderSize(0);
  leg1->SetTextFont(62);
  leg1->SetTextSize(0.04);
  leg1->SetHeader(lname);
  leg1->AddEntry(th1d_cv24, "v_{2}{4}", "p");
  leg1->AddEntry(th1d_cv22_nogap, "v_{2}{2}", "p");
  leg1->AddEntry(th1d_cv22, "v_{2}{2, |#Delta#eta|>2}", "p");
  leg1->Draw("same");


  c1->Print(Form("pdfs/cumulants_%s.pdf", pname));


  // TFile *fout = new TFile("dAu_cumulant_200GeV.root", "RECREATE");

  // th1d_cv22->SetName("th1d_AMPTdAu200_v22gap");
  // th1d_cv22_nogap->SetName("th1d_AMPTdAu200_v22");
  // th1d_cv24->SetName("th1d_AMPTdAu200_v24");
  // th1d_raa4->SetName("th1d_AMPTdAu200_four");
  // th1d_c2->SetName("th1d_AMPTdAu200_222");

  // th1d_cv22->SetTitle("th1d_AMPTdAu200_v22gap");
  // th1d_cv22_nogap->SetTitle("th1d_AMPTdAu200_v22");
  // th1d_cv24->SetTitle("th1d_AMPTdAu200_v24");
  // th1d_raa4->SetTitle("th1d_AMPTdAu200_four");
  // th1d_c2->SetTitle("th1d_AMPTdAu200_222");

  // th1d_cv22->Write();
  // th1d_cv22_nogap->Write();
  // th1d_cv24->Write();
  // th1d_raa4->Write();
  // th1d_c2->Write();

  // fout->Close();


}





































