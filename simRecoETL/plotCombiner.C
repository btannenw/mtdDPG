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
  h_singlePi->SetLineWidth(3);
  h_singleMu->SetLineColor(kBlue);
  h_singleMu->SetLineWidth(3);
  h_minBias->SetLineColor(kBlack);
  h_minBias->SetLineWidth(3);


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
  leg->SetTextSize(0.02);
  //leg->AddEntry(h_singleMu, ("Single Muon, <E> = "+s_meanE_mu+" MeV").c_str(), "l");
  //leg->AddEntry(h_singlePi, ("Single Pion, <E> = "+s_meanE_pi+" MeV").c_str(), "l");
  //leg->AddEntry(h_minBias,  ("Minimum Bias (200PU), <E> = "+s_meanE_minBias+" MeV").c_str(), "l");
  leg->AddEntry(h_singleMu, "Single Muon", "l");
  leg->AddEntry(h_singlePi, "Single Pion", "l");
  leg->AddEntry(h_minBias,  "Minimum Bias", "l");
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

  g_singleMu->GetYaxis()->SetRangeUser(0.0, 1.1);
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

  g_singleMu->GetYaxis()->SetRangeUser(0.0, 1.1);
  g_singleMu->Draw();
  g_singlePi->Draw("same");
  g_minBias->Draw("same");

  TLegend* leg = new TLegend();
  leg = new TLegend(0.25, 0.35, .45, .45);
  leg->SetTextSize(0.02);
  leg->AddEntry(g_singleMu, "Single Muons", "l");
  leg->AddEntry(g_singlePi, "Single Pions", "l");
  leg->AddEntry(g_minBias, "Minimum Bias", "l");
  leg->Draw("same");


  CMS_lumi( c0, 0, 0);

  c0->SetLeftMargin(0.15);
  //c0->SetRightMargin(0.05);
  //c0->SetBottomMargin(0.15);

  c0->Print( (topDir + "/" + hname + ".png").c_str() );
}

TGraphAsymmErrors* returnOccupancyCurve(TFile* f0, string s_recHitEnergy_ringN, int ringN, bool isEta=false, bool is200PU=false)
{
  int skipBins = 1;
  double totalModules = 0;
  if (ringN==0)  totalModules = (64+64+80+96+112+112+128+144+160+160+176)*2;
  if (ringN==1)  totalModules = 64*2;
  if (ringN==2)  totalModules = 64*2;
  if (ringN==3)  totalModules = 80*2;
  if (ringN==4)  totalModules = 96*2;
  if (ringN==5)  totalModules = 112*2;
  if (ringN==6)  totalModules = 112*2;
  if (ringN==7)  totalModules = 128*2;
  if (ringN==8)  totalModules = 144*2;
  if (ringN==9)  totalModules = 160*2;
  if (ringN==10) totalModules = 160*2;
  if (ringN==11) totalModules = 176*2;

  totalModules = (64+64+80+96+112+112+128+144+160+160+176)*2;

  TH1D* h_recHitEnergy_ringN = (TH1D*)f0->Get( s_recHitEnergy_ringN.c_str() );
  TH1D* h_nEvents = (TH1D*)f0->Get( "numberOfEvents" );

  string s_nom   = is200PU ? ("h_occupancy_numerator_"+s_recHitEnergy_ringN + "_200PU").c_str() : ("h_occupancy_numerator_"+s_recHitEnergy_ringN + "_noPU").c_str(); 
  string s_denom = is200PU ? ("h_occupancy_denominator_"+s_recHitEnergy_ringN + "_200PU").c_str() : ("h_occupancy_denominator_"+s_recHitEnergy_ringN + "_noPU").c_str(); 

  TH1D* h_occupancy_numerator = new TH1D( s_nom.c_str(), s_nom.c_str(), h_recHitEnergy_ringN->GetXaxis()->GetNbins()-skipBins, h_recHitEnergy_ringN->GetXaxis()->GetBinCenter(skipBins), h_recHitEnergy_ringN->GetXaxis()->GetXmax() );
  TH1D* h_occupancy_denominator = new TH1D( s_denom.c_str(), s_denom.c_str(), h_recHitEnergy_ringN->GetXaxis()->GetNbins()-skipBins, h_recHitEnergy_ringN->GetXaxis()->GetBinCenter(skipBins), h_recHitEnergy_ringN->GetXaxis()->GetXmax() );


  for (int iBin = skipBins+1; iBin < h_recHitEnergy_ringN->GetXaxis()->GetNbins(); iBin++)
    {
      h_occupancy_denominator->SetBinContent( iBin, totalModules*h_nEvents->GetEntries() );
      double numerator = isEta ? h_recHitEnergy_ringN->GetBinContent(iBin) : h_recHitEnergy_ringN->Integral(iBin, h_recHitEnergy_ringN->GetXaxis()->GetNbins()+1);
      //if (is200PU) 
      //numerator = 200*numerator < totalModules*h_nEvents->GetEntries() ? 200*numerator : totalModules*h_nEvents->GetEntries();
      h_occupancy_numerator->SetBinContent  ( iBin, numerator ) ;


      //if(is200PU)
	//cout <<"Occ= " << h_recHitEnergy_ringN->Integral(iBin, h_recHitEnergy_ringN->GetXaxis()->GetNbins()+1)/(totalModules*h_nEvents->GetEntries()) << "\tBin= " << iBin << "\tDenom= " << totalModules*h_nEvents->GetEntries() << "\tNum= " << h_recHitEnergy_ringN->Integral(iBin, h_recHitEnergy_ringN->GetXaxis()->GetNbins()+1) << endl;
	//cout << "Num Integral= " << h_occupancy_numerator->Integral() << "\tDenom integral= " << h_occupancy_denominator->Integral() << endl;
    }

  h_occupancy_numerator->Sumw2();
  h_occupancy_denominator->Sumw2();
  
  TGraphAsymmErrors* ge = new TGraphAsymmErrors(h_occupancy_numerator, h_occupancy_denominator);
  string name = is200PU ? ("occupancy_" + s_recHitEnergy_ringN + "_200PU").c_str() : ("occupancy_" + s_recHitEnergy_ringN + "_noPU").c_str(); 
  ge->SetTitle( name.c_str() );
  ge->SetName( name.c_str() );

  return ge;
}


