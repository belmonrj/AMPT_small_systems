//-----------------------------------------------
// Multi-particle cumulant method
//
// 2,4,6-particle cumulant.
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
#include <TComplex.h>

#include "TLorentzVector.h"
#include "TFile.h"
#include "TStyle.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TF1.h"
#include "TRandom3.h"

#include "range.h"
#include "consts.h"

using namespace std;


//-----------------------------------------------------------------------------------
// Function Prototypes

float def_ave_2particle_with_gap(float uxn, float uyn,
                                 float QxB, float QyB, float M);
float def_ave_2particle_correlation(float uxn, float uyn,
                                    float QxB, float QyB, float M);
float def_ave_4particle_correlation(float pxn, float pyn,
                                    float qx2n, float qy2n,
                                    float Qxn, float Qyn,
                                    float Qx2n, float Qy2n,
                                    float M, float mp, float mq);

float with_gap_calculation(float QxA, float QyA, float QxB, float QyB, float MA, float MB);

float ave_2particle_correlation(float Qxn, float Qyn, float M);
float ave_4particle_correlation(float Qxn, float Qyn, float Qx2n, float Qy2n, float M);
float ave_6particle_correlation(TComplex qn, TComplex q2n, TComplex q3n, float M);

void parseampt();
void processEvent(vector<particle> nucleons);





//-----------------------------------------------------------------------------------
//Global variables declarations


//-----------------------------------------------------------------------------------
//Vector declarations
vector<particle> pA;
vector<particle> pB;
vector<particle> all_particle;

vector<float> temp_raa2;
vector<float> temp_raa4;
vector<float> temp_raa6;

vector<float> temp_raa32;
vector<float> temp_raa34;
//-----------------------------------------------------------------------------------
//Graphs declarations
TProfile* comp;
TProfile* daa2_with_gap;
TProfile* daa2;
TProfile* daa4;

TProfile* comp_Ncharge;
TProfile* daa2_Ncharge;
TProfile* daa4_Ncharge;
TProfile* daa2_with_gap_Ncharge;

TProfile* raa2_Ncharge;
TProfile* raa4_Ncharge;
TProfile* raa6_Ncharge;

TProfile* raa32_Ncharge;
TProfile* raa34_Ncharge;

TH2D* th2d_raa2_Ncharge;
TH2D* th2d_raa4_Ncharge;
TH2D* th2d_raa6_Ncharge;



TH1F* dnch;
TH1F* bhis;


TTree* shorttree;

//-- ntp_event variables
float Npart;
int event;
float bbc_z;
float centrality;
float bbc_qn;
float bbc_qs;
int icent;
int npc1;
int nfvtxt;
int nfvtxt_south;
int nfvtxt_north;
int nfvtxt_raw;
unsigned int trigger_scaled;
unsigned int trigger_live;
float bc_x;
float bc_y;
float zvtx;
float vtx_z;
float FVTX_X;
float FVTX_Y;
float FVTX_Z;

//static const int nharm = 4;
float d_SouthQX[4];
float d_SouthQY[4];
float d_SouthQW;
float d_NorthQX[4];
float d_NorthQY[4];
float d_NorthQW;



