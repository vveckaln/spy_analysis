#include "LIP/TopTaus/interface/HistogramPlotter.hh"


// ROOT headers
#include "TMath.h"
#include "TH1D.h"
#include "TH2.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TObjString.h"
#include "TPaveText.h"
#include "TLine.h"
#include "TLatex.h"
#include "TClass.h"
#include "TLegendEntry.h"


HistogramPlotter::HistogramPlotter(): 
  PlotStyle() {
  c_ = 0; 
  plotHiggs_= false;
  showOnlyBR_=false; 
  includeErrors_=true; 
  LUM_ERR = 0.026;
  
  setTDRStyle();
  gStyle->SetPadTopMargin   (0.06);
  gStyle->SetPadBottomMargin(0.20);
  gStyle->SetPadRightMargin (0.16);
  gStyle->SetPadLeftMargin  (0.14);

  
}

HistogramPlotter::~HistogramPlotter()
{
}

void HistogramPlotter::processCanvas(){

  
  printf("begin HistogramPlotter::processCanvas()\n");
  if(c_) delete c_;

  for(int i=0; i<= idNumber_;i++){
    printf("%u %s\n", i, mapIdtag_[i].Data());
    char name[128];
    sprintf(name, "c1_%u", i);
    c_ = new TCanvas(name, name,800,800);// fianco

    // Set canvas as id name  ///
    c_->SetName( mapIdtag_[i]);
    /////////////////////////////
  
    processPlots(i);
     
    saveCanvas();
  } 
  printf("end HistogramPlotter:processCanvas()\n");

} 

