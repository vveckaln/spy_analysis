#ifndef _HistogramPlotter_hh
#define _HistogramPlotter_hh
/**                                                                                                                                                                              
 \class    HistogramPlotter HistogramPlotter.cc "UserCode/LIP/TopTaus/src/HistogramPlotter.cc"                                                                     
 \brief    class for managing the plotting of histograms from the output files
 
  \author   Pietro Vischia, pietro.vischia@gmail.com

  \version  $Id: HistogramPlotter.hh,v 1.1 2013/02/22 13:24:43 vischia Exp $                                                                                                       
*/

// System headers
//#if !defined(__CINT__) || defined(__MAKECINT__)
//#include <string>
//#include <iostream>
//#include <iomanip>
//#include <fstream>
//#endif

// ROOT headers
// FIXME: move headers to cc file when not needed here
#include "TCanvas.h"
#include "TStyle.h"
#include "TH1.h"
#include "THStack.h"
#include "TString.h"
#include "TList.h"

// User includes 
#include "LIP/TopTaus/interface/FileParser.hh"
#include "LIP/TopTaus/interface/PlotStyle.hh"

class HistogramPlotter : public FileParser, public PlotStyle {

  
public :
  HistogramPlotter();
  ~HistogramPlotter();
  
  
  void plot(){};
  void processCanvas();
  void processPlots(int i);
  void parse(TString samp, TString defs, TString outFolder ){  
    outFolder_ = outFolder; 
    FileParser::parse(samp,defs); 
    processCanvas();
  };
  void saveCanvas();
  void setHistoIdProperties(TH1 *h, int i, bool firstHisto);
  void normalize(TH1 *h, int i);
  void normalize(THStack *h, int i);
  void fixExtremityBins(TH1 *h,int i);
  void setHistoSampleProperties(TH1 *h, int i);
  void setStackIdProperties(THStack *h, int i);
  
  double getError2OnSystematics( TString sampName, double value );
  
  pair<bool,bool> getRatioOptions(TH1 *h, int i);
  
  void plotLegend( TH1 * higgs, TLegend *l, TString t,  vector<TH1*> & vhistos , vector< pair<TString,TString> > &vOptions, int data );
  void showRatio();
  
  TCanvas * c_;
  TString outFolder_;
  
  map< int, TFile *> samplesFiles_;
  
  //  TStyle * tdrStyle;
  
  bool plotHiggs_ ;
  bool showOnlyBR_ ;
  bool includeErrors_;
  double LUM_ERR;
  
};





#endif // _HistogramPlotter_hh