//-----------------------------------------------------------------------------------
//Functions declarations
void cumulant()
{

  comp = new TProfile("comp", "comp", 4, 0.5, 4.5, -10, 10);
  daa2 = new TProfile("daa2", "daa2", 9, 0.2, 2.0, -10, 10);
  daa2_with_gap = new TProfile("daa2_with_gap", "daa2_with_gap", 9, 0.2, 2.0, -10, 10);
  daa4 = new TProfile("daa4", "daa4", 9, 0.2, 2.0, -10, 10);

  // 60, -0.5, 599.5, -10, 10
  // 50, -0.5, 499.5, -10, 10
  // 100, -0.5, 5999.5, -10, 10   Pb+Pb
  // 200, -0.5, 199.5, -10, 10   d+Au
  comp_Ncharge = new TProfile("comp_Ncharge", "comp_Ncharge", NNCHBINS, nchlo, nchhi, -10, 10); //
  daa2_Ncharge = new TProfile("daa2_Ncharge", "daa2_Ncharge", NNCHBINS, nchlo, nchhi, -10, 10); //
  daa4_Ncharge = new TProfile("daa4_Ncharge", "daa4_Ncharge", NNCHBINS, nchlo, nchhi, -10, 10); //
  daa2_with_gap_Ncharge = new TProfile("daa2_with_gap_Ncharge", "daa2_with_gap_Ncharge", NNCHBINS, nchlo, nchhi, -10, 10); //

  raa2_Ncharge = new TProfile("raa2_Ncharge", "raa2_Ncharge", NNCHBINS, nchlo, nchhi, -10, 10); //
  raa4_Ncharge = new TProfile("raa4_Ncharge", "raa4_Ncharge", NNCHBINS, nchlo, nchhi, -10, 10); //
  raa6_Ncharge = new TProfile("raa6_Ncharge", "raa6_Ncharge", NNCHBINS, nchlo, nchhi, -10, 10); //

  raa32_Ncharge = new TProfile("raa32_Ncharge", "raa32_Ncharge", NNCHBINS, nchlo, nchhi, -10, 10); //
  raa34_Ncharge = new TProfile("raa34_Ncharge", "raa34_Ncharge", NNCHBINS, nchlo, nchhi, -10, 10); //

  th2d_raa2_Ncharge = new TH2D("th2d_raa2_Ncharge", "th2d_raa2_Ncharge", NNCHBINS, nchlo, nchhi, 1000, -0.1, 0.1); //
  th2d_raa4_Ncharge = new TH2D("th2d_raa4_Ncharge", "th2d_raa4_Ncharge", NNCHBINS, nchlo, nchhi, 1000, -0.01, 0.01); //
  th2d_raa6_Ncharge = new TH2D("th2d_raa6_Ncharge", "th2d_raa6_Ncharge", NNCHBINS, nchlo, nchhi, 1000, -0.01, 0.01); //

  dnch = new TH1F("dnch", "dnch", 6000, -0.5, 5999.5);
  bhis = new TH1F("bhis", "bhis", 100, 0, 20);


  // --- adding tree for Jamie's analysis
  shorttree = new TTree("shorttree", "Event-wise TTree");
  shorttree->SetAutoFlush(1000);
  shorttree->SetMaxTreeSize(100000000000LL);
  shorttree -> Branch("Npart", &Npart, "Npart/F");
  //shorttree -> Branch("bbc_z", &bbc_z, "bbc_z/F");
  //shorttree -> Branch("centrality", &centrality, "centrality/F");
  //shorttree -> Branch("nfvtxt", &nfvtxt, "nfvtxt/I");
  //shorttree -> Branch("nfvtxt_south", &nfvtxt_south, "nfvtxt_south/I");
  //shorttree -> Branch("nfvtxt_north", &nfvtxt_north, "nfvtxt_north/I");
  //shorttree -> Branch("nfvtxt_raw", &nfvtxt_raw, "nfvtxt_raw/I");
  //shorttree -> Branch("trigger_scaled", &trigger_scaled, "trigger_scaled/i");
  shorttree -> Branch("d_SouthQX", &d_SouthQX, "d_SouthQX[4]/F");
  shorttree -> Branch("d_SouthQY", &d_SouthQY, "d_SouthQY[4]/F");
  shorttree -> Branch("d_SouthQW", &d_SouthQW, "d_SouthQW/F");
  shorttree -> Branch("d_NorthQX", &d_NorthQX, "d_NorthQX[4]/F");
  shorttree -> Branch("d_NorthQY", &d_NorthQY, "d_NorthQY[4]/F");
  shorttree -> Branch("d_NorthQW", &d_NorthQW, "d_NorthQW/F");
  shorttree -> Branch("bbc_qn", &bbc_qn, "bbc_qn/F");
  shorttree -> Branch("bbc_qs", &bbc_qs, "bbc_qs/F");




  // Parse ampt.dat (run over all events)
  parseampt();

  //Make a file to store outputs
  TFile *fout = new TFile("six.root", "RECREATE");

  comp->Write();
  daa2->Write();
  daa2_with_gap->Write();
  daa4->Write();

  comp_Ncharge->Write();
  daa2_Ncharge->Write();
  daa4_Ncharge->Write();
  daa2_with_gap_Ncharge->Write();

  raa2_Ncharge->Write();
  raa4_Ncharge->Write();
  raa6_Ncharge->Write();

  raa32_Ncharge->Write();
  raa34_Ncharge->Write();

  th2d_raa2_Ncharge->Write();
  th2d_raa4_Ncharge->Write();
  th2d_raa6_Ncharge->Write();

  dnch->Write();
  bhis->Write();

  shorttree->Write();

  fout->Close();

}


