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

string topDir = "";

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

  c0->Print( (topDir + "/" + hname + ".png").c_str() );
}


void compareThreePlots(TCanvas* c0, TH1D *h_singleMu, TH1D *h_singlePi, TH1D *h_minBias, string hname, string xname, string yname, bool setLogY=false)
{
  
  c0->cd();
  c0->Clear();
  c0->SetGrid();

  addOverflow(h_singlePi);
  addOverflow(h_singleMu);
  addOverflow(h_minBias);

  h_singlePi->SetLineColor(kRed);
  h_singlePi->SetLineWidth(2);
  h_singleMu->SetLineColor(kBlue);
  h_singleMu->SetLineWidth(2);
  h_minBias->SetLineColor(kBlack);
  h_minBias->SetLineWidth(2);


  h_singleMu->GetYaxis()->SetTitle( yname.c_str() );
  h_singleMu->GetXaxis()->SetTitle( xname.c_str() );

  h_singleMu->SetMinimum(0.1);
  h_singleMu->SetMaximum(10000);
  h_singleMu->DrawNormalized("HIST");

  if (setLogY) {
    h_singleMu->SetMinimum(0.1);
    h_singlePi->SetMinimum(0.1);
    h_minBias->SetMinimum(0.1);
    h_singleMu->SetMaximum(10);
    h_singlePi->SetMaximum(10);
    h_minBias->SetMaximum(10);
    c0->SetLogy();
    h_singleMu->GetYaxis()->SetRangeUser(0.1, 10.1);
  }
  else
    h_singleMu->GetYaxis()->SetRangeUser(0.0, 1.1);

  h_singlePi->DrawNormalized("same HIST");
  h_minBias->DrawNormalized("same HIST");

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
  string s_meanE_minBias;          // string which will contain the result
  std::ostringstream os_mean_minBias;  // stream used for the conversion
  os_mean_minBias << std::fixed;
  os_mean_minBias << std::setprecision(2) << h_minBias->GetMean();
  s_meanE_minBias = os_mean_minBias.str(); // set 'Result' to the contents of the stream


  TLegend* leg = new TLegend();
  leg = new TLegend(0.55, 0.75, .85, .85);
  leg->AddEntry(h_singleMu, ("Single Muon, <E> = "+s_meanE_mu+" MeV").c_str(), "l");
  leg->AddEntry(h_singlePi, ("Single Pion, <E> = "+s_meanE_pi+" MeV").c_str(), "l");
  leg->AddEntry(h_minBias,  ("Minimum Bias (0PU), <E> = "+s_meanE_minBias+" MeV").c_str(), "l");
  leg->Draw("same");

  CMS_lumi( c0, 0, 0);

  c0->SetLeftMargin(0.15);
  //c0->SetRightMargin(0.05);
  //c0->SetBottomMargin(0.15);

  c0->Print( (topDir + "/" + hname + ".png").c_str() );
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

  c0->Print( (topDir + "/" + hname + ".png").c_str() );
}


void compareThreeEfficiencies(TCanvas* c0, TGraphAsymmErrors *g_singleMu, TGraphAsymmErrors *g_singlePi, TGraphAsymmErrors *g_minBias, string hname, string xname, string yname)
{
  
  c0->cd();
  c0->SetLogy(0);
  c0->Clear();
  c0->SetGrid();

  g_singlePi->SetLineColor(kRed);
  g_singlePi->SetLineWidth(2);
  g_singleMu->SetLineColor(kBlue);
  g_singleMu->SetLineWidth(2);
  g_minBias->SetLineColor(kBlack);
  g_minBias->SetLineWidth(2);

  g_singleMu->GetYaxis()->SetTitle( yname.c_str() );
  g_singleMu->GetXaxis()->SetTitle( xname.c_str() );

  //g_singleMu->GetYaxis()->SetRangeUser(0.0, 1.1);
  g_singleMu->Draw();
  g_singlePi->Draw("same");
  g_minBias->Draw("same");

  TLegend* leg = new TLegend();
  leg = new TLegend(0.65, 0.55, .85, .65);
  leg->AddEntry(g_singleMu, "Single Muons", "l");
  leg->AddEntry(g_singlePi, "Single Pions", "l");
  leg->AddEntry(g_singlePi, "Minimum Bias (0PU)", "l");
  leg->Draw("same");


  CMS_lumi( c0, 0, 0);

  c0->SetLeftMargin(0.15);
  //c0->SetRightMargin(0.05);
  //c0->SetBottomMargin(0.15);

  c0->Print( (topDir + "/" + hname + ".png").c_str() );
}

