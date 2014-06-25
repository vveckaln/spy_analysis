#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <TROOT.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TTree.h>
#include <TStyle.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TLorentzVector.h>
#include <TLegendEntry.h>
#include <vector>
#include <string>
#include <string.h>
#include <sstream>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TF1.h>
#include "Math/Functor.h"
//#include "Math/RootFinderAlgorithms.h"
//#include "Math/RootFinder.h"

//event dist. for tanc1

double ULValue = 100;
double ww, wh, hh;

void HeavyChHiggsEventDist(bool isEMu, bool ism250)
{


  double 
    prodXsec (ism250 ? 11.9413    : 18.4811),
    BR_htaunu(ism250 ? 0.04129783 : 0.228564),
    BR_htb(   1- BR_htaunu),
    // mutau
    Y_Htaunu (ism250 ? 0.4180 : 3.4506),
    Y_Htb    (ism250 ? 0.1304 : 0.1616);
  
  // emu
  if(isEMu){
    Y_Htaunu  = (ism250 ? 0.22 : 1.05);
    Y_Htb     = (ism250 ? 11.92 : 7.37);
  }

  double xsecMultiplier(1.);


  // Rescale according to chosen multiplier
  Y_Htaunu *= xsecMultiplier;
  Y_Htb    *= xsecMultiplier;


  //  double BR[17] = {0., 0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.70, 0.80, 0.90, 1.00};

  double BR[17] = {0., 0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.70, 0.80, 0.90, 1.00};

  double RY_Htaunu[17], RY_Htb[17]; // Rescaled yields

  for(int i = 0; i < 17; i++){
    RY_Htaunu[i] = Y_Htaunu*BR[i];
    RY_Htb[i]    = Y_Htb   *BR[i];
  }


  gROOT->ProcessLine(".L tdrstyle.C");
  setTDRStyle();
  c1 = new TCanvas();

  TGraph *evtH_taunu = new TGraph(17, BR, RY_Htaunu);
  TGraph *evtH_tb    = new TGraph(17, BR, RY_Htb);

  evtH_taunu->SetLineColor(2);
  evtH_taunu->SetLineWidth(3);
  evtH_taunu->SetLineStyle(2);
  evtH_tb->SetLineColor(3);
  evtH_tb->SetLineWidth(3);
  evtH_tb->SetLineStyle(7);

  evtH_taunu->GetXaxis()->SetTitle("BR(H^{+} #rightarrow tb (#tau#nu) )");
  evtH_taunu->GetYaxis()->SetTitle("Events");
  evtH_taunu->GetXaxis()->SetRangeUser(0, 1.0);
  evtH_taunu->GetYaxis()->SetRangeUser(0, 20.0);
  if(ism250) evtH_taunu->GetYaxis()->SetRangeUser(0, 20.0);
  evtH_taunu->GetYaxis()->SetTitleOffset(1.4);

  evtH_taunu->Draw("AL");
  evtH_tb->Draw("L same");

  TLegend *pl = new TLegend(0.05,0.7,0.35,0.9);
  if(isEMu)pl = new TLegend(0.5,0.7,0.8,0.9);
  pl->SetTextSize(0.04);
  pl->SetFillColor(0);
  pl->SetTextFont(132);
  pl->SetBorderSize(0);
  TLegendEntry *ple;
  ple = pl->AddEntry(evtH_taunu, "H^{#pm} #rightarrow #tau #nu", "l");
  ple = pl->AddEntry(evtH_tb, "H^{#pm} #rightarrow tb", "l");
  ple->SetMarkerSize(0.1);
  pl->Draw();

  TPaveText *pt = new TPaveText(0.15,0.93,0.9,1.0, "brNDC");
  pt->SetBorderSize(1);
  pt->SetFillColor(19);
  pt->SetFillStyle(0);
  pt->SetLineColor(0);
  pt->SetTextFont(132);
  pt->SetTextSize(0.045);
  if(!isEMu)TText *text = pt->AddText("CMS Preliminary, #sqrt{s} = 8 TeV,  19.7 fb^{-1}");
  if(isEMu) TText *text = pt->AddText("CMS Preliminary, #sqrt{s} = 8 TeV,  19.7 fb^{-1}");
  text->SetTextAlign(11);
  pt->Draw();


  TLatex *tex2 = new TLatex(0.05, 7.0, "#splitline{m_{H^{#pm}} = 200 GeV}{m_{H}^{mod+} scenario}");
  if(isEMu && !ism250)tex2 = new TLatex(0.65, 7.0, "#splitline{m_{H^{#pm}} = 200 GeV}{m_{H}^{mod+} scenario}");  
  if(ism250)tex2 = new TLatex(0.05, 7.0, "#splitline{m_{H^{#pm}} = 250 GeV}{m_{H}^{mod+} scenario}");  
  if(isEMu && ism250)tex2 = new TLatex(0.65, 7.0, "#splitline{m_{H^{#pm}} = 250 GeV}{m_{H}^{mod+} scenario}");
  tex2->SetTextSize(0.035);  
  tex2->Draw(); 

  TLatex *tex3;
  if(isEMu) tex3 = new TLatex(0.05, 13.0, "e#mu final state");  
  else tex3 = new TLatex(0.05, 13.0, "#mu#tau_{h} final state");  
  tex3->SetTextSize(0.035);  
  tex3->Draw(); 

  c1->Update();
  
}