void parseampt()
{
  //Read in data file
  ifstream dataFile;
  dataFile.open("ana/ampt.dat");

  //Skip the job if not dataFile
  if (!dataFile)
  {
    cout << "--> File ana/ampt.dat does not exist" << endl << endl;
    return;
  }
  else
  {
    cout << "--> Successfully opened ana/ampt.dat" << endl << endl;
  }

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


    //Get the header of each event
    dataFile >> evtnumber >> testnum >> nlist >> impactpar >> npartproj >> nparttarg >> npartprojelas >> npartprojinelas >> nparttargelas >> nparttarginelas >> junk;

    if (!dataFile) break;

    bhis->Fill(impactpar);

    Npart = npartprojinelas + nparttarginelas;
    bbc_qn = 0;
    bbc_qs = 0;
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

      if ( p.eta >  3.1 && p.eta <  3.9 ) ++bbc_qn;
      if ( p.eta < -3.1 && p.eta > -3.9 ) ++bbc_qs;

      // if ( ifFVTXS(p.eta) || ifFVTXN(p.eta) )
      if ( test_eff_s(p.pT, p.eta) || test_eff_n(p.pT, p.eta) )
      {
        all_particle.push_back(p);
      }
    }

    processEvent(all_particle);
    // cout << n_charge << endl;
    // cout << all_particle.size() << endl;
    dnch->Fill(all_particle.size());
    shorttree->Fill();
    all_particle.clear();
    pA.clear();
    pB.clear();
  }
}