TGraphAsymmErrors* returnOccupancyCurve(TFile* f0, string s_recHitEnergy_ringN)
{
  TH1D* h_recHitEnergy_ringN = (TH1D*)f0->Get( s_recHitEnergy_ringN.c_str() );
  TH1D* h_occupancy_numerator = new TH1D( ("h_occupancy_numerator_"+s_recHitEnergy_ringN).c_str(), ("h_occupancy_numerator_"+s_recHitEnergy_ringN).c_str(), h_recHitEnergy_ringN->GetXaxis()->GetNbins(), h_recHitEnergy_ringN->GetXaxis()->GetXmin(), h_recHitEnergy_ringN->GetXaxis()->GetXmax() );
  TH1D* h_occupancy_denominator = new TH1D( ("h_occupancy_denominator_"+s_recHitEnergy_ringN).c_str(), ("h_occupancy_denominator_"+s_recHitEnergy_ringN).c_str(), h_recHitEnergy_ringN->GetXaxis()->GetNbins(), h_recHitEnergy_ringN->GetXaxis()->GetXmin(), h_recHitEnergy_ringN->GetXaxis()->GetXmax() );
  
  for (int iBin = 0; iBin < h_recHitEnergy_ringN->GetXaxis()->GetNbins(); iBin++)
    {
      h_occupancy_numerator->SetBinContent  ( iBin, h_recHitEnergy_ringN->Integral(iBin, h_recHitEnergy_ringN->GetXaxis()->GetNbins()+1) );
      h_occupancy_denominator->SetBinContent( iBin, h_recHitEnergy_ringN->Integral(0,    h_recHitEnergy_ringN->GetXaxis()->GetNbins()+1) );
    }
  
  TGraphAsymmErrors* ge = new TGraphAsymmErrors(h_occupancy_numerator, h_occupancy_denominator);
  ge->SetTitle( ("occupancy_" + s_recHitEnergy_ringN).c_str() );
  ge->SetName( ("occupancy_" + s_recHitEnergy_ringN).c_str() );

  return ge;
}


TGraphAsymmErrors* returnTGraphAsymm(TFile* f0, string s_numerator, string s_denominator)
{
  TH1D* h_numerator = (TH1D*)f0->Get( s_numerator.c_str() );
  TH1D* h_denominator = (TH1D*)f0->Get( s_denominator.c_str() );
  TGraphAsymmErrors *ge = new TGraphAsymmErrors(h_numerator, h_denominator);

  return ge;
}