void LightChHiggsEventDist()
{
  bool isEMu = false;
  bool ism160 = true;
  //for hps1, 120 GeV
  double SF = 1.0;
  double WHYield = 1851.7; // WH yields
  double HHYield = 1625.3; // HH yields
  double BGYield = 222.0; //background from DD
  double TTYield = (306.2+22.9); // WW (ltau+ll+lqq) yield (MC)

  //for hps1, 160 GeV
  if(ism160){HHYield = 432.1; WHYield = 1431.0;}
  
  //dilepton
  if(isEMu && !ism160){WHYield = 1877.69; HHYield = 1257.63; TTYield = 3445.35;}

  //dilepton 160 GeV
  if(isEMu && ism160){WHYield = 1637.70; HHYield = 655.08; TTYield = 3445.35;}


  double BR[17] = {0., 5., 10., 15., 20., 25., 30., 35., 40., 45., 50., 55., 60., 70., 80., 90., 100.};
  double ttSMBR[17], WHBR[17], HHBR[17], bkgBR[17];
  double SUSYtt[17];

  for(int i = 0; i < 17; i++){
    ttSMBR[i] = (TTYield)*(1-BR[i]/100)*(1-BR[i]/100);
    WHBR[i] = WHYield*2*(BR[i]/100)*(1-BR[i]/100);
    HHBR[i] = HHYield*(BR[i]/100)*(BR[i]/100);
    bkgBR[i] = BGYield;
    SUSYtt[i] = ttSMBR[i] + WHBR[i] + HHBR[i];
    BR[i] = BR[i]/100.0;
  }
  
  gROOT->ProcessLine(".L tdrstyle.C");
  setTDRStyle();
  c1 = new TCanvas();

  TGraph *evtTTSM = new TGraph(17, BR, ttSMBR);
  TGraph *evtWH = new TGraph(17, BR, WHBR);
  TGraph *evtHH = new TGraph(17, BR, HHBR);
  TGraph *evtBKG = new TGraph(17, BR, bkgBR);
  TGraph *evtSUSYtt = new TGraph(17, BR, SUSYtt);

  evtTTSM->SetLineColor(4);
  evtTTSM->SetLineWidth(3);
  evtTTSM->SetLineStyle(3);
  evtWH->SetLineColor(2);
  evtWH->SetLineWidth(3);
  evtWH->SetLineStyle(2);
  evtHH->SetLineColor(3);
  evtHH->SetLineWidth(3);
  evtHH->SetLineStyle(7);
  evtBKG->SetLineColor(6);
  evtBKG->SetLineWidth(3);
  evtBKG->SetLineStyle(3);
  evtSUSYtt->SetLineWidth(4);
  evtSUSYtt->SetLineColor(6);
  

  evtTTSM->GetXaxis()->SetTitle("B(t #rightarrow H^{+}b)");
  evtTTSM->GetYaxis()->SetTitle("Events");
  evtTTSM->GetXaxis()->SetRangeUser(0, 1.0);
  evtTTSM->GetYaxis()->SetRangeUser(0, 2600.0);
  if(ism160) evtTTSM->GetYaxis()->SetRangeUser(0, 1600.0);
  evtTTSM->GetYaxis()->SetTitleOffset(1.4);

  evtTTSM->Draw("AL");
  evtWH->Draw("L same");
  //evtBKG->Draw("L same");
  evtHH->Draw("L same");
  evtSUSYtt ->Draw("L same");

  TLegend *pl = new TLegend(0.05,0.7,0.35,0.9);
  if(isEMu)pl = new TLegend(0.5,0.7,0.8,0.9);
  pl->SetTextSize(0.04);
  pl->SetFillColor(0);
  pl->SetTextFont(132);
  pl->SetBorderSize(0);
  TLegendEntry *ple;
  ple = pl->AddEntry(evtSUSYtt, "t#bar{t} (WW + WH^{#pm} + H^{+}H^{-})", "l");
  ple = pl->AddEntry(evtTTSM, "WW (t#bar{t} #rightarrow W^{+}bW^{-}#bar{b})", "l");
  ple = pl->AddEntry(evtWH, "WH^{#pm} (t#bar{t} #rightarrow W^{#pm}bH^{#pm}#bar{b})", "l");
  ple = pl->AddEntry(evtHH, "H^{#pm}H^{#pm} (t#bar{t} #rightarrow H^{+}bH^{-}#bar{b})", "l");
  //ple = pl->AddEntry(evtBKG, "Backgrounds", "l");
  //ple = pl->AddEntry(evtSUSYtt, "t#bar{t} (WW + WH^{#pm} + H^{#pm}H^{#pm})", "l");
  ple->SetMarkerSize(0.1);
  pl->Draw();

  TPaveText *pt = new TPaveText(0.15,0.93,0.9,1.0, "brNDC");
  pt->SetBorderSize(1);
  pt->SetFillColor(19);
  pt->SetFillStyle(0);
  pt->SetLineColor(0);
  pt->SetTextFont(132);
  pt->SetTextSize(0.045);
  if(!isEMu)TText *text = pt->AddText("#sqrt{s} = 7 TeV,  4.9 fb^{-1}  CMS Preliminary");
  if(isEMu)TText *text = pt->AddText("#sqrt{s} = 7 TeV,  2.3 fb^{-1}  CMS Preliminary");
  text->SetTextAlign(11);
  pt->Draw();


  TLatex *tex2 = new TLatex(0.05, 800.0, "m_{H^{#pm}} = 120 GeV");
  if(isEMu && !ism160)tex2 = new TLatex(0.65, 2300.0, "m_{H^{#pm}} = 120 GeV");  
  if(ism160)tex2 = new TLatex(0.05, 300.0, "m_{H^{#pm}} = 160 GeV");  
  if(isEMu && ism160)tex2 = new TLatex(0.65, 2300.0, "m_{H^{#pm}} = 160 GeV");
  tex2->SetTextSize(0.035);  
  tex2->Draw(); 

  TLatex *tex3 = new TLatex(0.05, 750.0, "#mu#tau_{h} final state");
  if(ism160)tex3 = new TLatex(0.05, 350.0, "#mu#tau_{h} final state");  
  if(isEMu && !ism160)tex3 = new TLatex(0.65, 2150.0, "e#mu final state");  
  if(isEMu && ism160)tex3 = new TLatex(0.65, 2150.0, "e#mu final state");
  tex3->SetTextSize(0.035);  
  tex3->Draw(); 

  c1->Update();
  
}