// void processEvent(vector<particle> nucleons, int flag)
void processEvent(vector<particle> nucleons)
{
  if (nucleons.size() < 2) return;

  // --- initialize Q-vectors for tree
  for ( int i = 0; i < 4; ++i )
    {
      d_NorthQX[i] = 0;
      d_NorthQY[i] = 0;
      d_SouthQX[i] = 0;
      d_SouthQY[i] = 0;
    }
  d_NorthQW = 0;
  d_SouthQW = 0;

  float Qx2 = 0;
  float Qy2 = 0;
  float Qx3 = 0;
  float Qy3 = 0;
  float Qx4 = 0;
  float Qy4 = 0;
  float Qx6 = 0;
  float Qy6 = 0;

  float QxA = 0;
  float QyA = 0;
  float QxB = 0;
  float QyB = 0;

  for (unsigned int i = 0; i < nucleons.size(); i++)
  {
    if (nucleons[i].eta < -1.0)
    {
      pA.push_back(nucleons[i]);
      QxA += TMath::Cos(2 * nucleons[i].phi);
      QyA += TMath::Sin(2 * nucleons[i].phi);
      for ( int h = 0; h < 4; ++h )
        {
          d_SouthQX[h] += TMath::Cos(h * nucleons[i].phi);
          d_SouthQY[h] += TMath::Sin(h * nucleons[i].phi);
        }
      ++d_SouthQW;
    }
    if (nucleons[i].eta > 1.0)
    {
      pB.push_back(nucleons[i]);
      QxB += TMath::Cos(2 * nucleons[i].phi);
      QyB += TMath::Sin(2 * nucleons[i].phi);
      for ( int h = 0; h < 4; ++h )
        {
          d_NorthQX[h] += TMath::Cos(h * nucleons[i].phi);
          d_NorthQY[h] += TMath::Sin(h * nucleons[i].phi);
        }
      ++d_NorthQW;
    }

    Qx2 += TMath::Cos(2 * nucleons[i].phi);
    Qy2 += TMath::Sin(2 * nucleons[i].phi);
    Qx3 += TMath::Cos(3 * nucleons[i].phi);
    Qy3 += TMath::Sin(3 * nucleons[i].phi);
    Qx4 += TMath::Cos(4 * nucleons[i].phi);
    Qy4 += TMath::Sin(4 * nucleons[i].phi);
    Qx6 += TMath::Cos(6 * nucleons[i].phi);
    Qy6 += TMath::Sin(6 * nucleons[i].phi);

  }

  nfvtxt_south = d_SouthQW;
  nfvtxt_north = d_NorthQW;
  nfvtxt = nfvtxt_north + nfvtxt_south;

  //----------------------------------------------------------------------------------------
  //two particle with eta gap
  float ave_2_with_gap = with_gap_calculation(QxA, QyA, QxB, QyB, (float)pA.size(), (float)pB.size());
  comp->Fill(1.0, ave_2_with_gap);
  comp_Ncharge->Fill(nucleons.size(), ave_2_with_gap);

  //----------------------------------------------------------------------------------------
  //Reference flow
  if (!(nucleons.size() < 2))
  {
    float ave_2 = ave_2particle_correlation(Qx2, Qy2, (float)nucleons.size());
    comp->Fill(2.0, ave_2);
    temp_raa2.push_back(ave_2);
    float ave_32 = ave_2particle_correlation(Qx3, Qy3, (float)nucleons.size());
    temp_raa32.push_back(ave_32);
  }

  if (!(nucleons.size() < 4))
  {
    float ave_4 = ave_4particle_correlation(Qx2, Qy2, Qx4, Qy4, (float)nucleons.size());
    comp->Fill(3.0, ave_4);
    temp_raa4.push_back(ave_4);
    float ave_34 = ave_4particle_correlation(Qx3, Qy3, Qx6, Qy6, (float)nucleons.size());
    temp_raa34.push_back(ave_34);
  }

  //-------------------------------------
  //six particle cumulant

  TComplex Qn_6, Q2n_6, Q3n_6;
  Qn_6 = TComplex(Qx2, Qy2);
  Q2n_6 = TComplex(Qx4, Qy4);
  Q3n_6 = TComplex(Qx6, Qy6);

  if (!(nucleons.size() < 6))
  {
    float ave_6 = ave_6particle_correlation(Qn_6, Q2n_6, Q3n_6, (float)nucleons.size());
    comp->Fill(4.0, ave_6);
    temp_raa6.push_back(ave_6);
  }

  //----------------------------------------------------------------------------------------
  float raa2 = 0; //<<2>>
  float raa4 = 0; //<<4>>
  float raa6 = 0; //<<6>>
  float raa32 = 0; //<<2>>
  float raa34 = 0; //<<4>>
  for (unsigned int i = 0; i < temp_raa2.size(); i++)
  {
    raa2 += temp_raa2[i];
    raa32 += temp_raa32[i];
  }

  raa2 = raa2 / temp_raa2.size();
  raa32 = raa32 / temp_raa2.size();

  for (unsigned int i = 0; i < temp_raa4.size(); i++)
  {
    raa4 += temp_raa4[i];
    raa34 += temp_raa34[i];
  }

  raa4 = raa4 / temp_raa4.size();
  raa34 = raa34 / temp_raa4.size();

  for (unsigned int i = 0; i < temp_raa6.size(); i++)
  {
    raa6 += temp_raa6[i];
  }

  raa6 = raa6 / temp_raa6.size();

  raa2_Ncharge->Fill(nucleons.size(), raa2);
  raa4_Ncharge->Fill(nucleons.size(), raa4);
  raa6_Ncharge->Fill(nucleons.size(), raa6);

  raa32_Ncharge->Fill(nucleons.size(), raa32);
  raa34_Ncharge->Fill(nucleons.size(), raa34);

  th2d_raa2_Ncharge->Fill(nucleons.size(), raa2);
  th2d_raa4_Ncharge->Fill(nucleons.size(), raa4);
  th2d_raa6_Ncharge->Fill(nucleons.size(), raa6);

  temp_raa2.clear();
  temp_raa4.clear();
  temp_raa6.clear();
  //----------------------------------------------------------------------------------------

  //----------------------------------------------------------------------------------------
  //Differential flow
  for (unsigned int i = 0; i < pA.size(); i++)
  {
    if (pB.size() < 2) continue;
    float ux2 = TMath::Cos(2 * pA[i].phi);
    float uy2 = TMath::Sin(2 * pA[i].phi);

    float def_ave_2 = def_ave_2particle_with_gap(ux2, uy2, QxB, QyB, (float)pB.size());
    // comp[flag]->Fill(4.0, def_ave_2);
    daa2_with_gap->Fill(pA[i].pT, def_ave_2);
    daa2_with_gap_Ncharge->Fill(nucleons.size(), def_ave_2);
  }

  for (unsigned int i = 0; i < nucleons.size(); i++)
  {
    float ux2 = TMath::Cos(2 * nucleons[i].phi);
    float uy2 = TMath::Sin(2 * nucleons[i].phi);

    float def_ave_2 = def_ave_2particle_correlation(ux2, uy2, Qx2, Qy2, (float)nucleons.size());
    daa2->Fill(nucleons[i].pT, def_ave_2);
    daa2_Ncharge->Fill(nucleons.size(), def_ave_2);
  }

  for (unsigned int i = 0; i < nucleons.size(); i++)
  {
    if (nucleons.size() < 4) continue;
    float px2 = TMath::Cos(2 * nucleons[i].phi);
    float py2 = TMath::Sin(2 * nucleons[i].phi);
    float qx4 = TMath::Cos(4 * nucleons[i].phi);
    float qy4 = TMath::Sin(4 * nucleons[i].phi);

    float def_ave_4 = def_ave_4particle_correlation(px2, py2, qx4, qy4, Qx2, Qy2, Qx4, Qy4, (float)nucleons.size(), 1, 1);

    // cout << setw(15) << def_ave_4 << setw(15) << ron_def_4 << endl;
    // comp[flag]->Fill(5.0, def_ave_4);
    daa4->Fill(nucleons[i].pT, def_ave_4);
    daa4_Ncharge->Fill(nucleons.size(), def_ave_4);
  }
}





