//-----------------------------------------------
// Event Plane Method
//
// 3-sub event method resolution from:
// BBC, FVTXS, CNT particles.
//
// Author: P. Yin
//-----------------------------------------------
// Modified: D. McGlinchey
//-----------------------------------------------

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

#include "TLorentzVector.h"
#include "TFile.h"
#include "TStyle.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TF1.h"
#include "TRandom3.h"

#include "range.h"
#include "consts.h"

using namespace std;

//------------------------------------------------------------------------------
// Function Prototypes

void processEvent(int index);
void parseampt(int energyidx);

bool test_eff_s(float pT, float eta);
bool test_eff_n(float pT, float eta);


//------------------------------------------------------------------------------
//Global variables declarations

vector<particle> hadrons;
vector<particle> pA;
vector<particle> pB;
vector<particle> pC;

TProfile* v2ep_fvtxs_pt[NCENT];
TProfile* v2ep_bbcs_pt[NCENT];
TProfile* v2ep_fvtxs_eta[NCENT];
TProfile* v2ep_bbcs_eta[NCENT];
TProfile* res_comp[NCENT];
TH1D* nch_eta[NCENT];
TH1D* nch_bbcs[NCENT];

TH1F* dhis_bbcs;
TH1F* hcount;

//------------------------------------------------------------------------------
void event_plane(int energy = 200)
{

  // get the energy index for defining centrality limits
  int eidx = -1;
  if ( energy == 200)
    eidx = 0;
  else if ( energy == 62 )
    eidx = 1;
  else if ( energy == 39 )
    eidx = 2;
  else if ( energy == 20 )
    eidx = 3;
  else
  {
    cout << "WARNING! Centrality limits are not defined for input "
         << "energy=" << energy << " ... using binning for 200 GeV ..."
         << endl;
    eidx = 0;
  }

  // centrality dependence
  for (int i = 0; i < NCENT; i++)
  {
    //CNT v2 vs pT w/ FVTXS EP
    v2ep_fvtxs_pt[i] = new TProfile(Form("v2ep_fvtxs_pt_%i", i), ";p_{T} [GeV/c];v_{2}",
                                    NPTBINS, ptlim, -1.1, 1.1);

    //CNT v2 vs pT w/ BBCS EP
    v2ep_bbcs_pt[i] = new TProfile(Form("v2ep_bbcs_pt_%i", i), ";p_{T} [GeV/c];v_{2}",
                                   NPTBINS, ptlim, -1.1, 1.1);

    //CNT v2 vs eta w/ FVTXS EP
    v2ep_fvtxs_eta[i] = new TProfile(Form("v2ep_fvtxs_eta_%i", i), ";#eta;v_{2}",
                                     NETABINS, etalo, etahi, -1.1, 1.1);

    //CNT v2 vs eta w/ BBCS EP
    v2ep_bbcs_eta[i] = new TProfile(Form("v2ep_bbcs_eta_%i", i), ";#eta;v_{2}",
                                    NETABINS, etalo, etahi, -1.1, 1.1);

    // resolution calculation
    res_comp[i] = new TProfile(Form("res_comp_%i", i), "", 3, 0.5, 3.5, -1.0, 1.0);

    // <N_ch> vs eta
    nch_eta[i] = new TH1D(Form("nch_eta_%i", i), ";#eta;N_{ch}",
                                    200, -5.0, 5.0);

    // dist of N_ch in BBCS 
    nch_bbcs[i] = new TH1D(Form("nch_bbcs_%i", i), ";N_{ch}",
                                    NNCHBINS, nchlo, nchhi);

  } // i




  dhis_bbcs = new TH1F("dhis_bbcs", "dhis_bbcs", 200, -0.5, 199.5);

  hcount = new TH1F("hcount", "count", 10, -0.5, 9.5);


  // run over the data
  parseampt(eidx);



  //Make a file to store outputs
  TFile *fout = new TFile("out_EP.root", "RECREATE");

  //Save graphs
  for (int i = 0; i < NCENT; i++)
  {
    v2ep_fvtxs_pt[i]->Write();
    v2ep_bbcs_pt[i]->Write();
    v2ep_fvtxs_eta[i]->Write();
    v2ep_bbcs_eta[i]->Write();
    res_comp[i]->Write();
    nch_eta[i]->Write();
    nch_bbcs[i]->Write();
  }
  hcount->Write();
  dhis_bbcs->Write();

  fout->Close();
}

