#ifndef _LandSShapesProducer_hh
#define _LandSShapesProducer_hh

/**                                                                                                                                                                              
																						 \class    LandSShapesProducer LandSShapesProducer.cc "UserCode/LIP/TopTaus/interface/LandSShapesProducer.hh"                                                                   																						 \brief    executable for performing multivariable likelihood fit in order to improve estimation of N_fakes
																						 																						 \author   Pietro Vischia
																						 																						 \version  $Id: LandSShapesProducer.hh,v 1.14 2012/12/17 18:01:51 vischia Exp $                                                                                                       
																																												 TODO List:
  - generalize to array of samples, simplifying the approach and maintainability
  - de-zero the zeroed bins (Pedro suggestion: essentially RooFit (->combine tool) does not like zero bins -> set them to 1E-06
  - multidimensional shapes
  - MVA shapes (creating an external reweighter and not this code)
  - Define enum for classifying, replacing isSignal_, isFromData_, isDD_
  - Define a class for all the sample properties, like names, files, trees, classifications, colours and so on
  
*/




#if !defined(__CINT__) || defined(__MAKECINT__)

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif

// System headers
#include <string>
#include <fstream>

// RooFit headers
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooKeysPdf.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooNLLVar.h"
#include "RooAddition.h"
//#include "RooMinuit.h"


// ROOT headers
#include "TString.h"
#include "TStyle.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TLegend.h"
#endif

#include "LIP/TopTaus/interface/Utilities.hh"

#include "LIP/TopTaus/interface/FitVar.hh"

class LandSShapesProducer: protected utilities::EditorialUtils, utilities::StatUtils {
  
public :
  
  // Constructor
  LandSShapesProducer(string, string, string, bool);
  // Destructor
  ~LandSShapesProducer();
  
  /**
     @short performs the variable fit to a given set of variables
  */
  void Produce();
  
private:

  void Init();
  void InitMassPoint(size_t);
  void InitPerVariableAmbient(size_t);
  void BuildDatasets(size_t);
  //  void BuildPDFs(size_t);
  void DrawTemplates(size_t);
  
  void UnrollMultiDimensionalShape();
  void StorePerMassSignalShapes();
  void DrawSignalShapesComparison();

  void ShapesToDatacard();
  
  // void BuildConstrainedModels(size_t);
  //  void DoPerVariableFit(size_t);
  //  void DrawPerVariableFit(size_t);
  //  void DoPerVariableLikelihoodFit(size_t);
  //  void DoCombinedLikelihoodFit();
  // Data members
  
  // Parameter set
  string parSet_;
  string whatToDo_; 

  // Output paths
  string outputPrefix_;
  string outFolder_;
  string outputFileName_;
  string datacardsBaseName_;
  vector<string> massPointName_;
  string resultsFileName_; // not used. txt.
  ofstream resultsFile_;
  streambuf* streamToFile_;

  ofstream datacard_;
  // Style
  TStyle* myStyle_;

  // Fit settings
  //  bool includeSignal_;
  //  bool standaloneTTbar_;
  
  // Display/produce settings
  // Can't do simultaneously at the moment because of renormalization of histograms,
  // so must revise the code for dropping the need of that
  bool produceOnly_;
  bool doMultiDimensionalShapes_;
  bool unsplitUncertainties_;
  // Input paths
  // Condense in sampleclass->GetBaseDir
  vector<TString> baseDir_;

  vector<Int_t> isFromData_;
  vector<Int_t> isDDbkg_;
  vector<Int_t> isSignal_;

  double displayMin_;
  double displayMax_;

  vector<double> signalNormFactor_;

  vector<TString> inputFileName_; // Build from mass ranges

  vector<string> sampleName_;

  vector<string> fancySampleName_;

  vector<Int_t> sampleColour_; // Non for data.
  vector<Int_t> sampleFillStyle_; // Non for data.

  // Condense in sampleclass->GetMinitreeName
  vector<TString> minitree_;
  
  vector<TString> systComponents_;
  vector<TString> systFancyComponents_;
  
  // Input files
  vector<TFile*> inputFile_;

  double osCutEff_; // For DD rescaling

  // Input base trees
  vector<TTree*> inputTree_;

  // Input syst trees
  vector<vector<TTree*> > systTree_;
  
  vector<string> uncSources_;
  size_t currentMassPoint_;
  size_t nMassPoints_;
  // Variables parameters
  size_t nVars_;
  size_t nSamples_;
  size_t nSysts_;

  vector<FitVar*> fitVars_;  
  vector<string> vars_;
  vector<string> fancyNames_;
  vector<double> mins_;
  vector<double> maxs_;
  vector<int> bins_;
  vector<string> binNames_;
  vector<double> hmin_;
  vector<double> hmax_;
  vector<Int_t> unbinned_;
  vector<Int_t> smoothOrder_;
  vector<Int_t> doLogy_;
  
  TCanvas* canvas_;

  // RooFit stuff

  // Per-var stuff
  string identifier_;
  
  RooRealVar* myvar_         ;
  RooRealVar* myvarMultiD_   ;
  RooRealVar* myvar_weights_ ;
  RooRealVar* isOSvar_       ;
  
  vector<string> myDSName_ ;

  vector<vector<string> > mySystDSName_ ; // must have dummy name for zero component (data)

  vector<RooDataHist*> histo_     ;
  
  vector<vector<RooDataHist*> > systHisto_     ;
  
  vector<RooDataSet*> myDS_         ; 
  
  vector<vector<RooDataSet*> >mySystDS_         ; 
  
  vector<vector<TH1*> > masterHist_;
  vector<vector<TString> > masterHistNames_;
  vector<TH1*> masterShapes_;

  vector<vector<TH1*> > signalShapesToCompare_;
  vector<vector<TH1*> > signalShapesToCompareHH_;
  vector<vector<TH1*> > signalShapesToCompareWH_;
  
  vector<TH1*> perMassPointSignalShapesToCompare_;
  vector<TH1*> perMassPointSignalShapesToCompareHH_;
  vector<TH1*> perMassPointSignalShapesToCompareWH_;
  
  TH1* ddbkgHistUp_;  // These are for showing the error bands. Not for LandS
  TH1* ddbkgHistDown_;// These are for showing the error bands. Not for LandS
  vector<TH1*> hist_;
  vector<TH1*> histStatNoNorm_;
  vector<TH1*> histStatUp_;

  vector<TH1*> histStatDown_;
  
  vector<vector<TH1*> > systHist_;
 
  vector<TH2D*> th2_;
  vector<TH2D*> th2StatUp_;
  vector<TH2D*> th2StatDown_;
  vector<vector<TH2D*> > th2Syst_;

  vector<TH1D*> unrolled_;
  vector<TH1D*> unrolledStatUp_;
  vector<TH1D*> unrolledStatDown_;
  vector<vector<TH1D*> > unrolledSyst_;

 
  TLegend* leg_;
  
  double sumWeights_;
  
};

#endif //_TauDileptonPDFBuilderFitter_hh


