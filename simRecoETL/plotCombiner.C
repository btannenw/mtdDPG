#include "TH1.h"
#include "TGraphAsymmErrors.h"
#include "TFile.h"
#include "TTree.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLatex.h"

#include "cmsStyle/tdrStyle.C"
#include "cmsStyle/CMS_lumi.C"

void addOverflow(TH1D*& histo)
{
  // put overflow bin at end
  int maxBin = histo->GetNbinsX();
  histo->SetBinContent( maxBin, histo->GetBinContent( maxBin ) + histo->GetBinContent( maxBin+1 ) );
  histo->SetBinError  ( maxBin, sqrt( histo->GetBinError(maxBin)*histo->GetBinError(maxBin) + histo->GetBinError(maxBin+1)*histo->GetBinError(maxBin+1) ) );
  histo->SetBinContent( maxBin + 1, 0 );
  histo->SetBinError( maxBin + 1, 0 );
}

void compareTwoPlots(TCanvas* c0, TH1D *h_singleMu, TH1D *h_singlePi, string hname, string xname, string yname, bool setLogY=false)
{
  
  c0->cd();
  c0->Clear();
  c0->SetGrid();

  addOverflow(h_singlePi);
  addOverflow(h_singleMu);

  h_singlePi->SetLineColor(kRed);
  h_singlePi->SetLineWidth(2);
  h_singleMu->SetLineColor(kBlue);
  h_singleMu->SetLineWidth(2);


  h_singleMu->GetYaxis()->SetTitle( yname.c_str() );
  h_singleMu->GetXaxis()->SetTitle( xname.c_str() );

  h_singleMu->SetMinimum(0.1);
  h_singleMu->SetMaximum(10000);
  h_singleMu->DrawNormalized("HIST");

  if (setLogY) {
    h_singleMu->SetMinimum(0.1);
    h_singlePi->SetMinimum(0.1);
    h_singleMu->SetMaximum(10);
    h_singlePi->SetMaximum(10);
    c0->SetLogy();
    h_singleMu->GetYaxis()->SetRangeUser(0.1, 10.1);
  }
  else
    h_singleMu->GetYaxis()->SetRangeUser(0.0, 1.1);

  h_singlePi->DrawNormalized("same HIST");

  string s_meanE_mu;          // string which will contain the result
  std::ostringstream os_mean_mu;  // stream used for the conversion
  os_mean_mu << std::fixed;
  os_mean_mu << std::setprecision(2) << h_singleMu->GetMean();
  s_meanE_mu = os_mean_mu.str(); // set 'Result' to the contents of the stream
  string s_meanE_pi;          // string which will contain the result
  std::ostringstream os_mean_pi;  // stream used for the conversion
  os_mean_pi << std::fixed;
  os_mean_pi << std::setprecision(2) << h_singlePi->GetMean();
  s_meanE_pi = os_mean_pi.str(); // set 'Result' to the contents of the stream


  TLegend* leg = new TLegend();
  leg = new TLegend(0.55, 0.75, .85, .85);
  leg->AddEntry(h_singleMu, ("Single Muon, <E> = "+s_meanE_mu+" MeV").c_str(), "l");
  leg->AddEntry(h_singlePi, ("Single Pion, <E> = "+s_meanE_pi+" MeV").c_str(), "l");
  leg->Draw("same");

  /*TLatex ltx1;
  ltx1.SetTextAlign(9);
  ltx1.SetTextFont(62);
  ltx1.SetTextSize(0.025);
  ltx1.SetNDC();
  ltx1.DrawLatex(0.3, 0.92, ("Single Muon <E> = "+h_singleMu->GetMean()).c_str() );
  ltx1.DrawLatex(0.3, 0.95, ("Single Pion <E> = "+h_singlePi->GetMean()).c_str() );
  */

  CMS_lumi( c0, 0, 0);

  c0->SetLeftMargin(0.15);
  //c0->SetRightMargin(0.05);
  //c0->SetBottomMargin(0.15);

  c0->Print( (hname + ".png").c_str() );
}


