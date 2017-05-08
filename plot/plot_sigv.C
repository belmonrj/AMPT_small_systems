////////////////////////////////////////////////////////////////////////////////
//
// Plot v2{2} and v2{4} and calculate sigma_v / v
//
////////////////////////////////////////////////////////////////////////////////
//
// Darren McGlinchey
// 19 Apr 2017
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

void plot_sigv()
{
  gStyle->SetOptStat(0);
gStyle->SetErrorX(0);

  //==========================================================================//
  // SET RUNNING CONDITIONS
  //==========================================================================//

  float v2_max = 0.089;
  int x_max = 70;   // PbPb: maximum 6000     pPb: 300
  int rebin = 2;
  // int x_max = 1000;   // PbPb: maximum 6000     pPb: 300
  // int rebin = 20;

  double deltav = 0; // nonflow delta^2 value

  bool printPlots = true;

  // const char* cFilename = "rootfiles/cumulants_ampt_dau200_b02_sigparton150_ptfilter_50M_set8.root";
  // const char* pFilename = "rootfiles/pplane_ampt_dau200_b02_sigparton150_ptfilter_50M_set8.root";
  // const char* lname = "AMPT -- d+Au 200 -- 50M b<2 fm -- #sigma_{parton}=1.50 mb -- p_{T} filter";
  // const char* pname = "ampt_dau200_b02_sigparton150_ptfilter_50M_rebin";

  // const char* cFilename = "rootfiles/cumulants_ampt_auau200_b20_sigparton150_ptfilter_1M_set1.root";
  // const char* pFilename = "rootfiles/pplane_ampt_auau200_b20_sigparton150_ptfilter_1M_set1.root";
  // const char* lname = "AMPT -- Au+Au 200 -- 1M b<20 fm -- #sigma_{parton}=1.50 mb -- p_{T} filter";
  // const char* pname = "ampt_auau200_b20_sigparton150_ptfilter_1M_rebin";

  // const char* cFilename = "rootfiles/cumulants_ampt_ppb5020_b02_sigparton150_ptfilter_50M_set2.root";
  // const char* pFilename = "rootfiles/pplane_ampt_ppb5020_b02_sigparton150_ptfilter_50M_set2.root";
  // const char* lname = "AMPT -- p+Pb 5020 -- 50M b<02 fm -- #sigma_{parton}=1.50 mb -- p_{T} filter";
  // const char* pname = "ampt_ppb5020_b02_sigparton150_ptfilter_50M_rebin";

  // const char* cFilename = "rootfiles/cumulants_ampt_pbpb5020_b20_sigparton150_ptfilter_1M_set0.root";
  // const char* pFilename = "rootfiles/pplane_ampt_pbpb5020_b20_sigparton150_ptfilter_1M_set0.root";
  // const char* lname = "AMPT -- Pb+Pb 5020 -- 1M b<20 fm -- #sigma_{parton}=1.50 mb -- p_{T} filter";
  // const char* pname = "ampt_pbpb5020_b20_sigparton150_ptfilter_1M_rebin";

  // const char* cFilename = "rootfiles/cumulants_ampt_dau200_11.root";
  // const char* pFilename = "rootfiles/pplane_ampt_dau200_11.root";
  // const char* lname = "AMPT -- d+Au 200 -- 50M b<20 fm -- #sigma_{parton}=0.75 mb -- p_{T} eta filter";
  // const char* pname = "ampt_dau200_b20_sigparton075_ptetafilter_50M_rebin";

  // const char* cFilename = "rootfiles/cumulants_ampt_dau62_11.root";
  // const char* pFilename = "rootfiles/pplane_ampt_dau62_11.root";
  // const char* lname = "AMPT -- d+Au 62.4 -- 50M b<20 fm -- #sigma_{parton}=0.75 mb -- p_{T} eta filter";
  // const char* pname = "ampt_dau62_b20_sigparton075_ptetafilter_50M_rebin";

  // const char* cFilename = "rootfiles/cumulants_ampt_dau39_10.root";
  // const char* pFilename = "rootfiles/pplane_ampt_dau39_10.root";
  // const char* lname = "AMPT -- d+Au 39 -- 50M b<20 fm -- #sigma_{parton}=0.75 mb -- p_{T} eta filter";
  // const char* pname = "ampt_dau39_b20_sigparton075_ptetafilter_50M_rebin";

  const char* cFilename = "rootfiles/cumulants_ampt_dau20_10.root";
  const char* pFilename = "rootfiles/pplane_ampt_dau20_10.root";
  const char* lname = "AMPT -- d+Au 19.6 -- 50M b<20 fm -- #sigma_{parton}=0.75 mb -- p_{T} eta filter";
  const char* pname = "ampt_dau20_b20_sigparton075_ptetafilter_50M_rebin";


  //==========================================================================//
  // GET HISTOGRAMS FROM FILE
  //==========================================================================//
  cout << endl;
  cout << "--> Reading histograms from file" << endl;

  TFile *cfile = TFile::Open(cFilename);
  if (!cfile)
  {
    cout << "ERROR!! Unable to open " << cFilename << endl;
    return;
  }
  else
  {
    cout << "----> Reading cumulants from " << cFilename << endl;
  }


  TProfile* tp1f_raa6 = (TProfile*) cfile->Get("raa6_Ncharge");
  TProfile* tp1f_raa4 = (TProfile*) cfile->Get("raa4_Ncharge");
  TProfile* tp1f_raa2 = (TProfile*) cfile->Get("raa2_Ncharge");
  TProfile* tp1f_gapp = (TProfile*) cfile->Get("comp_Ncharge");

  tp1f_raa6->Rebin(rebin);
  tp1f_raa4->Rebin(rebin);
  tp1f_raa2->Rebin(rebin);
  tp1f_gapp->Rebin(rebin);


  TH1D* th1d_raa6 = (TH1D*) tp1f_raa6->ProjectionX("th1d_raa6");
  TH1D* th1d_raa4 = (TH1D*) tp1f_raa4->ProjectionX("th1d_raa4");
  TH1D* th1d_raa2 = (TH1D*) tp1f_raa2->ProjectionX("th1d_raa2");
  TH1D* th1d_gapp = (TH1D*) tp1f_gapp->ProjectionX("th1d_gapp");


  TFile *pfile = TFile::Open(pFilename);
  if (!pfile)
  {
    cout << "ERROR! Unable to open " << pFilename << endl;
    return;
  }
  else
  {
    cout << "----> Reading parton plane from " << pFilename << endl;
  }

  TProfile* tp1f_epsilon2_nch = (TProfile*) pfile->Get("epsilon2_nch");
  TProfile* tp1f_v2pp = (TProfile*) pfile->Get("v2pp_nch");

  tp1f_epsilon2_nch->Rebin(rebin);
  tp1f_v2pp->Rebin(rebin);




  TH1D* th1d_epsilon2 = (TH1D*) tp1f_epsilon2_nch->ProjectionX("th1d_epsilon2");
  TH1D* th1d_v2pp = (TH1D*) tp1f_v2pp->ProjectionX("th1d_v2pp");


  //==========================================================================//
  // CALCULATE V2{6}
  //==========================================================================//

  // <<6>>
  TH1D* th1d_aa6 = (TH1D*)th1d_raa6->Clone("th1d_aa6");

  // 9*<<2>><<4>>
  TH1D* th1d_aa4 = (TH1D*)th1d_raa4->Clone("th1d_aa4");
  th1d_aa4->Multiply(th1d_raa2);
  th1d_aa4->Scale(9.0);

  // 12*<<2>>^3
  TH1D* th1d_aa2 = (TH1D*)th1d_raa2->Clone("th1d_aa2");
  th1d_aa2->Multiply(th1d_raa2);
  th1d_aa2->Multiply(th1d_raa2);
  th1d_aa2->Scale(12.0);

  // calculate c_{2}{6} = (<<6>> - 9<<2>><<4>> + 12<<2>>^3)
  TH1D* th1d_c26 = (TH1D*)th1d_aa6->Clone("th1d_c26");
  th1d_c26->Add(th1d_aa4, -1.0);
  th1d_c26->Add(th1d_aa2, 1.0);

  // calculate v_{2}{6} = (1/4*c26)^{1/6}
  TH1D* th1d_v26 = (TH1D*)th1d_c26->Clone("th1d_v26");
  for (int i = 1; i <= th1d_v26->GetNbinsX(); i++)
  {
    double temp_1 = th1d_c26->GetBinContent(i);
    double erro_1 = th1d_c26->GetBinError(i);

    if (temp_1 <= 0)
    { 
      th1d_v26->SetBinContent(i, 0);
      th1d_v26->SetBinError(i, 0);
    }
    else
    {
      double temp_2 = pow(0.25*temp_1, 1./6.);
      // double erro_2 = (1./6.) * erro_1 * abs(temp_2) / (0.25*abs(temp_1));
      double erro_2 = pow(0.25*erro_1, 1/6.);
      th1d_v26->SetBinContent(i, temp_2);
      // th1d_v26->SetBinError(i, erro_2);
    }
  }

  //==========================================================================//
  // CALCULATE V2{4}
  //==========================================================================//

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

  //==========================================================================//
  // CALCULATE V2{2}
  //==========================================================================//

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

  //==========================================================================//
  // CALCULATE V2{2, GAP}
  //==========================================================================//

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

  //==========================================================================//
  // CALCULATE V2 AND SIGMA_V2
  //==========================================================================//

  TH1D* th1d_v2_mid = (TH1D*)th1d_v24->Clone("th1d_v2_mid");
  TH1D* th1d_v2_sigma = (TH1D*)th1d_v24->Clone("th1d_v2_sigma");
  TH1D* th1d_sigvv = (TH1D*) th1d_v24->Clone("th1d_sigvv");
  th1d_v2_mid->Clear();
  th1d_v2_sigma->Clear();
  th1d_sigvv->Clear();

  for (int i = 1; i <= th1d_sigvv->GetNbinsX(); i++)
  {
    double v24 = th1d_v24->GetBinContent(i);
    double v22 = th1d_v22->GetBinContent(i);

    if (v24 > 0)
    {
      double v2_mid = (v22 * v22 + v24 * v24) / (2 + deltav * deltav);
      double v2_sigma = (v22 * v22 - v24 * v24) / (2 + deltav * deltav);
      v2_mid = TMath::Sqrt(v2_mid);
      if (v2_sigma > 0) v2_sigma = TMath::Sqrt(v2_sigma);
      else v2_sigma = -9999;
      th1d_v2_mid->SetBinContent(i, v2_mid);
      th1d_v2_sigma->SetBinContent(i, v2_sigma);
      th1d_sigvv->SetBinContent(i, v2_sigma / v2_mid);
    }
  }


  //==========================================================================//
  // calculate sigma_e2 / e2
  //==========================================================================//

  TH1D* th1d_sigee = (TH1D*) th1d_epsilon2->Clone("th1d_sigee");
  th1d_sigee->Clear();
  for (int i = 1; i <= th1d_sigee->GetNbinsX(); i++)
  {
    double e2 = th1d_epsilon2->GetBinContent(i);
    double sig = th1d_epsilon2->GetBinError(i);
    double n = tp1f_epsilon2_nch->GetBinEntries(i);

    if ( e2 > 0 )
    {
      // by default, tprofiles store sigma / sqrt(N)
      th1d_sigee->SetBinContent(i, sig * sqrt(n) / e2);
    }
  }

  //==========================================================================//
  // PLOT OBJECTS
  //==========================================================================//
  cout << endl;
  cout << "--> Plotting" << endl;

  //==========================================================================//
  // PLOT
  //==========================================================================//

  // plot  v24
  double w = 800;
  double h = 900;

  TCanvas *cv24 = new TCanvas("cv24", "c24", w, h);
  cv24->SetMargin(0, 0, 0, 0);

  cv24->cd();
  TPad *pcomp = new TPad("pcomp", "comp", 0, 0.45, 1, 1);
  pcomp->SetMargin(0.12, 0.02, 0, 0.1);
  pcomp->SetTicks(1, 1);
  pcomp->Draw();

  cv24->cd();
  TPad *pcompc24 = new TPad("pcompc24", "compc24", 0, 0, 1, 0.45);
  pcompc24->SetMargin(0.12, 0.02, 0.22, 0);
  pcompc24->SetTicks(1, 1);
  pcompc24->Draw();

  TH1D *axiscomp = new TH1D("axiscomp", "", 100, 0, x_max);
  axiscomp->GetXaxis()->SetTitle("N_char");
  axiscomp->GetYaxis()->SetTitle("v_{2}");
  // axiscomp->GetYaxis()->SetRangeUser(0.0, 0.1);
  axiscomp->GetYaxis()->SetRangeUser(0.0, v2_max);
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
  axisc24->GetYaxis()->SetTitle("");
  axisc24->GetYaxis()->SetRangeUser(-0.49, 0.99);
  axisc24->GetXaxis()->SetLabelFont(62);
  axisc24->GetXaxis()->SetTitleFont(62);
  axisc24->GetYaxis()->SetLabelFont(62);
  axisc24->GetYaxis()->SetTitleFont(62);
  axisc24->GetYaxis()->SetLabelSize(0.03);
  axisc24->GetYaxis()->SetTitleSize(0.03);
  axisc24->GetXaxis()->SetLabelSize(0.03);
  axisc24->GetXaxis()->SetTitleSize(0.03);

  th1d_v2pp->SetMarkerStyle(33);
  th1d_v2pp->SetMarkerColorAlpha(kBlack, 0.8);
  th1d_v2pp->SetLineColor(kBlack);
  th1d_v2pp->SetMarkerSize(1.5);

  th1d_v2_mid->SetMarkerStyle(kFullDiamond);
  th1d_v2_mid->SetMarkerSize(1.5);
  th1d_v2_mid->SetMarkerColorAlpha(kMagenta+2, 0.8);
  th1d_v2_mid->SetLineColor(kMagenta+2);

  th1d_v26->SetMarkerStyle(kFullStar);
  th1d_v26->SetMarkerSize(1.5);
  th1d_v26->SetMarkerColorAlpha(kGreen+2, 0.8);
  th1d_v26->SetLineColor(kGreen+2);

  th1d_v24->SetMarkerStyle(kFullSquare);
  th1d_v24->SetMarkerColorAlpha(kBlue, 0.8);
  th1d_v24->SetLineColor(kBlue);

  th1d_v22gap->SetMarkerStyle(24);
  th1d_v22gap->SetMarkerColorAlpha(kRed, 0.8);
  th1d_v22gap->SetLineColor(kRed);

  th1d_v22->SetMarkerStyle(20);
  th1d_v22->SetMarkerColorAlpha(kRed, 0.8);
  th1d_v22->SetLineColor(kRed);

  th1d_sigvv->SetMarkerStyle(kFullSquare);
  th1d_sigvv->SetMarkerColorAlpha(kBlue, 0.8);
  th1d_sigvv->SetLineColor(kBlue);

  th1d_sigee->SetMarkerStyle(kFullCircle);
  th1d_sigee->SetMarkerColorAlpha(kBlack, 0.8);
  th1d_sigee->SetLineColor(kBlack);

  TLatex lt;
  lt.SetNDC();
  lt.SetTextAlign(22);

  // TLegend *leg1 = new TLegend(0.2, 0.7, 0.8, 0.96);
  TLegend *leg1 = new TLegend(0.35, 0.04, 0.85, 0.24);
  leg1->SetFillStyle(0);
  leg1->SetBorderSize(0);
  leg1->SetTextFont(62);
  leg1->SetTextSize(0.04);
  // leg1->SetHeader(lname);
  leg1->SetNColumns(2);
  leg1->AddEntry(th1d_v22, "v_{2}{2} = #sqrt{v_{2}^{2} + #sigma_{v_{2}}^{2} + #delta^{2}}", "p");
  leg1->AddEntry(th1d_v2pp, "v_{2}{PP}", "p");
  leg1->AddEntry(th1d_v24, "v_{2}{4} = #sqrt{v_{2}^{2} - #sigma_{v_{2}}^{2}}", "p");
  leg1->AddEntry(th1d_v2_mid, Form("v_{2} (#delta / v_{2} = %.2f)", deltav), "p");
  leg1->AddEntry(th1d_v26, "v_{2}{6}", "p");
  // leg1->AddEntry(th1d_v22gap, "v_{2}{2, |#Delta#eta|>2}", "p");

  TLegend *leg2 = new TLegend(0.2, 0.85, 0.8, 0.98);
  leg2->SetFillStyle(0);
  leg2->SetBorderSize(0);
  leg2->SetTextFont(62);
  leg2->SetTextSize(0.04);
  leg2->SetNColumns(2);
  leg2->AddEntry(th1d_sigee, "#sigma_{#epsilon_{2}} / #epsilon_{2}", "P");
  leg2->AddEntry(th1d_sigvv, Form("#sigma_{v_{2}} / v_{2} (#delta / v_{2} = %.2f)", deltav), "P");

  //-- plot
  pcomp->cd();
  axiscomp->Draw();

  th1d_v2pp->Draw("p,same");
  // th1d_v22gap->Draw("p,same");
  th1d_v26->Draw("p,same");
  th1d_v24->Draw("p,same");
  th1d_v22->Draw("p,same");
  th1d_v2_mid->Draw("p,same");

  leg1->Draw("same");
  lt.DrawLatex(0.5, 0.95, lname);

  //-- ratio
  pcompc24->cd();
  axisc24->Draw();

  th1d_sigvv->Draw("p,same");
  th1d_sigee->Draw("p,same");

  leg2->Draw("same");




  //==========================================================================//
  // PRINT PLOTS
  //==========================================================================//
  if ( printPlots )
  {
    cout << endl;
    cout << "--> Printing plots" << endl;

    cv24->Print(Form("pdfs/sigvv_%s.pdf", pname));
  }


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





