//------------------------------------------------------------------------------
void parseampt(int energyidx)
{
  //Read in data file
  ifstream dataFile;
  dataFile.open("ana/ampt.dat");

  //Skip the job if not dataFile
  if (!dataFile)
  {
    cout << "-->No such data file" << endl << endl;
    return;
  }
  else cout << "--> Successfully open the data file!" << endl << endl;

  //In this while loop, program will read the data file line by line
  while (dataFile)
  {
    int    evtnumber;
    int    testnum;
    int    nlist;
    double impactpar;
    int    npartproj;
    int    nparttarg;
    int    npartprojelas;
    int    npartprojinelas;
    int    nparttargelas;
    int    nparttarginelas;
    double junk;

    int ct_bbcs = 0;

    //Get the header of each event
    dataFile >> evtnumber >> testnum >> nlist >> impactpar >> npartproj >> nparttarg >> npartprojelas >> npartprojinelas >> nparttargelas >> nparttarginelas >> junk;

    if (!dataFile) break;

    //Analysis each particle in the event
    for (int i = 0; i < nlist; i++)
    {
      int    partid;
      float  pv[3];
      float  mass;
      double space[4];

      dataFile >> partid >> pv[0] >> pv[1] >> pv[2] >> mass >> space[0] >> space[1] >> space[2] >> space[3];

      //Skip non-charged particles that we are not interested
      //+-211 --> pions,  +-321 --> kaons, +-2212 --> protons
      if (abs(partid) != 211 && abs(partid) != 321 && abs(partid) != 2212) continue;

      if (TMath::Sqrt(pv[0] * pv[0] + pv[1] * pv[1]) < 0.0001) continue;


      //Calculate the energy
      float energy = TMath::Sqrt(pv[0] * pv[0] + pv[1] * pv[1] + pv[2] * pv[2] + mass * mass);

      //Make Lorentz vector
      TLorentzVector ev(pv[0], pv[1], pv[2], energy);

      //Get pT, phi, pseudorapidity, particle id, px, py, and pz. Store them into p.
      particle p;

      p.eta = ev.Eta();
      p.pT  = ev.Pt();
      p.phi = ev.Phi();
      p.px  = pv[0];
      p.py  = pv[1];
      p.pz  = pv[2];

      hadrons.push_back(p);

      //mid-rapidity
      if ( ifCNT(p.eta) )
      {
        pC.push_back(p);
      }

      //BBCS
      if ( ifBBCS(p.eta) )
      {
        pB.push_back(p);
        ct_bbcs++;
        hcount->Fill(0);
      }

      //FVTXS
      if ( ifFVTXS(p.eta) )
      {
        pA.push_back(p);
        hcount->Fill(1);
      }
    }

    dhis_bbcs->Fill(ct_bbcs);

    // get the centrality bin
    int index = -1;
    for (int icent = 0; icent < NCENT; icent++)
    {
      if ( ct_bbcs >= centlim[energyidx][icent])
      {
        index = icent;
        break;
      }
    }
    if (index >= 0)
    {
      processEvent(index);
      hcount->Fill(index + 3);
    }
    else
    {
      cout << "WARNING!! Unable to find index for ct_bbcs=" << ct_bbcs << endl;
    }

    hadrons.clear();
    pA.clear();
    pB.clear();
    pC.clear();

    hcount->Fill(2);

    if (!dataFile) break;
  }
}

//------------------------------------------------------------------------------
void  processEvent(int index)
{
  if (pA.size() == 0) return;

  float qxA = 0;
  float qyA = 0;
  float qxB = 0;
  float qyB = 0;
  float qxC = 0;
  float qyC = 0;
  float psiA = 0;
  float psiB = 0;
  float psiC = 0;

  for (unsigned int i = 0; i < pA.size(); i++)
  {
    qxA += TMath::Cos(2 * pA[i].phi);
    qyA += TMath::Sin(2 * pA[i].phi);
  }

  qxA = qxA / pA.size();
  qyA = qyA / pA.size();
  psiA = TMath::ATan2(qyA, qxA) / 2;

  for (unsigned int i = 0; i < pB.size(); i++)
  {
    qxB += TMath::Cos(2 * pB[i].phi);
    qyB += TMath::Sin(2 * pB[i].phi);
  }

  qxB = qxB / pB.size();
  qyB = qyB / pB.size();
  psiB = TMath::ATan2(qyB, qxB) / 2;

  for (unsigned int i = 0; i < pC.size(); i++)
  {
    qxC += TMath::Cos(2 * pC[i].phi);
    qyC += TMath::Sin(2 * pC[i].phi);
  }

  qxC = qxC / pC.size();
  qyC = qyC / pC.size();
  psiC = TMath::ATan2(qyC, qxC) / 2;

  // v2 vs pT using FVTXS event plane for CNT tracks
  for (unsigned int i = 0; i < pC.size(); i++)
  {
    float v2 = TMath::Cos(2 * (pC.at(i).phi - psiA));
    v2ep_fvtxs_pt[index]->Fill(pC.at(i).pT, v2);
  }

  // v2 vs eta using FVTXS event plane
  for (unsigned int i = 0; i < hadrons.size(); i++)
  {
    float v2 = TMath::Cos(2 * (hadrons.at(i).phi - psiA));
    v2ep_fvtxs_eta[index]->Fill(hadrons.at(i).eta, v2);
  }

  // v2 vs pT using bbcs event plane for CNT tracks
  for (unsigned int i = 0; i < pC.size(); i++)
  {
    float v2 = TMath::Cos(2 * (pC.at(i).phi - psiB));
    v2ep_bbcs_pt[index]->Fill(pC.at(i).pT, v2);
  }

  // v2 vs eta using bbcs event plane
  for (unsigned int i = 0; i < hadrons.size(); i++)
  {
    float v2 = TMath::Cos(2 * (hadrons.at(i).phi - psiB));
    v2ep_bbcs_eta[index]->Fill(hadrons.at(i).eta, v2);
  }

  // dNch/deta
  for (unsigned int i = 0; i < hadrons.size(); i++)
  {
    nch_eta[index]->Fill(hadrons.at(i).eta);
  }

  nch_bbcs[index]->Fill(pB.size());

  res_comp[index]->Fill(1.0, TMath::Cos(2 * (psiA - psiB)));
  res_comp[index]->Fill(2.0, TMath::Cos(2 * (psiA - psiC)));
  res_comp[index]->Fill(3.0, TMath::Cos(2 * (psiB - psiC)));
}

