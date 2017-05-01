#ifndef CONSTS_H
#define CONSTS_H


const int NPTBINS = 16;
double ptlim[NPTBINS + 1] = {
  0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
  2.5, 3.0, 3.5, 4.0, 4.5, 5.0
};

const int NETABINS = 32;
double etalo = -3.2;
double etahi = 3.2;

const int NNCHBINS = 1200;
double nchlo = -0.5;
double nchhi = 1199.5;

struct particle
{
  int   id;
  float px;
  float py;
  float pz;
  float m;
  float x;
  float y;
  float z;
  float t;
  float eta;
  float phi;
  float pT;
  float rsquare;
};

// 200GeV: 
// [0%, 5%], [23,51] particles in BBCS 
// [5%, 10%], [19,22] particles in BBCS 
// [10%, 20%], [15,18] particles in BBCS 
// [20%, 40%], [10,14] particles in BBCS 
// [40%, 60%], [6,9] particles in BBCS 
// [60%, 100%] [0, 5] particles in BBCS

// 62GeV: 
// [0%, 5%], [13,33] particles in BBCS 
// [5%, 10%], [11,12] particles in BBCS 
// [10%, 20%], [8,10] particles in BBCS 
// [20%, 40%], [5,7] particles in BBCS 
// [40%, 60%], [3,4] particles in BBCS 
// [60%, 100%] [0, 2] particles in BBCS

// 39GeV: 
// [0%, 5%], [9,22] particles in BBCS 
// [5%, 10%], [8,8] particles in BBCS 
// [10%, 20%], [7,7] particles in BBCS 
// [20%, 40%], [4,6] particles in BBCS 
// [40%, 60%], [2,3] particles in BBCS 
// [60%, 100%] [0, 1] particles in BBCS

// 20GeV: 
// [0%, 5%], [6,16] particles in BBCS 
// [5%, 10%], [4,5] particles in BBCS 
// [10%, 20%], [3,3] particles in BBCS

// (n_bbcs >= centlim[energy][i])
const int NCENT = 6;
double centlim[4][NCENT] = 
{
  {23, 19, 15, 10, 6, 0}, // 200 
  {13, 11, 8, 5, 3, 0}, // 62
  {8, 7, 4, 2, 0, 0}, // 39
  {3, 0, 0, 0, 0, 0}, // 20
};



#endif