void plotCombiner()
{
  // *** 0. Drawing options
  topDir = "dpg_01-31-19/";
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
  TFile *f_singleMu = new TFile( ("./"+topDir+"/singleMuon_v0_r0_01-31-19.root").c_str(), "READ");
  TFile *f_singlePi = new TFile( ("./"+topDir+"/singlePion_v0_r0_01-31-19.root").c_str(), "READ");
  TFile *f_minBias  = new TFile( ("./"+topDir+"/minBias14TeV_v0_r0_01-31-19.root").c_str(), "READ");

  // *** 3. Plots
  TH1D *h_recHit_energy_dR05_ETL_singleMu = (TH1D*)f_singleMu->Get("recHit_energy_dR05_withTrackETL");
  TH1D *h_recHit_energy_dR05_ETL_fixedRange_singleMu = (TH1D*)f_singleMu->Get("recHit_energy_dR05_withTrackETL_fixedRange");
  TH1D *h_recHit_energy_dR05_BTL_singleMu = (TH1D*)f_singleMu->Get("recHit_energy_dR05_withTrackBTL");
  TH1D *h_recHit_maxEnergy_withTrack_ETL_singleMu = (TH1D*)f_singleMu->Get("recHit_maxEnergy_withTrackETL");
  TH1D *h_recHit_maxEnergy_dRpass_withTrack_ETL_singleMu = (TH1D*)f_singleMu->Get("recHit_maxEnergy_dRpass_withTrackETL");
  TH1D *h_recHit_maxEnergy_withTrack_BTL_singleMu = (TH1D*)f_singleMu->Get("recHit_maxEnergy_withTrackBTL");
  TH1D *h_recHit_maxEnergy_dRpass_withTrack_BTL_singleMu = (TH1D*)f_singleMu->Get("recHit_maxEnergy_dRpass_withTrackBTL");

  TH1D *h_recHit_energy_dR05_ETL_singlePi = (TH1D*)f_singlePi->Get("recHit_energy_dR05_withTrackETL");
  TH1D *h_recHit_energy_dR05_ETL_fixedRange_singlePi = (TH1D*)f_singlePi->Get("recHit_energy_dR05_withTrackETL_fixedRange");
  TH1D *h_recHit_energy_dR05_BTL_singlePi = (TH1D*)f_singlePi->Get("recHit_energy_dR05_withTrackBTL");
  TH1D *h_recHit_maxEnergy_withTrack_ETL_singlePi = (TH1D*)f_singlePi->Get("recHit_maxEnergy_withTrackETL");
  TH1D *h_recHit_maxEnergy_dRpass_withTrack_ETL_singlePi = (TH1D*)f_singlePi->Get("recHit_maxEnergy_dRpass_withTrackETL");
  TH1D *h_recHit_maxEnergy_withTrack_BTL_singlePi = (TH1D*)f_singlePi->Get("recHit_maxEnergy_withTrackBTL");
  TH1D *h_recHit_maxEnergy_dRpass_withTrack_BTL_singlePi = (TH1D*)f_singlePi->Get("recHit_maxEnergy_dRpass_withTrackBTL");

  TH1D *h_recHit_energy_dR05_ETL_minBias = (TH1D*)f_minBias->Get("recHit_energy_dR05_withTrackETL");
  TH1D *h_recHit_energy_dR05_ETL_fixedRange_minBias = (TH1D*)f_minBias->Get("recHit_energy_dR05_withTrackETL_fixedRange");
  TH1D *h_recHit_energy_dR05_BTL_minBias = (TH1D*)f_minBias->Get("recHit_energy_dR05_withTrackBTL");
  TH1D *h_recHit_maxEnergy_withTrack_ETL_minBias = (TH1D*)f_minBias->Get("recHit_maxEnergy_withTrackETL");
  TH1D *h_recHit_maxEnergy_dRpass_withTrack_ETL_minBias = (TH1D*)f_minBias->Get("recHit_maxEnergy_dRpass_withTrackETL");
  TH1D *h_recHit_maxEnergy_withTrack_BTL_minBias = (TH1D*)f_minBias->Get("recHit_maxEnergy_withTrackBTL");
  TH1D *h_recHit_maxEnergy_dRpass_withTrack_BTL_minBias = (TH1D*)f_minBias->Get("recHit_maxEnergy_dRpass_withTrackBTL");

  /*
  TGraphAsymmErrors *g_track_eta_energyThresholdEff_ETL_singleMu = returnTGraphAsymm( f_singleMu, "track_eta_atETL_overThreshE", "track_eta_atETL");
  TGraphAsymmErrors *g_track_eta_energyThresholdEff_ETL_singlePi = returnTGraphAsymm( f_singleMu, "track_eta_atETL_overThreshE", "track_eta_atETL");
  TGraphAsymmErrors *g_track_phi_energyThresholdEff_ETL_singleMu = returnTGraphAsymm( f_singlePi, "track_phi_atETL_overThreshE", "track_phi_atETL");
  TGraphAsymmErrors *g_track_phi_energyThresholdEff_ETL_singlePi = returnTGraphAsymm( f_singleMu, "track_phi_atETL_overThreshE", "track_phi_atETL");
  TGraphAsymmErrors *g_track_pt_energyThresholdEff_ETL_singleMu = returnTGraphAsymm( f_singlePi, "track_pt_atETL_overThreshE", "track_pt_atETL");
  TGraphAsymmErrors *g_track_pt_energyThresholdEff_ETL_singlePi = returnTGraphAsymm( f_singleMu, "track_pt_atETL_overThreshE", "track_pt_atETL");
  TGraphAsymmErrors *g_track_eta_energyThresholdEff_BTL_singleMu = returnTGraphAsymm( f_singlePi, "track_eta_atBTL_overThreshE", "track_eta_atBTL");
  TGraphAsymmErrors *g_track_eta_energyThresholdEff_BTL_singlePi = returnTGraphAsymm( f_singleMu, "track_eta_atBTL_overThreshE", "track_eta_atBTL");
  TGraphAsymmErrors *g_track_phi_energyThresholdEff_BTL_singleMu = returnTGraphAsymm( f_singlePi, "track_phi_atBTL_overThreshE", "track_phi_atBTL");
  TGraphAsymmErrors *g_track_phi_energyThresholdEff_BTL_singlePi = returnTGraphAsymm( f_singleMu, "track_phi_atBTL_overThreshE", "track_phi_atBTL");
  TGraphAsymmErrors *g_track_pt_energyThresholdEff_BTL_singleMu = returnTGraphAsymm( f_singlePi, "track_pt_atBTL_overThreshE", "track_pt_atBTL");
  TGraphAsymmErrors *g_track_pt_energyThresholdEff_BTL_singlePi = returnTGraphAsymm( f_singleMu, "track_pt_atBTL_overThreshE", "track_pt_atBTL");
  */
  // *** 4. Drawing
  compareTwoPlots(c1, h_recHit_energy_dR05_BTL_singleMu, h_recHit_energy_dR05_BTL_singlePi, "h_recHit_energy_BTL", "BTL #Sigma E^{#Delta R < 0.05}_{recHit} [MeV]", "Event Fraction / 1 MeV", true);
  compareTwoPlots(c1, h_recHit_energy_dR05_ETL_singleMu, h_recHit_energy_dR05_ETL_singlePi, "h_recHit_energy_ETL", "ETL #Sigma E^{#Delta R < 0.05}_{recHit} [MeV]", "Event Fraction / 0.01 MeV", true);
  compareTwoPlots(c1, h_recHit_energy_dR05_ETL_fixedRange_singleMu, h_recHit_energy_dR05_ETL_fixedRange_singlePi, "h_recHit_energy_ETL_fixedRange", "ETL #Sigma E^{#Delta R < 0.05}_{recHit} [MeV]", "Event Fraction / 0.1 MeV", true);
  
  compareTwoPlots(c1, h_recHit_maxEnergy_withTrack_BTL_singleMu, h_recHit_maxEnergy_withTrack_BTL_singlePi, "h_recHit_maxHitEnergy_withTrack_BTL", "BTL Max recHit Energy (No #Delta R) [MeV]", "Event Fraction / 0.1 MeV", true);
  compareTwoPlots(c1, h_recHit_maxEnergy_withTrack_ETL_singleMu, h_recHit_maxEnergy_withTrack_ETL_singlePi, "h_recHit_maxHitEnergy_withTrack_ETL", "ETL Max recHit Energy (No #Delta R) [MeV]", "Event Fraction / 0.1 MeV", true);
  
  compareTwoPlots(c1, h_recHit_maxEnergy_dRpass_withTrack_BTL_singleMu, h_recHit_maxEnergy_dRpass_withTrack_BTL_singlePi, "h_recHit_maxHitEnergy_dRpass_withTrack_BTL", "BTL Max recHit Energy (#Delta R < 0.05) [MeV]", "Event Fraction / 0.1 MeV", true);
  compareTwoPlots(c1, h_recHit_maxEnergy_dRpass_withTrack_ETL_singleMu, h_recHit_maxEnergy_dRpass_withTrack_ETL_singlePi, "h_recHit_maxHitEnergy_dRpass_withTrack_ETL", "ETL Max recHit Energy (#Delta R < 0.05) [MeV]", "Event Fraction / 0.1 MeV", true);


  compareThreePlots(c1, h_recHit_energy_dR05_BTL_singleMu, h_recHit_energy_dR05_BTL_singlePi, h_recHit_energy_dR05_ETL_minBias, "h_3_recHit_energy_BTL", "BTL #Sigma E^{#Delta R < 0.05}_{recHit} [MeV]", "Event Fraction / 1 MeV", true);
  compareThreePlots(c1, h_recHit_energy_dR05_ETL_singleMu, h_recHit_energy_dR05_ETL_singlePi, h_recHit_energy_dR05_ETL_minBias, "h_3_recHit_energy_ETL", "ETL #Sigma E^{#Delta R < 0.05}_{recHit} [MeV]", "Event Fraction / 0.01 MeV", true);

  /*compareTwoEfficiencies(c1, g_track_eta_energyThresholdEff_ETL_singleMu, g_track_eta_energyThresholdEff_ETL_singlePi, "h_track_eta_energyThresholdEff_ETL", "#eta at ETL", "Efficiency");
  compareTwoEfficiencies(c1, g_track_phi_energyThresholdEff_ETL_singleMu, g_track_phi_energyThresholdEff_ETL_singlePi, "h_track_phi_energyThresholdEff_ETL", "#phi at ETL", "Efficiency");
  compareTwoEfficiencies(c1, g_track_pt_energyThresholdEff_ETL_singleMu, g_track_pt_energyThresholdEff_ETL_singlePi, "h_track_pt_energyThresholdEff_ETL", "Track p_{T} (ETL-matched) [GeV]", "Efficiency");
  compareTwoEfficiencies(c1, g_track_eta_energyThresholdEff_BTL_singleMu, g_track_eta_energyThresholdEff_BTL_singlePi, "h_track_eta_energyThresholdEff_BTL", "#eta at BTL", "Efficiency");
  compareTwoEfficiencies(c1, g_track_phi_energyThresholdEff_BTL_singleMu, g_track_phi_energyThresholdEff_BTL_singlePi, "h_track_phi_energyThresholdEff_BTL", "#phi at BTL", "Efficiency");
  compareTwoEfficiencies(c1, g_track_pt_energyThresholdEff_BTL_singleMu, g_track_pt_energyThresholdEff_BTL_singlePi, "h_track_pt_energyThresholdEff_BTL", "Track p_{T} (BTL-matched) [GeV]", "Efficiency");
  */

  // *** 5. Occupancies
  TGraphAsymmErrors* recHit_occupancy_VS_energy_allRings_ETL = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsAllRings_ETL");
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring1_ETL    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing1_ETL");
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring2_ETL    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing2_ETL");
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring3_ETL    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing3_ETL");
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring4_ETL    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing4_ETL");
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring5_ETL    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing5_ETL");
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring6_ETL    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing6_ETL");
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring7_ETL    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing7_ETL");
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring8_ETL    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing8_ETL");
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring9_ETL    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing9_ETL");
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring10_ETL   = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing10_ETL");
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring11_ETL   = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing11_ETL");

  recHit_occupancy_VS_energy_allRings_ETL->Draw();
}