//----------------------------------------------------------------------------------------
// Helper Functions
float def_ave_2particle_with_gap(float uxn, float uyn, float QxB, float QyB, float M)
{
  float numerator = uxn * QxB + uyn * QyB;
  float denominator = M;

  return numerator / denominator;
}

float def_ave_2particle_correlation(float uxn, float uyn, float QxB, float QyB, float M)
{
  float numerator = uxn * QxB + uyn * QyB - 1;
  float denominator = M - 1;

  return numerator / denominator;
}

float def_ave_4particle_correlation(float pxn, float pyn, float qx2n, float qy2n, float Qxn, float Qyn, float Qx2n, float Qy2n, float M, float mp, float mq)
{
  float qxn = pxn;
  float qyn = pyn;

  float Qn_sq  = Qxn * Qxn + Qyn * Qyn;

  float one    = pxn * Qxn * Qxn * Qxn + pxn * Qxn * Qyn * Qyn + pyn * Qxn * Qxn * Qyn + pyn * Qyn * Qyn * Qyn;
  float two    = qx2n * Qxn * Qxn - qx2n * Qyn * Qyn + 2 * qy2n * Qxn * Qyn;
  float three  = pxn * Qxn * Qx2n - pyn * Qyn * Qx2n + pxn * Qyn * Qy2n + pyn * Qxn * Qy2n;
  float four   = 2 * M * (pxn * Qxn + pyn * Qyn);
  float five   = 2 * mq * Qn_sq;
  float six    = 7 * (qxn * Qxn + qyn * Qyn);
  float seven  = Qxn * qxn + Qyn * qyn;
  float eight  = qx2n * Qx2n + qy2n * Qy2n;
  float nine   = 2 * (pxn * Qxn + pyn * Qyn);
  float ten    = 2 * mq * M;
  float eleven = 6 * mq;

  float numerator = one - two - three - four - five + six - seven + eight + nine + ten - eleven;
  float denominator = (mp * M - 3 * mq) * (M - 1) * (M - 2);

  return numerator / denominator;
}

float with_gap_calculation(float QxA, float QyA, float QxB, float QyB, float MA, float MB)
{
  float numerator = QxA * QxB + QyA * QyB;
  float denominator = MA * MB;

  return numerator / denominator;
}

