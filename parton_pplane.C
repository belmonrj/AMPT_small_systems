//-----------------------------------------------
// Parton Participant Plane Method
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

bool test_eff_s(float pT, float eta);
bool test_eff_n(float pT, float eta);

void parse_afterPropagation_file();
void parse_ampt_file(int energyidx);

void  processEvent_melting();
void  processEvent_ampt(int evtnumber, int index);



//------------------------------------------------------------------------------
//Global variables declarations
int event_counter = 0;

//------------------------------------------------------------------------------
//Vector declarations
vector<particle> partons;

vector<float> psi2;
vector<float> ep2;

vector<particle> hadrons;
vector<particle> hadrons_CNT;
vector<particle> hadrons_FVTX;

//------------------------------------------------------------------------------
//Graphs declarations

TProfile* v2pp_nch;
TProfile* v2pp_pt[NCENT];
TProfile* v2pp_eta[NCENT];

TProfile* epsilon2_nch;

TH1F *epsilon2_dis;
TH1F *dhis_v2;
TH1F *dhis_qn;
TH1F *dhis_b;
TH1F *dhis_eta;

TH1F *nch;
TH1F *nch_CNT;
TH1F *nch_FVTX;

//-----------------------------------------------------------------------------------
void parton_pplane(int energy = 200)
{
  TFile *fout = new TFile("ppplane.root", "RECREATE");

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


  // FVTX v2 vs Nch
  v2pp_nch = new TProfile("v2pp_nch", "v2{PP}", NNCHBINS, nchlo, nchhi, -1.1, 1.1);
  epsilon2_nch = new TProfile("epsilon2_nch", "epsilon2_nch", NNCHBINS, nchlo, nchhi, -10, 10); //

  // CNT v2 vs pT
  for (int i = 0; i < NCENT; i++)
  {
    v2pp_pt[i] = new TProfile(Form("v2pp_pt_%i", i), ";p_{T} [GeV/c];v_{2}",
                              NPTBINS, ptlim, -1.1, 1.1);
  }

  // v2 vs eta
  for (int i = 0; i < NCENT; i++)
  {
    v2pp_eta[i] = new TProfile(Form("v2pp_eta_%i", i), ";#eta;v_{2}",
                               NETABINS, etalo, etahi, -1.1, 1.1);
  }


  dhis_v2 = new TH1F("dhis_v2", "dhis_v2", 200, -1, 1);
  epsilon2_dis = new TH1F("epsilon2_dis", "epsilon2_dis", 100, -0.01, 0.99);
  dhis_qn = new TH1F("dhis_qn", "dhis_qn", 200, 0, 20);
  dhis_b = new TH1F("dhis_b", "dhis_b", 50, 0, 20);
  dhis_eta = new TH1F("dhis_eta", "dhis_eta", 100, -10, 10);


  nch = new TH1F("nch", ";N_{charge}", NNCHBINS, nchlo, nchhi);
  nch_CNT = new TH1F("nch_CNT", ";N_{charge}", NNCHBINS, nchlo, nchhi);
  nch_FVTX = new TH1F("nch_FVTX", ";N_{charge}", NNCHBINS, nchlo, nchhi);


  parse_afterPropagation_file();
  parse_ampt_file(eidx);

  fout->Write();
  fout->Close();
}


