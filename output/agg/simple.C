void simple()
{

  TFile* file = TFile::Open("cumulants_ampt_pau200_all.root");

  TProfile* tp1f_two = (TProfile*)file->Get("raa2_Ncharge");
  TProfile* tp1f_for = (TProfile*)file->Get("raa4_Ncharge");

  TH1D* th1d_two = tp1f_two->ProjectionX("th1d_two");
  TH1D* th1d_for = tp1f_for->ProjectionX("th1d_for");
  TH1D* th1d_222 = (TH1D*)th1d_two->Clone("th1d_222");
  TH1D* th1d_c24 = (TH1D*)th1d_for->Clone("th1d_c24");

  th1d_two->GetXaxis()->SetRangeUser(0,100);
  th1d_for->GetXaxis()->SetRangeUser(0,100);
  th1d_222->GetXaxis()->SetRangeUser(0,100);
  th1d_c24->GetXaxis()->SetRangeUser(0,100);

  th1d_222->Multiply(th1d_two);
  th1d_222->Scale(2.0);
  th1d_c24->Add(th1d_222,-1.0);

  TFile* fout = TFile::Open("pau200_cumu_out.root","recreate");
  fout->cd();

  th1d_two->Write();
  th1d_for->Write();
  th1d_222->Write();
  th1d_c24->Write();

  fout->Write();
  fout->Close();

}