void HistogramPlotter::processPlots(int i){
  printf("begin HistogramPlotter::processPlots()\n");
  c_->cd();
  bool firstPlot(true);

  const float xpad[2] = {0.,1};
  const float ypad[4] = {0.,0.2,0.2,1.0};
  const float yscale = (ypad[3]-ypad[2])/(ypad[1]-ypad[0]);

  map< int, int >::iterator it;
  // find data file if present ////////////////////////////////////////////////////////////
  TH1 * dataHisto(0);
  pair<bool,bool> ratioOptions(false,false); //(ratio between stack and data, relative ratio)
  int dataSample(-1); // only supports one sample...
  for(it = mapIsData_.begin(); it!= mapIsData_.end(); it++){
    if( mapIsData_[ (*it).first ]){ 
      if( (*it).second ){ 
        dataSample = (*it).first; 
        TString plotName = mapIdFolder_[i]+TString("/")+mapIdHistoName_[i];
        dataHisto = ( TH1 * ) mapFiles_[dataSample]->Get(plotName);
        pair<bool,bool> rOptions( getRatioOptions(dataHisto,i) );
        ratioOptions.first  = rOptions.first;  ratioOptions.second = rOptions.second;
        break;
      } 
    }
  }
  ////////////////////////////////////////////////////////////////////////////////////////

  
  // prepare canvas if this is a ratio plot //////
  if(ratioOptions.first){
    c_->Divide(1,2);
    TPad *p = (TPad *)c_->cd(1);
    //c_->cd(1);
    //TPad* p = new TPad();
    p->SetBottomMargin(0.1);//0);
    p->SetPad(xpad[0],ypad[2],xpad[1],ypad[3]);
  }
  else{
    c_->SetWindowSize(800,800);
    c_->SetCanvasSize(800,800);
    TPad* p = (TPad*) c_->cd();
    //c_->cd();
    //TPad* p = new TPad();
    p->SetPad(0,0,1,1);
  }
  // See if log is enabled /////////////////////////////////////////////////////// 
  it = mapIdlogy_.find(i); 
  if(it != mapIdlogy_.end()){ bool logy = mapIdlogy_[i]; c_->cd(1)->SetLogy( (int) logy);}
  ////////////////////////////////////////////////////////////////////////////////



  vector<TH1*> vhistos;
  vector< pair<TString,TString> > vOptions;


  // get stackSamples /////////////////////////////////////////////
  vector<int> stackSamples; vector<int> noStackSamples;
  for(it = mapIsStack_.begin(); it!= mapIsStack_.end(); it++){
    if(! mapIsData_[ (*it).first ]){
      if( (*it).second ){ stackSamples.push_back(   (*it).first );  }
      else              { noStackSamples.push_back( (*it).first );  }
    }
  }
  //////////////////////////////////////////////////////////////////


  //Plot stack histograms ///////////////////////////////////////////////
  vector<TH1 *> histoStacks;
  TH1 * errorH=0;
  TGraphErrors* myError = new TGraphErrors();

  TH1 *    higgsH=0; TString histoHiggsPlotName("");
  if(stackSamples.size()){

    THStack * stack = new THStack("stack","stack");

    float totalIntegral(0);
    float totalIntegralWithHiggs(0);
    for(uint u=0;u<stackSamples.size();u++){
      int ind = stackSamples[u];
      TString plotName = mapIdFolder_[i]+TString("/")+mapIdHistoName_[i];
      TH1 * histo         = ( TH1 * ) mapFiles_[ind]->Get(plotName);
      printf("mapFiles_[ind] -> GetName %s plotName %s, *histo %p\n", mapFiles_[ind]-> GetName(), plotName.Data(), (void*) histo);
      TH1 * histoForHiggs = 0; 
 
      TString opt("lpf");
      map< int , TString >::iterator itopt ;   
      itopt = mapLegopt_.find(ind); if(itopt != mapLegopt_.end()){ opt = mapLegopt_[ind]; }
	  
      setHistoSampleProperties(histo,ind); 
      setHistoIdProperties(histo,i,firstPlot);
      stack->Add(histo, "hist");
      
      
      // Build Histo with errors //////////////////////////////////////////////////////////////////////////////////////////
      if(includeErrors_){
        if( u==0 ){ 
          TString errorName("systematics");
	  errorH = ( TH1 * ) histo->Clone(errorName); errorH->Sumw2(); 
          int numberOfBins = errorH->GetNbinsX();
          for(int n=1;n<=numberOfBins; n++){
            double value    = errorH->GetBinContent(n);
            double errorN   = errorH->GetBinError(n);

            TString sampName(mapName_[ind]);
            double errorContribution2 = getError2OnSystematics(sampName, value);
            errorH->SetBinError( n, sqrt( errorContribution2 + errorN*errorN ));
	    myError->SetPoint(n, errorH->GetBinCenter(n), errorH->GetBinContent(n) );
	    myError->SetPointError(n, errorH->GetBinWidth(n)/2, errorH->GetBinError(n) );
	  }
        }
        else {
          TH1 * temp = (TH1 *) histo->Clone();  
          int numberOfBins = temp->GetNbinsX();
          for(int n=1;n<=numberOfBins; n++){
            double value  = temp->GetBinContent(n);
            double errorN = temp->GetBinError(n);

	    double graphBinError = myError->GetErrorY(n);

	    TString sampName(mapName_[ind]);
            double errorContribution2 = getError2OnSystematics(sampName, value);
            temp->SetBinError( n, sqrt( errorContribution2 + errorN*errorN ));
	    myError->SetPoint(n, temp->GetBinCenter(n), temp->GetBinContent(n) + (myError->GetY())[n] );
	    myError->SetPointError(n, temp->GetBinWidth(n)/2, sqrt( graphBinError*graphBinError + temp->GetBinError(n)* temp->GetBinError(n) ) );
	  }
          errorH->Add(temp);
        }     
      }       
      //////////////////////////////////////////////////////////////////////////
      
      // is this needed?????????????????????????????????
      // for the higgs /////////////////////////////
      if(histoForHiggs){
        setHistoSampleProperties(histoForHiggs,ind); 
        setHistoIdProperties(histoForHiggs,i,firstPlot);
      }
      ///////////////////////////////////////////////
     

      if(plotHiggs_){
        histoHiggsPlotName = plotName;
        histoForHiggs = (TH1 * )  histo->Clone(plotName); // clone for the higgs stack
        //if( (mapName_[ind]).Contains("t#bar{t}") ){ histoForHiggs->Scale(0.8*0.8); }        // for the higgs : warning ttbar is scaled if name is found /////
        if( (mapName_[ind]).Contains("t#bar{t}") ){ histoForHiggs->Scale(0.95*0.95); }
        ///////////////////////////////////////////////////////////////////
        if(u==0){ higgsH = histoForHiggs; }/// anti quick fix ///
        else{ higgsH->Add(histoForHiggs); }/// anti quick fix ///
      }


      vhistos.push_back(histo);
      pair<TString,TString> key(mapName_[ind],opt); vOptions.push_back(key);
 
      totalIntegral += histo->Integral();
              
      firstPlot=false;
      histoStacks.push_back(histo);
	
    }
    normalize(stack, i);

    c_->cd(1);


    stack->Draw("hist");

    /*if(!ratioOptions.first)*/ stack->GetHistogram()->GetXaxis()->SetTitleOffset(0.8);
    /*if(!ratioOptions.first)*/ stack->GetHistogram()->GetXaxis()->SetTitleSize(0.05);
    /*if(!ratioOptions.first)*/ stack->GetHistogram()->GetXaxis()->SetLabelSize(0.07); // 0.05 is jodorStyle
    
    setStackIdProperties(stack,i);
    

    /// quick fix ///    if(higgsH){ 
    if(plotHiggs_){ 
      //get HH , get WH and add to it
      
      //      TFile * file1 = TFile::Open( "/lustre/data3/cmslocal/vischia/tau_dilepton/outputFiles444_3_nojer_2012-10-27/mt-2011-V1-mc-MU-20GeV/out-wh-pythia-m120.root");
      //      TFile * file2 = TFile::Open( "/lustre/data3/cmslocal/vischia/tau_dilepton/outputFiles444_3_nojer_2012-10-27/mt-2011-V1-mc-MU-20GeV/out-hh-pythia-m120.root");
      
      TFile * file1 = TFile::Open( "/lustre/data3/cmslocal/vischia/tau_dilepton/newRun/nomt-2011-V1-mc-MU-20GeV/out-wh-pythia-m120.root");
      TFile * file2 = TFile::Open( "/lustre/data3/cmslocal/vischia/tau_dilepton/newRun/nomt-2011-V1-mc-MU-20GeV/out-hh-pythia-m120.root");
      
      cout<<"\n plot name is : "<<histoHiggsPlotName<<endl;
      
      TH1F * wh  = (TH1F*) file1->Get(histoHiggsPlotName);
      TH1F * hh  = (TH1F*) file2->Get(histoHiggsPlotName);
      
      setHistoIdProperties(wh,i,firstPlot);
      setHistoIdProperties(hh,i,firstPlot);


      // Light charged higgs values
      //2(1-x)x + x2
      //20%
      //higgsH->Add(wh,0.32);
      //higgsH->Add(hh,0.04);

      higgsH->Add(wh,0.095);/// anti quick fix /// 
      /// anti quick fix ///      higgsH=wh;
      /// anti quick fix ///      higgsH->Scale(0.095/higgsH->Integral());
      higgsH->Add(hh,0.025);

      cout << "higgsH integral:" << higgsH->Integral() << endl;
      
      normalize(higgsH, i);
      higgsH->SetFillColor(0);
      higgsH->SetLineColor(616);/// anti quick fix ///      
      higgsH->SetLineWidth(2);
      higgsH->SetLineStyle(7);
      higgsH->Draw("same hist0 e0");
      higgsH->Draw("same hist");
    }
	 
  }
  ////////////////////////////////////////////////////////////////////////


  if(errorH){
    //normalize(errorH, i);
    errorH->SetFillColor(4);
    errorH->SetFillStyle(3001);

    myError->SetFillColor(kGray+2);
    myError->SetFillStyle(3004);
    errorH->SetFillStyle(3004);
    // 2011-12-03 PIETRO    myError->Draw("2same");


  }

  // Plot unstacked samples ////////////////////////////////////////////////////////////
  for( uint u=0;u<noStackSamples.size();u++){
    int ind = noStackSamples[u];
    //get plots from sample
    TString plotName = mapIdFolder_[i]+TString("/")+mapIdHistoName_[i];
    TH1 * histo = ( TH1 * )mapFiles_[ind]->Get(plotName);
    setHistoSampleProperties(histo,ind);
    setHistoIdProperties(histo,i,firstPlot);	

    TString opt("pl hist");
    map< int , TString >::iterator itopt ;   
    //itopt = mapLegopt_.find(ind); if(itopt != mapLegopt_.end()){ opt = mapLegopt_[ind]; }
    itopt = mapSampleOpt_.find(ind); if(itopt != mapSampleOpt_.end()){ opt = mapSampleOpt_[ind]; }

    normalize(histo, i);

    c_->cd(1);
    histo->Draw(opt);
    firstPlot = false;                
    
    vhistos.push_back(histo); 
    pair<TString,TString> key(mapName_[ind],TString("l")); vOptions.push_back(key);	 
		   
  }
  ////////////////////////////////////////////////////////////////////////////////////////

  if( dataSample!=-1){
    
    TString opt("pl");
    map< int , TString >::iterator itopt ;   
    itopt = mapLegopt_.find(dataSample); if(itopt != mapLegopt_.end()){ opt = mapLegopt_[dataSample]; }

    setHistoSampleProperties(dataHisto,dataSample); 
    setHistoIdProperties(dataHisto,i,firstPlot);
    normalize(dataHisto, i);

    if(firstPlot)  { dataHisto->Draw();     firstPlot = false;  }
    else           { dataHisto->Draw("same"); firstPlot = false;}

    vhistos.push_back(dataHisto);
    pair<TString,TString> key(mapName_[dataSample],opt); vOptions.push_back(key);
  }
  
  TLegend* leg = new TLegend(0.845,0.2,0.99,0.99,NULL,"NDC"); // On the side <3
//  //  leg->SetBorderSize(1);
//  leg->SetTextFont(132);
//  leg->SetTextSize(0.033);
//  leg->SetLineColor(0);
//  //  leg->SetLineStyle(1);
//  //  leg->SetLineWidth(1);
//  leg->SetFillColor(0);
//  leg->SetFillStyle(0);
//  

  TString title("");
  map< int , TString >::iterator ittitle ;   
  ittitle = mapIdtitle_.find(i); if(ittitle != mapIdtitle_.end()){ title = mapIdtitle_[i]; }
  plotLegend( higgsH, leg, title, vhistos, vOptions, dataSample);
  
  // draw pad with ratio ////////////////////////////////////////
  if(ratioOptions.first){

    TH1* denominator;
    for(uint u=0;u<stackSamples.size();u++){
      if(u == 0){
        TString cloneName = histoStacks[0]->GetName() + TString("_ratio");
        denominator = (TH1*) histoStacks[0]->Clone(cloneName); 
      }
      else{ denominator->Add(histoStacks[u]);}
    }
    
    TPad* p = (TPad*)c_->cd(2);   
    //c_->cd(2);
    //TPad* p = new TPad();
    /// test p->SetFillColor(0);
    //    p->SetPad(xpad[0],ypad[2],xpad[1],ypad[3]);

    TH1* dataClone = (TH1*) dataHisto->Clone("dataclone");
    

    TH1* iRatio = dataClone;    
    
    if(ratioOptions.second) {
      iRatio->Add(denominator,-1);            
      iRatio->GetYaxis()->SetTitle("Relative difference");
    }
    else iRatio->GetYaxis()->SetTitle("Data/#Sigma MC");
    
    
    iRatio->Divide(denominator); 
    
    
    //    TGraphErrors* myRelError = new TGraphErrors(myError->GetN());
    TGraphErrors* myRelError = new TGraphErrors(dataClone);
    TH1* myDataClone = (TH1*) dataClone->Clone("forValues");
    myDataClone->Add(denominator,-1);
    if(includeErrors_){
      //      TGraphErrors* myRelErrorTemp = (TGraphErrors*) myError->Clone();//"myRelError");
      //      myRelError = myRelErrorTemp;
      for(int n=1;n<myRelError->GetN(); n++){// FIXME: 1?
	double xValue, yValue;
	/*myRelError*/myError->GetPoint(n, xValue, yValue);
	double xError   = /*myRelError*/myError->GetErrorX(n);
	double yError   = /*myRelError*/myError->GetErrorY(n);
	
	double newX(iRatio->GetBinCenter(n)), newY(iRatio->GetBinContent(n));
	//  sqrt( (eA.B)^2 + (A.eB)^2 ), /B^2
       	double newError( sqrt( pow(myDataClone->GetBinError(n) * denominator->GetBinContent(n),2) + pow(myDataClone->GetBinContent(n) * yError,2) )/pow(denominator->GetBinContent(n),2)  );
	myRelError->SetPoint(n, newX, newY );
	myRelError->SetPointError(n, xError, newError );
	
	
	//	map< int, float >::iterator errRenormIt = mapIdnorm_.find(i);
	//	if( errRenormIt != mapIdnorm_.end() ){
	//	  newError =  sqrt( pow(   (myDataClone->GetBinError(n)/myDataClone->GetBinContent(n) )* denominator->GetBinContent(n),2) + pow(myDataClone->GetBinContent(n) * yError/yValue,2) )/pow(denominator->GetBinContent(n),2);
	//	  myRelError->SetPointError(n, xError, newError); 
	//	}
	
      }
    }
    
    // set iRatio x axis and title to the same as data /////////////////////////////////////////////////////////////////////////////////
    map<int, pair<float,float> >::iterator it1; it1=mapIdXrange_.find(i);   
    if( it1 != mapIdXrange_.end() ){ pair<float,float> r(mapIdXrange_[i]); iRatio->GetXaxis()->SetRangeUser(r.first,r.second);}
    map<int, TString >::iterator itxtitle;
    itxtitle=mapIdXtitle_.find(i);
    iRatio->GetXaxis()->SetTitle(mapIdXtitle_[i]);
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    

    if(ratioOptions.second)  iRatio->GetYaxis()->SetRangeUser(-1.1,1.1);

    iRatio->SetStats(0);
    gStyle->SetOptStat(0);

    iRatio->GetYaxis()->SetRangeUser(0.,2.);
 
    iRatio->GetXaxis()->SetTitleOffset(0.85);
    iRatio->GetXaxis()->SetLabelSize(0.04 * yscale);
    //    iRatio->GetXaxis()->SetBinLabel(7, "1l+ #geq 3j");
    iRatio->GetXaxis()->SetLabelOffset(0.02 * yscale);
//    iRatio->GetXaxis()->SetTitleSize(0.06 * yscale);
//    iRatio->GetXaxis()->SetTitleOffset(0.9);
//    iRatio->GetXaxis()->SetTickLength( 0.03 * yscale );
    iRatio->GetYaxis()->SetTitleOffset(0.7);
    iRatio->GetYaxis()->SetLabelSize(0.04 * yscale);
    iRatio->GetYaxis()->SetTitleSize(0.04 * yscale);     
    iRatio->GetYaxis()->SetNdivisions(5);     
    iRatio->GetXaxis()->SetTitle("");
    iRatio->SetMarkerSize(1);
    iRatio->SetMarkerColor(1);
    iRatio->SetLineColor(1);
    iRatio->SetFillColor(0);
    iRatio->SetMarkerStyle(20);
    TH1* displayRatio = (TH1*) iRatio->Clone("myratiofordisplay");
    if(includeErrors_) iRatio->Reset("ICE");
    iRatio->Draw();
    if(includeErrors_){
/////      myRelError->SetName("bl");
/////      myRelError->SetTitle("mmbm");
      //      myRelError->SetFillColor(kGray+2);
      // test memyRelError->SetLineColor(1);
      // test memyRelError->SetFillColor(kGray+2);
      // test memyRelError->SetFillStyle(3001);

      myRelError->SetLineColor(1);
      myRelError->SetFillStyle(3001);
      myRelError->SetFillColor(kGray+2);
      myRelError->SetMarkerColor(1);
      myRelError->SetMarkerStyle(1);

      


      //myRelError->SetFillStyle(3001);
      //      myRelError->Draw("2same");
      myRelError->Draw("2");
      displayRatio->Draw("same");
    }
    
    //draw the canvas
    p->SetTopMargin(0);
    p->SetBottomMargin(0.5);//3);
    p->SetPad(xpad[0],ypad[0],xpad[1],ypad[1]);
    p->SetGridx();
    p->SetGridy();
    p->Modified();
    p->Update();

  }
  ////////////////////////////////////////////////////////////////
  
  

  printf("end HistogramPlotter::processPlots()\n");
}

