#include <iostream>
#include <iomanip>
#include <sstream>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TText.h>




void teenyWeenyEmbedDaAss(){
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
  tdrStyle->SetMarkerSize(30);
  
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
  tdrStyle->SetTitleSize(0.2, "XYZ");//06, "XYZ");
  tdrStyle->SetLabelSize(0.1, "XYZ");
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








  // Acquire files
  TFile* embeddedData = new TFile("/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_TopPtRew_2/nomt-2012-V1-data-MU-20GeV/out-embedded-data.root");
  TFile* embeddedTTbar = new TFile("/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_TopPtRew_2/nomt-2012-V1-mc-MU-20GeV/out-embedded-ttbar.root");
  TFile* ztautau      = new TFile("/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_TopPtRew_2/nomt-2012-V1-mc-MU-20GeV/out-zjets.root");
  TFile* allmc      = new TFile("/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_TopPtRew_2/nomt-2012-V1-mc-MU-20GeV/out-mc.root");

  
  // Acquire simple cutflow histos
  TH1F* emd_cutflow = (TH1F*) embeddedData->Get("PFlow/Yields/m_tau/cutflow_yields_3");
  emd_cutflow->SetName("emd_cutflow");
  TH1F* emt_cutflow = (TH1F*) embeddedTTbar->Get("PFlow/Yields/m_tau/cutflow_yields_3");
  emt_cutflow->SetName("emt_cutflow");
  TH1F* ztt_cutflow = (TH1F*) ztautau     ->Get("PFlow/Yields/m_tau/cutflow_yields_3");
  ztt_cutflow->SetName("ztt_cutflow");
  TH1F* all_cutflow = (TH1F*) allmc       ->Get("PFlow/Yields/m_tau/cutflow_yields_3");  
  all_cutflow->SetName("all_cutflow");

  // Acquire MC cutflow per-channel
  TH2F* all_mccutflow = (TH2F*) allmc->Get("PFlow/YieldsMC/cutflow_yields_mc_1");
  all_mccutflow->SetName("all_mccutflow");
  
  // Acquire sample distribution for shape analysis
  TH1F* emd_taupt = (TH1F*) embeddedData->Get("PFlow/RecoSteps/m_tau/1 Tau/corrected_btagmultiplicity_j_30");  
  emd_taupt->SetName("emd_taupt");
  TH1F* emt_taupt = (TH1F*) embeddedTTbar->Get("PFlow/RecoSteps/m_tau/1 Tau/corrected_btagmultiplicity_j_30");
  emt_taupt->SetName("emt_taupt");
  TH1F* ztt_taupt = (TH1F*) ztautau     ->Get("PFlow/RecoSteps/m_tau/1 Tau/corrected_btagmultiplicity_j_30");
  ztt_taupt->SetName("ztt_taupt");

  
  cout << "Preliminary output: " << endl;
  cout << "-------------------- " << endl;
  cout << "embedded data:   "   << emd_cutflow->GetBinContent(10) << endl;
  cout << "embedded ttbar : " << emt_cutflow->GetBinContent(10) << endl;
  cout << "ztautau:         " << ztt_cutflow->GetBinContent(10) << endl;
  cout << "---" << endl;
  cout << "(MET step) embedded data:  " << emd_cutflow->GetBinContent(8) << endl;
  cout << "(MET step) embedded ttbar: " << emt_cutflow->GetBinContent(8) << endl;
  cout << "(MET step) ztautau:        " << ztt_cutflow->GetBinContent(8) << endl;
  cout << "--" << endl;
  cout << "(presel step) embedded data:  " << emd_cutflow->GetBinContent(7) << endl;
  cout << "(presel step) embedded ttbar: " << emt_cutflow->GetBinContent(7) << endl;
  cout << "(presel step) ztautau:        " << ztt_cutflow->GetBinContent(7) << endl;
  cout << "--------------------" << endl;
  cout << "MC flow info:" << endl;
  cout << "Z->(ll):     " << all_mccutflow->GetBinContent(10,11) << endl;
  cout << "Z->(tautau): " << all_mccutflow->GetBinContent(10,12) << endl;
  cout << "--------------------" << endl;


  //std::ostringstream strs;
  double ks = ztt_taupt->KolmogorovTest(emd_taupt);
  double chi2p = ztt_taupt->Chi2Test(emd_taupt,"");
  double nchi2p = ztt_taupt->Chi2Test(emd_taupt,"UU NORM");
  double chi2c = ztt_taupt->Chi2Test(emd_taupt,"CHI2");
  double chi2cn = ztt_taupt->Chi2Test(emd_taupt,"CHI2/NDF");
  double nchi2c = ztt_taupt->Chi2Test(emd_taupt,"UU NORM CHI2");
  double nchi2cn = ztt_taupt->Chi2Test(emd_taupt,"UU NORM CHI2/NDF");


//  cout << "ks is " << std::setprecision(2) << ks <<  endl;
//  cout << "pvalue for chi2 is " << std::setprecision(2) << chi2p <<  endl;
//  cout << "pvalue for norm-chi2 is " << std::setprecision(2) << nchi2p <<  endl;
//  cout << "chi2 for chi2 is " << std::setprecision(2) << chi2c <<  endl;
//  cout << "chi2/NDOF for chi2 is " << std::setprecision(2) << chi2cn <<  endl;
//  cout << "chi2 for norm-chi2 is " << std::setprecision(2) << nchi2c <<  endl;
//  cout << "chi2/NDOF for norm-chi2 is " << std::setprecision(2) << nchi2cn <<  endl;

  cout << "Base integrals (should be the OS yield):" << endl;
  cout << "embedded:    " << emd_taupt->Integral() << endl;
  cout << "Z->(tautau): " << ztt_taupt->Integral() << endl;
  cout << "--------------------------------------------" << endl;
  ztt_taupt->Sumw2();
  emd_taupt->Sumw2();
  emd_taupt->Scale(all_mccutflow->GetBinContent(10,12)/emd_taupt->Integral());
  ztt_taupt->Scale(all_mccutflow->GetBinContent(10,12)/ztt_taupt->Integral());


  cout << "New integrals (should be the ZTT MC OS yield):" << endl;
  cout << "embedded:    " << emd_taupt->Integral() << endl;
  cout << "Z->(tautau): " << ztt_taupt->Integral() << endl;
  cout << "--------------------------------------------" << endl;

  double norm_ks = ztt_taupt->KolmogorovTest(emd_taupt);
  double norm_chi2p = ztt_taupt->Chi2Test(emd_taupt,"");
  double norm_nchi2p = ztt_taupt->Chi2Test(emd_taupt,"UU NORM");
  double norm_chi2c = ztt_taupt->Chi2Test(emd_taupt,"CHI2");
  double norm_chi2cn = ztt_taupt->Chi2Test(emd_taupt,"CHI2/NDF");
  double norm_nchi2c = ztt_taupt->Chi2Test(emd_taupt,"UU NORM CHI2");
  double norm_nchi2cn = ztt_taupt->Chi2Test(emd_taupt,"UU NORM CHI2/NDF");

  cout << "Value        ---      before         after forced normalization" << endl;
  cout << "ks is              " << std::setprecision(2) << ks << "               " << norm_ks <<  endl;
  cout << "pvalue for chi2 is " << std::setprecision(2) << chi2p << "                 " << norm_chi2p <<  endl;
  cout << "pvalue for norm-chi2 is " << std::setprecision(2) << nchi2p << "                 " << norm_nchi2p <<  endl;
  cout << "chi2 for chi2 is " << std::setprecision(2) << chi2c << "                 " << norm_chi2c <<  endl;
  cout << "chi2/NDOF for chi2 is " << std::setprecision(2) << chi2cn << "                 " << norm_chi2cn <<  endl;
  cout << "chi2 for norm-chi2 is " << std::setprecision(2) << nchi2c << "                 " << norm_nchi2c <<  endl;
  cout << "chi2/NDOF for norm-chi2 is " << std::setprecision(2) << nchi2cn << "                 " << norm_nchi2cn <<  endl;


  ztt_taupt->GetXaxis()->SetRangeUser(0.,200.);

  TCanvas* superposition = new TCanvas("superposition","superposition",2000,2000);
  superposition->cd();
  emd_taupt->SetLineColor(2);
  ztt_taupt->DrawCopy("histe1");
  emd_taupt->DrawCopy("histe1same");
  TLegend* leg2_ = new TLegend(0.15,0.65,0.62,0.80,NULL,"brNDC");
  leg2_->SetTextFont(42);
  leg2_->SetBorderSize(0);
  leg2_->SetLineColor(1);
  leg2_->SetLineStyle(1);
  leg2_->SetLineWidth(1);
  leg2_->SetFillColor(0);
  leg2_->SetFillStyle(0);
  leg2_->AddEntry(ztt_taupt,"Z#rightarrow(#tau#tau) (MC)","lpf");
  leg2_->AddEntry(emd_taupt,"Z#rightarrow(#tau#tau) (Embedded)","lpf");
  leg2_->Draw();
  superposition->Modified();
  superposition->Update();
  

  TCanvas* residuals = new TCanvas("residuals","residuals",2000,2000);
  residuals->cd();
  //ztt_taupt->Sumw2();
  //emd_taupt->Sumw2();
  ztt_taupt->Add(emd_taupt,-1.);
  //  ztt_taupt->Sumw2();
  ztt_taupt->Draw("histe1");
  TLegend* leg_ = new TLegend(0.15,0.65,0.62,0.80,NULL,"brNDC");
  leg_->SetTextFont(42);
  leg_->SetBorderSize(0);
  leg_->SetLineColor(1);
  leg_->SetLineStyle(1);
  leg_->SetLineWidth(1);
  leg_->SetFillColor(0);
  leg_->SetFillStyle(0);
  leg_->AddEntry(ztt_taupt,"MC - embedded","lpf");

  ostringstream strs;
  strs << setprecision(2) << ks;
  string str = "p-value from KS test: " + strs.str();

  TPaveText *pt1 = new TPaveText(0.17,0.45,0.5,0.5, "brNDC");
  pt1->SetBorderSize(1);
  pt1->SetFillColor(19);
  pt1->SetFillStyle(0);
  pt1->SetLineColor(0);
  pt1->SetTextFont(132);
  pt1->SetTextSize(0.033);
  TText *text = pt1->AddText( str.c_str() );
  text->SetTextAlign(11);
  leg_->Draw();
  pt1->Draw("same");
  
  residuals->Modified();
  residuals->Update();

//  residuals->SaveAs("embeddedClosureTest.pdf");
//  residuals->SaveAs("embeddedClosureTest.png");


  
  
}