float ave_2particle_correlation(float Qxn, float Qyn, float M)
{
  float Qn_sq  = Qxn * Qxn + Qyn * Qyn;

  float numerator   = Qn_sq - M;
  float denominator = M * (M - 1);

  return numerator / denominator;
}

float ave_4particle_correlation(float Qxn, float Qyn, float Qx2n, float Qy2n, float M)
{
  float Qn_sq  = Qxn * Qxn + Qyn * Qyn;
  float Q2n_sq = Qx2n * Qx2n + Qy2n * Qy2n;
  float Qn_isq = Qxn * Qxn - Qyn * Qyn;

  float first  = Qn_sq * Qn_sq + Q2n_sq - 2 * (Qx2n * Qn_isq + 2 * Qy2n * Qxn * Qyn);
  float second = 2 * (2 * (M - 2) * Qn_sq - M * (M - 3));

  float numerator   = first - second;
  float denominator = M * (M - 1) * (M - 2) * (M - 3);

  return numerator / denominator;
}

float ave_6particle_correlation(TComplex qn, TComplex q2n, TComplex q3n, float M)
{
  TComplex temp1;

  // first term
  // |Qn|^6 + 9*|Q2n|^2|Qn|^2 - 6 x Re[Q2n x Qn x Qn* x Qn* x Qn*] / (Mx(M-1)x(M-2)x(M-3)x(M-4)x(M-5)
  double term1a = TMath::Power((qn * TComplex::Conjugate(qn)), 3);
  double term1b = 9.0 * q2n * TComplex::Conjugate(q2n) * qn * TComplex::Conjugate(qn);
  temp1 = q2n * qn * TComplex::Conjugate(qn) * TComplex::Conjugate(qn) * TComplex::Conjugate(qn);
  double term1c = -6.0 * temp1.Re();
  double term1 = (term1a + term1b + term1c) / (M * (M - 1) * (M - 2) * (M - 3) * (M - 4) * (M - 5));

  // second term
  // 4 * [Re[Q3nQn*Qn*Qn*] - 3 Re[Q3nQ2n*Qn*]] / (M(M-1)(M-2)(M-3)(M-4)(M-5)
  temp1 = q3n * TComplex::Conjugate(qn) * TComplex::Conjugate(qn) * TComplex::Conjugate(qn);
  double term2a = temp1.Re();
  temp1 = q3n * TComplex::Conjugate(q2n) * TComplex::Conjugate(qn);
  double term2b = -3.0 * temp1.Re();
  double term2 = 4.0 * (term2a + term2b) / (M * (M - 1) * (M - 2) * (M - 3) * (M - 4) * (M - 5));

  // third term
  // +2 * (9*(M-4)*Re[Q2nQn*qn*] + 2 |Q3n|^2) / ((M(M-1)(M-2)(M-3)(M-4)(M-5))
  temp1 = q2n * TComplex::Conjugate(qn) * TComplex::Conjugate(qn);
  double term3a = 9.0 * (M - 4) * temp1.Re();
  double term3b = 2.0 * q3n * TComplex::Conjugate(q3n);
  double term3 = 2.0 * (term3a + term3b) / (M * (M - 1) * (M - 2) * (M - 3) * (M - 4) * (M - 5));

  // fourth term
  //double term4 = -9.0 * (TMath::Power(qn*TComplex::Conjugate(qn),2)+q2n*TComplex::Conjugate(q2n)) / (M*(M-1)*(M-2)*(M-3)*(M-5));
  double term4 = -9.0 * (TMath::Power(qn * TComplex::Conjugate(qn), 2) + q2n * TComplex::Conjugate(q2n)) ;
  term4 /= (M * (M - 1) * (M - 2) * (M - 3) * (M - 5));

  // fifth term
  //double term5 = 18.0 * qn*TComplex::Conjugate(qn) / (M*(M-1)*(M-3)*(M-4));
  double term5 = 18.0 * qn * TComplex::Conjugate(qn) ;
  term5 /=  (M * (M - 1) * (M - 3) * (M - 4));

  // sixth term
  double term6 = -6.0 / ((M - 1) * (M - 2) * (M - 3));

  // cos(n(phi1+phi2+phi3-phi4-phi5-phi6))
  double six = term1 + term2 + term3 + term4 + term5 + term6;

  return (float)six;
}

