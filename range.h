#ifndef RANGE_H
#define RANGE_H

#include <iostream>
#include <cmath>
#include <TH2.h>
#include <TFile.h>
#include <TRandom3.h>

using namespace std;

// FVTXS
// -3.0 < eta < -1.0
bool ifFVTXS(double eta)
{
  if (eta > -3.0 && eta < -1.0) return true;
  else return false;
}

// FVTXN
// 1.0 < eta < 3.0
bool ifFVTXN(double eta)
{
  if (eta > 1.0 && eta < 3.0) return true;
  else return false;
}

// FVTX
// 1.0 < eta < 3.0  or  -3.0 < eta < -1.0
bool ifFVTX(double eta)
{
  if (ifFVTXS(eta) || ifFVTXN(eta)) return true;
  else return false;
}

// BBCS
// -3.9 < eta < -3.1
bool ifBBCS(double eta)
{
  if (eta > -3.9 && eta < -3.1) return true;
  else return false;
}

// BBCN
// 3.1 < eta < 3.9
bool ifBBCN(double eta)
{
  if (eta > 3.1 && eta < 3.9) return true;
  else return false;
}

// CNT
// -0.35 < eta < 0.35
bool ifCNT(double eta)
{
  if (eta > -0.35 && eta < 0.35) return true;
  else return false;
}

// FVTXS pT dependent efficiency
TH2D *eff_fvtx_s;
TRandom3 *rndm_s;
bool test_eff_s(float pT, float eta)
{

  //-------
  //-- pT dependent efficiency only
  //-------
  // // first check if the efficiency histogram already exists
  // if ( !eff_fvtx_s )
  // {
  //   TFile *f_fvtxs = new TFile("fvtx_acc.root");

  //   eff_fvtx_s = (TH2D*)f_fvtxs->Get("rh1");
  //   eff_fvtx_s->SetName("eff_fvtx_s");
  //   eff_fvtx_s->SetDirectory(0);
  //   eff_fvtx_s->Scale(1. / eff_fvtx_s->GetMaximum());

  //   f_fvtxs->Close();
  //   delete f_fvtxs;

  //   // intialize random 
  //   rndm_s = new TRandom3(0);
  // }

  // int pTbin = eff_fvtx_s->GetXaxis()->FindBin(pT);

  // float n = 1.;
  // if ( pTbin <= eff_fvtx_s->GetNbinsX() )
  //   n = eff_fvtx_s->GetBinContent(pTbin);

  // float test = rndm_s->Rndm();

  // if (test < n && ifFVTXS(eta)) return true;
  // else return false;


  //-------
  //-- pT & eta dependent efficiency
  //-------
  // first check if the efficiency histogram already exists
  if ( !eff_fvtx_s )
  {
    TFile *f_fvtxs = new TFile("fvtx_eff_pteta.root");

    eff_fvtx_s = (TH2D*)f_fvtxs->Get("hpteta_eff_rebin");
    eff_fvtx_s->SetName("eff_fvtx_s");
    eff_fvtx_s->SetDirectory(0);

    f_fvtxs->Close();
    delete f_fvtxs;

    // intialize random 
    rndm_s = new TRandom3(0);
  }

  int etaBin = eff_fvtx_s->GetXaxis()->FindBin(eta);
  int ptBin = eff_fvtx_s->GetYaxis()->FindBin(pT);

  // check for outside eta range
  if ( etaBin < 1 || etaBin > eff_fvtx_s->GetNbinsX() )
    return false;

  float n = 1.;
  if ( ptBin <= eff_fvtx_s->GetNbinsY() )
    n = eff_fvtx_s->GetBinContent(etaBin, ptBin);

  float test = rndm_s->Rndm();

  if ( test < n ) return true;
  else return false;

}

// FVTXN pT dependent efficiency
TH2D *eff_fvtx_n;
TRandom3 *rndm_n;
bool test_eff_n(float pT, float eta)
{
  //-------
  //-- pT dependent efficiency only
  //-------
  // // first check if the efficiency histogram already exists
  // if ( !eff_fvtx_n )
  // {
  //   TFile *f_fvtxn = new TFile("fvtx_acc_n.root");

  //   eff_fvtx_n = (TH2D*)f_fvtxn->Get("rh1");
  //   eff_fvtx_n->SetName("eff_fvtx_n");
  //   eff_fvtx_n->SetDirectory(0);
  //   eff_fvtx_n->Scale(1. / eff_fvtx_n->GetMaximum());

  //   f_fvtxn->Close();
  //   delete f_fvtxn;

  //   // intialize random 
  //   rndm_n = new TRandom3(0);
  // }

  // int pTbin = eff_fvtx_n->GetXaxis()->FindBin(pT);

  // float n = 1.;
  // if ( pTbin <= eff_fvtx_n->GetNbinsX() )
  //   n = eff_fvtx_n->GetBinContent(pTbin);

  // float test = rndm_n->Rndm();

  // if (test < n && ifFVTXN(eta)) return true;
  // else return false;


  //-------
  //-- pT & eta dependent efficiency
  //-------
  // first check if the efficiency histogram already exists
  if ( !eff_fvtx_s )
  {
    TFile *f_fvtxs = new TFile("fvtx_eff_pteta.root");

    eff_fvtx_s = (TH2D*)f_fvtxs->Get("hpteta_eff_rebin");
    eff_fvtx_s->SetName("eff_fvtx_s");
    eff_fvtx_s->SetDirectory(0);

    f_fvtxs->Close();
    delete f_fvtxs;

    // intialize random 
    rndm_s = new TRandom3(0);
  }

  int etaBin = eff_fvtx_s->GetXaxis()->FindBin(eta);
  int ptBin = eff_fvtx_s->GetYaxis()->FindBin(pT);

  // check for outside eta range
  if ( etaBin < 1 || etaBin > eff_fvtx_s->GetNbinsX() )
    return false;

  float n = 1.;
  if ( ptBin <= eff_fvtx_s->GetNbinsY() )
    n = eff_fvtx_s->GetBinContent(etaBin, ptBin);

  float test = rndm_s->Rndm();

  if ( test < n ) return true;
  else return false;


}


#endif