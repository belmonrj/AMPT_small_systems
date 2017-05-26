////////////////////////////////////////////////////////////////////////////////
//
// Calculate centrality bin limits on AMPT results
//
////////////////////////////////////////////////////////////////////////////////
//
// Darren McGlinchey
// 11 May 2017
//
////////////////////////////////////////////////////////////////////////////////
#include <TFile.h>
#include <TH1.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TLatex.h>

#include <iostream>

using namespace std;

void calc_centbins()
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
    "rootfiles/eplane_ampt_dau200_11-15.root",
    "rootfiles/eplane_ampt_dau62_11-15.root",
    "rootfiles/eplane_ampt_dau39_10-12.root",
    "rootfiles/eplane_ampt_dau20_14.root",
  };

  const int NCENTMAX = 6;
  int NCENT[] = {6, 6, 5, 4};
  float centlim[NE][NCENTMAX + 1] =
  {
    {0,  5, 10, 20,  40,  60, 100}, // 200 GeV
    {0,  5, 10, 20,  40,  60, 100}, // 62 GeV
    {0, 10, 20, 40,  60, 100,   0}, // 39 GeV
    {0, 20, 40, 60, 100,   0,   0}, // 20 GeV
  };
  int centColor[] = {kRed, kBlue, kGreen + 2, kMagenta, kCyan + 2, kYellow + 2};

  bool printPlots = true;

  //==========================================================================//
  // DECLARE VARIABLES
  //==========================================================================//

  //-- histos
  TH1D* nch_bbcs[NE];
  TH1D* nch_cent[NE][NCENTMAX];

  //-- values
  double nchlim_cent[NE][NCENTMAX];
  double nchlim_save[NE][NCENTMAX];
  double q_cent[NE][NCENTMAX];
  double frac_cent[NE][NCENTMAX];


  //-- tmp
  TFile *fin;

  //==========================================================================//
  // GET HISTOS FROM FILE
  //==========================================================================//
  cout << endl;
  cout << "--> Reading histograms from file " << endl;

  for (int ie = 0; ie < NE; ie++)
  {
    fin = TFile::Open(epFile[ie]);
    if ( !fin )
    {
      cout << "ERROR!! Unable to open " << epFile[ie] << endl;
      return;
    }

    nch_bbcs[ie] = (TH1D*) fin->Get("dhis_bbcs");
    if ( !nch_bbcs[ie] )
    {
      cout << "ERROR!! Unable to find dhis_bbcs in " << epFile[ie] << endl;
      return;
    }
    nch_bbcs[ie]->SetDirectory(0);
    nch_bbcs[ie]->SetName(Form("nch_bbcs_%i", ie));
    nch_bbcs[ie]->SetLineColor(kBlack);
    nch_bbcs[ie]->SetLineWidth(2);
  } // ie

  //==========================================================================//
  // CALC LIMITS
  //==========================================================================//
  cout << endl;
  cout << "--> Calculating limits" << endl;

  // for (int ie = 0; ie < NE; ie++)
  // {
  //   cout << "---- " << energy[ie] << " ----" << endl;

  //   // set quantiles
  //   for (int ic = 0; ic < NCENT[ie]; ic++)
  //   {
  //     q_cent[ie][ic] = (100 - centlim[ie][ic+1]) / 100.;
  //   }

  //   // calculate limits
  //   // nch_bbcs[ie]->Scale(1./nch_bbcs[ie]->Integral());
  //   nch_bbcs[ie]->GetQuantiles(NCENT[ie], nchlim_cent[ie], q_cent[ie]);

  //   if (ie == 3)
  //   {
  //     nchlim_cent[ie][0] = 3;
  //     nchlim_cent[ie][1] = 2;
  //     nchlim_cent[ie][2] = 1;
  //     nchlim_cent[ie][3] = 0;
  //     nchlim_cent[ie][4] = 0;
  //     nchlim_cent[ie][5] = 0;
  //   }

  //   // loop over limits
  //   for (int ic = 0; ic < NCENT[ie]; ic++)
  //   {
  //     nch_cent[ie][ic] = (TH1D*) nch_bbcs[ie]->Clone(Form("nch_cent_%i_%i", ie, ic));
  //     nch_cent[ie][ic]->Reset();
  //     nch_cent[ie][ic]->SetFillColorAlpha(centColor[ic], 0.7);

  //     if (nchlim_cent[ie][ic] == 0)
  //       nchlim_cent[ie][ic] = 1;

  //     int bl, bh;
  //     if ( ic == 0)
  //     {
  //       bl = nchlim_cent[ie][ic] + 1;
  //       bh = nch_bbcs[ie]->GetNbinsX();
  //     }
  //     else if (ic < NCENT[ie] - 1)
  //     {
  //       bl = nchlim_cent[ie][ic] + 1;
  //       bh = nchlim_cent[ie][ic - 1];
  //     }
  //     else
  //     {
  //       bl = 1;
  //       bh = nchlim_cent[ie][ic - 1];
  //     }

  //     nchlim_save[ie][ic] = nch_bbcs[ie]->GetBinCenter(bl);

  //     frac_cent[ie][ic] = nch_bbcs[ie]->Integral(bl, bh) / nch_bbcs[ie]->Integral();

  //     for (int ix = bl; ix <= bh; ix++)
  //     {
  //       nch_cent[ie][ic]->SetBinContent(ix, nch_bbcs[ie]->GetBinContent(ix));
  //     }

  //     cout << "  " << centlim[ie][ic] << " - " << centlim[ie][ic + 1]
  //          << " q:" << q_cent[ie][ic]
  //          << " l:" << nchlim_cent[ie][ic]
  //          << " bl:[" << bl << "," << bh << "]"
  //          << " nchlim_save:[" << nchlim_save[ie][ic]
  //          << "," << nch_bbcs[ie]->GetBinCenter(bh) << "]"
  //          << " f:" << frac_cent[ie][ic]
  //          << endl;

  //   } // ic


  // } // ie

  for (int ie = 0; ie < NE; ie++)
  {
    cout << "---- " << energy[ie] << " ----" << endl;

    // loop over limits
    for (int ic = 0; ic < NCENT[ie]; ic++)
    {
      nch_cent[ie][ic] = (TH1D*) nch_bbcs[ie]->Clone(Form("nch_cent_%i_%i", ie, ic));
      nch_cent[ie][ic]->Reset();
      nch_cent[ie][ic]->SetFillColorAlpha(centColor[ic], 0.7);

      int bl, bh;
      if (ic == 0)
      {
        bh = nch_bbcs[ie]->GetNbinsX();
      }
      else
      {
        bh = nchlim_cent[ie][ic - 1] - 1;
      }

      // find the lower bin (as long as it's not the most peripheral)
      if ( ic < NCENT[ie] - 1 )
      {
        double frac = (centlim[ie][ic + 1] - centlim[ie][ic]) / 100.;
        for (int ix = bh; ix >= 1; ix--)
        {
          double f = nch_bbcs[ie]->Integral(ix, bh) / nch_bbcs[ie]->Integral();

          if ( f > frac )
          {
            // check if the higher bin was closer
            double fh = nch_bbcs[ie]->Integral(ix + 1, bh) / nch_bbcs[ie]->Integral();

            if ( TMath::Abs(f - frac) > TMath::Abs(fh - frac) && (ix + 1) <= bh )
              bl = ix + 1;
            else
              bl = ix;

            break;
          }
        } // ix

        if (bl < 1)
          bl = 1;
      }
      else
        bl = 1; // most peripheral is always 1


      nchlim_cent[ie][ic] = bl;
      nchlim_save[ie][ic] = nch_bbcs[ie]->GetBinCenter(nchlim_cent[ie][ic]);

      frac_cent[ie][ic] = nch_bbcs[ie]->Integral(bl, bh) / nch_bbcs[ie]->Integral();

      for (int ix = bl; ix <= bh; ix++)
      {
        nch_cent[ie][ic]->SetBinContent(ix, nch_bbcs[ie]->GetBinContent(ix));
      }

      cout << "  " << centlim[ie][ic] << " - " << centlim[ie][ic + 1]
           << " bl:[" << bl << "," << bh << "]"
           << " nchlim_save:[" << nchlim_save[ie][ic]
           << "," << nch_bbcs[ie]->GetBinCenter(bh) << "]"
           << " f:" << frac_cent[ie][ic]
           << endl;

    } // ic


  } // ie

  //==========================================================================//
  // PLOT OBJECTS
  //==========================================================================//
  cout << endl;
  cout << "--> Plotting" << endl;

  TLatex lt;
  lt.SetNDC();
  lt.SetTextAlign(22);

  TLegend *legcent[NE];
  for (int ie = 0; ie < NE; ie++)
  {
    legcent[ie] = new TLegend(0.55, 0.3, 0.98, 0.88);
    legcent[ie]->SetFillStyle(0);
    legcent[ie]->SetBorderSize(0);
    legcent[ie]->SetTextFont(63);
    legcent[ie]->SetTextSize(18);
    legcent[ie]->SetHeader(Form("AMPT d+Au %i GeV", energy[ie]));
    legcent[ie]->AddEntry(nch_bbcs[ie], "Total", "L");
    for (int ic = 0; ic < NCENT[ie]; ic++)
    {
      int lo, hi;
      if (ic == NCENT[ie] - 1)
        lo = 0;
      else
        lo = nchlim_save[ie][ic];

      if (ic == 0)
        hi = 200;
      else
        hi = nchlim_save[ie][ic - 1];



      legcent[ie]->AddEntry(nch_cent[ie][ic],
                            Form("%.0f-%.0f%% [%i,%i) (f=%.3f)",
                                 centlim[ie][ic], centlim[ie][ic+1],
                                 lo, hi,
                                 frac_cent[ie][ic]),
                            "F");
    }
  } // ie

  float nch_hi[] = {119, 75, 59, 39};
  //==========================================================================//
  // PLOT
  //==========================================================================//

  TCanvas *cnch = new TCanvas("cnch", "nch", 1200, 800);
  cnch->Divide(2, 2);

  for (int ie = 0; ie < NE; ie++)
  {
    cnch->GetPad(ie + 1)->SetMargin(0.1, 0.01, 0.1, 0.1);
    cnch->cd(ie + 1);
    gPad->SetLogy();
    nch_bbcs[ie]->GetXaxis()->SetRangeUser(0, nch_hi[ie]);
    nch_bbcs[ie]->Draw("Hist");
    for (int ic = 0; ic < NCENT[ie]; ic++)
      nch_cent[ie][ic]->Draw("same");
    lt.DrawLatex(0.5, 0.95, Form("AMPT d+Au %i GeV", energy[ie]));
    legcent[ie]->Draw("same");
  }


  //==========================================================================//
  // SAVE
  //==========================================================================//
  if (printPlots)
  {
    cout << endl;
    cout << "--> Printing plots" << endl;

    cnch->Print("pdfs/ampt_centralitybinning.pdf");
  }
}