void compareTwoEfficiencies(TCanvas* c0, TGraphAsymmErrors *g_singleMu, TGraphAsymmErrors *g_singlePi, string hname, string xname, string yname)
{
  
  c0->cd();
  c0->SetLogy(0);
  c0->Clear();
  c0->SetGrid();

  g_singlePi->SetLineColor(kRed);
  g_singlePi->SetLineWidth(2);
  g_singleMu->SetLineColor(kBlue);
  g_singleMu->SetLineWidth(2);

  g_singleMu->GetYaxis()->SetTitle( yname.c_str() );
  g_singleMu->GetXaxis()->SetTitle( xname.c_str() );

  //g_singleMu->GetYaxis()->SetRangeUser(0.0, 1.1);
  g_singleMu->Draw();
  g_singlePi->Draw("same");

  TLegend* leg = new TLegend();
  leg = new TLegend(0.65, 0.55, .85, .65);
  leg->AddEntry(g_singleMu, "Single Muons", "l");
  leg->AddEntry(g_singlePi, "Single Pions", "l");
  leg->Draw("same");


  CMS_lumi( c0, 0, 0);

  c0->SetLeftMargin(0.15);
  //c0->SetRightMargin(0.05);
  //c0->SetBottomMargin(0.15);

  c0->Print( (hname + ".png").c_str() );
}


