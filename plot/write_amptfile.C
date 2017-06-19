////////////////////////////////////////////////////////////////////////////////
//
// Write AMPT info to file
//
////////////////////////////////////////////////////////////////////////////////
//
// Darren McGlinchey
// 26 May 2017
//
////////////////////////////////////////////////////////////////////////////////

#include <TFile.h>
#include <TProfile.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TMath.h>

#include <iostream>

using namespace std;

void write_amptfile()
{
  //==========================================================================//
  // SET RUNNING CONDITIONS
  //==========================================================================//

  const int NE = 4;
  int energy[] = {200, 62, 39, 20};
  const char* epFile[] =
  {
    "rootfiles/eplane_ampt_dau200_20-29.root",
    "rootfiles/eplane_ampt_dau62_20-24.root",
    "rootfiles/eplane_ampt_dau39_20-24.root",
    "rootfiles/eplane_ampt_dau20_20-23.root",
  };
  const char* ppFile[] =
  {
    "rootfiles/pplane_ampt_dau200_20-29.root",
    "rootfiles/pplane_ampt_dau62_20-24.root",
    "rootfiles/pplane_ampt_dau39_20-24.root",
    "rootfiles/pplane_ampt_dau20_20-23.root",
  };
  const char* cumFile[] =
  {
    "rootfiles/cumulants_ampt_dau200_11-29.root",
    "rootfiles/cumulants_ampt_dau62_11-24.root",
    "rootfiles/cumulants_ampt_dau39_10-24.root",
    "rootfiles/cumulants_ampt_dau20_10-23.root",
  };
  const char* nintepFile[] = 
  {
    "rootfiles/eplane_ampt_dau200_nint_1-4.root",
    "rootfiles/eplane_ampt_dau200_nint_1-4.root",//dummy!
    "rootfiles/eplane_ampt_dau200_nint_1-4.root",//dummy!
    "rootfiles/eplane_ampt_dau200_nint_1-4.root",//dummy!
  };
  const char* nintppFile[] = 
  {
    "rootfiles/pplane_ampt_dau200_nint_1-4.root",
    "rootfiles/pplane_ampt_dau200_nint_1-4.root",//dummy!
    "rootfiles/pplane_ampt_dau200_nint_1-4.root",//dummy!
    "rootfiles/pplane_ampt_dau200_nint_1-4.root",//dummy!
  };
  //const char* pauCumFile = "rootfiles/cumulants_ampt_pau200_0-14.root";
  const char* pauCumFile = "rootfiles/cumulants_ampt_pau200_all.root";

  int rebin = 2;

  const int NCENTMAX = 6;
  int NCENT[] = {6, 6, 5, 4};

  bool saveFile = true;
  const char* outFile = "AMPT_sigparton075_test.root";

  //==========================================================================//
  // DECLARE VARIABLES
  //==========================================================================//

  //-- parton plane ... v2{PP}
  TProfile *v2pp_pt[NE][NCENTMAX];
  TProfile *v2pp_eta[NE][NCENTMAX];

  TProfile *v2pp_nint_pt[NE][NCENTMAX];
  TProfile *v2pp_nint_eta[NE][NCENTMAX];

  //-- event plane ... v2{EP}
  TProfile *v2ep_pt[NE][NCENTMAX];
  TProfile *v2ep_eta[NE][NCENTMAX];

  TProfile *v2ep_nint_pt[NE][NCENTMAX];
  TProfile *v2ep_nint_eta[NE][NCENTMAX];

  //-- event plane resolutions
  double res_fvtxs[NE][NCENTMAX];
  double res_bbcs[NE][NCENTMAX];

  double res_fvtxs_nint[NE][NCENTMAX];
  double res_bbcs_nint[NE][NCENTMAX];

  //-- dNch/deta
  TH1D* dnchdeta[NE][NCENTMAX];

  //-- cumulants
  TH1D* cum_c24[NE];
  TH1D* cum_c26[NE];

  TH1D* cum_v22[NE];
  TH1D* cum_v22gap[NE];
  TH1D* cum_v24[NE];
  TH1D* cum_v26[NE];

  TH1D* cum_pau200_c24;
  TH1D* cum_pau200_c26;

  TH1D* cum_pau200_v22;
  TH1D* cum_pau200_v22gap;
  TH1D* cum_pau200_v24;
  TH1D* cum_pau200_v26;


  //-- temp
  TFile *fin;
  char name[500];
  TProfile *ptmp;
  TH1D* htmp;

  //==========================================================================//
  //==========================================================================//
  //==========================================================================//
  // D+AU
  //==========================================================================//
  //==========================================================================//
  //==========================================================================//
  cout << endl;
  cout << endl;
  cout << "----------------" << endl;
  cout << "--- d+Au BES ---" << endl;
  cout << "----------------" << endl;
  cout << endl;
  cout << endl;

  //==========================================================================//
  // READ FROM PARTON PLANE FILE
  //==========================================================================//
  cout << endl;
  cout << "--> Reading results from parton plane files" << endl;

  for (int ie = 0; ie < NE; ie++)
  {
    //-----
    //-- parton file
    //-----
    fin = TFile::Open(ppFile[ie]);
    if (!fin)
    {
      cout << "ERROR!! Unable to open " << ppFile[ie] << endl;
      return;
    }
    cout << "----> Successfully read " << ppFile[ie]
         << " for sqrts=" << energy[ie] << endl;

    //-- centrality dependence
    for (int ic = 0; ic < NCENT[ie]; ic++)
    {
      // v2 pp vs pt
      sprintf(name, "v2pp_pt_%i", ic);
      v2pp_pt[ie][ic] = (TProfile*) fin->Get(name);
      if ( !v2pp_pt[ie][ic] )
      {
        cout << "ERROR!! Unable to find " << name << " in " << ppFile[ie] << endl;
        return;
      }
      v2pp_pt[ie][ic]->SetDirectory(0);
      v2pp_pt[ie][ic]->SetLineColor(kGreen + 2);
      v2pp_pt[ie][ic]->SetLineWidth(3);

      // v2 pp vs eta
      sprintf(name, "v2pp_eta_%i", ic);
      v2pp_eta[ie][ic] = (TProfile*) fin->Get(name);
      if ( !v2pp_eta[ie][ic] )
      {
        cout << "ERROR!! Unable to find " << name << " in " << ppFile[ie] << endl;
        return;
      }
      v2pp_eta[ie][ic]->SetDirectory(0);
      v2pp_eta[ie][ic]->SetLineColor(kGreen + 2);
      v2pp_eta[ie][ic]->SetLineWidth(3);
    } // ic

    fin->Close();
    delete fin;

  } // ie



  //==========================================================================//
  // READ FROM EVENT PLANE FILE
  //==========================================================================//
  cout << endl;
  cout << "--> Reading results from event plane files" << endl;

  for (int ie = 0; ie < NE; ie++)
  {
    //-----
    //-- event plane file
    //-----
    fin = TFile::Open(epFile[ie]);
    if (!fin)
    {
      cout << "ERROR!! Unable to open " << epFile[ie] << endl;
      return;
    }
    cout << "----> Successfully read " << epFile[ie]
         << " for sqrts=" << energy[ie] << endl;

    //-- centrality dependence
    for (int ic = 0; ic < NCENT[ie]; ic++)
    {
      // v2 pp vs pt
      sprintf(name, "v2ep_fvtxs_pt_%i", ic);
      v2ep_pt[ie][ic] = (TProfile*) fin->Get(name);
      if ( !v2ep_pt[ie][ic] )
      {
        cout << "ERROR!! Unable to find " << name << " in " << epFile[ie] << endl;
        return;
      }
      v2ep_pt[ie][ic]->SetDirectory(0);
      v2ep_pt[ie][ic]->SetLineColor(kBlue);
      v2ep_pt[ie][ic]->SetLineWidth(3);

      // v2 pp vs eta
      sprintf(name, "v2ep_bbcs_eta_%i", ic);
      v2ep_eta[ie][ic] = (TProfile*) fin->Get(name);
      if ( !v2ep_eta[ie][ic] )
      {
        cout << "ERROR!! Unable to find " << name << " in " << epFile[ie] << endl;
        return;
      }
      v2ep_eta[ie][ic]->SetDirectory(0);
      v2ep_eta[ie][ic]->SetLineColor(kBlue + 2);
      v2ep_eta[ie][ic]->SetLineWidth(3);

      // resolutions
      sprintf(name, "res_comp_%i", ic);
      ptmp = (TProfile*) fin->Get(name);
      if ( !ptmp )
      {
        cout << "ERROR!! Unable to find " << name << " in " << epFile[ie] << endl;
        return;
      }
      htmp = (TH1D*) ptmp->ProjectionX("htmp");

      //A:FVTXS , B:BBCS , C:CNT
      float AB = htmp->GetBinContent(htmp->FindBin(1.0));
      float AC = htmp->GetBinContent(htmp->FindBin(2.0));
      float BC = htmp->GetBinContent(htmp->FindBin(3.0));

      res_fvtxs[ie][ic] = TMath::Sqrt(AB * AC / BC);
      res_bbcs[ie][ic] = TMath::Sqrt(AB * BC / AC);

      cout << " E:" << energy[ie]
           << " c:" << ic
           << " R(FVTXS):" << res_fvtxs[ie][ic]
           << " R(BBCS):" << res_bbcs[ie][ic]
           << endl;

      v2ep_pt[ie][ic]->Scale(1. / res_fvtxs[ie][ic]);
      v2ep_eta[ie][ic]->Scale(1. / res_bbcs[ie][ic]);

      delete ptmp;
      delete htmp;

      // dNch/deta
      sprintf(name, "nch_eta_%i", ic);
      dnchdeta[ie][ic] = (TH1D*) fin->Get(name);
      if ( !dnchdeta[ie][ic] )
      {
        cout << "ERROR!! Unable to find " << name << " in " << epFile[ie] << endl;
        return;
      }
      dnchdeta[ie][ic]->SetDirectory(0);

      htmp = (TH1D*) fin->Get("hcount");
      float nevents = htmp->GetBinContent(htmp->FindBin(3 + ic));

      float bw = dnchdeta[ie][ic]->GetBinWidth(1);
      dnchdeta[ie][ic]->Scale(1. / (bw * nevents));

      delete htmp;

    } // ic

    fin->Close();
    delete fin;

  } // ie

  //==========================================================================//
  // READ FROM CUMULANTS FILE
  //==========================================================================//
  cout << endl;
  cout << "--> Reading d+Au cumulants files" << endl;

  for (int ie = 0; ie < NE; ie++)
  {
    fin = TFile::Open(cumFile[ie]);
    if (!fin)
    {
      cout << "ERROR!! Unable to open " << cumFile[ie] << endl;
      return;
    }
    cout << "----> Successfully read " << cumFile[ie]
         << " for sqrts=" << energy[ie] << endl;


    TProfile* tp1f_raa6 = (TProfile*) fin->Get("raa6_Ncharge");
    TProfile* tp1f_raa4 = (TProfile*) fin->Get("raa4_Ncharge");
    TProfile* tp1f_raa2 = (TProfile*) fin->Get("raa2_Ncharge");
    TProfile* tp1f_gapp = (TProfile*) fin->Get("comp_Ncharge");

    tp1f_raa6->Rebin(rebin);
    tp1f_raa4->Rebin(rebin);
    tp1f_raa2->Rebin(rebin);
    tp1f_gapp->Rebin(rebin);

    TH1D* th1d_raa6 = (TH1D*) tp1f_raa6->ProjectionX("th1d_raa6");
    TH1D* th1d_raa4 = (TH1D*) tp1f_raa4->ProjectionX("th1d_raa4");
    TH1D* th1d_raa2 = (TH1D*) tp1f_raa2->ProjectionX("th1d_raa2");
    TH1D* th1d_gapp = (TH1D*) tp1f_gapp->ProjectionX("th1d_gapp");

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
    cum_c26[ie] = (TH1D*)th1d_aa6->Clone(Form("cum_c26_%i", ie));
    cum_c26[ie]->SetDirectory(0);
    cum_c26[ie]->Add(th1d_aa4, -1.0);
    cum_c26[ie]->Add(th1d_aa2, 1.0);

    // calculate v_{2}{6} = (1/4*c26)^{1/6}
    cum_v26[ie] = (TH1D*)cum_c26[ie]->Clone(Form("cum_v26_%i", ie));
    cum_v26[ie]->SetDirectory(0);
    for (int i = 1; i <= cum_v26[ie]->GetNbinsX(); i++)
    {
      double temp_1 = cum_c26[ie]->GetBinContent(i);
      double erro_1 = cum_c26[ie]->GetBinError(i);

      if (temp_1 <= 0)
      {
        cum_v26[ie]->SetBinContent(i, 0);
        cum_v26[ie]->SetBinError(i, 0);
      }
      else
      {
        double temp_2 = pow(0.25 * temp_1, 1. / 6.);
        // double erro_2 = (1./6.) * erro_1 * abs(temp_2) / (0.25*abs(temp_1));
        double erro_2 = pow(0.25 * erro_1, 1 / 6.);
        cum_v26[ie]->SetBinContent(i, temp_2);
        // cum_v26[ie]->SetBinError(i, erro_2);
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
    cum_c24[ie] = (TH1D*)th1d_raa4->Clone(Form("cum_c24_%i", ie));
    cum_c24[ie]->SetDirectory(0);
    cum_c24[ie]->Add(th1d_222, -1.0);

    // calculate v_{2}{4} = (c_{2}{4})^{1/4}
    cum_v24[ie] = (TH1D*)cum_c24[ie]->Clone(Form("cum_v24_%i", ie));
    cum_v24[ie]->SetDirectory(0);
    for (int i = 1; i <= cum_v24[ie]->GetNbinsX(); i++)
    {
      double temp_1 = cum_v24[ie]->GetBinContent(i);
      double erro_1 = cum_v24[ie]->GetBinError(i);

      if (temp_1 >= 0)
      {
        cum_v24[ie]->SetBinContent(i, 0);
        cum_v24[ie]->SetBinError(i, 0);
      }
      else
      {
        double temp_2 = pow(-temp_1, 0.25);
        double erro_2 = 0.25 * erro_1 * abs(temp_2) / abs(temp_1);
        cum_v24[ie]->SetBinContent(i, temp_2);
        cum_v24[ie]->SetBinError(i, erro_2);
      }
    }

    //==========================================================================//
    // CALCULATE V2{2}
    //==========================================================================//

    cum_v22[ie] = (TH1D*)th1d_raa2->Clone(Form("cum_v22_%i", ie));
    cum_v22[ie]->SetDirectory(0);
    for (int i = 1; i <= cum_v22[ie]->GetNbinsX(); i++)
    {
      double temp_1 = cum_v22[ie]->GetBinContent(i);
      double erro_1 = cum_v22[ie]->GetBinError(i);

      if (temp_1 <= 0) cum_v22[ie]->SetBinContent(i, -9999);
      else
      {
        double temp_2 = pow(temp_1, 0.5);
        double erro_2 = 0.5 * erro_1 * abs(temp_2) / abs(temp_1);
        cum_v22[ie]->SetBinContent(i, temp_2);
        cum_v22[ie]->SetBinError(i, erro_2);
      }
    }

    //==========================================================================//
    // CALCULATE V2{2, GAP}
    //==========================================================================//

    cum_v22gap[ie] = (TH1D*)th1d_gapp->Clone(Form("cum_v22gap_%i", ie));
    cum_v22gap[ie]->SetDirectory(0);
    for (int i = 1; i <= cum_v22gap[ie]->GetNbinsX(); i++)
    {
      double temp_1 = cum_v22gap[ie]->GetBinContent(i);
      double erro_1 = cum_v22gap[ie]->GetBinError(i);

      if (temp_1 <= 0) cum_v22gap[ie]->SetBinContent(i, -9999);
      else
      {
        double temp_2 = pow(temp_1, 0.5);
        double erro_2 = 0.5 * erro_1 * abs(temp_2) / abs(temp_1);
        cum_v22gap[ie]->SetBinContent(i, temp_2);
        cum_v22gap[ie]->SetBinError(i, erro_2);
      }
    }



    fin->Close();
    delete fin;

  } // ie


  //==========================================================================//
  // READ FROM PARTON PLANE FILE (NO INTERACTIONS)
  //==========================================================================//
  cout << endl;
  cout << "--> Reading results from nint parton plane files" << endl;

  for (int ie = 0; ie < NE; ie++)
  {
    //-----
    //-- parton file
    //-----
    fin = TFile::Open(nintppFile[ie]);
    if (!fin)
    {
      cout << "ERROR!! Unable to open " << nintppFile[ie] << endl;
      return;
    }
    cout << "----> Successfully read " << nintppFile[ie]
         << " for sqrts=" << energy[ie] << endl;

    //-- centrality dependence
    for (int ic = 0; ic < NCENT[ie]; ic++)
    {
      // v2 pp vs pt
      sprintf(name, "v2pp_pt_%i", ic);
      v2pp_nint_pt[ie][ic] = (TProfile*) fin->Get(name);
      if ( !v2pp_nint_pt[ie][ic] )
      {
        cout << "ERROR!! Unable to find " << name << " in " << nintppFile[ie] << endl;
        return;
      }
      v2pp_nint_pt[ie][ic]->SetDirectory(0);
      v2pp_nint_pt[ie][ic]->SetLineColor(kGreen + 2);
      v2pp_nint_pt[ie][ic]->SetLineWidth(3);

      // v2 pp vs eta
      sprintf(name, "v2pp_eta_%i", ic);
      v2pp_nint_eta[ie][ic] = (TProfile*) fin->Get(name);
      if ( !v2pp_nint_eta[ie][ic] )
      {
        cout << "ERROR!! Unable to find " << name << " in " << nintppFile[ie] << endl;
        return;
      }
      v2pp_nint_eta[ie][ic]->SetDirectory(0);
      v2pp_nint_eta[ie][ic]->SetLineColor(kGreen + 2);
      v2pp_nint_eta[ie][ic]->SetLineWidth(3);
    } // ic

    fin->Close();
    delete fin;

  } // ie



  //==========================================================================//
  // READ FROM EVENT PLANE FILE (NO INTERACTIONS)
  //==========================================================================//
  cout << endl;
  cout << "--> Reading results from nint event plane files" << endl;

  for (int ie = 0; ie < NE; ie++)
  {
    //-----
    //-- event plane file
    //-----
    fin = TFile::Open(nintepFile[ie]);
    if (!fin)
    {
      cout << "ERROR!! Unable to open " << nintepFile[ie] << endl;
      return;
    }
    cout << "----> Successfully read " << nintepFile[ie]
         << " for sqrts=" << energy[ie] << endl;

    //-- centrality dependence
    for (int ic = 0; ic < NCENT[ie]; ic++)
    {
      // v2 pp vs pt
      sprintf(name, "v2ep_fvtxs_pt_%i", ic);
      v2ep_nint_pt[ie][ic] = (TProfile*) fin->Get(name);
      if ( !v2ep_nint_pt[ie][ic] )
      {
        cout << "ERROR!! Unable to find " << name << " in " << nintepFile[ie] << endl;
        return;
      }
      v2ep_nint_pt[ie][ic]->SetDirectory(0);
      v2ep_nint_pt[ie][ic]->SetLineColor(kBlue);
      v2ep_nint_pt[ie][ic]->SetLineWidth(3);

      // v2 pp vs eta
      sprintf(name, "v2ep_bbcs_eta_%i", ic);
      v2ep_nint_eta[ie][ic] = (TProfile*) fin->Get(name);
      if ( !v2ep_nint_eta[ie][ic] )
      {
        cout << "ERROR!! Unable to find " << name << " in " << nintepFile[ie] << endl;
        return;
      }
      v2ep_nint_eta[ie][ic]->SetDirectory(0);
      v2ep_nint_eta[ie][ic]->SetLineColor(kBlue + 2);
      v2ep_nint_eta[ie][ic]->SetLineWidth(3);

      // resolutions
      sprintf(name, "res_comp_%i", ic);
      ptmp = (TProfile*) fin->Get(name);
      if ( !ptmp )
      {
        cout << "ERROR!! Unable to find " << name << " in " << nintepFile[ie] << endl;
        return;
      }
      htmp = (TH1D*) ptmp->ProjectionX("htmp");

      //A:FVTXS , B:BBCS , C:CNT
      float AB = htmp->GetBinContent(htmp->FindBin(1.0));
      float AC = htmp->GetBinContent(htmp->FindBin(2.0));
      float BC = htmp->GetBinContent(htmp->FindBin(3.0));

      res_fvtxs_nint[ie][ic] = TMath::Sqrt(AB * AC / BC);
      res_bbcs_nint[ie][ic] = TMath::Sqrt(AB * BC / AC);

      cout << " E:" << energy[ie]
           << " c:" << ic
           << " R(FVTXS):" << res_fvtxs_nint[ie][ic]
           << " R(BBCS):" << res_bbcs_nint[ie][ic]
           << endl;

      v2ep_nint_pt[ie][ic]->Scale(1. / res_fvtxs_nint[ie][ic]);
      v2ep_nint_eta[ie][ic]->Scale(1. / res_bbcs_nint[ie][ic]);

      delete ptmp;
      delete htmp;

    } // ic

    fin->Close();
    delete fin;

  } // ie


  //==========================================================================//
  //==========================================================================//
  //==========================================================================//
  // P+AU
  //==========================================================================//
  //==========================================================================//
  //==========================================================================//
  cout << endl;
  cout << endl;
  cout << "--------------------" << endl;
  cout << "--- p+Au 200 GeV ---" << endl;
  cout << "--------------------" << endl;
  cout << endl;
  cout << endl;

  //==========================================================================//
  // READ FROM CUMULANTS FILE
  //==========================================================================//
  cout << endl;
  cout << "--> Reading p+Au cumulants files" << endl;

  fin = TFile::Open(pauCumFile);
  if (!fin)
  {
    cout << "ERROR!! Unable to open " << pauCumFile << endl;
    return;
  }
  cout << "----> Successfully read " << pauCumFile
       << " for pAu 200 GeV" << endl;


  TProfile* tp1f_raa6 = (TProfile*) fin->Get("raa6_Ncharge");
  TProfile* tp1f_raa4 = (TProfile*) fin->Get("raa4_Ncharge");
  TProfile* tp1f_raa2 = (TProfile*) fin->Get("raa2_Ncharge");
  TProfile* tp1f_gapp = (TProfile*) fin->Get("comp_Ncharge");

  tp1f_raa6->Rebin(rebin);
  tp1f_raa4->Rebin(rebin);
  tp1f_raa2->Rebin(rebin);
  tp1f_gapp->Rebin(rebin);

  TH1D* th1d_raa6 = (TH1D*) tp1f_raa6->ProjectionX("th1d_raa6");
  TH1D* th1d_raa4 = (TH1D*) tp1f_raa4->ProjectionX("th1d_raa4");
  TH1D* th1d_raa2 = (TH1D*) tp1f_raa2->ProjectionX("th1d_raa2");
  TH1D* th1d_gapp = (TH1D*) tp1f_gapp->ProjectionX("th1d_gapp");

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
  cum_pau200_c26 = (TH1D*)th1d_aa6->Clone("cum_pau200_c26");
  cum_pau200_c26->SetDirectory(0);
  cum_pau200_c26->Add(th1d_aa4, -1.0);
  cum_pau200_c26->Add(th1d_aa2, 1.0);

  // calculate v_{2}{6} = (1/4*c26)^{1/6}
  cum_pau200_v26 = (TH1D*)cum_pau200_c26->Clone("cum_pau200_v26");
  cum_pau200_v26->SetDirectory(0);
  for (int i = 1; i <= cum_pau200_v26->GetNbinsX(); i++)
  {
    double temp_1 = cum_pau200_c26->GetBinContent(i);
    double erro_1 = cum_pau200_c26->GetBinError(i);

    if (temp_1 <= 0)
    {
      cum_pau200_v26->SetBinContent(i, 0);
      cum_pau200_v26->SetBinError(i, 0);
    }
    else
    {
      double temp_2 = pow(0.25 * temp_1, 1. / 6.);
      // double erro_2 = (1./6.) * erro_1 * abs(temp_2) / (0.25*abs(temp_1));
      double erro_2 = pow(0.25 * erro_1, 1 / 6.);
      cum_pau200_v26->SetBinContent(i, temp_2);
      // cum_pau200_v26->SetBinError(i, erro_2);
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
  cum_pau200_c24 = (TH1D*)th1d_raa4->Clone("cum_pau200_c24");
  cum_pau200_c24->SetDirectory(0);
  cum_pau200_c24->Add(th1d_222, -1.0);

  // calculate v_{2}{4} = (c_{2}{4})^{1/4}
  cum_pau200_v24 = (TH1D*)cum_pau200_c24->Clone("cum_pau200_v24");
  cum_pau200_v24->SetDirectory(0);
  for (int i = 1; i <= cum_pau200_v24->GetNbinsX(); i++)
  {
    double temp_1 = cum_pau200_v24->GetBinContent(i);
    double erro_1 = cum_pau200_v24->GetBinError(i);

    if (temp_1 >= 0)
    {
      cum_pau200_v24->SetBinContent(i, 0);
      cum_pau200_v24->SetBinError(i, 0);
    }
    else
    {
      double temp_2 = pow(-temp_1, 0.25);
      double erro_2 = 0.25 * erro_1 * abs(temp_2) / abs(temp_1);
      cum_pau200_v24->SetBinContent(i, temp_2);
      cum_pau200_v24->SetBinError(i, erro_2);
    }
  }

  //==========================================================================//
  // CALCULATE V2{2}
  //==========================================================================//

  cum_pau200_v22 = (TH1D*)th1d_raa2->Clone("cum_pau200_v22");
  cum_pau200_v22->SetDirectory(0);
  for (int i = 1; i <= cum_pau200_v22->GetNbinsX(); i++)
  {
    double temp_1 = cum_pau200_v22->GetBinContent(i);
    double erro_1 = cum_pau200_v22->GetBinError(i);

    if (temp_1 <= 0) cum_pau200_v22->SetBinContent(i, -9999);
    else
    {
      double temp_2 = pow(temp_1, 0.5);
      double erro_2 = 0.5 * erro_1 * abs(temp_2) / abs(temp_1);
      cum_pau200_v22->SetBinContent(i, temp_2);
      cum_pau200_v22->SetBinError(i, erro_2);
    }
  }

  //==========================================================================//
  // CALCULATE V2{2, GAP}
  //==========================================================================//

  cum_pau200_v22gap = (TH1D*)th1d_gapp->Clone("cum_pau200_v22gap");
  cum_pau200_v22gap->SetDirectory(0);
  for (int i = 1; i <= cum_pau200_v22gap->GetNbinsX(); i++)
  {
    double temp_1 = cum_pau200_v22gap->GetBinContent(i);
    double erro_1 = cum_pau200_v22gap->GetBinError(i);

    if (temp_1 <= 0) cum_pau200_v22gap->SetBinContent(i, -9999);
    else
    {
      double temp_2 = pow(temp_1, 0.5);
      double erro_2 = 0.5 * erro_1 * abs(temp_2) / abs(temp_1);
      cum_pau200_v22gap->SetBinContent(i, temp_2);
      cum_pau200_v22gap->SetBinError(i, erro_2);
    }
  }




  fin->Close();
  delete fin;







  //==========================================================================//
  //==========================================================================//
  //==========================================================================//
  // SAVE TO FILE
  //==========================================================================//
  //==========================================================================//
  //==========================================================================//
  if ( saveFile )
  {
    cout << endl;
    cout << "--> Writing results to " << outFile << endl;

    TFile *fout = new TFile(outFile, "RECREATE");

    fout->cd();

    //-- d+Au BES
    for (int ie = 0; ie < NE; ie++)
    {
      //-- parton & event plane results
      for (int ic = 0; ic < NCENT[ie]; ic++)
      {
        v2pp_pt[ie][ic]->Write(Form("tprofile_AMPTdAu%i_c%i_v2pt_pp", energy[ie], ic));
        v2ep_pt[ie][ic]->Write(Form("tprofile_AMPTdAu%i_c%i_v2pt_ep", energy[ie], ic));

        v2pp_eta[ie][ic]->Write(Form("tprofile_AMPTdAu%i_c%i_v2eta_pp", energy[ie], ic));
        v2ep_eta[ie][ic]->Write(Form("tprofile_AMPTdAu%i_c%i_v2eta_ep", energy[ie], ic));

        v2pp_nint_pt[ie][ic]->Write(Form("tprofile_AMPTdAu%i_c%i_v2pt_pp_nint", energy[ie], ic));
        v2ep_nint_pt[ie][ic]->Write(Form("tprofile_AMPTdAu%i_c%i_v2pt_ep_nint", energy[ie], ic));

        v2pp_nint_eta[ie][ic]->Write(Form("tprofile_AMPTdAu%i_c%i_v2eta_pp_nint", energy[ie], ic));
        v2ep_nint_eta[ie][ic]->Write(Form("tprofile_AMPTdAu%i_c%i_v2eta_ep_nint", energy[ie], ic));

        dnchdeta[ie][ic]->Write(Form("th1d_AMPTdAu%i_c%i_dndeta", energy[ie], ic));
      } // ic

      //-- cumulants
      cum_c24[ie]->Write(Form("th1d_AMPTdAu%i_c24", energy[ie]));
      cum_c26[ie]->Write(Form("th1d_AMPTdAu%i_c26", energy[ie]));

      cum_v22gap[ie]->Write(Form("th1d_AMPTdAu%i_v22gap", energy[ie]));
      cum_v22[ie]->Write(Form("th1d_AMPTdAu%i_v22", energy[ie]));
      cum_v24[ie]->Write(Form("th1d_AMPTdAu%i_v24", energy[ie]));
      cum_v26[ie]->Write(Form("th1d_AMPTdAu%i_v26", energy[ie]));
    } // ie

    cum_pau200_c24->Write("th1d_AMPTpAu200_c24");
    cum_pau200_c26->Write("th1d_AMPTpAu200_c26");

    cum_pau200_v22gap->Write("th1d_AMPTpAu200_v22gap");
    cum_pau200_v22->Write("th1d_AMPTpAu200_v22");
    cum_pau200_v24->Write("th1d_AMPTpAu200_v24");
    cum_pau200_v26->Write("th1d_AMPTpAu200_v26");


    fout->Close();
    delete fout;
  }




}