TGraphAsymmErrors* returnTGraphAsymm(TFile* f0, string s_numerator, string s_denominator)
{
  TH1D* h_numerator = (TH1D*)f0->Get( s_numerator.c_str() );
  TH1D* h_denominator = (TH1D*)f0->Get( s_denominator.c_str() );

  h_numerator->Sumw2();
  h_denominator->Sumw2();

  TGraphAsymmErrors *ge = new TGraphAsymmErrors(h_numerator, h_denominator);

  return ge;
}

void makeOccupancyPlot(TCanvas* c0, TGraphAsymmErrors *g_occupancy, string hname, string xname, string yname, bool isLogX=true, bool isLogY=false, bool is200PU=false)
{
  c0->cd();
  c0->SetLogy(0);
  c0->SetLogx(0);
  c0->Clear();
  c0->SetGrid();
  if(isLogX) c0->SetLogx();
  if(isLogY) c0->SetLogy();
  //c0->SetLogy();

  g_occupancy->SetLineColor(kBlack);
  g_occupancy->SetLineWidth(2);

  //g_occupancy->Scale(200 );
  g_occupancy->GetYaxis()->SetTitle( yname.c_str() );
  g_occupancy->GetXaxis()->SetTitle( xname.c_str() );
  g_occupancy->GetXaxis()->SetRangeUser(0.01,3);
  g_occupancy->GetYaxis()->SetRangeUser(0.00001, 2.1);
  
  g_occupancy->Draw();

  TLegend* leg = new TLegend();
  leg = new TLegend(0.50, 0.75, .70, .85);
  leg->SetTextSize(0.03);
  if(is200PU) leg->AddEntry(g_occupancy, "Min Bias 200PU", "l");
  else        leg->AddEntry(g_occupancy, "Min Bias No PU", "l");
  leg->Draw("same");


  CMS_lumi( c0, 0, 0);

  c0->SetLeftMargin(0.15);
  //c0->SetRightMargin(0.05);
  //c0->SetBottomMargin(0.15);

  c0->Print( (topDir + "/" + hname + ".png").c_str() );

}


