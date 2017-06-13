////////////////////////////////////////////////////////////////////////////////
//
// Plot the v2 vs pT and eta from AMPT in bins of centrality
//
////////////////////////////////////////////////////////////////////////////////
//
// Darren McGlinchey
// 4 May 2017
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

void plot_v2pteta()
{
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  //==========================================================================//
  // SET RUNNING CONDITIONS
  //==========================================================================//

  const int NE = 4;
  int energy[] = {200, 62, 39, 20};
  const char* epFile[] =
  {
    // "rootfiles/eplane_ampt_dau200_nint_0.root",
    "rootfiles/eplane_ampt_dau200_20-29.root",
    "rootfiles/eplane_ampt_dau62_20-24.root",
    "rootfiles/eplane_ampt_dau39_20-24.root",
    "rootfiles/eplane_ampt_dau20_20-23.root",
  };
  const char* ppFile[] =
  {
    // "rootfiles/pplane_ampt_dau200_nint_0.root",
    "rootfiles/pplane_ampt_dau200_20-29.root",
    "rootfiles/pplane_ampt_dau62_20-24.root",
    "rootfiles/pplane_ampt_dau39_20-24.root",
    "rootfiles/pplane_ampt_dau20_20-23.root",
  };

  const int NCENTMAX = 6;
  int NCENT[] = {6, 6, 5, 4};

  bool saveFile = false;
  const char* outFile = "AMPT_sigparton075_test.root";

  //==========================================================================//
  // DECLARE VARIABLES
  //==========================================================================//

  //-- parton plane ... v2{PP}
  TProfile *v2pp_pt[NE][NCENTMAX];
  TProfile *v2pp_eta[NE][NCENTMAX];

  //-- event plane ... v2{EP}
  TProfile *v2ep_pt[NE][NCENTMAX];
  TProfile *v2ep_eta[NE][NCENTMAX];

  //-- event plane resolutions
  double res_fvtxs[NE][NCENTMAX];
  double res_bbcs[NE][NCENTMAX];

  //-- dNch/deta
  TH1D* dnchdeta[NE][NCENTMAX];

  //-- temp
  TFile *fin;
  char name[500];
  TProfile *ptmp;
  TH1D* htmp;

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
    TFile *fin = TFile::Open(ppFile[ie]);
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
    TFile *fin = TFile::Open(epFile[ie]);
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
  // PLOT OBJECTS
  //==========================================================================//
  cout << endl;
  cout << "--> Plotting" << endl;

  TH1D* haxis_vnptcent = new TH1D("haxis_vnptcent", ";p_{T} [GeV/c]; v_{2}^{raw}",
                                  // 300, 0.01, 2.99);
                                  500, 0.01, 5.09);
  haxis_vnptcent->SetMinimum(0.0);
  haxis_vnptcent->SetMaximum(0.499);

  TH1D* haxis_vneta = new TH1D("haxis_vneta", ";#eta; v_{2}",
                               // 62, -3.9, 3.9);
                               62, -3.2, 3.2);
  haxis_vneta->SetMinimum(0.0);
  haxis_vneta->SetMaximum(0.089);

  TH1D* haxis_dndeta = new TH1D("haxis_dndeta", ";#eta; dN_{ch}/d#eta",
                               // 62, -3.9, 3.9);
                               62, -3.2, 3.2);
  haxis_dndeta->SetMinimum(0.0);
  haxis_dndeta->SetMaximum(40);


  //==========================================================================//
  // PLOT
  //==========================================================================//

  //-- v2 pT
  TCanvas *cv2pt[NE];
  for (int ie = 0; ie < NE; ie++)
  {
    cv2pt[ie] = new TCanvas(Form("cv2pt_%i", ie),
                            Form("v2 pt %i", energy[ie]),
                            1200, 1000);
    cv2pt[ie]->SetMargin(0, 0, 0, 0);
    cv2pt[ie]->Divide(3, 2, 0, 0);

    for (int ic = 0; ic < NCENT[ie]; ic++)
    {
      cv2pt[ie]->GetPad(ic + 1)->SetMargin(0.1, 0.02, 0.1, 0.02);
      cv2pt[ie]->GetPad(ic + 1)->SetTicks(1, 1);
      cv2pt[ie]->cd(ic + 1);
      haxis_vnptcent->DrawCopy();

      v2pp_pt[ie][ic]->Draw("same");
      v2ep_pt[ie][ic]->Draw("same");
    } // ic
  } // ie

  //-- v2 pT
  TCanvas *cv2eta[NE];
  for (int ie = 0; ie < NE; ie++)
  {
    cv2eta[ie] = new TCanvas(Form("cv2eta_%i", ie),
                            Form("v2 eta %i", energy[ie]),
                            1200, 1000);
    cv2eta[ie]->SetMargin(0, 0, 0, 0);
    cv2eta[ie]->Divide(3, 2, 0, 0);

    for (int ic = 0; ic < NCENT[ie]; ic++)
    {
      cv2eta[ie]->GetPad(ic + 1)->SetMargin(0.1, 0.02, 0.1, 0.02);
      cv2eta[ie]->GetPad(ic + 1)->SetTicks(1, 1);
      cv2eta[ie]->cd(ic + 1);
      haxis_vneta->DrawCopy();

      v2pp_eta[ie][ic]->Draw("same");
      v2ep_eta[ie][ic]->Draw("same");
    } // ic
  } // ie


  //-- dNch/deta
  TCanvas *cdndeta[NE];
  for (int ie = 0; ie < NE; ie++)
  {
    cdndeta[ie] = new TCanvas(Form("cdndeta_%i", ie),
                            Form("dNch/deta %i", energy[ie]),
                            1200, 1000);
    cdndeta[ie]->SetMargin(0, 0, 0, 0);
    cdndeta[ie]->Divide(3, 2, 0, 0);

    for (int ic = 0; ic < NCENT[ie]; ic++)
    {
      cdndeta[ie]->GetPad(ic + 1)->SetMargin(0.1, 0.02, 0.1, 0.02);
      cdndeta[ie]->GetPad(ic + 1)->SetTicks(1, 1);
      cdndeta[ie]->cd(ic + 1);
      haxis_dndeta->DrawCopy();

      dnchdeta[ie][ic]->Draw("same");
      dnchdeta[ie][ic]->Draw("same");
    } // ic
  } // ie

  //==========================================================================//
  // SAVE TO FILE
  //==========================================================================//
  if ( saveFile )
  {
    TFile *fout = new TFile(outFile, "RECREATE");

    fout->cd();
    for (int ie = 0; ie < NE; ie++)
    {
      for (int ic = 0; ic < NCENT[ie]; ic++)
      {
        v2pp_pt[ie][ic]->Write(Form("tprofile_AMPTdAu%i_c%i_v2pt_pp", energy[ie], ic));
        v2ep_pt[ie][ic]->Write(Form("tprofile_AMPTdAu%i_c%i_v2pt_ep", energy[ie], ic));

        v2pp_eta[ie][ic]->Write(Form("tprofile_AMPTdAu%i_c%i_v2eta_pp", energy[ie], ic));
        v2ep_eta[ie][ic]->Write(Form("tprofile_AMPTdAu%i_c%i_v2eta_ep", energy[ie], ic));

        dnchdeta[ie][ic]->Write(Form("th1d_AMPTdAu%i_c%i_dndeta", energy[ie], ic));
      } // ic
    } // ie
  }
}