//------------------------------------------------------------------------------
// Parse the AMPT parton information
//   called from parton_pplane()
//   calls processEvent_melting()
void parse_afterPropagation_file()
{
  //Read in data file
  ifstream partonFile;
  //partonFile.open("parton-initial-afterPropagation.dat");
  partonFile.open("ana/parton-initial-afterPropagation.dat");

  //Skip the job if not partonFile
  if (!partonFile)
  {
    cout << "-->No such data file" << endl << endl;
    return;
  }
  else cout << "--> Successfully open the afterPropagation file!" << endl << endl;

  //In this while loop, program will read the data file line by line
  while (partonFile)
  {
    int    evtnumber;
    int    iteration;
    int    nlist;
    int    n_baryon_formed;
    int    n_meson_formed;
    int    n_inipart;
    int    n_inipart_notinzpc;

    //Get the header of each event
    partonFile >> evtnumber >> iteration >> nlist >> n_baryon_formed >> n_meson_formed >> n_inipart >> n_inipart_notinzpc;

    if (!partonFile) break;

    //Analysis each particle in the event
    for (int i = 0; i < nlist; i++)
    {
      int    partid;
      float  pv[3];
      float  mass;
      double space[4];

      partonFile >> partid >> pv[0] >> pv[1] >> pv[2] >> mass >> space[0] >> space[1] >> space[2] >> space[3];

      if (TMath::Sqrt(pv[0] * pv[0] + pv[1] * pv[1]) < 0.0001) continue;

      //Calculate the energy
      float energy = TMath::Sqrt(pv[0] * pv[0] + pv[1] * pv[1] + pv[2] * pv[2] + mass * mass);

      //Make Lorentz vector
      TLorentzVector ev(pv[0], pv[1], pv[2], energy);

      //Get pT, phi, pseudorapidity, particle id, px, py, and pz. Store them into p.
      particle p;

      p.eta = ev.Eta();
      p.pT  = ev.Pt();
      p.px  = pv[0];
      p.py  = pv[1];
      p.pz  = pv[2];
      p.m   = mass;
      p.x   = space[0];
      p.y   = space[1];
      p.z   = space[2];
      p.t   = space[3];

      if (abs(p.eta) < 3 && p.t < 3) partons.push_back(p);
    }

    if (iteration > 0)
    {
      ep2.pop_back();
      psi2.pop_back();
    }

    processEvent_melting();

    partons.clear();
  }
}


//------------------------------------------------------------------------------
// calculate e2 and Psi2 from parton information for each event
//   called from parse_afterPropagation_file()
void  processEvent_melting()
{
  float cmx = 0;
  float cmy = 0;
  float qx2 = 0;
  float qx3 = 0;
  float qy2 = 0;
  float qy3 = 0;
  float aver2 = 0;

  for (unsigned int i = 0; i < partons.size(); i++)
  {
    cmx = cmx + partons[i].x;
    cmy = cmy + partons[i].y;
  }

  int count = partons.size();

  cmx = cmx / (float)count;
  cmy = cmy / (float)count;

  for (unsigned int i = 0; i < partons.size(); i++)
  {
    //Shift to center of mass frame
    partons[i].x = partons[i].x - cmx;
    partons[i].y = partons[i].y - cmy;

    partons[i].phi = TMath::ATan2(partons[i].y, partons[i].x);
    partons[i].rsquare = partons[i].x * partons[i].x + partons[i].y * partons[i].y;
  }

  for (unsigned int i = 0; i < partons.size(); i++)
  {
    qx2 = qx2 + partons[i].rsquare * TMath::Cos(2 * partons[i].phi);
    qy2 = qy2 + partons[i].rsquare * TMath::Sin(2 * partons[i].phi);

    qx3 = qx3 + partons[i].rsquare * TMath::Cos(3 * partons[i].phi);
    qy3 = qy3 + partons[i].rsquare * TMath::Sin(3 * partons[i].phi);

    aver2    = aver2 + partons[i].rsquare;
  }

  qx2 = qx2 / (float)count;
  qy2 = qy2 / (float)count;

  // qx3 = qx3 / (float)count;
  // qy3 = qy3 / (float)count;

  aver2    = aver2 / (float)count;

  float temp_psi2 = (TMath::ATan2(qy2, qx2) + TMath::Pi()) / 2;
  float e2 = TMath::Sqrt(qx2 * qx2 + qy2 * qy2) / aver2;

  epsilon2_dis->Fill(e2);

  ep2.push_back(e2);
  psi2.push_back(temp_psi2);
}



