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
  c0->SetLogy(0);
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
  c0->SetLogy(0);
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
      double denominator = isEta ? totalModules*h_nEvents->GetEntries() : h_recHitEnergy_ringN->Integral(0, h_recHitEnergy_ringN->GetXaxis()->GetNbins()+1);
      double numerator = isEta ? h_recHitEnergy_ringN->GetBinContent(iBin) : h_recHitEnergy_ringN->Integral(iBin, h_recHitEnergy_ringN->GetXaxis()->GetNbins()+1);

      h_occupancy_numerator->SetBinContent  ( iBin, numerator ) ;
      h_occupancy_denominator->SetBinContent( iBin, denominator );

      //if(ringN==0)
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
  if (hname.find("_eta_")!=string::npos)
    leg = new TLegend(0.30, 0.60, .50, .85);
  else
    leg = new TLegend(0.30, 0.20, .50, .45);

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

void makeComparisonPlots(TFile *singleMu, TFile *singlePi, TFile *minBias, TCanvas *c0, string variable, string xTitle, string yTitle, bool isLogY=true)
{
  TH1D *h_singleMu = (TH1D*)singleMu->Get( variable.c_str() );
  TH1D *h_singlePi = (TH1D*)singlePi->Get( variable.c_str() );
  TH1D *h_minBias  = (TH1D*)minBias->Get( variable.c_str() );

  compareTwoPlots(c0, h_singleMu, h_singlePi, ("h_"+variable).c_str(), xTitle.c_str(), yTitle.c_str(), isLogY);
  compareThreePlots(c0, h_singleMu, h_singlePi, h_minBias, ("h_3_"+variable).c_str(), xTitle.c_str(), yTitle.c_str(), isLogY);

}


void makeEfficiencyCurves(TFile *singleMu, TFile *singlePi, TFile *minBias, TCanvas *c0, string variable, string xTitle, string yTitle)
{
  TGraphAsymmErrors *g_minBias  = returnTGraphAsymm( minBias,  (variable + "_overThreshE").c_str(), variable.c_str() );
  TGraphAsymmErrors *g_singleMu = returnTGraphAsymm( singleMu, (variable + "_overThreshE").c_str(), variable.c_str() );
  TGraphAsymmErrors *g_singlePi = returnTGraphAsymm( singlePi, (variable + "_overThreshE").c_str(), variable.c_str() );

  compareTwoEfficiencies(c0, g_singleMu, g_singlePi, ("h_" + variable + "_energyThresholdEff").c_str(), xTitle.c_str(), "Efficiency");
  compareThreeEfficiencies(c0, g_singleMu, g_singlePi, g_minBias, ("h_3_" + variable + "_energyThresholdEff").c_str(), xTitle.c_str(), "Efficiency");

}