void plotCombiner()
{
  // *** 0. Drawing options
  TCanvas* c1 = new TCanvas("c1", "c1", 800, 800);

  // *** 1. CMS Style
  setTDRStyle();
  writeExtraText = true;       // if extra text                                                
  extraText  = "      Phase 2 Simulation";  // default extra text is "Preliminary"
  //lumi_sqrtS = "Phase 2 Simulation";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)
  int iPeriod = 0;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV, 0=free form (uses lumi_sqrtS)  
  cmsTextSize = 0.3;
  lumiTextSize = 0.3;

  // *** 2. Files and trees
  TFile *f_singleMu = new TFile("./dpg_01-25-19/singleMu_noPU_v10.root", "READ");
  TFile *f_singlePi = new TFile("./dpg_01-25-19/singlePi_noPU_v10.root", "READ");

  // *** 3. Plots
  TH1D *h_recHit_energy_dR05_ETL_singleMu = (TH1D*)f_singleMu->Get("recHit_energy_dR05_withTrackETL");
  TH1D *h_recHit_energy_dR05_ETL_singlePi = (TH1D*)f_singlePi->Get("recHit_energy_dR05_withTrackETL");
  TH1D *h_recHit_energy_dR05_ETL_fixedRange_singleMu = (TH1D*)f_singleMu->Get("recHit_energy_dR05_withTrackETL_fixedRange");
  TH1D *h_recHit_energy_dR05_ETL_fixedRange_singlePi = (TH1D*)f_singlePi->Get("recHit_energy_dR05_withTrackETL_fixedRange");
  TH1D *h_recHit_energy_dR05_BTL_singleMu = (TH1D*)f_singleMu->Get("recHit_energy_dR05_withTrackBTL");
  TH1D *h_recHit_energy_dR05_BTL_singlePi = (TH1D*)f_singlePi->Get("recHit_energy_dR05_withTrackBTL");

  TH1D *h_recHit_maxEnergy_withTrack_ETL_singleMu = (TH1D*)f_singleMu->Get("recHit_maxEnergy_withTrackETL");
  TH1D *h_recHit_maxEnergy_dRpass_withTrack_ETL_singleMu = (TH1D*)f_singleMu->Get("recHit_maxEnergy_dRpass_withTrackETL");
  TH1D *h_recHit_maxEnergy_withTrack_BTL_singleMu = (TH1D*)f_singleMu->Get("recHit_maxEnergy_withTrackBTL");
  TH1D *h_recHit_maxEnergy_dRpass_withTrack_BTL_singleMu = (TH1D*)f_singleMu->Get("recHit_maxEnergy_dRpass_withTrackBTL");
  TH1D *h_recHit_maxEnergy_withTrack_ETL_singlePi = (TH1D*)f_singlePi->Get("recHit_maxEnergy_withTrackETL");
  TH1D *h_recHit_maxEnergy_dRpass_withTrack_ETL_singlePi = (TH1D*)f_singlePi->Get("recHit_maxEnergy_dRpass_withTrackETL");
  TH1D *h_recHit_maxEnergy_withTrack_BTL_singlePi = (TH1D*)f_singlePi->Get("recHit_maxEnergy_withTrackBTL");
  TH1D *h_recHit_maxEnergy_dRpass_withTrack_BTL_singlePi = (TH1D*)f_singlePi->Get("recHit_maxEnergy_dRpass_withTrackBTL");


  TGraphAsymmErrors *g_track_eta_energyThresholdEff_ETL_singleMu = (TGraphAsymmErrors*)f_singleMu->Get("divide_track_eta_atETL_overThreshE_by_track_eta_atETL");
  TGraphAsymmErrors *g_track_eta_energyThresholdEff_ETL_singlePi = (TGraphAsymmErrors*)f_singlePi->Get("divide_track_eta_atETL_overThreshE_by_track_eta_atETL");
  TGraphAsymmErrors *g_track_phi_energyThresholdEff_ETL_singleMu = (TGraphAsymmErrors*)f_singleMu->Get("divide_track_phi_atETL_overThreshE_by_track_phi_atETL");
  TGraphAsymmErrors *g_track_phi_energyThresholdEff_ETL_singlePi = (TGraphAsymmErrors*)f_singlePi->Get("divide_track_phi_atETL_overThreshE_by_track_phi_atETL");
  TGraphAsymmErrors *g_track_pt_energyThresholdEff_ETL_singleMu = (TGraphAsymmErrors*)f_singleMu->Get("divide_track_pt_atETL_overThreshE_by_track_pt_atETL");
  TGraphAsymmErrors *g_track_pt_energyThresholdEff_ETL_singlePi = (TGraphAsymmErrors*)f_singlePi->Get("divide_track_pt_atETL_overThreshE_by_track_pt_atETL");
  TGraphAsymmErrors *g_track_eta_energyThresholdEff_BTL_singleMu = (TGraphAsymmErrors*)f_singleMu->Get("divide_track_eta_atBTL_overThreshE_by_track_eta_atBTL");
  TGraphAsymmErrors *g_track_eta_energyThresholdEff_BTL_singlePi = (TGraphAsymmErrors*)f_singlePi->Get("divide_track_eta_atBTL_overThreshE_by_track_eta_atBTL");
  TGraphAsymmErrors *g_track_phi_energyThresholdEff_BTL_singleMu = (TGraphAsymmErrors*)f_singleMu->Get("divide_track_phi_atBTL_overThreshE_by_track_phi_atBTL");
  TGraphAsymmErrors *g_track_phi_energyThresholdEff_BTL_singlePi = (TGraphAsymmErrors*)f_singlePi->Get("divide_track_phi_atBTL_overThreshE_by_track_phi_atBTL");
  TGraphAsymmErrors *g_track_pt_energyThresholdEff_BTL_singleMu = (TGraphAsymmErrors*)f_singleMu->Get("divide_track_pt_atBTL_overThreshE_by_track_pt_atBTL");
  TGraphAsymmErrors *g_track_pt_energyThresholdEff_BTL_singlePi = (TGraphAsymmErrors*)f_singlePi->Get("divide_track_pt_atBTL_overThreshE_by_track_pt_atBTL");

  // *** 4. Drawing
  //compareTwoPlots(c1, h_recHit_energy_dR05_BTL_singleMu, h_recHit_energy_dR05_BTL_singlePi, "h_recHit_energy_BTL", "BTL #Sigma E^{#Delta R < 0.05}_{recHit} [MeV]", "Event Fraction / 1 MeV", true);
  //compareTwoPlots(c1, h_recHit_energy_dR05_ETL_singleMu, h_recHit_energy_dR05_ETL_singlePi, "h_recHit_energy_ETL", "ETL #Sigma E^{#Delta R < 0.05}_{recHit} [MeV]", "Event Fraction / 0.01 MeV", true);
  //compareTwoPlots(c1, h_recHit_energy_dR05_ETL_fixedRange_singleMu, h_recHit_energy_dR05_ETL_fixedRange_singlePi, "h_recHit_energy_ETL_fixedRange", "ETL #Sigma E^{#Delta R < 0.05}_{recHit} [MeV]", "Event Fraction / 0.1 MeV", true);
  compareTwoPlots(c1, h_recHit_energy_dR05_BTL_singleMu, h_recHit_energy_dR05_BTL_singlePi, "h_recHit_energy_BTL", "BTL #Sigma E^{#Delta R < 0.02}_{recHit} [MeV]", "Event Fraction / 1 MeV", true);
  compareTwoPlots(c1, h_recHit_energy_dR05_ETL_singleMu, h_recHit_energy_dR05_ETL_singlePi, "h_recHit_energy_ETL", "ETL #Sigma E^{#Delta R < 0.02}_{recHit} [MeV]", "Event Fraction / 0.01 MeV", true);
  compareTwoPlots(c1, h_recHit_energy_dR05_ETL_fixedRange_singleMu, h_recHit_energy_dR05_ETL_fixedRange_singlePi, "h_recHit_energy_ETL_fixedRange", "ETL #Sigma E^{#Delta R < 0.02}_{recHit} [MeV]", "Event Fraction / 0.1 MeV", true);
  //compareTwoPlots(c1, h_recHit_energy_dR05_BTL_singleMu, h_recHit_energy_dR05_BTL_singlePi, "h_recHit_energy_BTL", "BTL #Sigma E^{No #Delta R}_{recHit} [MeV]", "Event Fraction / 1 MeV", true);
  //compareTwoPlots(c1, h_recHit_energy_dR05_ETL_singleMu, h_recHit_energy_dR05_ETL_singlePi, "h_recHit_energy_ETL", "ETL #Sigma E^{No #Delta R}_{recHit} [MeV]", "Event Fraction / 0.01 MeV", true);
  //compareTwoPlots(c1, h_recHit_energy_dR05_ETL_fixedRange_singleMu, h_recHit_energy_dR05_ETL_fixedRange_singlePi, "h_recHit_energy_ETL_fixedRange", "ETL #Sigma E^{No #Delta R}_{recHit} [MeV]", "Event Fraction / 0.1 MeV", true);
  compareTwoPlots(c1, h_recHit_maxEnergy_withTrack_BTL_singleMu, h_recHit_maxEnergy_withTrack_BTL_singlePi, "h_recHit_maxHitEnergy_withTrack_BTL", "BTL Max recHit Energy [MeV]", "Event Fraction / 0.1 MeV", true);
  compareTwoPlots(c1, h_recHit_maxEnergy_withTrack_ETL_singleMu, h_recHit_maxEnergy_withTrack_ETL_singlePi, "h_recHit_maxHitEnergy_withTrack_ETL", "ETL Max recHit Energy [MeV]", "Event Fraction / 0.1 MeV", true);
  //compareTwoPlots(c1, h_recHit_maxEnergy_dRpass_withTrack_BTL_singleMu, h_recHit_maxEnergy_dRpass_withTrack_BTL_singlePi, "h_recHit_maxHitEnergy_dRpass_withTrack_BTL", "BTL Max recHit Energy (#Delta R < 0.5) [MeV]", "Event Fraction / 0.1 MeV", true);
  //compareTwoPlots(c1, h_recHit_maxEnergy_dRpass_withTrack_ETL_singleMu, h_recHit_maxEnergy_dRpass_withTrack_ETL_singlePi, "h_recHit_maxHitEnergy_dRpass_withTrack_ETL", "ETL Max recHit Energy (#Delta R < 0.5) [MeV]", "Event Fraction / 0.1 MeV", true);

  compareTwoPlots(c1, h_recHit_maxEnergy_dRpass_withTrack_BTL_singleMu, h_recHit_maxEnergy_dRpass_withTrack_BTL_singlePi, "h_recHit_maxHitEnergy_dRpass_withTrack_BTL", "BTL Max recHit Energy (No #Delta R) [MeV]", "Event Fraction / 0.1 MeV", true);
  compareTwoPlots(c1, h_recHit_maxEnergy_dRpass_withTrack_ETL_singleMu, h_recHit_maxEnergy_dRpass_withTrack_ETL_singlePi, "h_recHit_maxHitEnergy_dRpass_withTrack_ETL", "ETL Max recHit Energy (No #Delta R) [MeV]", "Event Fraction / 0.1 MeV", true);

  compareTwoEfficiencies(c1, g_track_eta_energyThresholdEff_ETL_singleMu, g_track_eta_energyThresholdEff_ETL_singlePi, "h_track_eta_energyThresholdEff_ETL", "#eta at ETL", "Efficiency");
  compareTwoEfficiencies(c1, g_track_phi_energyThresholdEff_ETL_singleMu, g_track_phi_energyThresholdEff_ETL_singlePi, "h_track_phi_energyThresholdEff_ETL", "#phi at ETL", "Efficiency");
  compareTwoEfficiencies(c1, g_track_pt_energyThresholdEff_ETL_singleMu, g_track_pt_energyThresholdEff_ETL_singlePi, "h_track_pt_energyThresholdEff_ETL", "Track p_{T} (ETL-matched) [GeV]", "Efficiency");
  compareTwoEfficiencies(c1, g_track_eta_energyThresholdEff_BTL_singleMu, g_track_eta_energyThresholdEff_BTL_singlePi, "h_track_eta_energyThresholdEff_BTL", "#eta at BTL", "Efficiency");
  compareTwoEfficiencies(c1, g_track_phi_energyThresholdEff_BTL_singleMu, g_track_phi_energyThresholdEff_BTL_singlePi, "h_track_phi_energyThresholdEff_BTL", "#phi at BTL", "Efficiency");
  compareTwoEfficiencies(c1, g_track_pt_energyThresholdEff_BTL_singleMu, g_track_pt_energyThresholdEff_BTL_singlePi, "h_track_pt_energyThresholdEff_BTL", "Track p_{T} (BTL-matched) [GeV]", "Efficiency");

}