void HistogramPlotter::plotLegend( TH1 * higgs, TLegend *l, TString title, vector<TH1*> & vhistos , vector< pair<TString,TString> > &vOptions , int data){
  
  //FormatLegendForMonitor(l,title);
  
  int size = (int) vhistos.size();
  
  TLegend *leg = new TLegend(0.845,0.2,0.99,0.99,NULL,"NDC"); // On the side
  
  leg->SetBorderSize(1);
  leg->SetTextFont(132);
  leg->SetTextSize(0.033);
  leg->SetLineColor(0);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  
  // Old style (higgs and data on top)
  // for(int i=(size-1); i>-1; i=i-1){ 
  //   l->AddEntry(vhistos[i],(vOptions[i]).first ,(vOptions[i]).second ); 
  //   if(i==size-1 && higgs ){l->AddEntry(higgs,"with H^{#pm} #rightarrow #tau^{#pm}#nu","l");}
  //   /// antifix /// if(i==size-1 && higgs ){l->AddEntry(higgs,"H^{#pm} #rightarrow #tau^{#pm}#nu","l");}
  // }
  
  
  // New style (higgs and data on bottom)
  for(int i=0; i<size; ++i){ 
    l->AddEntry(vhistos[i],(vOptions[i]).first ,(vOptions[i]).second ); 
    if(i==size-1 && higgs ){l->AddEntry(higgs,"with H^{#pm} #rightarrow #tau^{#pm}#nu","l");}
    /// antifix /// if(i==size-1 && higgs ){l->AddEntry(higgs,"H^{#pm} #rightarrow #tau^{#pm}#nu","l");}
  }
  


  l->SetFillColor(0);
  l->SetTextFont(42);
  l->SetBorderSize(0);
  l->Draw();
  
  TPaveText *pt = new TPaveText(0.15,0.93,0.9,1.0, "brNDC");
  pt->SetBorderSize(1);
  pt->SetFillColor(19);
  pt->SetFillStyle(0);
  pt->SetLineColor(0);
  //  pt->SetTextFont(132);
  pt->SetTextSize(0.045);
  TText *text = pt->AddText("CMS Preliminary, #sqrt{s} = 8 TeV, #scale[0.5]{#int} L=19.7 fb^{-1}");
  text->SetTextAlign(11);
  pt->Draw();
  
  
  if(plotHiggs_){
    TPaveText *pt1 = new TPaveText(0.17,0.85,0.65,0.895, "brNDC");
    pt1->SetBorderSize(1);
    pt1->SetFillColor(19);
    pt1->SetFillStyle(0);
    pt1->SetLineColor(0);
    pt1->SetTextFont(132);
    pt1->SetTextSize(0.033);
    // TText *text = pt1->AddText("#splitline{m_{H^{#pm}} = 120 GeV/c^{2},}{BR(t #rightarrow H^{+}b) = 0.2}");
    TText *text = pt1->AddText("#splitline{m_{H^{#pm}} = 120 GeV/c^{2}}{BR(t #rightarrow H^{+}b) = 0.05}");
    text->SetTextAlign(11);
    pt1->Draw();
  }
  if(showOnlyBR_){
    TPaveText *pt1 = new TPaveText(0.17,0.85,0.65,0.895, "brNDC");
    pt1->SetBorderSize(1);
    pt1->SetFillColor(19);
    pt1->SetFillStyle(0);
    pt1->SetLineColor(0);
    pt1->SetTextFont(132);
    pt1->SetTextSize(0.033);
    // TText *text = pt1->AddText("BR(t #rightarrow H^{+}b) = 0.2");
    TText *text = pt1->AddText("BR(t #rightarrow H^{+}b) = 0.05");
    text->SetTextAlign(11);
    pt1->Draw();
  }
}


