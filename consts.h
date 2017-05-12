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

//---
// Old from Bill (incorrect)
//---
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


//---
// D. McGlinchey 11 May 2017: b < 20 AMPT 
//---
// ---- 200 ----
//      0 - 5  Nch -3.9<eta<-3.1:[27,199] f:0.048012
//     5 - 10  Nch -3.9<eta<-3.1:[23,26]  f:0.0505165
//    10 - 20  Nch -3.9<eta<-3.1:[18,22]  f:0.104634
//    20 - 40  Nch -3.9<eta<-3.1:[12,17]  f:0.186047
//    40 - 60  Nch -3.9<eta<-3.1:[7,11]   f:0.200146
//   60 - 100  Nch -3.9<eta<-3.1:[0,6]    f:0.410644
// ---- 62 ----
//      0 - 5  Nch -3.9<eta<-3.1:[14,199] f:0.0484021
//     5 - 10  Nch -3.9<eta<-3.1:[12,13]  f:0.0417207
//    10 - 20  Nch -3.9<eta<-3.1:[9,11]   f:0.107944
//    20 - 40  Nch -3.9<eta<-3.1:[6,8]    f:0.172583
//    40 - 60  Nch -3.9<eta<-3.1:[4,5]    f:0.154104
//   60 - 100  Nch -3.9<eta<-3.1:[0,3]    f:0.475247
// ---- 39 ----
//     0 - 10  Nch -3.9<eta<-3.1:[8,199] f:0.0943204
//    10 - 20  Nch -3.9<eta<-3.1:[6,7]   f:0.102709
//    20 - 40  Nch -3.9<eta<-3.1:[4,5]   f:0.16679
//    40 - 60  Nch -3.9<eta<-3.1:[2,3]   f:0.251707
//   60 - 100  Nch -3.9<eta<-3.1:[0,1]   f:0.384474
// ---- 20 ----
//     0 - 20  Nch -3.9<eta<-3.1:[3,199] f:0.227607
//    20 - 40  Nch -3.9<eta<-3.1:[2,2]   f:0.158104
//    40 - 60  Nch -3.9<eta<-3.1:[1,1]   f:0.250262
//   60 - 100  Nch -3.9<eta<-3.1:[0,0]   f:0.364028

// (n_bbcs >= centlim[energy][i])
const int NCENT = 6;
double centlim[4][NCENT] = 
{
  {27, 23, 18, 12, 7, 0}, // 200 
  {14, 12, 9, 6, 4, 0}, // 62
  {8, 6, 4, 2, 0, 0}, // 39
  {3, 2, 1, 0, 0, 0}, // 20
};



#endif