//------------------------------------------------------------------------------
// parse the AMPT hadron information file
//   called from from parton_pplane()
//   calls processEvent_ampt()
void parse_ampt_file(int energyidx)
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

    dhis_b->Fill(impactpar);

    //Analysis each particle in the event
    for (int i = 0; i < nlist; i++)
    {
      int    partid;
      float  pv[3];
      float  mass;
      double space[4];

      dataFile >> partid >> pv[0] >> pv[1] >> pv[2] >> mass >> space[0] >> space[1] >> space[2] >> space[3];

      //Skip non-charged particles that we are not interested
      //
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

      dhis_eta->Fill(p.eta);

      // count particles in BBCS region for centrality purposes
      if (ifBBCS(p.eta))
        ct_bbcs++;

      // save all particles (for eta dependence, so make loose cut)
      if ( fabs(p.eta) < 4)
        hadrons.push_back(p);

      // save particles in CNT
      if (ifCNT(p.eta))
        hadrons_CNT.push_back(p);

      // save particles in FVTX (S or N)
      // if ( ifFVTXS(p.eta) || ifFVTXN(p.eta) )
      if ( test_eff_s(p.pT, p.eta) || test_eff_n(p.pT, p.eta) )
        hadrons_FVTX.push_back(p);
    }

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
      processEvent_ampt(event_counter, index);
    }
    else
    {
      cout << "WARNING!! Unable to find index for ct_bbcs=" << ct_bbcs << endl;
    }

    // clear vectors
    hadrons.clear();
    hadrons_CNT.clear();
    hadrons_FVTX.clear();

    event_counter += 1;

    if (!dataFile) break;
  }
}

//------------------------------------------------------------------------------
// calculate event by event v2 from hadron information
//   called from parse_ampt_file()
void  processEvent_ampt(int evtnumber, int index)
{

  // calculate v2 vs eta
  nch->Fill(hadrons.size());
  for (unsigned int i = 0; i < hadrons.size(); i++)
  {
    float v2 = TMath::Cos(2 * (hadrons[i].phi - psi2[evtnumber]));
    v2pp_eta[index]->Fill(hadrons[i].eta, v2);
  }


  // calculate v2 vs N_{ch} in FVTX region (for cumulants)
  nch_FVTX->Fill(hadrons_FVTX.size());
  epsilon2_nch->Fill(hadrons_FVTX.size(), ep2[evtnumber]);
  for (unsigned int i = 0; i < hadrons_FVTX.size(); i++)
  {
    float v2 = TMath::Cos(2 * (hadrons_FVTX[i].phi - psi2[evtnumber]));
    v2pp_nch->Fill(hadrons_FVTX.size(), v2);
  }

  // Calculate v2 vs pT in CNT region
  nch_CNT->Fill(hadrons_CNT.size());
  for (unsigned int i = 0; i < hadrons_CNT.size(); i++)
  {
    float v2 = TMath::Cos(2 * (hadrons_CNT[i].phi - psi2[evtnumber]));
    v2pp_pt[index]->Fill(hadrons_CNT[i].pT, v2);

    dhis_v2->Fill(v2);
  }


  float cmx = 0;
  float cmy = 0;
  float qx2 = 0;
  float qy2 = 0;

  for (unsigned int i = 0; i < hadrons_CNT.size(); i++)
  {
    cmx = cmx + hadrons_CNT[i].x;
    cmy = cmy + hadrons_CNT[i].y;
  }

  int count = hadrons_CNT.size();

  cmx = cmx / (float)count;
  cmy = cmy / (float)count;

  for (unsigned int i = 0; i < hadrons_CNT.size(); i++)
  {
    //Shift to center of mass frame
    hadrons_CNT[i].x = hadrons_CNT[i].x - cmx;
    hadrons_CNT[i].y = hadrons_CNT[i].y - cmy;

    hadrons_CNT[i].phi = TMath::ATan2(hadrons_CNT[i].y, hadrons_CNT[i].x);
  }

  for (unsigned int i = 0; i < hadrons_CNT.size(); i++)
  {
    qx2 = qx2 + TMath::Cos(2 * hadrons_CNT[i].phi);
    qy2 = qy2 + TMath::Sin(2 * hadrons_CNT[i].phi);
  }

  float numerator = TMath::Sqrt(qx2 * qx2 + qy2 * qy2);
  float denominator = TMath::Sqrt(hadrons_CNT.size());

  float qn = numerator / denominator;

  dhis_qn->Fill(qn);
}