void makeOccupancyPlotAllRings(TCanvas* c0, TGraphAsymmErrors *g_r1, TGraphAsymmErrors *g_r2, TGraphAsymmErrors *g_r3, TGraphAsymmErrors *g_r4, TGraphAsymmErrors *g_r5, TGraphAsymmErrors *g_r6, TGraphAsymmErrors *g_r7, TGraphAsymmErrors *g_r8, TGraphAsymmErrors *g_r9, TGraphAsymmErrors *g_r10, TGraphAsymmErrors *g_r11, string hname, string xname, string yname, bool isLogX=true, bool isLogY=false, bool is200PU=false)
{
  c0->cd();
  c0->SetLogy(0);
  c0->SetLogx(0);
  c0->Clear();
  c0->SetGrid();
  if(isLogX) c0->SetLogx();
  if(isLogY) c0->SetLogy();

  g_r1->Draw();
  g_r1->SetLineColor(kPink+10);
  g_r1->SetMarkerColor(kPink+10);
  //g_r1->SetMarkerStyle(0);
  g_r1->SetLineWidth(3);
  g_r2->SetLineColor(kBlue);
  g_r2->SetMarkerColor(kBlue);
  g_r2->SetLineWidth(3);
  g_r3->SetLineColor(kOrange+10);
  g_r3->SetMarkerColor(kOrange+10);
  g_r3->SetLineWidth(3);
  g_r4->SetLineColor(kMagenta+2);
  g_r4->SetMarkerColor(kMagenta+2);
  g_r4->SetLineWidth(3);
  g_r5->SetLineColor(kGreen+1);
  g_r5->SetMarkerColor(kGreen+1);
  g_r5->SetLineWidth(3);
  g_r6->SetLineColor(kMagenta-4);
  g_r6->SetMarkerColor(kMagenta-4);
  g_r6->SetLineWidth(3);
  g_r7->SetLineColor(kCyan+1);
  g_r7->SetMarkerColor(kCyan+1);
  g_r7->SetLineWidth(3);
  g_r8->SetLineColor(kRed-6);
  g_r8->SetMarkerColor(kRed-6);
  g_r8->SetLineWidth(3);
  g_r9->SetLineColor(kOrange-6);
  g_r9->SetMarkerColor(kOrange-6);
  g_r9->SetLineWidth(3);
  g_r10->SetLineColor(kRed);
  g_r10->SetMarkerColor(kRed);
  g_r10->SetLineWidth(3);
  g_r11->SetLineColor(kBlue-10);
  g_r11->SetMarkerColor(kBlue-10);
  g_r11->SetLineWidth(3);

  g_r1->GetYaxis()->SetTitle( yname.c_str() );
  g_r1->GetXaxis()->SetTitle( xname.c_str() );
  //g_r1->GetXaxis()->SetRangeUser(0.01,3);
  if(is200PU)    g_r1->GetYaxis()->SetRangeUser(0.001, 2.1);
  else           g_r1->GetYaxis()->SetRangeUser(0.00001, 2.1);

  g_r2->Draw("same");
  g_r3->Draw("same");
  g_r4->Draw("same");
  g_r5->Draw("same");
  g_r6->Draw("same");
  g_r7->Draw("same");
  g_r8->Draw("same");
  g_r9->Draw("same");
  g_r10->Draw("same");
  g_r11->Draw("same");

  TLegend* leg = new TLegend();
  leg = new TLegend(0.30, 0.60, .50, .85);
  leg->SetTextSize(0.02);
  leg->AddEntry(g_r1, "ETL Ring 1", "l");
  leg->AddEntry(g_r2, "ETL Ring 2", "l");
  leg->AddEntry(g_r3, "ETL Ring 3", "l");
  leg->AddEntry(g_r4, "ETL Ring 4", "l");
  leg->AddEntry(g_r5, "ETL Ring 5", "l");
  leg->AddEntry(g_r6, "ETL Ring 6", "l");
  leg->AddEntry(g_r7, "ETL Ring 7", "l");
  leg->AddEntry(g_r8, "ETL Ring 8", "l");
  leg->AddEntry(g_r9, "ETL Ring 9", "l");
  leg->AddEntry(g_r10, "ETL Ring 10", "l");
  leg->AddEntry(g_r11, "ETL Ring 11", "l");
  leg->Draw("same");

  TLatex ltx1;
  ltx1.SetTextAlign(9);
  ltx1.SetTextFont(62);
  ltx1.SetTextSize(0.022);
  ltx1.SetNDC();
  if(is200PU)  ltx1.DrawLatex(0.72, 0.87, "Min Bias 200 PU");
  else         ltx1.DrawLatex(0.72, 0.87, "Min Bias No PU");

  CMS_lumi( c0, 0, 0);

  c0->SetLeftMargin(0.15);
  //c0->SetRightMargin(0.05);
  //c0->SetBottomMargin(0.15);

  c0->Print( (topDir + "/" + hname + ".png").c_str() );

}



