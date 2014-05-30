#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <TH1F.h>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TText.h>


void teenyWeenyQGJetsFractions(){
  
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
  tdrStyle->SetTitleSize(0.05, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(1.1);
  tdrStyle->SetTitleYOffset(1.25);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset
//  tdrStyle->SetTitleOffset(.3, "X"); // Another way to set the Offset
//  tdrStyle->SetTitleOffset(1., "Y"); // Another way to set the Offset
//  
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

//
//  ned in current scope  (tmpfile):1:
//    (const int)0
//*** Interpreter error recovered ***
//root [4] .ls
//      TFile**/lustre/ncg.ingrid.pt/cmslocal/samples/CMSSW_5_3_7_patch4/chiggs/fakerate_fractionsEstimate/trees/TauFRWMuMCFailing_PFlow.root
//      TFile*/lustre/ncg.ingrid.pt/cmslocal/samples/CMSSW_5_3_7_patch4/chiggs/fakerate_fractionsEstimate/trees/TauFRWMuMCFailing_PFlow.root
//root [5] .q
//[vischia@ui-cms01 TopTaus]$ ls /lustre/ncg.ingrid.pt/
//cmslocal/ cmst3/    data1/    data2/    data3/    
//[vischia@ui-cms01 TopTaus]$ ls /lustre/ncg.ingrid.pt/cmslocal/vischia/
//MyTMVA_temporaneo/      backupFirstFullRun2012/ myt
//
//																									   
//	

//   TFile* ttll = new TFile("/lustre/ncg.ingrid.pt/cmslocal/samples/CMSSW_5_3_7_patch4/chiggs//nomt-2012-V1-mc-MU-20GeV/out-ttbar_ddbkg.root");
//   TFile* wjet = new TFile("/lustre/ncg.ingrid.pt/cmslocal/samples/CMSSW_5_3_7_patch4/chiggs//nomt-2012-V1-mc-MU-20GeV/out-wjets.root");

  TFile* signal = new TFile("/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_noTopPtRew//nomt-2012-V1-mc-MU-20GeV/out-htb-pythia-m250.root");
  TFile* ttll = new TFile("/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_noTopPtRew//nomt-2012-V1-mc-MU-20GeV/out-ttbar-ddbkg.root");
  TFile* wjet = new TFile("/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_noTopPtRew//nomt-2012-V1-mc-MU-20GeV/out-wjets.root");


  TH1F* hsignalq = (TH1F*) ((TH1F*) signal->Get("PFlow/Bkg/m_tau/wplusjets_pt_allquark_3"))->Clone("hsignalq_wplusjets_pt");	       
  TH1F* hsignalg = (TH1F*) ((TH1F*) signal->Get("PFlow/Bkg/m_tau/wplusjets_pt_gluon_3"))->Clone("hsignalg_wplusjets_pt");	       
  TH1F* hsignala = (TH1F*) ((TH1F*) signal->Get("PFlow/Bkg/m_tau/wplusjets_pt_3"))->Clone("hsignala_wplusjets_pt");	       

  
  TH1F* httllq = (TH1F*) ((TH1F*) ttll->Get("PFlow/Bkg/m_tau/wplusjets_pt_allquark_3"))->Clone("httllq_wplusjets_pt");	       
  TH1F* httllg = (TH1F*) ((TH1F*) ttll->Get("PFlow/Bkg/m_tau/wplusjets_pt_gluon_3"))->Clone("httllg_wplusjets_pt");	       
  TH1F* httlla = (TH1F*) ((TH1F*) ttll->Get("PFlow/Bkg/m_tau/wplusjets_pt_3"))->Clone("httlla_wplusjets_pt");	       

  TH1F* hwjetq = (TH1F*) ((TH1F*) wjet->Get("PFlow/Bkg/m_tau/wplusjets_pt_allquark_3")         )->Clone("hwjetq_wplusjets_pt");	       
  TH1F* hwjetg = (TH1F*) ((TH1F*) wjet->Get("PFlow/Bkg/m_tau/wplusjets_pt_gluon_3")         )->Clone("hwjetg_wplusjets_pt");	       
  TH1F* hwjetu = (TH1F*) ((TH1F*) wjet->Get("PFlow/Bkg/m_tau/wplusjets_pt_unknown_3")         )->Clone("hwjetu_wplusjets_pt");	       
  TH1F* hwjeta = (TH1F*) ((TH1F*) wjet->Get("PFlow/Bkg/m_tau/wplusjets_pt_3")         )->Clone("hwjeta_wplusjets_pt");	       

  hwjetg->Add(hwjetu,1.);

  double signalfq = hsignalq->Integral() / (hsignalq->Integral() + hsignalg->Integral());
  double signalfg = hsignalg->Integral() / (hsignalq->Integral() + hsignalg->Integral());

  double ttllfq = httllq->Integral() / (httllq->Integral() + httllg->Integral());
  double ttllfg = httllg->Integral() / (httllq->Integral() + httllg->Integral());

  double wjetfq = hwjetq->Integral() / (hwjetq->Integral() + hwjetg->Integral());
  double wjetfg = hwjetg->Integral() / (hwjetq->Integral() + hwjetg->Integral());

  
  httllq->Sumw2();
  httllg->Sumw2();

  httllq->Add(hwjetq,1.);
  httllg->Add(hwjetg,1.);
  httlla->Add(hwjeta,1.);

  double fqf= httllq->Integral() / (httllq->Integral() + httllg->Integral());
  double fgf= httllg->Integral() / (httllq->Integral() + httllg->Integral());


  cout << "Fractions in ttll sample: quarks " << ttllfq << ", gluons " << ttllfg << endl;
  cout << "Fractions in wjet sample: quarks " << wjetfq << ", gluons " << wjetfg << endl;
  cout << "Combination: quarks " << 0.5*ttllfq + 0.5*wjetfq << ", gluons " << 0.5*ttllfg+0.5*wjetfg << endl;
  cout << "Fractions in main anal. sample: quarks " << fqf << ", gluons " << fgf << endl;
  cout << "-------------------------------" << endl;

  TFile* wmuP = new TFile(" /lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_noTopPtRew/fakerate/trees/TauFRWMuMCPassing_PFlow.root");
  TFile* wmuF = new TFile(" /lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_noTopPtRew/fakerate/trees/TauFRWMuMCFailing_PFlow.root");
  TFile* qcdP = new TFile(" /lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_noTopPtRew/fakerate/trees/TauFRQCDMCPassing_PFlow.root");
  TFile* qcdF = new TFile(" /lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_noTopPtRew/fakerate/trees/TauFRQCDMCFailing_PFlow.root");
    
  std::vector<TH1F*> v_wplusjets_pt;
  std::vector<TH1F*> v_wplusjets_pt_unknown;
  std::vector<TH1F*> v_wplusjets_pt_quark;
  std::vector<TH1F*> v_wplusjets_pt_bquark;
  std::vector<TH1F*> v_wplusjets_pt_gluon;
  std::vector<TH1F*> v_wplusjets_pt_allquark;

  std::vector<TH1F*> v_qf;
  std::vector<TH1F*> v_gf;


  v_wplusjets_pt         .push_back( (TH1F*) ((TH1F*) wmuP->Get("wplusjets_pt")         )->Clone("wmuP_wplusjets_pt")          );	       
  v_wplusjets_pt_unknown .push_back( (TH1F*) ((TH1F*) wmuP->Get("wplusjets_pt_unknown") )->Clone("wmuP_wplusjets_pt_unknown")  ); 
  v_wplusjets_pt_quark	 .push_back( (TH1F*) ((TH1F*) wmuP->Get("wplusjets_pt_quark")   )->Clone("wmuP_wplusjets_pt_quark")    );   
  v_wplusjets_pt_bquark	 .push_back( (TH1F*) ((TH1F*) wmuP->Get("wplusjets_pt_bquark")  )->Clone("wmuP_wplusjets_pt_bquark")   );  
  v_wplusjets_pt_gluon	 .push_back( (TH1F*) ((TH1F*) wmuP->Get("wplusjets_pt_gluon")   )->Clone("wmuP_wplusjets_pt_gluon")    );   
  v_wplusjets_pt_allquark.push_back( (TH1F*) ((TH1F*) wmuP->Get("wplusjets_pt_allquark"))->Clone("wmuP_wplusjets_pt_allquark") );

  v_wplusjets_pt         .push_back( (TH1F*) ((TH1F*) wmuF->Get("wplusjets_pt")         )->Clone("wmuF_wplusjets_pt")          );	       
  v_wplusjets_pt_unknown .push_back( (TH1F*) ((TH1F*) wmuF->Get("wplusjets_pt_unknown") )->Clone("wmuF_wplusjets_pt_unknown")  ); 
  v_wplusjets_pt_quark	 .push_back( (TH1F*) ((TH1F*) wmuF->Get("wplusjets_pt_quark")   )->Clone("wmuF_wplusjets_pt_quark")    );   
  v_wplusjets_pt_bquark	 .push_back( (TH1F*) ((TH1F*) wmuF->Get("wplusjets_pt_bquark")  )->Clone("wmuF_wplusjets_pt_bquark")   );  
  v_wplusjets_pt_gluon	 .push_back( (TH1F*) ((TH1F*) wmuF->Get("wplusjets_pt_gluon")   )->Clone("wmuF_wplusjets_pt_gluon")    );   
  v_wplusjets_pt_allquark.push_back( (TH1F*) ((TH1F*) wmuF->Get("wplusjets_pt_allquark"))->Clone("wmuF_wplusjets_pt_allquark") );

  v_wplusjets_pt         .push_back( (TH1F*) ((TH1F*) qcdP->Get("wplusjets_pt")         )->Clone("qcdP_wplusjets_pt")          );	       
  v_wplusjets_pt_unknown .push_back( (TH1F*) ((TH1F*) qcdP->Get("wplusjets_pt_unknown") )->Clone("qcdP_wplusjets_pt_unknown")  ); 
  v_wplusjets_pt_quark	 .push_back( (TH1F*) ((TH1F*) qcdP->Get("wplusjets_pt_quark")   )->Clone("qcdP_wplusjets_pt_quark")    );   
  v_wplusjets_pt_bquark	 .push_back( (TH1F*) ((TH1F*) qcdP->Get("wplusjets_pt_bquark")  )->Clone("qcdP_wplusjets_pt_bquark")   );  
  v_wplusjets_pt_gluon	 .push_back( (TH1F*) ((TH1F*) qcdP->Get("wplusjets_pt_gluon")   )->Clone("qcdP_wplusjets_pt_gluon")    );   
  v_wplusjets_pt_allquark.push_back( (TH1F*) ((TH1F*) qcdP->Get("wplusjets_pt_allquark"))->Clone("qcdP_wplusjets_pt_allquark") );

  v_wplusjets_pt         .push_back( (TH1F*) ((TH1F*) qcdF->Get("wplusjets_pt")         )->Clone("qcdF_wplusjets_pt")          );	       
  v_wplusjets_pt_unknown .push_back( (TH1F*) ((TH1F*) qcdF->Get("wplusjets_pt_unknown") )->Clone("qcdF_wplusjets_pt_unknown")  ); 
  v_wplusjets_pt_quark	 .push_back( (TH1F*) ((TH1F*) qcdF->Get("wplusjets_pt_quark")   )->Clone("qcdF_wplusjets_pt_quark")    );   
  v_wplusjets_pt_bquark	 .push_back( (TH1F*) ((TH1F*) qcdF->Get("wplusjets_pt_bquark")  )->Clone("qcdF_wplusjets_pt_bquark")   );  
  v_wplusjets_pt_gluon	 .push_back( (TH1F*) ((TH1F*) qcdF->Get("wplusjets_pt_gluon")   )->Clone("qcdF_wplusjets_pt_gluon")    );   
  v_wplusjets_pt_allquark.push_back( (TH1F*) ((TH1F*) qcdF->Get("wplusjets_pt_allquark"))->Clone("qcdF_wplusjets_pt_allquark") );

  std::vector<int> colours;
  colours.push_back(1);
  colours.push_back(2);
  colours.push_back(4);  
  colours.push_back(5);

  std::vector<TString> labels;
  labels.push_back("W+1jet ");
  labels.push_back("W+1jet ");
  labels.push_back("QCDmulti ");
  labels.push_back("QCDmulti ");

  std::vector<TString> fBase;
  fBase.push_back("WMu1");
  fBase.push_back("WMu2");
  fBase.push_back("QCD1");
  fBase.push_back("QCD2");

  for(size_t i=0; i<v_wplusjets_pt.size(); ++i){
    v_wplusjets_pt         [i]->SetLineColor(colours[i]);    
    v_wplusjets_pt_unknown [i]->SetLineColor(colours[i]);
    v_wplusjets_pt_quark   [i]->SetLineColor(colours[1]);	 
    v_wplusjets_pt_bquark  [i]->SetLineColor(colours[i]);	 
    v_wplusjets_pt_gluon   [i]->SetLineColor(colours[2]);	 
    v_wplusjets_pt_allquark[i]->SetLineColor(colours[1]);    

    v_wplusjets_pt         [i]->SetLineWidth(3);    
    v_wplusjets_pt_unknown [i]->SetLineWidth(3);
    v_wplusjets_pt_quark   [i]->SetLineWidth(3);	 
    v_wplusjets_pt_bquark  [i]->SetLineWidth(3);	 
    v_wplusjets_pt_gluon   [i]->SetLineWidth(3);	 
    v_wplusjets_pt_allquark[i]->SetLineWidth(3);    

    //    v_wplusjets_pt_gluon[i]->Add(v_wplusjets_pt_unknown[i],1.);

    TH1F* numQ = v_wplusjets_pt_allquark[i]->Clone(TString(v_wplusjets_pt_allquark[i]->GetName())+"numQ");
    TH1F* numG = v_wplusjets_pt_gluon[i]->Clone(TString(v_wplusjets_pt_gluon[i]->GetName())+"numG");
    TH1F* den =  v_wplusjets_pt_allquark[i]->Clone(TString(v_wplusjets_pt_allquark[i]->GetName())+"den");

    numQ->GetYaxis()->SetTitle("Jet content");	 
    numG->GetYaxis()->SetTitle("Jet content");	 
    

    cout << "Fractions for " << fBase[i] << ": quark " << v_wplusjets_pt_allquark[i]->Integral() / (v_wplusjets_pt_allquark[i]->Integral() + v_wplusjets_pt_gluon[i]->Integral() ) << ", gluon " << v_wplusjets_pt_gluon[i]->Integral() / (v_wplusjets_pt_allquark[i]->Integral() + v_wplusjets_pt_gluon[i]->Integral() ) << endl;
    
    numQ->Sumw2();
    numG->Sumw2();
    den->Sumw2();
    
    den->Add(v_wplusjets_pt_gluon[i],1.);
    
    numQ->Divide(den);
    numG->Divide(den);
    cout << "I divided" << endl;

    v_qf.push_back(numQ);
    v_gf.push_back(numG);
    
    
    //    TLegend* leg_ = new TLegend(0.15,0.65,0.62,0.80,NULL,"brNDC");
    TLegend* leg_ = new TLegend(0.40,0.50,0.80,0.70,NULL,"brNDC");
    leg_->SetTextFont(42);
    leg_->SetBorderSize(0);
    leg_->SetLineColor(1);
    leg_->SetLineStyle(1);
    leg_->SetLineWidth(1);
    leg_->SetFillColor(0);
    leg_->SetFillStyle(0);
    leg_->AddEntry(numQ,/*labels[i]+*/"Quark jets fraction","lpf");  
    leg_->AddEntry(numG,/*labels[i]+*/"Gluon jets fraction","lpf");

    TPaveText *pt1 = new TPaveText(0.17,0.45,0.5,0.5, "brNDC");
    pt1->SetBorderSize(1);
    pt1->SetFillColor(19);
    pt1->SetFillStyle(0);
    pt1->SetLineColor(0);
    pt1->SetTextFont(132);
    pt1->SetTextSize(0.033);
    //  TText *text = pt1->AddText("#splitline{m_{H^{#pm}} = 120 GeV/c^{2},}{BR(t #rightarrow H^{+}b) = 0.05}");
    TCanvas* myCanva = new TCanvas(labels[i],labels[i],2000,2000);
    myCanva->cd();

    if(i<2){
      numQ->GetXaxis()->SetTitleOffset(1.0);
      numQ->GetYaxis()->SetTitleOffset(1.0);
      numQ->Draw("hist");
      numG->Draw("histsame");
    }
    else{
      numG->GetXaxis()->SetTitleOffset(1.0);
      numG->GetYaxis()->SetTitleOffset(1.0);
      numG->Draw("hist");
      numQ->Draw("histsame");
    }
    leg_->Draw();
    //    pt1->Draw("same");
    myCanva->SaveAs(fBase[i]+".pdf");
    myCanva->SaveAs(fBase[i]+".png");
    myCanva->SaveAs(fBase[i]+".root");
    myCanva->SaveAs(fBase[i]+".C");
    myCanva->Clear();
    delete myCanva;
    delete pt1;
    delete numQ;
    delete numG;
    delete den;
    delete leg_;
  }




  double fqf= httllq->Integral() / (httllq->Integral() + httllg->Integral());
  double fgf= httllg->Integral() / (httllq->Integral() + httllg->Integral());


  TH1F* finalNum_qf = (TH1F*) httllq->Clone("finalNum_qf");
  TH1F* finalNum_gf = (TH1F*) httllg->Clone("finalNum_gf");

  TH1F* signalFinalNum_qf = (TH1F*) hsignalq->Clone("signalFinalNum_qf");
  TH1F* signalFinalNum_gf = (TH1F*) hsignalg->Clone("signalFinalNum_gf");
  
  TH1F* finalDen = (TH1F*) httllq->Clone("finalDen_qf"); finalDen_qf->Add(httllg);

  TH1F* signalFinalDen = (TH1F*) hsignalq->Clone("signalFinalDen_qf"); signalFinalDen_qf->Add(hsignalg);


  finalNum_qf->Divide(finalDen);
  finalNum_gf->Divide(finalDen);


  finalNum_qf->SetLineColor(2);    
  finalNum_gf->SetLineColor(4);
  
  finalNum_qf->SetLineWidth(3);    
  finalNum_gf->SetLineWidth(3);    


  signalFinalNum_qf->Divide(signalFinalDen);
  signalFinalNum_gf->Divide(signalFinalDen);


  signalFinalNum_qf->SetLineColor(2);    
  signalFinalNum_gf->SetLineColor(4);
  
  signalFinalNum_qf->SetLineWidth(3);    
  signalFinalNum_gf->SetLineWidth(3);    
  
  //    TLegend* leg_ = new TLegend(0.15,0.65,0.62,0.80,NULL,"brNDC");
  TLegend* leg_ = new TLegend(0.40,0.50,0.80,0.70,NULL,"brNDC");
  leg_->SetTextFont(42);
  leg_->SetBorderSize(0);
  leg_->SetLineColor(1);
  leg_->SetLineStyle(1);
  leg_->SetLineWidth(1);
  leg_->SetFillColor(0);
  leg_->SetFillStyle(0);
  leg_->AddEntry(finalNum_qf,/*labels[i]+*/"Quark jets fraction","lpf");  
  leg_->AddEntry(finalNum_gf,/*labels[i]+*/"Gluon jets fraction","lpf");
  
  TPaveText *pt1 = new TPaveText(0.17,0.45,0.5,0.5, "brNDC");
  pt1->SetBorderSize(1);
  pt1->SetFillColor(19);
  pt1->SetFillStyle(0);
  pt1->SetLineColor(0);
  pt1->SetTextFont(132);
  pt1->SetTextSize(0.033);
  //  TText *text = pt1->AddText("#splitline{m_{H^{#pm}} = 120 GeV/c^{2},}{BR(t #rightarrow H^{+}b) = 0.05}");
  TCanvas* myCanva = new TCanvas("mainsel","Main selection",2000,2000);
  myCanva->cd();

  finalNum_qf->GetYaxis()->SetTitle("Jet content");	 
  finalNum_qf->GetXaxis()->SetTitleSize(0.05);	 
  finalNum_qf->GetYaxis()->SetTitleSize(0.05);	 
  finalNum_qf->GetXaxis()->SetTitleOffset(1.0);
  finalNum_qf->GetYaxis()->SetTitleOffset(1.0);
    
  finalNum_qf->Draw("hist");
  finalNum_gf->Draw("histsame");
  
  leg_->Draw();
  //    pt1->Draw("same");
  myCanva->SaveAs("finalSelection.pdf");
  myCanva->SaveAs("finalSelection.png");
  myCanva->SaveAs("finalSelection.root");
  myCanva->SaveAs("finalSelection.C");
  myCanva->Clear();


  TCanvas* myCanva2 = new TCanvas("signal","Signal",2000,2000);
  myCanva2->cd();
  signalFinalNum_qf->GetYaxis()->SetTitle("Jet content");	 
  signalFinalNum_qf->GetXaxis()->SetTitleSize(0.05);	 
  signalFinalNum_qf->GetYaxis()->SetTitleSize(0.05);	 
  signalFinalNum_qf->GetXaxis()->SetTitleOffset(1.0);
  signalFinalNum_qf->GetYaxis()->SetTitleOffset(1.0);
    
  signalFinalNum_qf->Draw("hist");
  signalFinalNum_gf->Draw("histsame");
  
  leg_->Draw();
  //    pt1->Draw("same");
  myCanva2->SaveAs("signalFinalSelection.pdf");
  myCanva2->SaveAs("signalFinalSelection.png");
  myCanva2->SaveAs("signalFinalSelection.root");
  myCanva2->SaveAs("signalFinalSelection.C");
  myCanva2->Clear();
  delete myCanva2;


  delete myCanva;
  delete pt1;
  delete finalNum_qf;
  delete finalNum_qf;
  delete finalDen;
  delete leg_;


}
