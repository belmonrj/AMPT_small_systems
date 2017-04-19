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
  // int x_max = 140;   // PbPb: maximum 6000     pPb: 300
  int x_max = 400;   // PbPb: maximum 6000     pPb: 300
  // int bin_size = 50; // PbPb: maximum 100      pPb: 50
  int rebin = 10;

  // const char* fname = "rootfiles/cumulants_ampt_dau200_b02_sigparton075_ptfilter_50M_set0.root";
  // const char* lname = "AMPT -- d+Au 200 -- 50M b<2 fm -- #sigma_{parton}=0.75 mb -- p_{T} filter";
  // const char* pname = "ampt_dau200_b02_sigparton075_ptfilter";

  // const char* fname = "rootfiles/cumulants_ampt_dau200_b02_sigparton075_nofilter_50M_set1.root";
  // const char* lname = "AMPT -- d+Au 200 -- 50M b<2 fm -- #sigma_{parton}=0.75 mb -- no filter";
  // const char* pname = "ampt_dau200_b02_sigparton075_nofilter";

  // const char* fname = "rootfiles/cumulants_ampt_dau200_b02_sigparton150_nofilter_50M_set2.root";
  // const char* lname = "AMPT -- d+Au 200 -- 50M b<2 fm -- #sigma_{parton}=1.50 mb -- no filter";
  // const char* pname = "ampt_dau200_b02_sigparton150_nofilter";

  // const char* fname = "rootfiles/cumulants_ampt_dau200_b02_sigparton150_ptfilter_50M_set3.root";
  // const char* lname = "AMPT -- d+Au 200 -- 50M b<2 fm -- #sigma_{parton}=1.50 mb -- p_{T} filter";
  // const char* pname = "ampt_dau200_b02_sigparton150_ptfilter";

  // const char* fname = "rootfiles/cumulants_ampt_dau200_b20_sigparton150_ptfilter_50M_set4.root";
  // const char* lname = "AMPT -- d+Au 200 -- 50M b<20 fm -- #sigma_{parton}=1.50 mb -- p_{T} filter";
  // const char* pname = "ampt_dau200_b20_sigparton150_ptfilter";

  // const char* fname = "rootfiles/cumulants_ampt_pau200_b02_sigparton150_ptfilter_50M_set0.root";
  // const char* lname = "AMPT -- p+Au 200 -- 50M b<2 fm -- #sigma_{parton}=1.50 mb -- p_{T} filter";
  // const char* pname = "ampt_pau200_b02_sigparton150_ptfilter";

  // const char* fname = "rootfiles/cumulants_ampt_dau200_b20_sigparton150_ptfilter_5M_set5.root";
  // const char* lname = "AMPT -- d+Au 200 -- 5M b<20 fm -- #sigma_{parton}=1.50 mb -- p_{T} filter";
  // const char* pname = "ampt_dau200_b20_sigparton150_ptfilter_5M";

  // const char* fname = "rootfiles/cumulants_ampt_dau200_b20_sigparton150_ptfilter_50M_set5.root";
  // const char* lname = "AMPT -- d+Au 200 -- 50M b<20 fm -- #sigma_{parton}=1.50 mb -- p_{T} filter";
  // const char* pname = "ampt_dau200_b20_sigparton150_ptfilter_50M_rebin";

  const char* fname = "rootfiles/cumulants_ampt_auau200_0_0.root";
  const char* lname = "AMPT -- Au+Au 200 -- 1M b<20 fm -- #sigma_{parton}=1.50 mb -- p_{T} filter";
  const char* pname = "ampt_auau200_b20_sigparton150_ptfilter_1M_rebin";

  TFile *file = TFile::Open(fname);

  TProfile* tp1f_raa6 = (TProfile*) file->Get("raa6_Ncharge");
  TProfile* tp1f_raa4 = (TProfile*) file->Get("raa4_Ncharge");
  TProfile* tp1f_raa2 = (TProfile*) file->Get("raa2_Ncharge");
  TProfile* tp1f_gapp = (TProfile*) file->Get("comp_Ncharge");

  tp1f_raa6->Rebin(rebin);
  tp1f_raa4->Rebin(rebin);
  tp1f_raa2->Rebin(rebin);
  tp1f_gapp->Rebin(rebin);


  TH1D* th1d_raa6 = (TH1D*) tp1f_raa6->ProjectionX("th1d_raa6");
  TH1D* th1d_raa4 = (TH1D*) tp1f_raa4->ProjectionX("th1d_raa4");
  TH1D* th1d_raa2 = (TH1D*) tp1f_raa2->ProjectionX("th1d_raa2");
  TH1D* th1d_gapp = (TH1D*) tp1f_gapp->ProjectionX("th1d_gapp");

  //----------------------------------------------------------------------------
  // v_{2}{4}

  // calculate 2<<2>>^2
  TH1D* th1d_222 = (TH1D*)th1d_raa2->Clone("th1d_222");
  th1d_222->Multiply(th1d_raa2);
  th1d_222->Scale(2.0);

  // calculate c_{2}{4} = <<4>> - 2<<2>>^2
  TH1D* th1d_c24 = (TH1D*)th1d_raa4->Clone("th1d_c24");
  th1d_c24->Add(th1d_222, -1.0);

  // calculate v_{2}{4} = (c_{2}{4})^{1/4}
  TH1D* th1d_v24 = (TH1D*)th1d_c24->Clone("th1d_v24");
  for (int i = 1; i <= th1d_v24->GetNbinsX(); i++)
  {
    double temp_1 = th1d_v24->GetBinContent(i);
    double erro_1 = th1d_v24->GetBinError(i);

    if (temp_1 >= 0)
    {
      th1d_v24->SetBinContent(i, 0);
      th1d_v24->SetBinError(i, 0);
    }
    else
    {
      double temp_2 = pow(-temp_1, 0.25);
      double erro_2 = 0.25 * erro_1 * abs(temp_2) / abs(temp_1);
      th1d_v24->SetBinContent(i, temp_2);
      th1d_v24->SetBinError(i, erro_2);
    }
  }

  //----------------------------------------------------------------------------
  // v_{2}{2}
  TH1D* th1d_v22 = (TH1D*)th1d_raa2->Clone("th1d_v22");
  for (int i = 1; i <= th1d_v22->GetNbinsX(); i++)
  {
    double temp_1 = th1d_v22->GetBinContent(i);
    double erro_1 = th1d_v22->GetBinError(i);

    if (temp_1 <= 0) th1d_v22->SetBinContent(i, -9999);
    else
    {
      double temp_2 = pow(temp_1, 0.5);
      double erro_2 = 0.5 * erro_1 * abs(temp_2) / abs(temp_1);
      th1d_v22->SetBinContent(i, temp_2);
      th1d_v22->SetBinError(i, erro_2);
    }
  }

  //----------------------------------------------------------------------------
  // v_{2}{2, |deta|>2}
  TH1D* th1d_v22gap = (TH1D*)th1d_gapp->Clone("th1d_v22gap");
  for (int i = 1; i <= th1d_v22gap->GetNbinsX(); i++)
  {
    double temp_1 = th1d_v22gap->GetBinContent(i);
    double erro_1 = th1d_v22gap->GetBinError(i);

    if (temp_1 <= 0) th1d_v22gap->SetBinContent(i, -9999);
    else
    {
      double temp_2 = pow(temp_1, 0.5);
      double erro_2 = 0.5 * erro_1 * abs(temp_2) / abs(temp_1);
      th1d_v22gap->SetBinContent(i, temp_2);
      th1d_v22gap->SetBinError(i, erro_2);
    }
  }

  //----------------------------------------------------------------------------
  // v2 and sigma_v2
  TH1D* th1d_v2_mid = (TH1D*)th1d_v24->Clone("th1d_v2_mid");
  TH1D* th1d_v2_sigma = (TH1D*)th1d_v24->Clone("th1d_v2_sigma");
  for (int i = 1; i <= th1d_v24->GetNbinsX(); i++)
  {
    double temp_1 = th1d_v24->GetBinContent(i);
    double temp_2 = th1d_v22->GetBinContent(i);

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


  //----------------------------------------------------------------------------
  // common plot objects


  //----------------------------------------------------------------------------
  // plot components of v24
  double w = 800;
  double h = 900;

  TCanvas *cc24 = new TCanvas("cc24", "c24", w, h);
  cc24->SetMargin(0, 0, 0, 0);

  cc24->cd();
  TPad *pcomp = new TPad("pcomp", "comp", 0, 0.45, 1, 1);
  pcomp->SetMargin(0.12, 0.02, 0, 0.1);
  pcomp->SetTicks(1, 1);
  pcomp->Draw();

  cc24->cd();
  TPad *pcompc24 = new TPad("pcompc24", "compc24", 0, 0, 1, 0.45);
  pcompc24->SetMargin(0.12, 0.02, 0.22, 0);
  pcompc24->SetTicks(1, 1);
  pcompc24->Draw();

  TH1D *axiscomp = new TH1D("axiscomp", "", 100, 0, x_max);
  axiscomp->GetXaxis()->SetTitle("N_char");
  axiscomp->GetYaxis()->SetTitle("components");
  axiscomp->GetYaxis()->SetRangeUser(-4.9e-6, 34.9e-6);
  axiscomp->GetXaxis()->SetLabelFont(62);
  axiscomp->GetXaxis()->SetTitleFont(62);
  axiscomp->GetYaxis()->SetLabelFont(62);
  axiscomp->GetYaxis()->SetTitleFont(62);
  axiscomp->GetYaxis()->SetLabelSize(0.03);
  axiscomp->GetYaxis()->SetTitleSize(0.03);
  axiscomp->GetXaxis()->SetLabelSize(0.03);
  axiscomp->GetXaxis()->SetTitleSize(0.03);

  TH1D *axisc24 = new TH1D("axisc24", "", 100, 0, x_max);
  axisc24->GetXaxis()->SetTitle("N_char");
  axisc24->GetYaxis()->SetTitle("c_{2}{4}");
  axisc24->GetYaxis()->SetRangeUser(-19.9e-6, 19.9e-6);
  axisc24->GetXaxis()->SetLabelFont(62);
  axisc24->GetXaxis()->SetTitleFont(62);
  axisc24->GetYaxis()->SetLabelFont(62);
  axisc24->GetYaxis()->SetTitleFont(62);
  axisc24->GetYaxis()->SetLabelSize(0.03);
  axisc24->GetYaxis()->SetTitleSize(0.03);
  axisc24->GetXaxis()->SetLabelSize(0.03);
  axisc24->GetXaxis()->SetTitleSize(0.03);


  TLegend *legc24 = new TLegend(0.2, 0.7, 0.8, 0.96);
  legc24->SetFillStyle(0);
  legc24->SetBorderSize(0);
  legc24->SetTextFont(62);
  legc24->SetTextSize(0.04);
  legc24->SetHeader(lname);
  legc24->AddEntry(th1d_raa4, "#LT#LT4#GT#GT", "p");
  legc24->AddEntry(th1d_222, "2#LT#LT2#GT#GT^{2}", "p");


  th1d_raa4->SetMarkerStyle(kFullSquare);
  th1d_raa4->SetMarkerColor(kGreen+2);
  th1d_raa4->SetLineColor(kGreen+2);

  th1d_222->SetMarkerStyle(kFullCircle);
  th1d_222->SetMarkerColor(kOrange+9);
  th1d_222->SetLineColor(kOrange+9);

  th1d_c24->SetMarkerStyle(kOpenSquare);
  th1d_c24->SetMarkerColor(kBlue);
  th1d_c24->SetLineColor(kBlue);

  //-- plot
  pcomp->cd();
  axiscomp->Draw();

  th1d_222->Draw("P same");
  th1d_raa4->Draw("P same");

  legc24->Draw("same");

  //-- ratio
  pcompc24->cd();
  axisc24->Draw();

  // l1.DrawLine(0, 0, 49.5, 0);

  th1d_c24->Draw("P same");


  cc24->Print(Form("pdfs/c24_%s.pdf", pname));

  //----------------------------------------------------------------------------
  // plot v2's
  TCanvas *cv2 = new TCanvas("cv2", "cv2", 800, 600);
  cv2->SetMargin(0.12, 0.02, 0.12, 0.02);
  cv2->SetTicks(1, 1);

  cv2->cd();

  gPad->SetGridx();
  gPad->SetGridy();

  TH1D *axisv24 = new TH1D("axisv24", "0", 100, 0, x_max);

  axisv24->SetTitle("");
  axisv24->GetXaxis()->SetTitle("N_char");
  axisv24->GetYaxis()->SetTitle("v_{2}");
  axisv24->GetYaxis()->SetRangeUser(0, v2_max);
  axisv24->GetXaxis()->SetLabelFont(62);
  axisv24->GetXaxis()->SetTitleFont(62);
  axisv24->GetYaxis()->SetLabelFont(62);
  axisv24->GetYaxis()->SetTitleFont(62);
  axisv24->GetYaxis()->SetLabelSize(0.03);
  axisv24->GetYaxis()->SetTitleSize(0.03);
  axisv24->GetXaxis()->SetLabelSize(0.03);
  axisv24->GetXaxis()->SetTitleSize(0.03);
  axisv24->Draw();

  // ppplane->SetMarkerStyle(33);
  // ppplane->SetMarkerColor(1);
  // ppplane->SetMarkerSize(1.5);

  th1d_v2_mid->SetMarkerStyle(27);
  th1d_v2_mid->SetMarkerColor(1);
  th1d_v2_mid->SetMarkerSize(1.5);

  th1d_v24->SetMarkerStyle(25);
  th1d_v24->SetMarkerColor(kBlue);
  th1d_v24->SetLineColor(kBlue);

  th1d_v22gap->SetMarkerStyle(24);
  th1d_v22gap->SetMarkerColor(kRed);
  th1d_v22gap->SetLineColor(kRed);

  th1d_v22->SetMarkerStyle(20);
  th1d_v22->SetMarkerColor(kRed);
  th1d_v22->SetLineColor(kRed);

  th1d_v22gap->Draw("p,same");
  th1d_v24->Draw("p,same");
  th1d_v22->Draw("p,same");

  TLegend *leg1 = new TLegend(0.2, 0.7, 0.8, 0.96);
  leg1->SetFillStyle(0);
  leg1->SetBorderSize(0);
  leg1->SetTextFont(62);
  leg1->SetTextSize(0.04);
  leg1->SetHeader(lname);
  leg1->AddEntry(th1d_v24, "v_{2}{4}", "p");
  leg1->AddEntry(th1d_v22, "v_{2}{2}", "p");
  leg1->AddEntry(th1d_v22gap, "v_{2}{2, |#Delta#eta|>2}", "p");
  leg1->Draw("same");


  cv2->Print(Form("pdfs/cumulants_%s.pdf", pname));

  //----------------------------------------------------------------------------
  // write to file
  // TFile *fout = new TFile("dAu_cumulant_200GeV.root", "RECREATE");

  // th1d_v22gap->SetName("th1d_AMPTdAu200_v22gap");
  // th1d_v22->SetName("th1d_AMPTdAu200_v22");
  // th1d_v24->SetName("th1d_AMPTdAu200_v24");
  // th1d_raa4->SetName("th1d_AMPTdAu200_four");
  // th1d_c2->SetName("th1d_AMPTdAu200_222");

  // th1d_v22gap->SetTitle("th1d_AMPTdAu200_v22gap");
  // th1d_v22->SetTitle("th1d_AMPTdAu200_v22");
  // th1d_v24->SetTitle("th1d_AMPTdAu200_v24");
  // th1d_raa4->SetTitle("th1d_AMPTdAu200_four");
  // th1d_c2->SetTitle("th1d_AMPTdAu200_222");

  // th1d_v22gap->Write();
  // th1d_v22->Write();
  // th1d_v24->Write();
  // th1d_raa4->Write();
  // th1d_c2->Write();

  // fout->Close();


}





