void HistogramPlotter::saveCanvas(){

  TString canvasName( c_->GetName() );
  TString pngfile  = outFolder_ + canvasName + TString(".png");
  TString pdffile  = outFolder_ + canvasName + TString(".pdf");
  TString epsfile  = outFolder_ + canvasName + TString(".eps");
  TString rootfile = outFolder_ + canvasName + TString(".root");
  TString cfile    = outFolder_ + canvasName + TString(".C");

//  c_->Modified();
//  c_->Update();
  c_->cd();

  c_->SaveAs(pdffile);
  c_->SaveAs(epsfile);
  c_->SaveAs(pngfile);
  c_->SaveAs(rootfile);
  c_->SaveAs(cfile);

}

 
void HistogramPlotter::fixExtremityBins(TH1 *h,int i){

  int addoverflow(0), addunderflow(0);
  map<int, int >::iterator it;
  it=mapIduflow_.find(i); 
  if( it != mapIduflow_.end() ){ addunderflow =  mapIduflow_[i]; }
  it=mapIdoflow_.find(i); 
  if( it != mapIdoflow_.end() ){ addoverflow =  mapIdoflow_[i]; cout<<endl<<" applying overflow... "<<endl;}

  FixExtremityBins(h,addoverflow,addunderflow);
}


void HistogramPlotter::setHistoIdProperties(TH1 *h, int i, bool firstplot){

  if(firstplot){
    h->SetTitle(mapIdtitle_[i]);

    
    // set axis properties if present ///////////////////////////////////////////////////////////////////////////////////////////
    map<int, pair<float,float> >::iterator it1, it2;
    it1=mapIdXrange_.find(i); it2=mapIdYrange_.find(i); 
    if( it1 != mapIdXrange_.end() ){ pair<float,float> r(mapIdXrange_[i]); h->GetXaxis()->SetRangeUser(r.first,r.second);}
    if( it2 != mapIdYrange_.end() ){ pair<float,float> r(mapIdYrange_[i]); h->GetYaxis()->SetRangeUser(r.first,r.second);}
    map<int, TString >::iterator itx, ity;
    itx=mapIdXtitle_.find(i); ity=mapIdYtitle_.find(i); 
 
    if( itx != mapIdXtitle_.end() ){ h->GetXaxis()->SetTitle(mapIdXtitle_[i]);  }
    if( ity != mapIdYtitle_.end() ){ h->GetYaxis()->SetTitle(mapIdYtitle_[i]); /* h->GetYaxis()->SetTitleOffset(1.2);*/}
  }

  map< int, int >::iterator it = mapIdXrebin_.find(i);
  if( it != mapIdXrebin_.end() ){ 
    if(mapIdXrebin_[i]!=0) h->Rebin(mapIdXrebin_[i]); }
  fixExtremityBins(h,i);
  
}



