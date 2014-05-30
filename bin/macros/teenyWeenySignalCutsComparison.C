#include <iostream>
#include <sstream>
#include <iomanip>
#include <TH1F.h>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TText.h>
#include <TGraph.h>
#include <TVectorT.h>

void teenyWeenyLight(){

  // TDR style crap - include from Utilities.cc
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
  // For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);
  
  // For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);
  
  // For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);
  
  // For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);
  
  tdrStyle->SetEndErrorSize(2);
  //tdrStyle->SetErrorMarker(20);
  tdrStyle->SetErrorX(0.);
  
  tdrStyle->SetMarkerStyle(20);
  
  //For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);
  
  //For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);
  
  // For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

  // Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.16);
  tdrStyle->SetPadRightMargin(0.04);
  
  // For the Global title:

  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);
  
  // For the axis titles:
  
  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.25);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

  // For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

  // For the axis:

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

  // Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

  // Postscript options:
  tdrStyle->SetPaperSize(20.,20.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  tdrStyle->cd();

  // Meat of the stuff

  Double_t mHiggs[7];

  Double_t HHeffCut_met[7];
  Double_t HHeffCut_btag[7];
  Double_t HHeffCut_tau[7];
  Double_t HHeffCut_os[7];

  Double_t WHeffCut_met[7];
  Double_t WHeffCut_btag[7];
  Double_t WHeffCut_tau[7];
  Double_t WHeffCut_os[7];

  vector<TString> histLabels;

  histLabels.push_back( "E_{T}^{miss} #geq 40GeV" );
  histLabels.push_back( "#geq 1 b-tag" );
  histLabels.push_back( "N_{#tau} = 1");
  histLabels.push_back( "OS");

  // \multicolumn{1}{| c|}{ } &  \multicolumn{1}{ c|}{ $N_{lep}=1$ + $N_{jets}\ge 3$ } & \multicolumn{1}{ c|}{  }  &  \multicolumn{1}{ c |}{ $\ge$ 1btag } & \multicolumn{1}{ c| }{ $N_{\tau}=1$ } & \multicolumn{1}{ c |}{ OS }  & \multicolumn{1}{ c |}{ $\ge$ 2btags } \\ 

  mHiggs[0] =  80.;
  mHiggs[1] = 100.;
  mHiggs[2] = 120.;
  mHiggs[3] = 140.;
  mHiggs[4] = 150.;
  mHiggs[5] = 155.;
  mHiggs[6] = 160.;
  
  HHeffCut_met[0] =  14136.5 / 18284.0 ;  
  HHeffCut_met[1] =  15155.3 / 19912.9 ;  
  HHeffCut_met[2] =  16565.5 / 21142.8 ;  
  HHeffCut_met[3] =  15147.7 / 18699.4 ;  
  HHeffCut_met[4] =  12471.2 / 14960.9 ;  
  HHeffCut_met[5] =  11295.6 / 13235.8 ;  
  HHeffCut_met[6] =   9657.7 / 11071.8 ;  
  
  HHeffCut_btag[0] =   13200.7 / 14136.5  ;
  HHeffCut_btag[1] =   14015.2 / 15155.3  ;
  HHeffCut_btag[2] =   14927.6 / 16565.5  ;
  HHeffCut_btag[3] =   12708.1 / 15147.7  ;
  HHeffCut_btag[4] =    9192.6 / 12471.2  ;
  HHeffCut_btag[5] =    7503.1 / 11295.6  ;
  HHeffCut_btag[6] =    5723.0 /  9657.7  ;

  HHeffCut_tau[0] =    1280.1 / 13200.7  ;
  HHeffCut_tau[1] =    1479.7 / 14015.2  ;
  HHeffCut_tau[2] =    1635.1 / 14927.6  ;
  HHeffCut_tau[3] =    1303.1 / 12708.1  ;
  HHeffCut_tau[4] =     740.3 /  9192.6  ;
  HHeffCut_tau[5] =     712.1 /  7503.1  ;
  HHeffCut_tau[6] =     442.4 /  5723.0  ;

  HHeffCut_os[0] =  1264.8 / 1280.1    ;
  HHeffCut_os[1] =  1448.5 / 1479.7    ;
  HHeffCut_os[2] =  1625.3 / 1635.1    ;
  HHeffCut_os[3] =  1290.4 / 1303.1    ;
  HHeffCut_os[4] =   732.9 /  740.3    ;
  HHeffCut_os[5] =   706.4 /  712.1    ;
  HHeffCut_os[6] =   432.1 /  442.4    ;


  WHeffCut_met[0] =   27232.4  / 36098.7   ;  
  WHeffCut_met[1] =   28110.1  / 37094.3   ;  
  WHeffCut_met[2] =   28455.3  / 36942.4   ;  
  WHeffCut_met[3] =   28843.6  / 36411.5   ;  
  WHeffCut_met[4] =   27540.4  / 33740.4   ;  
  WHeffCut_met[5] =   27143.8  / 32958.1   ;  
  WHeffCut_met[6] =   27281.1  / 32696.3   ;  
  
  WHeffCut_btag[0] =  25424.2 /  27232.4    ;
  WHeffCut_btag[1] =  26135.2 /  28110.1    ;
  WHeffCut_btag[2] =  25996.0 /  28455.3    ;
  WHeffCut_btag[3] =  25514.3 /  28843.6    ;
  WHeffCut_btag[4] =  23904.4 /  27540.4    ;
  WHeffCut_btag[5] =  22847.4 /  27143.8    ;
  WHeffCut_btag[6] =  22896.0 /  27281.1    ;

  WHeffCut_tau[0] =  1822.8  / 25424.2     ;
  WHeffCut_tau[1] =  2002.2  / 26135.2     ;
  WHeffCut_tau[2] =  1891.6  / 25996.0     ;
  WHeffCut_tau[3] =  1960.3  / 25514.3     ;
  WHeffCut_tau[4] =  1409.6  / 23904.4     ;
  WHeffCut_tau[5] =  1272.6  / 22847.4     ;
  WHeffCut_tau[6] =  1457.0  / 22896.0     ;

  WHeffCut_os[0] =   1798.7  / 1822.8    ;
  WHeffCut_os[1] =   1975.3  / 2002.2    ;
  WHeffCut_os[2] =   1851.7  / 1891.6    ;
  WHeffCut_os[3] =   1893.5  / 1960.3    ;
  WHeffCut_os[4] =   1399.0  / 1409.6    ;
  WHeffCut_os[5] =   1248.8  / 1272.6    ;
  WHeffCut_os[6] =   1431.0  / 1457.0    ;


  TGraph* HH_met  = new TGraph(7, mHiggs, HHeffCut_met );
  TGraph* HH_btag = new TGraph(7, mHiggs, HHeffCut_btag);
  TGraph* HH_tau  = new TGraph(7, mHiggs, HHeffCut_tau );
  TGraph* HH_os   = new TGraph(7, mHiggs, HHeffCut_os  );

  TGraph* WH_met  = new TGraph(7, mHiggs, WHeffCut_met );
  TGraph* WH_btag = new TGraph(7, mHiggs, WHeffCut_btag);
  TGraph* WH_tau  = new TGraph(7, mHiggs, WHeffCut_tau );
  TGraph* WH_os   = new TGraph(7, mHiggs, WHeffCut_os  );


  HH_met  ->SetLineColor(1);
  HH_btag ->SetLineColor(2);
  HH_tau  ->SetLineColor(4);
  HH_os   ->SetLineColor(6);
  
  HH_met  ->SetMarkerColor(1);
  HH_btag ->SetMarkerColor(2);
  HH_tau  ->SetMarkerColor(4);
  HH_os   ->SetMarkerColor(6);

  WH_met  ->SetLineColor(1);
  WH_btag ->SetLineColor(2);
  WH_tau  ->SetLineColor(4);
  WH_os   ->SetLineColor(6);
  
  WH_met  ->SetMarkerColor(1);
  WH_btag ->SetMarkerColor(2);
  WH_tau  ->SetMarkerColor(4);
  WH_os   ->SetMarkerColor(6);


  HH_met  ->SetLineWidth(3);
  HH_btag ->SetLineWidth(3);
  HH_tau  ->SetLineWidth(3);
  HH_os   ->SetLineWidth(3);

  WH_met  ->SetLineWidth(3);
  WH_btag ->SetLineWidth(3);
  WH_tau  ->SetLineWidth(3);
  WH_os   ->SetLineWidth(3);

  HH_met  ->SetMarkerSize(5);
  HH_btag ->SetMarkerSize(5);
  HH_tau  ->SetMarkerSize(5);
  HH_os   ->SetMarkerSize(5);

  WH_met  ->SetMarkerSize(5);
  WH_btag ->SetMarkerSize(5);
  WH_tau  ->SetMarkerSize(5);
  WH_os   ->SetMarkerSize(5);

  
  //  (const Double_t)4.83393830509829703e-01
 
  TLegend* leg_ = new TLegend(0.15,0.35,0.62,0.65,NULL,"brNDC");
  leg_->SetTextFont(42);
  leg_->SetBorderSize(0);
  leg_->SetLineColor(1);
  leg_->SetLineStyle(1);
  leg_->SetLineWidth(1);
  leg_->SetFillColor(0);
  leg_->SetFillStyle(0);
  leg_->AddEntry(HH_met , histLabels[0], "lp");
  leg_->AddEntry(HH_btag, histLabels[1], "lp");
  leg_->AddEntry(HH_tau , histLabels[2], "lp");
  leg_->AddEntry(HH_os  , histLabels[3], "lp");

  TPaveText *pt1 = new TPaveText(0.17,0.30,0.5,0.35, "brNDC");
  pt1->SetBorderSize(1);
  pt1->SetFillColor(19);
  pt1->SetFillStyle(0);
  pt1->SetLineColor(0);
  pt1->SetTextFont(132);
  pt1->SetTextSize(0.033);
  //  TText *text = pt1->AddText("BR(t #rightarrow H^{+}b) = 0.05");

  HH_met->GetXaxis()->SetTitle("H^{+} mass [GeV]");
  WH_met->GetXaxis()->SetTitle("H^{+} mass [GeV]");

  HH_met->GetYaxis()->SetTitle("Cut efficiency (%)");
  WH_met->GetYaxis()->SetTitle("Cut efficiency (%)");


  TCanvas* myCanva = new TCanvas("signalCutEffs","signalCutEffs",2000,2000);
  
  HH_met->GetYaxis()->SetRangeUser(0,1.);
  WH_met->GetYaxis()->SetRangeUser(0,1.);

  myCanva->cd();
  HH_met  ->Draw("LPA");
  HH_btag ->Draw("LPsame");
  HH_tau  ->Draw("LPsame");
  HH_os   ->Draw("LPsame");
  leg_->Draw();
  pt1->Draw("same");
  myCanva->SaveAs("HHcutEffs.pdf");
  myCanva->SaveAs("HHcutEffs.png");

  myCanva->Clear();

  myCanva->cd();
  WH_met  ->Draw("LPA");
  WH_btag ->Draw("LPsame");
  WH_tau  ->Draw("LPsame");
  WH_os   ->Draw("LPsame");
  leg_->Draw();
  pt1->Draw("same");
  myCanva->SaveAs("WHcutEffs.pdf");
  myCanva->SaveAs("WHcutEffs.png");

  myCanva->Clear();
  

}