void makeAndDrawOccupancyPlots(TFile *f0, TCanvas *c0, string variable, bool isEta=false, bool is200PU=true, bool isLogX=true, bool isLogY=false)
{
  TGraphAsymmErrors* recHit_occupancy_VS_var_allRings_ETL_200PU = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsAllRings_ETL").c_str(), 0, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring1_ETL_200PU    = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing1_ETL").c_str(),    1, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring2_ETL_200PU    = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing2_ETL").c_str(),    2, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring3_ETL_200PU    = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing3_ETL").c_str(),    3, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring4_ETL_200PU    = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing4_ETL").c_str(),    4, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring5_ETL_200PU    = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing5_ETL").c_str(),    5, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring6_ETL_200PU    = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing6_ETL").c_str(),    6, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring7_ETL_200PU    = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing7_ETL").c_str(),    7, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring8_ETL_200PU    = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing8_ETL").c_str(),    8, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring9_ETL_200PU    = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing9_ETL").c_str(),    9, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring10_ETL_200PU   = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing10_ETL").c_str(),  10, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring11_ETL_200PU   = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing11_ETL").c_str(),  11, isEta, is200PU);

  TGraphAsymmErrors* recHit_occupancy_VS_var_allRings_ETL_plusSide_200PU = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsAllRings_ETL_plusSide").c_str(), 0, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring1_ETL_plusSide_200PU    = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing1_ETL_plusSide").c_str(),    1, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring2_ETL_plusSide_200PU    = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing2_ETL_plusSide").c_str(),    2, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring3_ETL_plusSide_200PU    = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing3_ETL_plusSide").c_str(),    3, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring4_ETL_plusSide_200PU    = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing4_ETL_plusSide").c_str(),    4, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring5_ETL_plusSide_200PU    = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing5_ETL_plusSide").c_str(),    5, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring6_ETL_plusSide_200PU    = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing6_ETL_plusSide").c_str(),    6, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring7_ETL_plusSide_200PU    = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing7_ETL_plusSide").c_str(),    7, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring8_ETL_plusSide_200PU    = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing8_ETL_plusSide").c_str(),    8, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring9_ETL_plusSide_200PU    = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing9_ETL_plusSide").c_str(),    9, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring10_ETL_plusSide_200PU   = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing10_ETL_plusSide").c_str(),  10, isEta, is200PU);
  TGraphAsymmErrors* recHit_occupancy_VS_var_Ring11_ETL_plusSide_200PU   = returnOccupancyCurve(f0, ("recHit_" + variable + "_allHitsRing11_ETL_plusSide").c_str(),  11, isEta, is200PU);

  string s_PU = is200PU ? "200PU" : "noPU";
  string s_XTitle = isEta ? "ETL Rec Hit #eta" : "Threshold Energy [MeV]";
  makeOccupancyPlot(c0, recHit_occupancy_VS_var_allRings_ETL_200PU, ("occupancy_recHit_"+variable+"_allHitsAllRings_"+s_PU+"_ETL").c_str(), s_XTitle.c_str(), "ETL Occupancy", isLogX, isLogY, is200PU);
  makeOccupancyPlot(c0, recHit_occupancy_VS_var_allRings_ETL_plusSide_200PU, ("occupancy_recHit_"+variable+"_allHitsAllRings_"+s_PU+"_ETL_plusSide").c_str(), s_XTitle.c_str(), "ETL Occupancy", isLogX, isLogY, is200PU);

  makeOccupancyPlotAllRings(c0, recHit_occupancy_VS_var_Ring1_ETL_200PU, recHit_occupancy_VS_var_Ring2_ETL_200PU, recHit_occupancy_VS_var_Ring3_ETL_200PU, recHit_occupancy_VS_var_Ring4_ETL_200PU, recHit_occupancy_VS_var_Ring5_ETL_200PU, recHit_occupancy_VS_var_Ring6_ETL_200PU, recHit_occupancy_VS_var_Ring7_ETL_200PU, recHit_occupancy_VS_var_Ring8_ETL_200PU, recHit_occupancy_VS_var_Ring9_ETL_200PU, recHit_occupancy_VS_var_Ring10_ETL_200PU, recHit_occupancy_VS_var_Ring11_ETL_200PU, ("occupancy_recHit_"+variable+"_allHitsRings1to11_"+s_PU+"_ETL").c_str(), s_XTitle.c_str(), "ETL Occupancy", isLogX, isLogY, is200PU);
  makeOccupancyPlotAllRings(c0, recHit_occupancy_VS_var_Ring1_ETL_plusSide_200PU, recHit_occupancy_VS_var_Ring2_ETL_plusSide_200PU, recHit_occupancy_VS_var_Ring3_ETL_plusSide_200PU, recHit_occupancy_VS_var_Ring4_ETL_plusSide_200PU, recHit_occupancy_VS_var_Ring5_ETL_plusSide_200PU, recHit_occupancy_VS_var_Ring6_ETL_plusSide_200PU, recHit_occupancy_VS_var_Ring7_ETL_plusSide_200PU, recHit_occupancy_VS_var_Ring8_ETL_plusSide_200PU, recHit_occupancy_VS_var_Ring9_ETL_plusSide_200PU, recHit_occupancy_VS_var_Ring10_ETL_plusSide_200PU, recHit_occupancy_VS_var_Ring11_ETL_plusSide_200PU, ("occupancy_recHit_"+variable+"_allHitsRings1to11_"+s_PU+"_ETL_plusSide").c_str(), s_XTitle.c_str(), "ETL Occupancy", isLogX, isLogY, is200PU);

}