pair<bool,bool> HistogramPlotter::getRatioOptions(TH1 *h, int i){

  pair<bool,bool> ratioOptions(false,false);

  map< int, int>::iterator it = mapIddoRatio_.find(i);
  if( it != mapIddoRatio_.end() ){
    bool doRatio = mapIddoRatio_[i]; ratioOptions.first= doRatio;
    if(doRatio){
      map< int, int >::iterator it2 = mapIdrelRatio_.find(i);
      if( it != mapIdrelRatio_.end() ){
        ratioOptions.second = mapIdrelRatio_[i];
      }
      
    }
  }
  
  return ratioOptions;
}



void HistogramPlotter::normalize(TH1 *h, int i){
  // renormalize ///////////////////////////////////
  map< int, float >::iterator it = mapIdnorm_.find(i);
  if( it != mapIdnorm_.end() ){
    float norm =  mapIdnorm_[i];
    float integral = h->Integral();
    h->Scale(norm/integral);  
  }
  //////////////////////////////////////////////////
}


void HistogramPlotter::normalize(THStack *hstack, int i){
  // renormalize ///////////////////////////////////
  map< int, float >::iterator it = mapIdnorm_.find(i);
  float norm(0);
  double total(0);

  if( it != mapIdnorm_.end() ){ norm =  mapIdnorm_[i];}
  else{ return;}

  TList * list = hstack->GetHists();

  int listSize = list->GetSize();

  for(int i=0;i< listSize;i++){
    TH1 *h = (TH1*)list->At(i);
    if( h ) { total += h->Integral();}
  }
  float factor(0);
  if( total ){ factor=norm/total;}
  for(int i=0;i< listSize;i++){
    TH1 *h = (TH1*)list->At(i);
    if( h ) { h->Scale(factor);}
  }  
  
  //////////////////////////////////////////////////
}