void teenyWeenyHeavy(){

  // TDR style crap - include from Utilities.cc
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
  // For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);
  
  // For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);
  
  // For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);
  
  // For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);
  
  tdrStyle->SetEndErrorSize(2);
  //tdrStyle->SetErrorMarker(20);
  tdrStyle->SetErrorX(0.);
  
  tdrStyle->SetMarkerStyle(20);
  
  //For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);
  
  //For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);
  
  // For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

  // Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.16);
  tdrStyle->SetPadRightMargin(0.04);
  
  // For the Global title:

  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);
  
  // For the axis titles:
  
  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.25);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

  // For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

  // For the axis:

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

  // Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

  // Postscript options:
  tdrStyle->SetPaperSize(20.,20.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  tdrStyle->cd();

  // Meat of the stuff

  Double_t mTBHHiggs[5];
  Double_t mHTBHiggs[12];

  Double_t TBHeffCut_met[5];
  Double_t TBHeffCut_btag[5];
  Double_t TBHeffCut_tau[5];
  Double_t TBHeffCut_os[5];

  Double_t HTBeffCut_met[12];
  Double_t HTBeffCut_btag[12];
  Double_t HTBeffCut_tau[12];
  Double_t HTBeffCut_os[12];

  vector<TString> histLabels;

  histLabels.push_back( "E_{T}^{miss} #geq 40GeV" );
  histLabels.push_back( "#geq 1 b-tag" );
  histLabels.push_back( "N_{#tau} = 1");
  histLabels.push_back( "OS");

  // \multicolumn{1}{| c|}{ } &  \multicolumn{1}{ c|}{ $N_{lep}=1$ + $N_{jets}\ge 3$ } & \multicolumn{1}{ c|}{  }  &  \multicolumn{1}{ c |}{ $\ge$ 1btag } & \multicolumn{1}{ c| }{ $N_{\tau}=1$ } & \multicolumn{1}{ c |}{ OS }  & \multicolumn{1}{ c |}{ $\ge$ 2btags } \\ 


  TBHeffCut_met[0] =   19260. /  21626.;
  TBHeffCut_met[1] =    4489. /   5002.;
  TBHeffCut_met[2] =    1848. /   2041.;
  TBHeffCut_met[3] =     604. /    658.;
  TBHeffCut_met[4] =     272. /    293.;
  
  TBHeffCut_btag[0] =   13210. / 19260.;
  TBHeffCut_btag[1] =    3039. /  4489.;
  TBHeffCut_btag[2] =    1256. /  1848.;
  TBHeffCut_btag[3] =     407. /   604.;
  TBHeffCut_btag[4] =     183. /   272.;
  
  TBHeffCut_tau[0] =    1630. /  13210.;
  TBHeffCut_tau[1] =     352. /   3039.;
  TBHeffCut_tau[2] =     149. /   1256.;
  TBHeffCut_tau[3] =      44. /    407.;
  TBHeffCut_tau[4] =      19. /    183.;
  
  TBHeffCut_os[0] =   1574. / 1630.;
  TBHeffCut_os[1] =    343. /  352.;
  TBHeffCut_os[2] =    145. /  149.;
  TBHeffCut_os[3] =     43. /   44.;
  TBHeffCut_os[4] =     18. /   19.;
  
  

  HTBeffCut_met[0]  =   4161. /     5703.; 
  HTBeffCut_met[1]  =  15675. /    21443.; 
  HTBeffCut_met[2]  =  15802. /    21534.; 
  HTBeffCut_met[3]  =  14181. /    19356.; 
  HTBeffCut_met[4]  =  13256. /    18041.; 
  HTBeffCut_met[5]  =  12712. /    17116.; 
  HTBeffCut_met[6]  =  11223. /    15006.; 
  HTBeffCut_met[7]  =   9697. /    12964.; 
  HTBeffCut_met[8]  =     68. /       89.; 
  HTBeffCut_met[9]  =     26. /       32.; 
  HTBeffCut_met[10] =     15. /       18.; 
  HTBeffCut_met[11] =      8. /       10.; 
  
  
  
  HTBeffCut_btag[0]  =    3442. /  4161.; 
  HTBeffCut_btag[1]  =   13168. / 15675.; 
  HTBeffCut_btag[2]  =   13777. / 15802.; 
  HTBeffCut_btag[3]  =   12682. / 14181.; 
  HTBeffCut_btag[4]  =   11882. / 13256.; 
  HTBeffCut_btag[5]  =   11430. / 12712.; 
  HTBeffCut_btag[6]  =   10218. / 11223.; 
  HTBeffCut_btag[7]  =    8833. /  9697.; 
  HTBeffCut_btag[8]  =      63. /    68.; 
  HTBeffCut_btag[9]  =      24. /    26.; 
  HTBeffCut_btag[10] =      13. /    15.; 
  HTBeffCut_btag[11] =       8. /     8.; 
  
  
  
  HTBeffCut_tau[0]  =    83.  /   3442.;  
  HTBeffCut_tau[1]  =   329.  /  13168.;  
  HTBeffCut_tau[2]  =   330.  /  13777.;  
  HTBeffCut_tau[3]  =   291.  /  12682.;  
  HTBeffCut_tau[4]  =   254.  /  11882.;  
  HTBeffCut_tau[5]  =   271.  /  11430.;  
  HTBeffCut_tau[6]  =   257.  /  10218.;  
  HTBeffCut_tau[7]  =   214.  /   8833.;  
  HTBeffCut_tau[8]  =     1.  /     63.;  
  HTBeffCut_tau[9]  =     1.  /     24.;  
  HTBeffCut_tau[10] =     0.3 /     13.;  
  HTBeffCut_tau[11] =     0.2 /      8.;  
  
  
  HTBeffCut_os[0]  =  70.  /      83.;   	  
  HTBeffCut_os[1]  = 261.  /     329.;   	  
  HTBeffCut_os[2]  = 261.  /     330.;   	  
  HTBeffCut_os[3]  = 243.  /     291.;   	  
  HTBeffCut_os[4]  = 210.  /     254.;   	  
  HTBeffCut_os[5]  = 224.  /     271.;   	  
  HTBeffCut_os[6]  = 213.  /     257.;   	  
  HTBeffCut_os[7]  = 179.  /     214.;   	  
  HTBeffCut_os[8]  =   1.  /       1.;   	  
  HTBeffCut_os[9]  =   0.  /       1.;   	  
  HTBeffCut_os[10] =   0.2 /      0.3; 
  HTBeffCut_os[11] =   0.1 /      0.2; 
  
  
  mTBHHiggs[0] = 180.;
  mTBHHiggs[1] = 200.;
  mTBHHiggs[2] = 220.;
  mTBHHiggs[3] = 250.;
  mTBHHiggs[4] = 300.;


  mHTBHiggs[0]  = 180.;
  mHTBHiggs[1]  = 200.;
  mHTBHiggs[2]  = 220.;
  mHTBHiggs[3]  = 240.;
  mHTBHiggs[4]  = 250.;
  mHTBHiggs[5]  = 260.;
  mHTBHiggs[6]  = 280.;
  mHTBHiggs[7]  = 300.;
  mHTBHiggs[8]  = 350.;
  mHTBHiggs[9]  = 500.;
  mHTBHiggs[10]  = 600.;
  mHTBHiggs[11] = 700.;
  

  TGraph* TBH_met  = new TGraph(5, mTBHHiggs, TBHeffCut_met );
  TGraph* TBH_btag = new TGraph(5, mTBHHiggs, TBHeffCut_btag);
  TGraph* TBH_tau  = new TGraph(5, mTBHHiggs, TBHeffCut_tau );
  TGraph* TBH_os   = new TGraph(5, mTBHHiggs, TBHeffCut_os  );

  TGraph* HTB_met  = new TGraph(8/*12*/, mHTBHiggs, HTBeffCut_met );
  TGraph* HTB_btag = new TGraph(8/*12*/, mHTBHiggs, HTBeffCut_btag);
  TGraph* HTB_tau  = new TGraph(8/*12*/, mHTBHiggs, HTBeffCut_tau );
  TGraph* HTB_os   = new TGraph(8/*12*/, mHTBHiggs, HTBeffCut_os  );


  TBH_met  ->SetLineColor(1);
  TBH_btag ->SetLineColor(2);
  TBH_tau  ->SetLineColor(4);
  TBH_os   ->SetLineColor(6);
  
  TBH_met  ->SetMarkerColor(1);
  TBH_btag ->SetMarkerColor(2);
  TBH_tau  ->SetMarkerColor(4);
  TBH_os   ->SetMarkerColor(6);

  HTB_met  ->SetLineColor(1);
  HTB_btag ->SetLineColor(2);
  HTB_tau  ->SetLineColor(4);
  HTB_os   ->SetLineColor(6);
  
  HTB_met  ->SetMarkerColor(1);
  HTB_btag ->SetMarkerColor(2);
  HTB_tau  ->SetMarkerColor(4);
  HTB_os   ->SetMarkerColor(6);


  TBH_met  ->SetLineWidth(3);
  TBH_btag ->SetLineWidth(3);
  TBH_tau  ->SetLineWidth(3);
  TBH_os   ->SetLineWidth(3);

  HTB_met  ->SetLineWidth(3);
  HTB_btag ->SetLineWidth(3);
  HTB_tau  ->SetLineWidth(3);
  HTB_os   ->SetLineWidth(3);

  TBH_met  ->SetMarkerSize(5);
  TBH_btag ->SetMarkerSize(5);
  TBH_tau  ->SetMarkerSize(5);
  TBH_os   ->SetMarkerSize(5);

  HTB_met  ->SetMarkerSize(5);
  HTB_btag ->SetMarkerSize(5);
  HTB_tau  ->SetMarkerSize(5);
  HTB_os   ->SetMarkerSize(5);

  
  //  (const Double_t)4.83393830509829703e-01
 
  TLegend* leg_ = new TLegend(0.15,0.35,0.62,0.65,NULL,"brNDC");
  leg_->SetTextFont(42);
  leg_->SetBorderSize(0);
  leg_->SetLineColor(1);
  leg_->SetLineStyle(1);
  leg_->SetLineWidth(1);
  leg_->SetFillColor(0);
  leg_->SetFillStyle(0);
  leg_->AddEntry(TBH_met , histLabels[0], "lp");
  leg_->AddEntry(TBH_btag, histLabels[1], "lp");
  leg_->AddEntry(TBH_tau , histLabels[2], "lp");
  leg_->AddEntry(TBH_os  , histLabels[3], "lp");

  TPaveText *pt1 = new TPaveText(0.17,0.30,0.5,0.35, "brNDC");
  pt1->SetBorderSize(1);
  pt1->SetFillColor(19);
  pt1->SetFillStyle(0);
  pt1->SetLineColor(0);
  pt1->SetTextFont(132);
  pt1->SetTextSize(0.033);
  //  TText *text = pt1->AddText("BR(t #rightarrow H^{+}b) = 0.05");

  TBH_met->GetXaxis()->SetTitle("H^{+} mass [GeV]");
  HTB_met->GetXaxis()->SetTitle("H^{+} mass [GeV]");

  TBH_met->GetYaxis()->SetTitle("Cut efficiency (%)");
  HTB_met->GetYaxis()->SetTitle("Cut efficiency (%)");

  TBH_met->GetYaxis()->SetRangeUser(0,1.);
  HTB_met->GetYaxis()->SetRangeUser(0,1.);

  TCanvas* myCanva1 = new TCanvas("signalCutEffs","signalCutEffs",2000,2000);
  myCanva1->cd();
  HTB_met  ->Draw("LPA");
  HTB_btag ->Draw("LPsame");
  HTB_tau  ->Draw("LPsame");
  HTB_os   ->Draw("LPsame");
  leg_->Draw();
  pt1->Draw("same");
  myCanva1->SaveAs("HTBcutEffs.pdf");
  myCanva1->SaveAs("HTBcutEffs.png");

  TCanvas* myCanva2 = new TCanvas("signalCutEffs2","signalCutEffs2",2000,2000);
  myCanva2->cd();
  TBH_met  ->Draw("LPA");
  TBH_btag ->Draw("LPsame");
  TBH_tau  ->Draw("LPsame");
  TBH_os   ->Draw("LPsame");
  leg_->Draw();
  pt1->Draw("same");
  myCanva2->SaveAs("TBHcutEffs.pdf");
  myCanva2->SaveAs("TBHcutEffs.png");


}




void teenyWeenySignalCutsComparison(string whichOne){

  if(whichOne=="heavy") teenyWeenyHeavy();
  if(whichOne=="light") teenyWeenyLight();

}