void plotCombiner()
{
  // *** 0. Drawing options
  string date      = "02-11-19";
  string revisionN = "r4";
  topDir = ("dpg_"+date+"/").c_str();
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
  TFile *f_singleMu = new TFile( ("./"+topDir+"/singleMuon_v1_" + revisionN + "_" + date + ".root").c_str(), "READ");
  TFile *f_singlePi = new TFile( ("./"+topDir+"/singlePion_v1_" + revisionN + "_" + date + ".root").c_str(), "READ");
  TFile *f_minBias  = new TFile( ("./"+topDir+"/nuGun200PU_v1_" + revisionN + "_" + date + ".root").c_str(), "READ");


  // *** 3. Comparison Plots
  makeComparisonPlots(f_singleMu, f_singlePi, f_minBias, c1, "recHit_energy_dR05_withTrackBTL", "BTL #Sigma E^{#Delta R < 0.05}_{recHit} [MeV]", "Event Fraction / 1 MeV");
  makeComparisonPlots(f_singleMu, f_singlePi, f_minBias, c1, "recHit_energy_dR05_withTrackETL", "ETL #Sigma E^{#Delta R < 0.05}_{recHit} [MeV]", "Event Fraction / 0.01 MeV");
  makeComparisonPlots(f_singleMu, f_singlePi, f_minBias, c1, "recHit_energy_dR05_withTrackETL_fixedRange", "ETL #Sigma E^{#Delta R < 0.05}_{recHit} [MeV]", "Event Fraction / 0.1 MeV");
  makeComparisonPlots(f_singleMu, f_singlePi, f_minBias, c1, "recHit_maxEnergy_withTrackETL", "ETL Max recHit Energy (No #Delta R) [MeV]", "Event Fraction / 0.1 MeV");
  makeComparisonPlots(f_singleMu, f_singlePi, f_minBias, c1, "recHit_maxEnergy_dRpass_withTrackETL", "ETL Max recHit Energy (#Delta R < 0.05) [MeV]", "Event Fraction / 0.1 MeV");
  makeComparisonPlots(f_singleMu, f_singlePi, f_minBias, c1, "recHit_maxEnergy_withTrackBTL", "BTL Max recHit Energy (No #Delta R) [MeV]", "Event Fraction / 0.1 MeV");
  makeComparisonPlots(f_singleMu, f_singlePi, f_minBias, c1, "recHit_maxEnergy_dRpass_withTrackBTL", "BTL Max recHit Energy (#Delta R < 0.05) [MeV]", "Event Fraction / 0.1 MeV");
  makeComparisonPlots(f_singleMu, f_singlePi, f_minBias, c1, "track_pt", "Track p_{T} [GeV]", "Event Fraction / 0.1 GeV");

 
  // *** 4. Efficiencies
  makeEfficiencyCurves(f_singleMu, f_singlePi, f_minBias, c1, "track_pt_atETL", "Track p_{T} (ETL-matched) [GeV]", "Efficiency");
  makeEfficiencyCurves(f_singleMu, f_singlePi, f_minBias, c1, "track_eta_atETL", "#phi at ETL", "Efficiency");
  makeEfficiencyCurves(f_singleMu, f_singlePi, f_minBias, c1, "track_phi_atETL", "#eta at ETL", "Efficiency");
  makeEfficiencyCurves(f_singleMu, f_singlePi, f_minBias, c1, "track_pt_atBTL", "Track p_{T} (BTL-matched) [GeV]", "Efficiency");
  makeEfficiencyCurves(f_singleMu, f_singlePi, f_minBias, c1, "track_eta_atBTL", "#phi at BTL", "Efficiency");
  makeEfficiencyCurves(f_singleMu, f_singlePi, f_minBias, c1, "track_phi_atBTL", "#eta at BTL", "Efficiency");


  // *** 5. Occupancies
  //makeAndDrawOccupancyPlots(TFile *f0, TCanvas *c0, string variable, bool isEta=false, bool is200PU=true, bool isLogX=true, bool isLogY=false)
  makeAndDrawOccupancyPlots(f_minBias, c1, "energy", false, true, true, true);
  makeAndDrawOccupancyPlots(f_minBias, c1, "eta", true, true, false, true);

}