void HistogramPlotter::setStackIdProperties(THStack *h, int i){
  
  // set histo title
  h->SetTitle(mapIdtitle_[i]); 
  // set axis properties if present ///////////////////////////////////////////////////////////////////////////////////////////
  map<int, pair<float,float> >::iterator it1, it2;
  it1=mapIdXrange_.find(i); it2=mapIdYrange_.find(i); 
  if( it1 != mapIdXrange_.end() ){ pair<float,float> r(mapIdXrange_[i]); h->GetXaxis()->SetRangeUser(r.first,r.second);}
  // Does not work in stack
  //if( it2 != mapIdYrange_.end() ){ pair<float,float> r(mapIdYrange_[i]); h->GetYaxis()->SetRangeUser(r.first,r.second);}
  if( it2 != mapIdYrange_.end() ){  
    pair<float,float> r(mapIdYrange_[i]); 
    float min(r.first), max(r.second);
    h->SetMinimum(min); h->SetMaximum(r.second); 
    cout<<"\n min :"<<(r.first)<<" max : "<<(r.second)<<endl;
  }

  map<int, TString >::iterator itx, ity;
  itx=mapIdXtitle_.find(i); ity=mapIdYtitle_.find(i); 
  if( itx != mapIdXtitle_.end() ){ cout<<"\n setting xtitle to : "<<(mapIdXtitle_[i])<<endl; h->GetXaxis()->SetTitle(mapIdXtitle_[i]); }
  if( ity != mapIdYtitle_.end() ){ cout<<"\n setting ytitle to : "<<(mapIdYtitle_[i])<<endl; h->GetYaxis()->SetTitle(mapIdYtitle_[i]);
/*  h->GetYaxis()->SetTitleOffset(1.2);*/}

}