double myfunc(double x) { 
  double res=ww*(1-x/100)*(1-x/100)+wh*2*x/100*(1-x/100)+hh*x/100*x/100 - ww;
  return res;
}

double myfunc2(double x) { 
  double res=ww*(1-x/100)*(1-x/100)+wh*2*x/100*(1-x/100)+hh*x/100*x/100 - ww - ULValue;
  return res;
}

void EventDist1(string tautype = "pf"){
  double UL, ULExp, ULExpP, ULExpN;

  TString filename("temp");

  TString TauAlgoName("PF Shrinking Cone");
  TString ULMethod("Bayesian");
  double ww_withlqq = 0;
  
  if(tautype == "hps0btag"){ //old
    double SF = 1;
    ww = (56.74 + 12.33)*SF;
    ww_withlqq = (56.74 + 12.33 + 33.3)*SF;
    wh = 365.77*SF;
    hh = 346.39*SF;
    UL = 36.3861; ULExp = 46.88053;
    ULExpP = 61.34276; ULExpN = 31.42294;

    TauAlgoName = "HPS (Loose)";
    filename = TString("EventDist_brLimit_hpsLoose_1btag_lqqExcluded");
    //cout<<"SF = "<<SF<<endl;
  }
  else if(tautype == "hps0btagCLs"){
    double SF = 1;
    ww = (50.7 + 6.4)*SF;
    ww_withlqq = (50.7 + 6.7 + 32.1)*SF;
    wh = 321.9*SF;
    hh = 292.5*SF;
    UL = 37.2962; ULExp = 53.84434;
    ULExpP = 72.72782; ULExpN = 34.49270;
    
    TauAlgoName = "HPS (Loose)";
    ULMethod = "CLs";
    filename = TString("EventDist_brLimit_hpsLoose_1btag_lqqExcluded_CLs");
    //cout<<"SF = "<<SF<<endl;
  }

  double BR[21] = {0., 5., 10., 15., 20., 25., 30., 35., 40., 45., 50., 55., 60., 65., 70., 75., 80., 85., 90., 95., 100.};
  double ExpTotalBR[21];
  double ULBR[3], ULExpBR[3] , ULExpPBR[3], ULExpNBR[3];
  double ULBR_X[3], ULExpBR_X[3], ULExpPBR_X[3], ULExpNBR_X[3];

  for(int i = 0; i < 21; i++){
    ExpTotalBR[i] = myfunc(BR[i]);
  }

  for(int i = 0; i < 2; i++){
    ULBR[i] = UL; ULExpBR[i] = ULExp; ULExpPBR[i] = ULExpP; ULExpNBR[i] = ULExpN;
    ULBR_X[i] = 0; ULExpBR_X[i] = 0; ULExpPBR_X[i] = 0; ULExpNBR_X[i] = 0;
  }
  ULBR[2] = 0; ULExpBR[2] = 0; ULExpPBR[2] = 0; ULExpNBR[2] = 0;

  //data
  ULValue = UL;
  ROOT::Math::Roots::Brent rfb;
  ROOT::Math::Functor1D f21D(&myfunc2);
  rfb.SetFunction(f21D, 0, 100);
  if(!rfb.Solve(1000)){
    ULBR_X[1] = rfb.Root();
    ULBR_X[2] = rfb.Root();
    //cout<<rfb.Root()<<endl;
  }
  else { 
    rfb.SetFunction(f21D, 0, 55); 
    if(!rfb.Solve(1000)){ 
      ULBR_X[1] = rfb.Root(); 
      ULBR_X[2] = rfb.Root();
    } 
    else{ 
      rfb.SetFunction(f21D, 50, 70); 
      if(!rfb.Solve(1000)){ 
	ULBR_X[1] = rfb.Root(); 
	ULBR_X[2] = rfb.Root();
      } 
      else{ 
	ULBR_X[1] = 100;
	ULBR_X[2] = 100;
      }
    }
  }
  
  //MC
  ULValue = ULExp;
  f21D = ROOT::Math::Functor1D(&myfunc2);
  rfb.SetFunction(f21D, 0, 100);
  if(!rfb.Solve(1000)){
    ULExpBR_X[1] = rfb.Root();
    ULExpBR_X[2] = rfb.Root();
  }
  else { 
    rfb.SetFunction(f21D, 0, 55); 
    if(!rfb.Solve(1000)){ 
      ULExpBR_X[1] = rfb.Root(); 
      ULExpBR_X[2] = rfb.Root();
    } 
    else{ 
      rfb.SetFunction(f21D, 50, 70); 
      if(!rfb.Solve(1000)){ 
	ULExpBR_X[1] = rfb.Root(); 
	ULExpBR_X[2] = rfb.Root();
      } 
      else{ 
	ULExpBR_X[1] = 100;
	ULExpBR_X[2] = 100;
      }
    }
  }
  
  //MC Upper Error
  ULValue = ULExpP;
  f21D = ROOT::Math::Functor1D(&myfunc2);
  rfb.SetFunction(f21D, 0, 100);
  if(!rfb.Solve(1000)){
    ULExpPBR_X[1] = rfb.Root();
    ULExpPBR_X[2] = rfb.Root();
  }
  else { 
    rfb.SetFunction(f21D, 0, 55); 
    if(!rfb.Solve(1000)){ 
      ULExpPBR_X[1] = rfb.Root(); 
      ULExpPBR_X[2] = rfb.Root();
    } 
    else{ 
      rfb.SetFunction(f21D, 50, 70); 
      if(!rfb.Solve(1000)){ 
	ULExpPBR_X[1] = rfb.Root(); 
	ULExpPBR_X[2] = rfb.Root();
      } 
      else{ 
	ULExpPBR_X[1] = 100;
	ULExpPBR_X[2] = 100;
      }
    }
  }
  //MC Lower Error
  ULValue = ULExpN;
  f21D = ROOT::Math::Functor1D(&myfunc2);
  rfb.SetFunction(f21D, 0, 50);
  if(!rfb.Solve(1000)){
    ULExpNBR_X[1] = rfb.Root();
    ULExpNBR_X[2] = rfb.Root();
  }  
  else { 
    rfb.SetFunction(f21D, 0, 55); 
    if(!rfb.Solve(1000)){ 
      ULExpNBR_X[1] = rfb.Root(); 
      ULExpNBR_X[2] = rfb.Root();
    } 
    else{ 
      rfb.SetFunction(f21D, 0, 5); 
      if(!rfb.Solve(1000)){ 
	ULExpNBR_X[1] = rfb.Root(); 
	ULExpNBR_X[2] = rfb.Root();
      } 
      else{ 
	ULExpNBR_X[1] = 0;
	ULExpNBR_X[2] = 0;
	checkroot();
      }
    }
  }


  //additional curve with lqq included
  double ExpTotalBR_wLQQ[21];
  ww = ww_withlqq;
  for(int i = 0; i < 21; i++){
    ExpTotalBR_wLQQ[i] = myfunc(BR[i]);
  }
  
  gROOT->ProcessLine(".L tdrstyle.C");
  setTDRStyle();
  c1 = new TCanvas();
  
  
  TGraph *evtTotal = new TGraph(21, BR, ExpTotalBR);
  TGraph *evtTotal_wLQQ = new TGraph(21, BR, ExpTotalBR_wLQQ);

  TGraph *evtUL = new TGraph(3, ULBR_X, ULBR);

  evtTotal->SetLineWidth(3);
  evtTotal->SetLineStyle(7);
  evtTotal->SetLineColor(4);

  evtTotal_wLQQ->SetLineWidth(3);
  evtTotal_wLQQ->SetLineStyle(2);
  evtTotal_wLQQ->SetLineColor(6);

  evtUL->SetLineWidth(5);
  evtUL->SetLineStyle(1);
  evtUL->SetLineColor(1);

  evtTotal->GetXaxis()->SetTitle("BR(t #rightarrow bH^{+}) [%]");
  evtTotal->GetYaxis()->SetTitle("N_{up}");

  
  TGraph *evtULExp = new TGraph(3, ULExpBR_X, ULExpBR);
  TGraph *evtULExpP = new TGraph(3, ULExpPBR_X, ULExpPBR);
  TGraph *evtULExpN = new TGraph(3, ULExpNBR_X, ULExpNBR);

  evtULExp->SetLineColor(2);
  evtULExp->SetFillColor(5);
  evtULExp->SetLineWidth(3);

  evtULExpP->SetLineColor(5);
  evtULExpP->SetFillColor(5);
  evtULExpP->SetLineWidth(3);
  
  evtULExpN->SetLineColor(5);
  evtULExpN->SetFillColor(0);
  evtULExpN->SetLineWidth(3);


  //Add two separate box plot
  double ULExpPBR1[4], ULExpNBR1[4];
  double ULExpPBR_X1[4], ULExpNBR_X1[4];
  ULExpPBR1[0] = 0; ULExpNBR1[0] = 0;
  ULExpPBR_X1[0] = 0; ULExpNBR_X1[0] = 0;
  for(int i = 1; i < 4; i++){
    ULExpPBR1[i] = ULExpPBR[i-1];
    ULExpNBR1[i] = ULExpNBR[i-1];
    ULExpPBR_X1[i] = ULExpPBR_X[i-1];
    ULExpNBR_X1[i] = ULExpNBR_X[i-1];
  }
  
  TGraph *evtULExpP1 = new TGraph(4, ULExpPBR_X1, ULExpPBR1);
  TGraph *evtULExpN1 = new TGraph(4, ULExpNBR_X1, ULExpNBR1);

  evtULExpP1->SetLineColor(5);
  evtULExpP1->SetFillColor(5);
  evtULExpP1->SetLineWidth(3);
  
  evtULExpN1->SetLineColor(5);
  evtULExpN1->SetFillColor(0);
  evtULExpN1->SetLineWidth(3);
     
  evtTotal->GetXaxis()->SetRangeUser(0, 20.);
  evtTotal->GetYaxis()->SetRangeUser(0, 140.);
  evtTotal->Draw("AL");
  evtULExpP1->Draw("LF same");
  evtULExpN1->Draw("LF same");
  evtULExpP->Draw("L same");
  evtULExpN->Draw("L same");
  evtTotal->Draw("L same");
  evtTotal_wLQQ->Draw("L same");
  evtULExp->Draw("L same");
  evtUL->Draw("L same");

 
  TLegend *pl = new TLegend(0.2,0.73,0.6,0.98);
  pl->SetTextSize(0.04);
  pl->SetFillColor(0);
  pl->SetTextFont(132);
  pl->SetBorderSize(0);
  TLegendEntry *ple;
  ple = pl->AddEntry(evtTotal, "t#bar{t} with H^{+} included - SM t#bar{t}", "l");
  ple = pl->AddEntry(evtUL, "Observed 95% CL Limit", "l");
  ple = pl->AddEntry(evtULExp, "Expected Limits (mean with #pm1#sigma band)", "lf");
  ple = pl->AddEntry(evtTotal_wLQQ, "t#bar{t} with H^{+} included - SM t#bar{t} (lq#bar{q} included)", "l");
  //ple = pl->AddEntry(evtULExp, "Expected Limit #pm 1#sigma", "lf");
  //ple = pl->AddEntry(evtULExpP, "Expected Limit on Signal #pm 1#sigma Uncert", "l");
  ple->SetMarkerSize(0.1);
  pl->Draw();

  TLegend *pl1 = new TLegend(0.18,0.6,0.4,0.68);
  pl1->SetTextSize(0.03);
  pl1->SetFillColor(0);
  pl1->SetTextFont(132);
  pl1->SetBorderSize(0);
  TLegendEntry *pl1e;
  pl1e = pl1->AddEntry("a", "CMS Preliminary", "");
  pl1e = pl1->AddEntry("b", "L = 190 pb^{-1}", "");
  pl1e = pl1->AddEntry("c", "#sqrt{s} = 7 TeV", "");
  pl1e->SetMarkerSize(0.1);
  pl1->Draw();


  gPad->RedrawAxis();


  tex = new TLatex(12.0, 20.0, TauAlgoName);
  tex->SetTextSize(0.035);
  tex->Draw();
  
  tex1 = new TLatex(12.0, 12.0, ULMethod);
  tex1->SetTextSize(0.035);
  tex1->Draw();  

  tex2 = new TLatex(12.0, 30.0, "M_{H^{#pm}} = 120 GeV/c^{2}"); 
  tex2->SetTextSize(0.035); 
  tex2->Draw();

  c1->SaveAs("Results_V6/plots/"+filename+".C");
  c1->SaveAs("Results_V6/plots/"+filename+".root");
  c1->SaveAs("Results_V6/plots/"+filename+".png");

  c1->Update();
  //c1->Close();
  
}  

void checkroot() //for cross checking the result
{
  //initialize(ttww, ttwwo, ttwh, tthh);
  for(int i = 0; i <= 100; i++){
    cout<<"x = "<<i<<" value = "<<myfunc2(i)<<endl; //" Susy tt~ - SM "<<myfunc2(i)+UL_signal<<" tt~ "<<(tt_dilep+tt_other+tt_lqq)<<" UL "<<UL_signal<<endl;  //br limit should be the value of x where the myfunc2(x) = 0
  }
}



void teenyWeenyExpectedEventsDistributions(){
  // mH+ = 200
  HeavyChHiggsEventDist(0,0); // mutau
  HeavyChHiggsEventDist(1,0); // emu
  //
  //  // mH+ = 250
  HeavyChHiggsEventDist(0,1); // mutau
  HeavyChHiggsEventDist(1,1); // emu
  

  //LightChHiggsEventDist();
}