void plotCombiner()
{
  // *** 0. Drawing options
  topDir = "dpg_02-08-19/";
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
  TFile *f_singleMu = new TFile( ("./"+topDir+"/singleMuon_v1_r0_02-08-19.root").c_str(), "READ");
  TFile *f_singlePi = new TFile( ("./"+topDir+"/singlePion_v1_r0_02-08-19.root").c_str(), "READ");
  TFile *f_minBias  = new TFile( ("./"+topDir+"/nuGun200PU_v1_r0_02-08-19.root").c_str(), "READ");

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

  TGraphAsymmErrors *g_track_eta_energyThresholdEff_ETL_singleMu = returnTGraphAsymm( f_singleMu, "track_eta_atETL_overThreshE", "track_eta_atETL");
  TGraphAsymmErrors *g_track_phi_energyThresholdEff_ETL_singleMu = returnTGraphAsymm( f_singleMu, "track_phi_atETL_overThreshE", "track_phi_atETL");
  TGraphAsymmErrors *g_track_pt_energyThresholdEff_ETL_singleMu = returnTGraphAsymm( f_singleMu, "track_pt_atETL_overThreshE", "track_pt_atETL");
  TGraphAsymmErrors *g_track_eta_energyThresholdEff_BTL_singleMu = returnTGraphAsymm( f_singleMu, "track_eta_atBTL_overThreshE", "track_eta_atBTL");
  TGraphAsymmErrors *g_track_phi_energyThresholdEff_BTL_singleMu = returnTGraphAsymm( f_singleMu, "track_phi_atBTL_overThreshE", "track_phi_atBTL");
  TGraphAsymmErrors *g_track_pt_energyThresholdEff_BTL_singleMu = returnTGraphAsymm( f_singleMu, "track_pt_atBTL_overThreshE", "track_pt_atBTL");

  TGraphAsymmErrors *g_track_eta_energyThresholdEff_ETL_singlePi = returnTGraphAsymm( f_singlePi, "track_eta_atETL_overThreshE", "track_eta_atETL");
  TGraphAsymmErrors *g_track_phi_energyThresholdEff_ETL_singlePi = returnTGraphAsymm( f_singlePi, "track_phi_atETL_overThreshE", "track_phi_atETL");
  TGraphAsymmErrors *g_track_pt_energyThresholdEff_ETL_singlePi = returnTGraphAsymm( f_singlePi, "track_pt_atETL_overThreshE", "track_pt_atETL");
  TGraphAsymmErrors *g_track_eta_energyThresholdEff_BTL_singlePi = returnTGraphAsymm( f_singlePi, "track_eta_atBTL_overThreshE", "track_eta_atBTL");
  TGraphAsymmErrors *g_track_phi_energyThresholdEff_BTL_singlePi = returnTGraphAsymm( f_singlePi, "track_phi_atBTL_overThreshE", "track_phi_atBTL");
  TGraphAsymmErrors *g_track_pt_energyThresholdEff_BTL_singlePi = returnTGraphAsymm( f_singlePi, "track_pt_atBTL_overThreshE", "track_pt_atBTL");

  TGraphAsymmErrors *g_track_eta_energyThresholdEff_ETL_minBias = returnTGraphAsymm( f_minBias, "track_eta_atETL_overThreshE", "track_eta_atETL");
  TGraphAsymmErrors *g_track_phi_energyThresholdEff_ETL_minBias = returnTGraphAsymm( f_minBias, "track_phi_atETL_overThreshE", "track_phi_atETL");
  TGraphAsymmErrors *g_track_pt_energyThresholdEff_ETL_minBias = returnTGraphAsymm( f_minBias, "track_pt_atETL_overThreshE", "track_pt_atETL");
  TGraphAsymmErrors *g_track_eta_energyThresholdEff_BTL_minBias = returnTGraphAsymm( f_minBias, "track_eta_atBTL_overThreshE", "track_eta_atBTL");
  TGraphAsymmErrors *g_track_phi_energyThresholdEff_BTL_minBias = returnTGraphAsymm( f_minBias, "track_phi_atBTL_overThreshE", "track_phi_atBTL");
  TGraphAsymmErrors *g_track_pt_energyThresholdEff_BTL_minBias = returnTGraphAsymm( f_minBias, "track_pt_atBTL_overThreshE", "track_pt_atBTL");
 
 
  // *** 4. Drawing
  compareTwoPlots(c1, h_recHit_energy_dR05_BTL_singleMu, h_recHit_energy_dR05_BTL_singlePi, "h_recHit_energy_BTL", "BTL #Sigma E^{#Delta R < 0.05}_{recHit} [MeV]", "Event Fraction / 1 MeV", true);
  compareTwoPlots(c1, h_recHit_energy_dR05_ETL_singleMu, h_recHit_energy_dR05_ETL_singlePi, "h_recHit_energy_ETL", "ETL #Sigma E^{#Delta R < 0.05}_{recHit} [MeV]", "Event Fraction / 0.01 MeV", true);
  compareTwoPlots(c1, h_recHit_energy_dR05_ETL_fixedRange_singleMu, h_recHit_energy_dR05_ETL_fixedRange_singlePi, "h_recHit_energy_ETL_fixedRange", "ETL #Sigma E^{#Delta R < 0.05}_{recHit} [MeV]", "Event Fraction / 0.1 MeV", true);
  
  compareTwoPlots(c1, h_recHit_maxEnergy_withTrack_BTL_singleMu, h_recHit_maxEnergy_withTrack_BTL_singlePi, "h_recHit_maxHitEnergy_withTrack_BTL", "BTL Max recHit Energy (No #Delta R) [MeV]", "Event Fraction / 0.1 MeV", true);
  compareTwoPlots(c1, h_recHit_maxEnergy_withTrack_ETL_singleMu, h_recHit_maxEnergy_withTrack_ETL_singlePi, "h_recHit_maxHitEnergy_withTrack_ETL", "ETL Max recHit Energy (No #Delta R) [MeV]", "Event Fraction / 0.1 MeV", true);
  
  compareTwoPlots(c1, h_recHit_maxEnergy_dRpass_withTrack_BTL_singleMu, h_recHit_maxEnergy_dRpass_withTrack_BTL_singlePi, "h_recHit_maxHitEnergy_dRpass_withTrack_BTL", "BTL Max recHit Energy (#Delta R < 0.05) [MeV]", "Event Fraction / 0.1 MeV", true);
  compareTwoPlots(c1, h_recHit_maxEnergy_dRpass_withTrack_ETL_singleMu, h_recHit_maxEnergy_dRpass_withTrack_ETL_singlePi, "h_recHit_maxHitEnergy_dRpass_withTrack_ETL", "ETL Max recHit Energy (#Delta R < 0.05) [MeV]", "Event Fraction / 0.1 MeV", true);

  compareThreePlots(c1, h_recHit_energy_dR05_BTL_singleMu, h_recHit_energy_dR05_BTL_singlePi, h_recHit_energy_dR05_BTL_minBias, "h_3_recHit_energy_BTL", "BTL #Sigma E^{#Delta R < 0.05}_{recHit} [MeV]", "Event Fraction / 1 MeV", true);
  compareThreePlots(c1, h_recHit_energy_dR05_ETL_singleMu, h_recHit_energy_dR05_ETL_singlePi, h_recHit_energy_dR05_ETL_minBias, "h_3_recHit_energy_ETL", "ETL #Sigma E^{#Delta R < 0.05}_{recHit} [MeV]", "Event Fraction / 0.01 MeV", true);

  compareTwoEfficiencies(c1, g_track_eta_energyThresholdEff_ETL_singleMu, g_track_eta_energyThresholdEff_ETL_singlePi, "h_track_eta_energyThresholdEff_ETL", "#eta at ETL", "Efficiency");
  compareTwoEfficiencies(c1, g_track_phi_energyThresholdEff_ETL_singleMu, g_track_phi_energyThresholdEff_ETL_singlePi, "h_track_phi_energyThresholdEff_ETL", "#phi at ETL", "Efficiency");
  compareTwoEfficiencies(c1, g_track_pt_energyThresholdEff_ETL_singleMu, g_track_pt_energyThresholdEff_ETL_singlePi, "h_track_pt_energyThresholdEff_ETL", "Track p_{T} (ETL-matched) [GeV]", "Efficiency");
  compareTwoEfficiencies(c1, g_track_eta_energyThresholdEff_BTL_singleMu, g_track_eta_energyThresholdEff_BTL_singlePi, "h_track_eta_energyThresholdEff_BTL", "#eta at BTL", "Efficiency");
  compareTwoEfficiencies(c1, g_track_phi_energyThresholdEff_BTL_singleMu, g_track_phi_energyThresholdEff_BTL_singlePi, "h_track_phi_energyThresholdEff_BTL", "#phi at BTL", "Efficiency");
  compareTwoEfficiencies(c1, g_track_pt_energyThresholdEff_BTL_singleMu, g_track_pt_energyThresholdEff_BTL_singlePi, "h_track_pt_energyThresholdEff_BTL", "Track p_{T} (BTL-matched) [GeV]", "Efficiency");

  compareThreeEfficiencies(c1, g_track_eta_energyThresholdEff_ETL_singleMu, g_track_eta_energyThresholdEff_ETL_singlePi, g_track_eta_energyThresholdEff_ETL_minBias, "h_3_track_eta_energyThresholdEff_ETL", "#eta at ETL", "Efficiency");
  compareThreeEfficiencies(c1, g_track_phi_energyThresholdEff_ETL_singleMu, g_track_phi_energyThresholdEff_ETL_singlePi, g_track_phi_energyThresholdEff_ETL_minBias, "h_3_track_phi_energyThresholdEff_ETL", "#phi at ETL", "Efficiency");
  compareThreeEfficiencies(c1, g_track_pt_energyThresholdEff_ETL_singleMu, g_track_pt_energyThresholdEff_ETL_singlePi, g_track_pt_energyThresholdEff_ETL_minBias, "h_3_track_pt_energyThresholdEff_ETL", "Track p_{T} (ETL-matched) [GeV]", "Efficiency");
  compareThreeEfficiencies(c1, g_track_eta_energyThresholdEff_BTL_singleMu, g_track_eta_energyThresholdEff_BTL_singlePi, g_track_eta_energyThresholdEff_BTL_minBias, "h_3_track_eta_energyThresholdEff_BTL", "#eta at BTL", "Efficiency");
  compareThreeEfficiencies(c1, g_track_phi_energyThresholdEff_BTL_singleMu, g_track_phi_energyThresholdEff_BTL_singlePi, g_track_phi_energyThresholdEff_BTL_minBias, "h_3_track_phi_energyThresholdEff_BTL", "#phi at BTL", "Efficiency");
  compareThreeEfficiencies(c1, g_track_pt_energyThresholdEff_BTL_singleMu, g_track_pt_energyThresholdEff_BTL_singlePi, g_track_pt_energyThresholdEff_BTL_minBias, "h_3_track_pt_energyThresholdEff_BTL", "Track p_{T} (BTL-matched) [GeV]", "Efficiency");
  

  // *** 5. Occupancies
  /*TGraphAsymmErrors* recHit_occupancy_VS_energy_allRings_ETL_noPU = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsAllRings_ETL", 0, false);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring1_ETL_noPU    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing1_ETL",    1, false);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring2_ETL_noPU    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing2_ETL",    2, false);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring3_ETL_noPU    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing3_ETL",    3, false);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring4_ETL_noPU    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing4_ETL",    4, false);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring5_ETL_noPU    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing5_ETL",    5, false);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring6_ETL_noPU    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing6_ETL",    6, false);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring7_ETL_noPU    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing7_ETL",    7, false);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring8_ETL_noPU    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing8_ETL",    8, false);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring9_ETL_noPU    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing9_ETL",    9, false);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring10_ETL_noPU   = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing10_ETL",  10, false);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring11_ETL_noPU   = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing11_ETL",  11, false);
  */

  TGraphAsymmErrors* recHit_occupancy_VS_energy_allRings_ETL_200PU = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsAllRings_ETL", 0, false, true);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring1_ETL_200PU    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing1_ETL",    1, false, true);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring2_ETL_200PU    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing2_ETL",    2, false, true);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring3_ETL_200PU    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing3_ETL",    3, false, true);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring4_ETL_200PU    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing4_ETL",    4, false, true);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring5_ETL_200PU    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing5_ETL",    5, false, true);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring6_ETL_200PU    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing6_ETL",    6, false, true);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring7_ETL_200PU    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing7_ETL",    7, false, true);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring8_ETL_200PU    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing8_ETL",    8, false, true);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring9_ETL_200PU    = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing9_ETL",    9, false, true);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring10_ETL_200PU   = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing10_ETL",  10, false, true);
  TGraphAsymmErrors* recHit_occupancy_VS_energy_Ring11_ETL_200PU   = returnOccupancyCurve(f_minBias, "recHit_energy_allHitsRing11_ETL",  11, false, true);
  
  /*TGraphAsymmErrors* recHit_occupancy_VS_eta_allRings_ETL_noPU = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsAllRings_ETL", 0, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring1_ETL_noPU    = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing1_ETL",    1, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring2_ETL_noPU    = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing2_ETL",    2, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring3_ETL_noPU    = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing3_ETL",    3, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring4_ETL_noPU    = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing4_ETL",    4, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring5_ETL_noPU    = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing5_ETL",    5, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring6_ETL_noPU    = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing6_ETL",    6, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring7_ETL_noPU    = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing7_ETL",    7, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring8_ETL_noPU    = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing8_ETL",    8, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring9_ETL_noPU    = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing9_ETL",    9, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring10_ETL_noPU   = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing10_ETL",  10, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring11_ETL_noPU   = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing11_ETL",  11, true);
  */
  TGraphAsymmErrors* recHit_occupancy_VS_eta_allRings_ETL_200PU = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsAllRings_ETL", 0, true, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring1_ETL_200PU    = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing1_ETL",    1, true, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring2_ETL_200PU    = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing2_ETL",    2, true, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring3_ETL_200PU    = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing3_ETL",    3, true, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring4_ETL_200PU    = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing4_ETL",    4, true, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring5_ETL_200PU    = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing5_ETL",    5, true, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring6_ETL_200PU    = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing6_ETL",    6, true, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring7_ETL_200PU    = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing7_ETL",    7, true, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring8_ETL_200PU    = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing8_ETL",    8, true, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring9_ETL_200PU    = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing9_ETL",    9, true, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring10_ETL_200PU   = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing10_ETL",  10, true, true);
  TGraphAsymmErrors* recHit_occupancy_VS_eta_Ring11_ETL_200PU   = returnOccupancyCurve(f_minBias, "recHit_eta_allHitsRing11_ETL",  11, true, true);
  
  
  //makeOccupancyPlot(c1, recHit_occupancy_VS_energy_allRings_ETL_noPU, "occupancy_recHit_energy_allHitsAllRings_noPU", "Threshold Energy [MeV]", "ETL Occupancy", false, true);
  makeOccupancyPlot(c1, recHit_occupancy_VS_energy_allRings_ETL_200PU, "occupancy_recHit_energy_allHitsAllRings_200PU", "Threshold Energy [MeV]", "ETL Occupancy", false, true, true);
  /*makeOccupancyPlot(c1, recHit_occupancy_VS_energy_Ring1_ETL, "occupancy_recHit_energy_allHitsRing1", "Threshold Energy [MeV]", "Occupancy");
  makeOccupancyPlot(c1, recHit_occupancy_VS_energy_Ring2_ETL, "occupancy_recHit_energy_allHitsRing2", "Threshold Energy [MeV]", "Occupancy");
  makeOccupancyPlot(c1, recHit_occupancy_VS_energy_Ring3_ETL, "occupancy_recHit_energy_allHitsRing3", "Threshold Energy [MeV]", "Occupancy");
  makeOccupancyPlot(c1, recHit_occupancy_VS_energy_Ring4_ETL, "occupancy_recHit_energy_allHitsRing4", "Threshold Energy [MeV]", "Occupancy");
  makeOccupancyPlot(c1, recHit_occupancy_VS_energy_Ring5_ETL, "occupancy_recHit_energy_allHitsRing5", "Threshold Energy [MeV]", "Occupancy");
  makeOccupancyPlot(c1, recHit_occupancy_VS_energy_Ring6_ETL, "occupancy_recHit_energy_allHitsRing6", "Threshold Energy [MeV]", "Occupancy");
  makeOccupancyPlot(c1, recHit_occupancy_VS_energy_Ring7_ETL, "occupancy_recHit_energy_allHitsRing7", "Threshold Energy [MeV]", "Occupancy");
  makeOccupancyPlot(c1, recHit_occupancy_VS_energy_Ring8_ETL, "occupancy_recHit_energy_allHitsRing8", "Threshold Energy [MeV]", "Occupancy");
  makeOccupancyPlot(c1, recHit_occupancy_VS_energy_Ring9_ETL, "occupancy_recHit_energy_allHitsRing9", "Threshold Energy [MeV]", "Occupancy");
  makeOccupancyPlot(c1, recHit_occupancy_VS_energy_Ring10_ETL, "occupancy_recHit_energy_allHitsRing10", "Threshold Energy [MeV]", "Occupancy");
  makeOccupancyPlot(c1, recHit_occupancy_VS_energy_Ring11_ETL, "occupancy_recHit_energy_allHitsRing11", "Threshold Energy [MeV]", "Occupancy");  */

  //makeOccupancyPlot(c1, recHit_occupancy_VS_eta_allRings_ETL_noPU, "occupancy_recHit_eta_allHitsAllRings_noPU", "ETL Rec Hit #eta", "ETL Occupancy", false, true);
  makeOccupancyPlot(c1, recHit_occupancy_VS_eta_allRings_ETL_200PU, "occupancy_recHit_eta_allHitsAllRings_200PU", "ETL Rec Hit #eta", "ETL Occupancy", false, true, true);
  /*makeOccupancyPlot(c1, recHit_occupancy_VS_eta_Ring1_ETL, "occupancy_recHit_eta_allHitsRing1", "ETL Rec Hit #eta", "Occupancy", false);
  makeOccupancyPlot(c1, recHit_occupancy_VS_eta_Ring2_ETL, "occupancy_recHit_eta_allHitsRing2", "ETL Rec Hit #eta", "Occupancy", false);
  makeOccupancyPlot(c1, recHit_occupancy_VS_eta_Ring3_ETL, "occupancy_recHit_eta_allHitsRing3", "ETL Rec Hit #eta", "Occupancy", false);
  makeOccupancyPlot(c1, recHit_occupancy_VS_eta_Ring4_ETL, "occupancy_recHit_eta_allHitsRing4", "ETL Rec Hit #eta", "Occupancy", false);
  makeOccupancyPlot(c1, recHit_occupancy_VS_eta_Ring5_ETL, "occupancy_recHit_eta_allHitsRing5", "ETL Rec Hit #eta", "Occupancy", false);
  makeOccupancyPlot(c1, recHit_occupancy_VS_eta_Ring6_ETL, "occupancy_recHit_eta_allHitsRing6", "ETL Rec Hit #eta", "Occupancy", false);
  makeOccupancyPlot(c1, recHit_occupancy_VS_eta_Ring7_ETL, "occupancy_recHit_eta_allHitsRing7", "ETL Rec Hit #eta", "Occupancy", false);
  makeOccupancyPlot(c1, recHit_occupancy_VS_eta_Ring8_ETL, "occupancy_recHit_eta_allHitsRing8", "ETL Rec Hit #eta", "Occupancy", false);
  makeOccupancyPlot(c1, recHit_occupancy_VS_eta_Ring9_ETL, "occupancy_recHit_eta_allHitsRing9", "ETL Rec Hit #eta", "Occupancy", false);
  makeOccupancyPlot(c1, recHit_occupancy_VS_eta_Ring10_ETL, "occupancy_recHit_eta_allHitsRing10", "ETL Rec Hit #eta", "Occupancy", false);
  makeOccupancyPlot(c1, recHit_occupancy_VS_eta_Ring11_ETL, "occupancy_recHit_eta_allHitsRing11", "ETL Rec Hit #eta", "Occupancy", false);  */

  //makeOccupancyPlotAllRings(c1, recHit_occupancy_VS_eta_Ring1_ETL_noPU, recHit_occupancy_VS_eta_Ring2_ETL_noPU, recHit_occupancy_VS_eta_Ring3_ETL_noPU, recHit_occupancy_VS_eta_Ring4_ETL_noPU, recHit_occupancy_VS_eta_Ring5_ETL_noPU, recHit_occupancy_VS_eta_Ring6_ETL_noPU, recHit_occupancy_VS_eta_Ring7_ETL_noPU, recHit_occupancy_VS_eta_Ring8_ETL_noPU, recHit_occupancy_VS_eta_Ring9_ETL_noPU, recHit_occupancy_VS_eta_Ring10_ETL_noPU, recHit_occupancy_VS_eta_Ring11_ETL_noPU, "occupancy_recHit_eta_allHitsRings1to11_noPU", "ETL Rec Hit #eta", "ETL Occupancy", false, true, false);
  //makeOccupancyPlotAllRings(c1, recHit_occupancy_VS_energy_Ring1_ETL_noPU, recHit_occupancy_VS_energy_Ring2_ETL_noPU, recHit_occupancy_VS_energy_Ring3_ETL_noPU, recHit_occupancy_VS_energy_Ring4_ETL_noPU, recHit_occupancy_VS_energy_Ring5_ETL_noPU, recHit_occupancy_VS_energy_Ring6_ETL_noPU, recHit_occupancy_VS_energy_Ring7_ETL_noPU, recHit_occupancy_VS_energy_Ring8_ETL_noPU, recHit_occupancy_VS_energy_Ring9_ETL_noPU, recHit_occupancy_VS_energy_Ring10_ETL_noPU, recHit_occupancy_VS_energy_Ring11_ETL_noPU, "occupancy_recHit_energy_allHitsRings1to11_noPU", "Threshold Energy [MeV]", "ETL Occupancy", false, true, false);

  makeOccupancyPlotAllRings(c1, recHit_occupancy_VS_eta_Ring1_ETL_200PU, recHit_occupancy_VS_eta_Ring2_ETL_200PU, recHit_occupancy_VS_eta_Ring3_ETL_200PU, recHit_occupancy_VS_eta_Ring4_ETL_200PU, recHit_occupancy_VS_eta_Ring5_ETL_200PU, recHit_occupancy_VS_eta_Ring6_ETL_200PU, recHit_occupancy_VS_eta_Ring7_ETL_200PU, recHit_occupancy_VS_eta_Ring8_ETL_200PU, recHit_occupancy_VS_eta_Ring9_ETL_200PU, recHit_occupancy_VS_eta_Ring10_ETL_200PU, recHit_occupancy_VS_eta_Ring11_ETL_200PU, "occupancy_recHit_eta_allHitsRings1to11_200PU", "ETL Rec Hit #eta", "ETL Occupancy", false, true, true);
  makeOccupancyPlotAllRings(c1, recHit_occupancy_VS_energy_Ring1_ETL_200PU, recHit_occupancy_VS_energy_Ring2_ETL_200PU, recHit_occupancy_VS_energy_Ring3_ETL_200PU, recHit_occupancy_VS_energy_Ring4_ETL_200PU, recHit_occupancy_VS_energy_Ring5_ETL_200PU, recHit_occupancy_VS_energy_Ring6_ETL_200PU, recHit_occupancy_VS_energy_Ring7_ETL_200PU, recHit_occupancy_VS_energy_Ring8_ETL_200PU, recHit_occupancy_VS_energy_Ring9_ETL_200PU, recHit_occupancy_VS_energy_Ring10_ETL_200PU, recHit_occupancy_VS_energy_Ring11_ETL_200PU, "occupancy_recHit_energy_allHitsRings1to11_200PU", "Threshold Energy [MeV]", "ETL Occupancy", false, true, true);


}