void HistogramPlotter::setHistoSampleProperties(TH1 *h, int i){

  FormatHisto(h, mapColor_[i],mapLine_[i], mapLineWidth_[i], mapMarker_[i], mapFill_[i], false,true, mapLineColor_[i], mapFillColor_[i], mapMarkerColor_[i]  );
  map<int, int>::iterator it;
  it = mapLineStyle_.find(i);
  if( it != mapLineStyle_.end() )
    h->SetLineStyle(mapLineStyle_[i]);
  h->Scale( mapWeights_[i] );
  
  
  
  for(int i=1; i<= h->GetXaxis()->GetNbins() ; i++){
    TString label=h->GetXaxis()->GetBinLabel(i);
    if( label==TString("MET") ) h->GetXaxis()->SetBinLabel(i,"E_{T}^{miss}");
  }

  
  
}



void HistogramPlotter::showRatio(){

  cout<<endl<<"show ratio!"<<endl;

  //Pad positioning stuff
  const float xpad[2] = {0.,1};
  const float ypad[4] = {0.,0.3,0.3,1.0};

  TPad * originalPad = (TPad *)c_->cd();
  // test originalPad->SetFillColor(0);

  TCanvas * clone = new TCanvas("newName","newName",1000,1000);
  clone->Divide(1,2);

  TPad *p = (TPad *)clone->cd(1);
  // test p->SetFillColor(0);
  p->SetBottomMargin(0);
  p->SetPad(xpad[0],ypad[2],xpad[1],ypad[3]);
  p->SetGridx();
  p->SetGridy();

  originalPad->Draw();
  clone->Update();
  clone->cd(2);
  originalPad->Draw();  
  clone->Update();
  clone->Draw();
  clone->SaveAs("test.png");

  /*p->SetBottomMargin(0);
  p->SetPad(xpad[0],ypad[2],xpad[1],ypad[3]);
  p->SetGridx();
  p->SetGridy();

  c_->cd();
  c_->Update();
  c_->Draw();
*/


}


/*
TLegend * HistogramPlotter::ShowRatio(TString name, TString title, TList &plots, TString opt, bool drawNormalized, Int_t w, Int_t h, TPad *c, TLegend *leg, TString legopt, bool relative)
{
  //Pad positioning stuff
  const float xpad[2] = {0.,1};
  const float ypad[4] = {0.,0.3,0.3,1.0};


  //build the canvas
  if(c!=0) { c->Clear(); }
  else     {
    TCanvas *cnv = new TCanvas(name,title,w,h); 
    c = (TPad *)cnv->cd();
  }
  c->Divide(1,2);
  TPad *p = (TPad *)c->cd(1);
  p->SetBottomMargin(0);
  p->SetPad(xpad[0],ypad[2],xpad[1],ypad[3]);
  p->SetGridx();
  p->SetGridy();

  leg=Show(name,title,plots,opt,drawNormalized,w,h,p,leg,legopt);

  //compute the ratio of histograms
  TList ratiosH;
  TIterator *plotsIt = plots.MakeIterator();
  TH1 *ref = (TH1 *) plotsIt->Next();
  TObject *key = 0;
  while ( (key = plotsIt->Next()) ) 
    {
      TH1 *p = (TH1 *) key;
      if(p==0) continue;
      TString name=p->GetName() + TString("_ratio");
      TString title= TString("(")+p->GetTitle();
      if(relative) title+= TString("-")+ref->GetTitle();
      title += TString(")/")+ref->GetTitle();
      
      TH1D *iRatio = (TH1D *) p->Clone(name);
      if(relative) 
	{
	  iRatio->Add(ref,-1);
	  iRatio->GetYaxis()->SetTitle("Relative difference");
	}
      else
	  iRatio->GetYaxis()->SetTitle("Ratio");
      iRatio->SetTitle(title);
      iRatio->Divide(ref);      
      ratiosH.Add(iRatio);
    }
  
  //draw the canvas
  p = (TPad *)c->cd(2);
  p->SetTopMargin(0);
  p->SetBottomMargin(0.3);
  p->SetPad(xpad[0],ypad[0],xpad[1],ypad[1]);
  p->SetGridx();
  p->SetGridy();
  TLegend *leg2=Show(name,title,ratiosH,"e1",false,w,h,p,0,legopt);
  leg2->Delete();

  const float yscale = (ypad[3]-ypad[2])/(ypad[1]-ypad[0]);
  plotsIt = ratiosH.MakeIterator();
  while ( (key = plotsIt->Next()) ) 
    {
      TH1 *p = (TH1 *) key;
      if(p==0) continue;
      if(relative)  p->GetYaxis()->SetRangeUser(-1.1,1.1);
      p->GetXaxis()->SetTitleOffset(0.85);
      p->GetXaxis()->SetLabelSize(0.04 * yscale);
      p->GetXaxis()->SetTitleSize(0.05 * yscale);
      p->GetXaxis()->SetTickLength( 0.03 * yscale );
      p->GetYaxis()->SetTitleOffset(0.5);
      p->GetYaxis()->SetLabelSize(0.04 * yscale);
      p->GetYaxis()->SetTitleSize(0.04 * yscale);
      }
    p->Modified();
  p->Update();

  if(relative)
    {
    TLine *line = new TLine(((TH1 *)ratiosH.At(0))->GetXaxis()->GetXmin(),0,
			      ((TH1 *)ratiosH.At(0))->GetXaxis()->GetXmax(),0);
			      line->SetLineColor(14);
			      line->SetLineWidth(2);
			      line->Draw("same");
    }

  c->cd();
  c->Update();
  c->cd(1);
  return leg;
  }
  
  }
*/

double HistogramPlotter::getError2OnSystematics( TString sampName, double value ){
  
  double xsecError(0);
  double puError(0);
  double btagError(0);
  double jesError(0);
  double tauIdError(0);
  double leptonError(0);
  double effError(0);
  double osError(0.12);

  //  leptonError=0.03; // electron case
  leptonError=0.02; // muon case

  
  
  if( sampName.Contains("QCD")                  ){ xsecError = 0.50; puError= 0.05; btagError=0.05; jesError=0.05 ; tauIdError=0.15;                }
  if( sampName.Contains("Single t")             ){ xsecError = 0.08; puError= 0.02; btagError=0.07; jesError=0.06 ; tauIdError=0.06;                }
  if( sampName.Contains("t#bar{t} #rightarrow") ){ xsecError = 0.10; puError= 0.02; btagError=0.05; jesError=0.05 ; tauIdError=0.06; effError=0.04; }
  if( sampName.Contains("other t#bar{t}")       ){ xsecError = 0.10; puError= 0.08; btagError=0.04; jesError=0.04 ; tauIdError=0.06;                } 
  if( sampName.Contains("DY+jets")              ){ xsecError = 0.04; puError= 0.04; btagError=0.08; jesError=0.05 ; tauIdError=0.15;                }
  if( sampName.Contains("Diboson")              ){ xsecError = 0.04; puError= 0.04; btagError=0.08; jesError=0.05 ; tauIdError=0.15;                }
  if( sampName.Contains("W+jets" )              ){ xsecError = 0.05; puError= 0.08; btagError=0.08; jesError=0.06 ; tauIdError=0.06;                }
 
  
  // if btag plot
  //btagError=0; 
  //tauIdError=0; osError=0;

  double totalError2 = pow(value*btagError,2)+ pow(value*LUM_ERR,2)+pow(value*xsecError,2)+pow(value*puError,2)+pow(value*tauIdError,2)+pow(value*jesError,2)+pow(value*leptonError,2) + pow(value*effError,2);

  return totalError2;


}




