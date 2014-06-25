#include "LIP/TopTaus/interface/LandSShapesProducer.hh"
#include "LIP/TopTaus/interface/CommonDefinitions.hh" // Purely for writing down lumi to datacards file, for reference

// System includes
#include <sstream>
#include <algorithm>

// CMSSW includes
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// ROOT includes
#include <THStack.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TText.h>
#include <TPaveText.h>
#include <TSystem.h>

using namespace std;
using namespace RooFit;

LandSShapesProducer::LandSShapesProducer(string parSet, string whatToDo, string outputPrefix, bool produceOnly):
  parSet_(parSet),
  whatToDo_(whatToDo),
  outputPrefix_(outputPrefix),
  produceOnly_(produceOnly)
{
  
  using namespace std; 
  using namespace RooFit ;
  
  Init();

}

LandSShapesProducer::~LandSShapesProducer(){

//  delete streamToFile_;
//  delete myStyle_;
//  delete inputFile_;
//  delete inputTree_;
//  delete[] systTree_;
//  delete[] fitVars_;  
//  delete canvas_;
//  delete myvar_         ;
//  delete myvarMultiD_   ;
//  delete myvar_weights_ ;
//  delete isOSvar_       ;
//  delete[] histo_     ;
//  delete[] systHisto_     ;
//  delete[] myDS_         ; 
//  delete[] mySystDS_         ; 
//  delete[] masterHist_;
//  delete[] masterShapes_;
//  delete[] signalShapesToCompare_;
//  delete[] signalShapesToCompareHH_;
//  delete[] signalShapesToCompareWH_;
//  delete[] perMassPointSignalShapesToCompare_;
//  delete[] perMassPointSignalShapesToCompareHH_;
//  delete[] perMassPointSignalShapesToCompareWH_;
//  delete ddbkgHistUp_;  
//  delete ddbkgHistDown_;
//  delete[] hist_;
//  delete[] histStatNoNorm_;
//  delete[] histStatUp_;
//  delete[] histStatDown_;
//  delete[] systHist_;
//  delete[] th2_;
//  delete[] th2StatUp_;
//  delete[] th2StatDown_;
//  delete[] th2Syst_;
//  delete[] unrolled_;
//  delete[] unrolledStatUp_;
//  delete[] unrolledStatDown_;
//  delete[] unrolledSyst_;
//  delete leg_;
}


void LandSShapesProducer::Init(){

  displayMin_ = 0.001;
  displayMax_ = 5.001;

//  displayMin_ = 0.001; R
//  displayMax_ = 1.001;

//  displayMin_ = 0.;
//  displayMax_ = 200.;
  
  // Clear vectors
  nVars_ = 0;
  //  uncSources_.clear();
  systComponents_.clear();
  systFancyComponents_.clear();
  nSamples_ = 0;
  nMassPoints_ = 0;
  currentMassPoint_ = 999;
  fitVars_.clear();
  vars_.clear();
  fancyNames_.clear();
  mins_.clear(); 
  maxs_.clear();
  bins_.clear();
  binNames_.clear();
  hmin_.clear();
  hmax_.clear();
  unbinned_.clear(); 
  smoothOrder_.clear();
  
  minitree_.clear();
  baseDir_.clear();

  signalShapesToCompare_.clear();
  signalShapesToCompareHH_.clear();
  signalShapesToCompareWH_.clear();

  signalNormFactor_.clear();

  // Reset identifier string
  identifier_ = "";

  // Get ParameterSet from cfg file
  edm::ParameterSet mFitPars;

  if     (whatToDo_=="tb")            mFitPars  = edm::readPSetsFrom(parSet_)->getParameter<edm::ParameterSet>("LandSShapesProducerParSetTb");
  else if(whatToDo_=="taunu")         mFitPars  = edm::readPSetsFrom(parSet_)->getParameter<edm::ParameterSet>("LandSShapesProducerParSetTaunu");
  else if(whatToDo_=="mhmax")         mFitPars  = edm::readPSetsFrom(parSet_)->getParameter<edm::ParameterSet>("LandSShapesProducerParSetMhmax");
  else if(whatToDo_=="mhmodp_tanb5")  mFitPars  = edm::readPSetsFrom(parSet_)->getParameter<edm::ParameterSet>("LandSShapesProducerParSetMhmodp_tanb5");
  else if(whatToDo_=="mhmodp_tanb30") mFitPars  = edm::readPSetsFrom(parSet_)->getParameter<edm::ParameterSet>("LandSShapesProducerParSetMhmodp_tanb30");
  else if(whatToDo_=="plot")          mFitPars  = edm::readPSetsFrom(parSet_)->getParameter<edm::ParameterSet>("LandSShapesProducerParSetPlot");
  else { cout << "BREAK: I don't know what to do" << endl; return;}
  
  
  outFolder_        = outputPrefix_ + mFitPars.getParameter<std::string>("outFolder");
  string cmd = "mkdir -p "; cmd+=outFolder_;
  gSystem->Exec(cmd.c_str());


  outputFileName_  = mFitPars.getParameter<std::string>("outputFileName");
  datacardsBaseName_ = mFitPars.getParameter<std::string>("datacardsBaseName");

  massPointName_  = mFitPars.getParameter<vector<std::string> >("massPointName");
  nMassPoints_ = massPointName_.size();

  signalNormFactor_ = mFitPars.getParameter<vector<double> >("signalNormFactor");

  for(size_t si=0; si<signalNormFactor_.size(); ++si )
    cout << "Signal normalization factor " << si << " is " << signalNormFactor_[si] << endl;


  vector<string>baseDirTemp = mFitPars.getParameter<vector<std::string> >("baseDir");
  for(size_t f=0; f<baseDirTemp.size(); f++)
    baseDir_.push_back( baseDirTemp[f] );

 
  vector<string>inputFileNameTemp = mFitPars.getParameter<vector<std::string> >("inputFileName");
  for(size_t f=0; f<inputFileNameTemp.size(); f++)
    inputFileName_.push_back( inputFileNameTemp[f] );
  
  sampleName_      = mFitPars.getParameter<vector<std::string> >("sampleName");
  
  fancySampleName_      = mFitPars.getParameter<vector<std::string> >("fancySampleName");
  
  sampleColour_     = mFitPars.getParameter<vector<Int_t> >("sampleColour");
  sampleFillStyle_ = mFitPars.getParameter<vector<Int_t> >("sampleFillStyle");
  
  doMultiDimensionalShapes_       = mFitPars.getParameter<bool>("doMultiDimensionalShapes"); 

  unsplitUncertainties_       = mFitPars.getParameter<bool>("unsplitUncertainties"); // The finnish guys for some reason stack all the variations simultaneously.

  vector<string>minitreeTemp = mFitPars.getParameter<vector<std::string> >("minitree");
  for(size_t f=0; f<minitreeTemp.size(); f++)
    minitree_.push_back( minitreeTemp[f] );
  
  isFromData_       = mFitPars.getParameter<vector<Int_t> >("isFromData"); // Protect data and DD-bkg from syst
  isDDbkg_       = mFitPars.getParameter<vector<Int_t> >("isDDbkg"); // DD
  isSignal_       = mFitPars.getParameter<vector<Int_t> >("isSignal"); // DD

  vector<string>systComponentsTemp = mFitPars.getParameter<vector<std::string> >("systComponents");
  for(size_t f=0; f<systComponentsTemp.size(); f++)
    systComponents_.push_back( systComponentsTemp[f] );
  
  vector<string>systFancyComponentsTemp = mFitPars.getParameter<vector<std::string> >("systFancyComponents");
  for(size_t f=0; f<systFancyComponentsTemp.size(); f++)
    systFancyComponents_.push_back( systFancyComponentsTemp[f] );
  
  
  osCutEff_    = mFitPars.getParameter<double>("osCutEff");
  
  vars_        = mFitPars.getParameter<vector<string> >("vars");
  fancyNames_  = mFitPars.getParameter<vector<string> >("fancyName");
  mins_        = mFitPars.getParameter<vector<double> >("mins");
  maxs_        = mFitPars.getParameter<vector<double> >("maxs");
  bins_        = mFitPars.getParameter<vector<int> >("bins");
  binNames_    = mFitPars.getParameter<vector<string> >("binNames"),
  hmin_        = mFitPars.getParameter<vector<double> >("hmin");
  hmax_        = mFitPars.getParameter<vector<double> >("hmax");
  unbinned_    = mFitPars.getParameter<vector<Int_t> >("unbinned");
  smoothOrder_ = mFitPars.getParameter<vector<Int_t> >("smoothOrder");
  bool toNorm  = mFitPars.getParameter<bool>("toNorm");     
  doLogy_      = mFitPars.getParameter<vector<Int_t> >("doLogy");
  
  // Open files and get trees
  // ddBkg is the only to be taken from data driven estimation (tree)
  nSamples_ = inputFileName_.size();
  nSysts_     = systComponents_.size();
  
  // Set variables
  nVars_ = vars_.size();
  
  
  for(size_t i=0; i<nVars_; i++){
    fitVars_.push_back( new FitVar(vars_[i], mins_[i], maxs_[i], bins_[i], hmin_[i], hmax_[i], unbinned_[i], smoothOrder_[i], toNorm, doLogy_[i]));
    fitVars_[i]->setFancyName(fancyNames_[i]); // Temporary. Must put that in constructor in the whole package
    fitVars_[i]->setBinNames(binNames_);
    cout << "\t Acquired parameters for variable: " << fitVars_[i]->getVarName() << endl;
  }
  
  // Set canvas
  SetTDRStyle();
  gStyle->SetPadTopMargin   (0.06);
  gStyle->SetPadBottomMargin(0.20);
  gStyle->SetPadRightMargin (0.16);
  gStyle->SetPadLeftMargin  (0.14);
  gStyle->SetLabelSize(0.07, "XYZ");

  canvas_ = new TCanvas("canvas","My plots ",0,0,1000,1000);
  canvas_->cd();



  // set logy
  
  resultsFile_.open ((outFolder_+resultsFileName_).c_str());
  //  Uncomment following line in order to redirect stdout to file
  //  streamToFile_ = std::cout.rdbuf(resultsFile_.rdbuf());
  
  cout << "Init process complete" << endl;
}


void LandSShapesProducer::InitMassPoint(size_t s){

  cout << "Initializing mass point " << massPointName_[s] << endl;

  th2_         .clear();
  th2StatUp_   .clear();
  th2StatDown_ .clear();
  th2Syst_     .clear();

  unrolled_         .clear();
  unrolledStatUp_   .clear();
  unrolledStatDown_ .clear();
  unrolledSyst_     .clear();
  
  
  currentMassPoint_ = s;
  inputFile_.clear();
  inputTree_.clear();
  
  systTree_.clear();

  masterHist_.clear();
  masterHistNames_.clear();
  masterShapes_.clear();

  perMassPointSignalShapesToCompareHH_.clear();
  perMassPointSignalShapesToCompareWH_.clear();
  perMassPointSignalShapesToCompare_  .clear();

  // Get files
  for(size_t f=0; f<nSamples_; f++){
    TString myTempFileName = inputFileName_[f];
    if( isSignal_[f] ) myTempFileName = inputFileName_[f] + massPointName_[s] + TString(".root"); // Signals name manipulation // TBH
    inputFile_.push_back( TFile::Open(baseDir_[f]   + myTempFileName  ) );
  }
  
  // Get base trees
  for(size_t f=0; f<nSamples_; f++)
    inputTree_.push_back( (TTree*) inputFile_[f]     ->Get(minitree_[f]) );
  
  // Get syst trees
  for(size_t a=0; a<nSysts_; a++){
    vector<TTree*> temp;
    temp.clear();
    for(size_t f=0; f<nSamples_; f++){
      if(isFromData_[f]) temp.push_back( (TTree*) inputFile_[f] ->Get(minitree_[f]) ); // dummy fill with base minitree. Will not be used, just needed for vectors size
      else temp.push_back( (TTree*) inputFile_[f]     ->Get(minitree_[f] + systComponents_[a]) );
    }
    systTree_.push_back(temp);
  }
  
  cout << "Mass points set" << endl;
}

void LandSShapesProducer::InitPerVariableAmbient(size_t i){
  
  // Base trees
  for(size_t f=0; f<nSamples_; f++) inputTree_[f] ->ResetBranchAddresses()  ;

  // Syst trees
  for(size_t a=0; a<nSysts_; a++)
    for(size_t f=0; f<nSamples_; f++)
      systTree_[a][f]->ResetBranchAddresses();
  
  histo_.clear();
  
  myDSName_.clear();
  myDS_.clear();
  
  for(size_t a=0; a<nSysts_; a++)
    mySystDSName_.clear();
  
  myDS_.clear();

  mySystDS_.clear();

  hist_.clear();
  histStatNoNorm_.clear();
  histStatUp_.clear();
  histStatDown_.clear();
  systHist_.clear();
  
  histo_.clear();
  systHisto_.clear();

  leg_            = 0;
  
  
  // Binned fit variable
  myvar_           = new RooRealVar(fitVars_[i]->getVarName().c_str(), fitVars_[i]->getVarName().c_str(), fitVars_[i]->getMin(), fitVars_[i]->getMax());  myvar_->setBins(fitVars_[i]->getBins()); 
  myvar_weights_   = new RooRealVar("weight","weight",0,1000);
  isOSvar_         = new RooRealVar("is_os","is_os",0,2);
  
  // In multidimensional case, add the next variable too
  if(doMultiDimensionalShapes_){
    size_t nextVar;
    if(i <nVars_-1)
      nextVar = i+1;
    else if(i== nVars_-1)
      nextVar = i-1;
    myvarMultiD_ = new RooRealVar(fitVars_[nextVar]->getVarName().c_str(), fitVars_[nextVar]->getVarName().c_str(), fitVars_[nextVar]->getMin(), fitVars_[nextVar]->getMax());
    myvarMultiD_->setBins(fitVars_[nextVar]->getBins());
  }
  
  
  //Define data sets /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  for(size_t f=0; f<nSamples_; f++){
    myDSName_.push_back( fitVars_[i]->getVarName().c_str() + sampleName_[f] + massPointName_[currentMassPoint_] );
  }

  // Define syst data sets
  for(size_t a=0; a<nSysts_; a++){
    vector<string> temp;
    temp.clear();
    for(size_t f=0; f<nSamples_; f++){
      temp.push_back( myDSName_[f] + string(systComponents_[a]) );
    }
    mySystDSName_.push_back(temp);
  }
  
  sumWeights_ = 0;
  cout << "Per-variable ambient initialized" << endl;
}

void LandSShapesProducer::BuildDatasets(size_t i){

  
  // Temp variables for setting branch addresses
  double myVarAllocator, myVarWeightAllocator;
  double isOSsig;

  // In multidimensional case, add the next variable too
  if(doMultiDimensionalShapes_){
    size_t nextVar;
    if(i <nVars_-1){
      nextVar = i+1;
      
      double myNextVarAllocator;
      
      for(size_t f=0; f<nSamples_; f++){
	
	th2_.push_back(new TH2D(massPointName_[currentMassPoint_] + sampleName_[f] + fitVars_[i]->getVarName().c_str() + TString("th2") , fitVars_[i]->getVarName().c_str() + TString("th2"), fitVars_[i]->getBins(), fitVars_[i]->getMin(), fitVars_[i]->getMax(), fitVars_[nextVar]->getBins(), fitVars_[nextVar]->getMin(), fitVars_[nextVar]->getMax())    );
	th2StatUp_.push_back(new TH2D(massPointName_[currentMassPoint_] + sampleName_[f] + fitVars_[i]->getVarName().c_str() + TString("th2StatUp") , fitVars_[i]->getVarName().c_str() + TString("th2StatUp"), fitVars_[i]->getBins(), fitVars_[i]->getMin(), fitVars_[i]->getMax(), fitVars_[nextVar]->getBins(), fitVars_[nextVar]->getMin(), fitVars_[nextVar]->getMax())    );
	th2StatDown_.push_back(new TH2D(massPointName_[currentMassPoint_] + sampleName_[f] + fitVars_[i]->getVarName().c_str() + TString("th2StatDown") , fitVars_[i]->getVarName().c_str() + TString("th2StatDown"), fitVars_[i]->getBins(), fitVars_[i]->getMin(), fitVars_[i]->getMax(), fitVars_[nextVar]->getBins(), fitVars_[nextVar]->getMin(), fitVars_[nextVar]->getMax())    );
	th2_[f]->Sumw2();
	th2StatUp_[f]->Sumw2();
	th2StatDown_[f]->Sumw2();
	
	myDS_.push_back( new RooDataSet(myDSName_[f].c_str(),myDSName_[f].c_str(), RooArgSet(*myvar_,*myvarMultiD_,*myvar_weights_), "weight") ); // This constructor does not accept the cut parameter
	// Get events
	inputTree_[f]->SetBranchAddress(fitVars_[i]->getVarName().c_str(), &myVarAllocator);
	inputTree_[f]->SetBranchAddress(fitVars_[nextVar]->getVarName().c_str(), &myNextVarAllocator);
	inputTree_[f]->SetBranchAddress("weight", &myVarWeightAllocator);
	inputTree_[f]->SetBranchAddress("is_os", &isOSsig);
	//    cout << "getIsoS      ";
	for(unsigned int ev=0; ev<inputTree_[f]->GetEntries(); ev++){
	  inputTree_[f]->GetEntry(ev);
	  if(isOSsig < 0.5 || myVarAllocator < fitVars_[i]->getMin()) continue;
	  if(nextVar== i-1 ) continue;
	  myvar_->setVal(myVarAllocator);
	  myvarMultiD_->setVal(myNextVarAllocator);
	  sumWeights_ += myVarWeightAllocator;
	  if(isDDbkg_[f]){
	    myvar_weights_->setVal(osCutEff_*myVarWeightAllocator);
	    myDS_[f]->add(RooArgSet(*myvar_,*myvar_weights_),osCutEff_*myVarWeightAllocator);
	  }
	  else{
	    myvar_weights_->setVal(myVarWeightAllocator);
	    myDS_[f]->add(RooArgSet(*myvar_,*myvar_weights_),myVarWeightAllocator);
	  }
	  
	  th2_[f]->Fill(myVarAllocator, myNextVarAllocator, myVarWeightAllocator);
	}
      }
      
      ///// Syst get
      for(size_t a=0; a<nSysts_; a++){ // Loop on syst components
	
	vector<TH2D*> tempTh2;
	tempTh2.clear();
	
	vector<RooDataSet*> temp;
	temp.clear();
	for(size_t f=0; f<nSamples_; f++){
	  tempTh2.push_back(new TH2D(massPointName_[currentMassPoint_] + sampleName_[f] + fitVars_[i]->getVarName().c_str() + systComponents_[a] + TString("th2") , fitVars_[i]->getVarName().c_str() + systComponents_[a] + TString("th2"), fitVars_[i]->getBins(), fitVars_[i]->getMin(), fitVars_[i]->getMax(), fitVars_[nextVar]->getBins(), fitVars_[nextVar]->getMin(), fitVars_[nextVar]->getMax())    );
	  tempTh2[f]->Sumw2();
	  temp.push_back( new RooDataSet(mySystDSName_[a][f].c_str(),mySystDSName_[a][f].c_str(), RooArgSet(*myvar_,*myvarMultiD_,*myvar_weights_), "weight") ); // This constructor does not accept the cut parameter
	}
	mySystDS_.push_back(temp);
	th2Syst_.push_back(tempTh2);
	for(size_t f=0; f<nSamples_; f++){
	  // Get MCBkg events
	  systTree_[a][f]->SetBranchAddress(fitVars_[i]->getVarName().c_str(), &myVarAllocator);
	  systTree_[a][f]->SetBranchAddress(fitVars_[nextVar]->getVarName().c_str(), &myNextVarAllocator);
	  systTree_[a][f]->SetBranchAddress("weight", &myVarWeightAllocator);
	  systTree_[a][f]->SetBranchAddress("is_os", &isOSsig);
	  for(unsigned int ev=0; ev<systTree_[a][f]->GetEntries(); ev++){
	    systTree_[a][f]->GetEntry(ev);
	    if(isOSsig < 0.5 || myVarAllocator < fitVars_[i]->getMin()) continue;
	    if(nextVar== i-1 ) continue;
	    myvar_->setVal(myVarAllocator);
	    myvarMultiD_->setVal(myNextVarAllocator);
	    sumWeights_ += myVarWeightAllocator;
	    if(isDDbkg_[f]){
	      myvar_weights_->setVal(osCutEff_*myVarWeightAllocator);
	      mySystDS_[a][f]->add(RooArgSet(*myvar_,*myvar_weights_),osCutEff_*myVarWeightAllocator);
	    }
	    else{
	      myvar_weights_->setVal(myVarWeightAllocator);
	      mySystDS_[a][f]->add(RooArgSet(*myvar_,*myvar_weights_),myVarWeightAllocator);
	    }
	    th2Syst_[a][f]->Fill(myVarAllocator, myNextVarAllocator, myVarWeightAllocator);
	  }
	}
	
      } // End loop on syst components
      ///// End syst get
    }
    //    cout << "Done multiD stuff" << endl;
  }
  else {
  
    for(size_t f=0; f<nSamples_; f++){
      myDS_.push_back( new RooDataSet(myDSName_[f].c_str(),myDSName_[f].c_str(), RooArgSet(*myvar_,*myvar_weights_), "weight") ); // This constructor does not accept the cut parameter
      // Get events
      inputTree_[f]->SetBranchAddress(fitVars_[i]->getVarName().c_str(), &myVarAllocator);
      inputTree_[f]->SetBranchAddress("weight", &myVarWeightAllocator);
      inputTree_[f]->SetBranchAddress("is_os", &isOSsig);
      for(unsigned int ev=0; ev<inputTree_[f]->GetEntries(); ev++){
	inputTree_[f]->GetEntry(ev);
	if(( !isDDbkg_[f] && isOSsig < 0.5) || myVarAllocator < fitVars_[i]->getMin()) continue; // Don't check isOS for non-datadriven samples
	myvar_->setVal(myVarAllocator);
	sumWeights_ += myVarWeightAllocator;
	if(isDDbkg_[f]){
	  myvar_weights_->setVal(osCutEff_*myVarWeightAllocator);
	  myDS_[f]->add(RooArgSet(*myvar_,*myvar_weights_),osCutEff_*myVarWeightAllocator);
	}
	else{
	  myvar_weights_->setVal(myVarWeightAllocator);
	  myDS_[f]->add(RooArgSet(*myvar_,*myvar_weights_),myVarWeightAllocator);
	}
      }
    }
    
    ///// Syst get
    for(size_t a=0; a<nSysts_; a++){ // Loop on syst components
      vector<RooDataSet*> temp;
      temp.clear();
      for(size_t f=0; f<nSamples_; f++){
	temp.push_back( new RooDataSet(mySystDSName_[a][f].c_str(),mySystDSName_[a][f].c_str(), RooArgSet(*myvar_,*myvar_weights_), "weight") ); // This constructor does not accept the cut parameter
      }
      mySystDS_.push_back(temp);
      for(size_t f=0; f<nSamples_; f++){
	// Get MCBkg events
	systTree_[a][f]->SetBranchAddress(fitVars_[i]->getVarName().c_str(), &myVarAllocator);
	systTree_[a][f]->SetBranchAddress("weight", &myVarWeightAllocator);
	systTree_[a][f]->SetBranchAddress("is_os", &isOSsig);
	for(unsigned int ev=0; ev<systTree_[a][f]->GetEntries(); ev++){
	  systTree_[a][f]->GetEntry(ev);
	  if(isOSsig < 0.5 || myVarAllocator < fitVars_[i]->getMin()) continue;
	  myvar_->setVal(myVarAllocator);
	  sumWeights_ += myVarWeightAllocator;
	  if(isDDbkg_[f]){
	    myvar_weights_->setVal(osCutEff_*myVarWeightAllocator);
	    mySystDS_[a][f]->add(RooArgSet(*myvar_,*myvar_weights_),osCutEff_*myVarWeightAllocator);
	  }
	  else{
	    myvar_weights_->setVal(myVarWeightAllocator);
	    mySystDS_[a][f]->add(RooArgSet(*myvar_,*myvar_weights_),myVarWeightAllocator);
	  }
	}
      }
      
    } // End loop on syst components
    ///// End syst get
    
    cout << "All datasets built" << endl;
  
    // // Legacy: seems not to give correct inputs
    //  string myOsCut = "is_os>0.5";
    //  unrMyDDBkgDS_      = new RooDataSet(myDDBkgDSName_.c_str(), myDDBkgDSName_.c_str(),  ddBkgTree_,  RooArgSet(*myvar_,*myvar_weights_,*isOSvar_),myOsCut.c_str(),"weight" );
    //  myDDBkgDS_ = (RooDataSet*) unrMyDDBkgDS_->reduce(RooArgSet(*myvar_,*myvar_weights_));
    //  if(standaloneTTbar_){
    //    unrMyTTBARMCBkgDS_ = new RooDataSet(myTTBARMCBkgDSName_.c_str(), myTTBARMCBkgDSName_.c_str(),  ttbarmcBkgTree_,  RooArgSet(*myvar_,*myvar_weights_,*isOSvar_),myOsCut.c_str(),"weight" );
    //    myTTBARMCBkgDS_ = (RooDataSet*) unrMyTTBARMCBkgDS_->reduce(RooArgSet(*myvar_,*myvar_weights_));
    //  }
    //  unrMyMCBkgDS_      = new RooDataSet(myMCBkgDSName_.c_str(), myMCBkgDSName_.c_str(),  mcBkgTree_,  RooArgSet(*myvar_,*myvar_weights_,*isOSvar_),myOsCut.c_str(),"weight" );
    //  myMCBkgDS_ = (RooDataSet*) unrMyMCBkgDS_->reduce(RooArgSet(*myvar_,*myvar_weights_));
    //  unrMyDataDS_       = new RooDataSet(myDataDSName_.c_str(),  myDataDSName_.c_str(),   dataTree_,   RooArgSet(*myvar_,*isOSvar_), myOsCut.c_str() );
    //  myDataDS_ = (RooDataSet*) unrMyDataDS_->reduce(RooArgSet(*myvar_,*myvar_weights_));
    //  
    
    // Build binned clones
    for(size_t f=0; f<nSamples_; f++) histo_.push_back( myDS_[f]->binnedClone());
    
    for(size_t f=0; f<nSamples_; f++)
      cout << "DEBUG: \n" << "Dataset " << myDSName_[f] << " has " << myDS_[f]->numEntries() << " unbinned entries and " << histo_[f]->sum(kFALSE) << " weighted entries." << endl;
    
    cout << "BuildDatasets successful" << endl;
    
    
    
    /// Build binned clones - syst case
    for(size_t a=0; a<nSysts_; a++){ // Loop on syst components
      vector<RooDataHist*> temp;
      temp.clear();
      for(size_t f=0; f<nSamples_; f++){
	temp.push_back( mySystDS_[a][f]->binnedClone() );
      }
      systHisto_.push_back(temp);
    
    } // End syst loop
    /// End build binned clones - syst case
  }  
  cout << "BuildDatasets successful" << endl;
}

//void LandSShapesProducer::BuildPDFs(size_t i){
//  
//  switch(fitVars_[i]->getUnbinned()){
//  case 1 : // Unbinned
//    if(includeSignal_) u_signalModel_ = new RooKeysPdf(signalModelName_.c_str(), signalModelName_.c_str(), *myvar_, *mySignalDS_, RooKeysPdf::MirrorBoth,2);
//    u_ddbkgModel_  = new RooKeysPdf(ddbkgModelName_.c_str(),  ddbkgModelName_.c_str(),  *myvar_, *myDDBkgDS_ , RooKeysPdf::MirrorBoth,2);
//    if(standaloneTTbar_) u_ttbarmcbkgModel_  = new RooKeysPdf(ttbarmcbkgModelName_.c_str(),  ttbarmcbkgModelName_.c_str(),  *myvar_, *myTTBARMCBkgDS_ , RooKeysPdf::MirrorBoth,2);
//    u_mcbkgModel_  = new RooKeysPdf(mcbkgModelName_.c_str(),  mcbkgModelName_.c_str(),  *myvar_, *myMCBkgDS_ , RooKeysPdf::MirrorBoth,2);
//    break;
//  case 0:  // Binned (w/ or w/out smoothing)
//    if(includeSignal_) b_signalModel_ = new RooHistPdf(signalModelName_.c_str(), signalModelName_.c_str(), *myvar_, *signalHisto_, fitVars_[i]->getSmoothOrder());
//    b_ddbkgModel_  = new RooHistPdf(ddbkgModelName_.c_str(),  ddbkgModelName_.c_str(), *myvar_, *ddbkgHisto_ , fitVars_[i]->getSmoothOrder());
//    if(standaloneTTbar_) b_ttbarmcbkgModel_ = new RooHistPdf(ttbarmcbkgModelName_.c_str(),  ttbarmcbkgModelName_.c_str(),  *myvar_, *ttbarmcbkgHisto_  , fitVars_[i]->getSmoothOrder()); 
//    b_mcbkgModel_  = new RooHistPdf(mcbkgModelName_.c_str(),  mcbkgModelName_.c_str(),  *myvar_, *mcbkgHisto_  , fitVars_[i]->getSmoothOrder()); 
//    break;
//  default : // Dummy - should never arrive here
//    cout<<"Neither binned not unbinned. Check your options motherfucker."<<endl;
//  }
//}


void LandSShapesProducer::DrawTemplates(size_t i){
  
  // Open file for storing shapes
  string outputFileName = outputFileName_ + string("_") + massPointName_[currentMassPoint_] + string("_") + fitVars_[i]->getVarName();
  TFile* outputFile(0);
  if(produceOnly_ && !doMultiDimensionalShapes_)
    outputFile = new TFile((outFolder_+outputFileName+string(".root")).c_str(), "RECREATE");
  
  // Reset canvas
  canvas_->cd();
  canvas_->Clear();
  


  //  /////////////////////////////////////////////////////////////////////
  
  // mc bkg histogram ////////////////////////////////////////////////
  for(size_t f=0; f<nSamples_; f++){
    hist_.push_back( histo_[f]->createHistogram(fitVars_[i]->getVarName().c_str(),fitVars_[i]->getBins() ) );
    hist_[f]->Sumw2();
    if(binNames_.size())
      for(int ibin=1; ibin<=hist_[f]->GetNbinsX(); ++ibin) // Only when defined
	hist_[f]->GetXaxis()->SetBinLabel(ibin, (fitVars_[i]->getBinName(ibin)).c_str() );
    if(isSignal_[f]){
      hist_[f]->SetOption("0000"); // What did that do, again? LoL
      hist_[f]->SetLineColor(sampleColour_[f]);
    }
    else
      hist_[f]->SetFillColor(sampleColour_[f]);
    hist_[f]->SetLineWidth(3);
    hist_[f]->SetFillStyle(sampleFillStyle_[f]); //1001 for background and data, 0 for signal // 3017);
 

    if(sampleName_[f] == "tt_ll")   
      hist_[f]->Scale(94./hist_[f]->Integral());
    else if(sampleName_[f] == "tt_ltau" )
      hist_[f]->Scale(2839./hist_[f]->Integral());
    
    if(isDDbkg_[f]){
      hist_[f]->Scale(1544./hist_[f]->Integral());
      ddbkgHistUp_ =   (TH1*) hist_[f]->Clone(hist_[f]->GetName() + TString("Up") );
      ddbkgHistDown_ = (TH1*) hist_[f]->Clone(hist_[f]->GetName() + TString("Down") );
      
      ddbkgHistUp_->SetFillColor(sampleColour_[f]);
      ddbkgHistUp_->SetLineWidth(3);
      ddbkgHistUp_->SetFillStyle(sampleFillStyle_[f]);//3017);
      
      ddbkgHistDown_->SetFillColor(sampleColour_[f]);
      ddbkgHistDown_->SetLineWidth(3);
      ddbkgHistDown_->SetFillStyle(sampleFillStyle_[f]);//3017);
    }
  }
  //    ///////////////////////////////////////////////////////////////////
  
  
  // FIXME: legacy from 2011A paper is clone. Must switch to full fetch from allmc.root
  hist_.push_back( (TH1*)hist_[nSamples_-1]->Clone( sampleName_[nSamples_].c_str())   ); // nSamples-1 Must always be di-bosons
  hist_.push_back( (TH1*)hist_[nSamples_-1]->Clone( sampleName_[nSamples_+1].c_str()) ); // nSamples-1 Must always be di-bosons
  hist_[nSamples_]  ->Sumw2();
  hist_[nSamples_+1]->Sumw2();
  
  hist_[nSamples_]->Scale(150./hist_[nSamples_]->Integral());      // Normalize to sample  Ztautau  
  hist_[nSamples_+1]->Scale(12/hist_[nSamples_+1]->Integral()); // Normalize to sample    Zee,mumu

  ddbkgHistUp_ =   (TH1*) hist_[3]->Clone(hist_[3]->GetName() + TString("Up") );
  ddbkgHistDown_ = (TH1*) hist_[3]->Clone(hist_[3]->GetName() + TString("Down") );

  
  
  for(size_t f=0; f<nSamples_+2; f++){
    if(f>nSamples_-1){ // Repeat colours for newly cloned histos
      if(isSignal_[f]){
	hist_[f]->SetOption("0000"); // What did that do, again? LoL
	hist_[f]->SetLineColor(sampleColour_[f]);
      }
      else
	hist_[f]->SetFillColor(sampleColour_[f]);
      hist_[f]->SetLineWidth(3);
      hist_[f]->SetFillStyle(sampleFillStyle_[f]); //1001 for background and data, 0 for signal // 3017);
    }
    
    histStatUp_  .push_back( (TH1*)hist_[f]->Clone(hist_[f]->GetName() + TString("_StatUp")));
    histStatDown_.push_back( (TH1*)hist_[f]->Clone(hist_[f]->GetName() + TString("_StatDown")));
    histStatUp_[f]->Sumw2();
    histStatDown_[f]->Sumw2();
  }
  
  
  /// Produce plots - syst case
  for(size_t a=0; a<nSysts_; a++){ // Loop on syst components
    
    // mc bkg histogram ////////////////////////////////////////////////
    vector<TH1*> temp;
    temp.clear();
    for(size_t f=0; f<nSamples_; f++){
      temp.push_back( systHisto_[a][f]->createHistogram(fitVars_[i]->getVarName().c_str(),fitVars_[i]->getBins() ) );
    }
    systHist_.push_back(temp);
    
    for(size_t f=0; f<nSamples_; f++){
      systHist_[a][f]->Sumw2();
      systHist_[a][f]->SetFillColor(sampleColour_[f]);
      systHist_[a][f]->SetFillStyle(sampleFillStyle_[f]);
      if(sampleName_[f] == "tt_ltau" || sampleName_[f] == "tt_ll")
	if( systComponents_[a] == "_topptuncplus" || systComponents_[a] == "_topptuncminus") 
	  systHist_[a][f]->Scale(hist_[f]->Integral()/systHist_[a][f]->Integral());
      
      if(isDDbkg_[f]){    
	if(a == 0 || a == 2 || a == 4)
	  systHist_[a][f]->Scale((222+11.4/5)/systHist_[a][f]->Integral());
	else
	  systHist_[a][f]->Scale((222-11.4/5)/systHist_[a][f]->Integral());
      }
    }
    ///////////////////////////////////////////////////////////////////
    
    systHist_[a].push_back( (TH1*)systHist_[a][nSamples_-1]->Clone(  sampleName_[nSamples_].c_str()) );
    systHist_[a].push_back( (TH1*)systHist_[a][nSamples_-1]->Clone(  sampleName_[nSamples_+1].c_str()) );
    systHist_[a][nSamples_]  ->Sumw2();
    systHist_[a][nSamples_+1]->Sumw2();
    
    systHist_[a][nSamples_]  ->Scale( (150./systHist_[a][nSamples_]->Integral()  )*( systHist_[a][nSamples_-1]->Integral() / hist_[nSamples_-1]->Integral()  )    );
    systHist_[a][nSamples_+1]->Scale( (12/systHist_[a][nSamples_+1]->Integral() )*( systHist_[a][nSamples_-1]->Integral() / hist_[nSamples_-1]->Integral()  )  );
    
    for(size_t f=0; f<nSamples_+2; f++){
      if(f>nSamples_-1){ // Repeat colours for newly cloned histos
	systHist_[a][f]->Sumw2();
	if(isSignal_[f]){
	  systHist_[a][f]->SetOption("0000"); // What did that do, again? LoL
	  systHist_[a][f]->SetLineColor(sampleColour_[f]);
	}
	else
	  systHist_[a][f]->SetFillColor(sampleColour_[f]);
	systHist_[a][f]->SetLineWidth(3);
	systHist_[a][f]->SetFillStyle(sampleFillStyle_[f]); //1001 for background and data, 0 for signal // 3017);
      }
    }
  } // End loop on systs
  
  /// End produce plots - syst case
  
  hist_[nSamples_]->Scale(150./hist_[nSamples_]->Integral());      // Normalize to sample
  hist_[nSamples_+1]->Scale(12/hist_[nSamples_+1]->Integral()); // Normalize to sample


  if(!produceOnly_){ // For now raw indexes. This will improve.
    
    // This numbering changed.
    // 0: data
    // 1: WH
    // 2: HH
    // 3: DD
    // 4: other MCs

    // Current numbering is:
    // 0: data
    // 1: HTB
    // 2: TBH
    // 3: DD
    // 4: other MCs
    
    TGraphAsymmErrors ddbkgBands;
    getErrorBands(*(hist_[3]), *ddbkgHistUp_, *ddbkgHistDown_, ddbkgBands);

    ddbkgBands.SetFillColor(1);
    ddbkgBands.SetFillStyle(3004);
    ddbkgBands.GetYaxis()->SetTitle("Events/bin");
    ddbkgBands.GetYaxis()->SetTitleOffset(0.85);
    ddbkgBands.GetXaxis()->SetTitle((fitVars_[i]->getFancyName()).c_str());
    ddbkgBands.GetXaxis()->SetTitleOffset(0.85);
    ddbkgBands.GetXaxis()->SetTitleSize(5);
    ddbkgBands.GetXaxis()->SetRange(displayMin_,displayMax_);    
    ddbkgBands.GetXaxis()->SetRangeUser(displayMin_,displayMax_);    


    TLegend* leg2_ = new TLegend(0.23,0.65,0.62,0.80,NULL,"brNDC");
    leg2_->SetTextFont(62);
    leg2_->SetBorderSize(0);
    leg2_->SetLineColor(1);
    leg2_->SetLineStyle(1);
    leg2_->SetLineWidth(1);
    leg2_->SetFillColor(0);
    leg2_->SetFillStyle(0);
    leg2_->AddEntry(hist_[1],"Base","l");
    leg2_->AddEntry(systHist_[0][1],"JESup","l");
    leg2_->AddEntry(systHist_[1][1],"JESdown","l");
    leg2_->AddEntry(&ddbkgBands, "stat+method","f");

    leg_ = new TLegend(0.23,0.65,0.62,0.80,NULL,"brNDC");
    leg_->SetTextFont(62);
    leg_->SetBorderSize(0);
    leg_->SetLineColor(1);
    leg_->SetLineStyle(1);
    leg_->SetLineWidth(1);
    leg_->SetFillColor(0);
    leg_->SetFillStyle(0);
    leg_->AddEntry(hist_[1],"Base","l");
    leg_->AddEntry(systHist_[0][1],"JESup","l");
    leg_->AddEntry(systHist_[1][1],"JESdown","l");

    hist_[1]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    hist_[1]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    systHist_[0][1]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    systHist_[0][1]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    systHist_[1][1]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    systHist_[1][1]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    

    systHist_[0][1]->GetXaxis()->SetTitle((fitVars_[i]->getFancyName()).c_str());
    hist_[1]->SetLineColor(1);
    systHist_[0][1]->SetLineColor(2);
    systHist_[1][1]->SetLineColor(3);
    systHist_[0][1]->Draw("hist");
    systHist_[1][1]->Draw("histsame");
    hist_[1]->Draw("histsame");
    leg_->Draw();
    canvas_->cd(); 
    // Order chosen to have good Y axis boundaries

    canvas_->SaveAs((outFolder_+outputFileName+string("_jes_sigVariationsWH.pdf")).c_str());
    canvas_->SaveAs((outFolder_+outputFileName+string("_jes_sigVariationsWH.png")).c_str());
    canvas_->Clear();
    hist_[1]->SetLineColor(sampleColour_[1]); // Reset line color from black to final one

    hist_[2]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    hist_[2]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    systHist_[0][2]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    systHist_[0][2]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    systHist_[1][2]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    systHist_[1][2]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    

    systHist_[0][2]->GetXaxis()->SetTitle((fitVars_[i]->getFancyName()).c_str());
    hist_[2]->SetLineColor(1);
    systHist_[0][2]->SetLineColor(2);
    systHist_[1][2]->SetLineColor(3);
    systHist_[0][2]->Draw("hist");
    systHist_[1][2]->Draw("histsame");
    hist_[2]->Draw("histsame");
    leg_->Draw();
    canvas_->SaveAs((outFolder_+outputFileName+string("_jes_sigVariationsHH.pdf")).c_str());
    canvas_->SaveAs((outFolder_+outputFileName+string("_jes_sigVariationsHH.png")).c_str());
    canvas_->Clear();
    hist_[2]->SetLineColor(sampleColour_[2]); // Reset line color from black to final one
    
    systHist_[0][3]->SetFillStyle(0);
    systHist_[1][3]->SetFillStyle(0);
    hist_[3]->SetFillStyle(0);

    hist_[3]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    hist_[3]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    systHist_[0][3]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    systHist_[0][3]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    systHist_[1][3]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    systHist_[1][3]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    

    systHist_[0][3]->GetXaxis()->SetTitle((fitVars_[i]->getFancyName()).c_str());
    hist_[3]->SetMarkerColor(1);
    systHist_[0][3]->SetLineColor(2);
    systHist_[1][3]->SetLineColor(3);
    systHist_[0][3]->Draw("hist");
    systHist_[1][3]->Draw("histsame");
    hist_[3]->Draw("same");
    ddbkgBands.Draw("2same");
    leg2_->Draw();
    canvas_->SaveAs((outFolder_+outputFileName+string("_jes_sigVariationsDD.pdf")).c_str());
    canvas_->SaveAs((outFolder_+outputFileName+string("_jes_sigVariationsDD.png")).c_str());
    canvas_->Clear();
    hist_[3]->SetLineColor(sampleColour_[3]); // Reset line color from black to final one

    systHist_[0][3]->SetFillStyle(1001);
    systHist_[1][3]->SetFillStyle(1001);
    hist_[3]->SetFillStyle(1001);
    
    for(size_t f=4; f<nSamples_+2; f++){
      
      systHist_[0][f]->SetFillStyle(0);
      systHist_[1][f]->SetFillStyle(0);
      hist_[f]->SetFillStyle(0);
      
      hist_[f]->GetXaxis()->SetRange(displayMin_,displayMax_);    
      hist_[f]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
      systHist_[0][f]->GetXaxis()->SetRange(displayMin_,displayMax_);    
      systHist_[0][f]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
      systHist_[1][f]->GetXaxis()->SetRange(displayMin_,displayMax_);    
      systHist_[1][f]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
 
      systHist_[0][f]->GetXaxis()->SetTitle((fitVars_[i]->getFancyName()).c_str());
      hist_[f]->SetLineColor(1);
      systHist_[0][f]->SetLineColor(2);
      systHist_[1][f]->SetLineColor(3);
      systHist_[0][f]->Draw("hist");
      systHist_[1][f]->Draw("histsame");
      hist_[f]->Draw("histsame");
      leg_->Draw();
      canvas_->SaveAs((outFolder_+outputFileName+string("_jes_sigVariations")+sampleName_[f].c_str()+string(".pdf")).c_str());
      canvas_->SaveAs((outFolder_+outputFileName+string("_jes_sigVariations")+sampleName_[f].c_str()+string(".png")).c_str());
      canvas_->Clear();
      
      hist_[f]->SetLineColor(sampleColour_[f]);
      systHist_[0][f]->SetFillStyle(1001);
      systHist_[1][f]->SetFillStyle(1001);
      hist_[f]->SetFillStyle(1001);

    }

    
    ////////////////////////////
    leg2_  = new TLegend(0.23,0.65,0.62,0.80,NULL,"brNDC");
    leg2_->SetTextFont(62);
    leg2_->SetBorderSize(0);
    leg2_->SetLineColor(1);
    leg2_->SetLineStyle(1);
    leg2_->SetLineWidth(1);
    leg2_->SetFillColor(0);
    leg2_->SetFillStyle(0);
    leg2_->AddEntry(hist_[1],"Base","l");
    leg2_->AddEntry(systHist_[0][1],"METup","l");
    leg2_->AddEntry(systHist_[1][1],"METdown","l");
    leg2_->AddEntry(&ddbkgBands, "stat+method","f"); // Dummy style show


    leg_ = new TLegend(0.23,0.65,0.62,0.80,NULL,"brNDC");
    leg_->SetTextFont(62);
    leg_->SetBorderSize(0);
    leg_->SetLineColor(1);
    leg_->SetLineStyle(1);
    leg_->SetLineWidth(1);
    leg_->SetFillColor(0);
    leg_->SetFillStyle(0);
    leg_->AddEntry(hist_[1],"Base","l");
    leg_->AddEntry(systHist_[2][1],"METup","l");
    leg_->AddEntry(systHist_[3][1],"METdown","l");

    hist_[1]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    hist_[1]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    systHist_[2][1]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    systHist_[2][1]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    systHist_[3][1]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    systHist_[3][1]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    

    systHist_[2][1]->GetXaxis()->SetTitle((fitVars_[i]->getFancyName()).c_str());
    hist_[1]->SetLineColor(1);
    systHist_[2][1]->SetLineColor(2);
    systHist_[3][1]->SetLineColor(3);
    systHist_[2][1]->Draw("hist");
    systHist_[3][1]->Draw("histsame");
    hist_[1]->Draw("histsame");
    leg_->Draw();
    canvas_->cd(); 
    // Order chosen to have good Y axis boundaries

    canvas_->SaveAs((outFolder_+outputFileName+string("_met_sigVariationsWH.pdf")).c_str());
    canvas_->SaveAs((outFolder_+outputFileName+string("_met_sigVariationsWH.png")).c_str());
    canvas_->Clear();
    hist_[1]->SetLineColor(sampleColour_[1]);

    hist_[2]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    hist_[2]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    systHist_[2][2]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    systHist_[2][2]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    systHist_[3][2]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    systHist_[3][2]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    

    systHist_[2][2]->GetXaxis()->SetTitle((fitVars_[i]->getFancyName()).c_str());
    hist_[2]->SetLineColor(1);
    systHist_[2][2]->SetLineColor(2);
    systHist_[3][2]->SetLineColor(3);
    systHist_[2][2]->Draw("hist");
    systHist_[3][2]->Draw("histsame");
    hist_[2]->Draw("histsame");
    leg_->Draw();
    canvas_->SaveAs((outFolder_+outputFileName+string("_met_sigVariationsHH.pdf")).c_str());
    canvas_->SaveAs((outFolder_+outputFileName+string("_met_sigVariationsHH.png")).c_str());
    canvas_->Clear();
    hist_[2]->SetLineColor(sampleColour_[2]);
    
    systHist_[2][3]->SetFillStyle(0);
    systHist_[3][3]->SetFillStyle(0);
    hist_[3]->SetFillStyle(0);

    hist_[3]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    hist_[3]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    systHist_[2][3]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    systHist_[2][3]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    systHist_[3][3]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    systHist_[3][3]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    

    systHist_[2][3]->GetXaxis()->SetTitle((fitVars_[i]->getFancyName()).c_str());
    hist_[3]->SetMarkerColor(1);
    systHist_[2][3]->SetLineColor(2);
    systHist_[3][3]->SetLineColor(3);
    systHist_[2][3]->Draw("hist");
    systHist_[3][3]->Draw("histsame");
    hist_[3]->Draw("same");
    ddbkgBands.Draw("2same");
    leg2_->Draw();
    canvas_->SaveAs((outFolder_+outputFileName+string("_met_sigVariationsDD.pdf")).c_str());
    canvas_->SaveAs((outFolder_+outputFileName+string("_met_sigVariationsDD.png")).c_str());
    canvas_->Clear();
    hist_[3]->SetLineColor(sampleColour_[3]);
    
    systHist_[2][3]->SetFillStyle(1001);
    systHist_[3][3]->SetFillStyle(1001);
    hist_[3]->SetFillStyle(1001);


    for(size_t f=4; f<nSamples_+2; f++){
      
      systHist_[2][f]->SetFillStyle(0);
      systHist_[3][f]->SetFillStyle(0);
      hist_[f]->SetFillStyle(0);

      hist_[f]->GetXaxis()->SetRange(displayMin_,displayMax_);    
      hist_[f]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
      systHist_[2][f]->GetXaxis()->SetRange(displayMin_,displayMax_);    
      systHist_[2][f]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
      systHist_[3][f]->GetXaxis()->SetRange(displayMin_,displayMax_);    
      systHist_[3][f]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
            
      systHist_[2][f]->GetXaxis()->SetTitle((fitVars_[i]->getFancyName()).c_str());
      hist_[f]->SetLineColor(1);
      systHist_[2][f]->SetLineColor(2);
      systHist_[3][f]->SetLineColor(3);
      systHist_[2][f]->Draw("hist");
      systHist_[3][f]->Draw("histsame");
      hist_[f]->Draw("histsame");
      leg_->Draw();
      canvas_->SaveAs((outFolder_+outputFileName+string("_met_sigVariations")+sampleName_[f].c_str()+string(".pdf")).c_str());
      canvas_->SaveAs((outFolder_+outputFileName+string("_met_sigVariations")+sampleName_[f].c_str()+string(".png")).c_str());
      canvas_->Clear();
      
      hist_[f]->SetLineColor(sampleColour_[f]);
      systHist_[2][f]->SetFillStyle(1001);
      systHist_[3][f]->SetFillStyle(1001);
      hist_[f]->SetFillStyle(1001);

    }

    /////////////////////////

    leg2_  = new TLegend(0.23,0.65,0.62,0.80,NULL,"brNDC");
    leg2_->SetTextFont(62);
    leg2_->SetBorderSize(0);
    leg2_->SetLineColor(1);
    leg2_->SetLineStyle(1);
    leg2_->SetLineWidth(1);
    leg2_->SetFillColor(0);
    leg2_->SetFillStyle(0);
    leg2_->AddEntry(hist_[1],"Base","l");
    leg2_->AddEntry(systHist_[0][1],"JERup","l");
    leg2_->AddEntry(systHist_[1][1],"JERdown","l");
    leg2_->AddEntry(&ddbkgBands, "stat+method","f");

    leg_ = new TLegend(0.23,0.65,0.62,0.80,NULL,"brNDC");
    leg_->SetTextFont(62);
    leg_->SetBorderSize(0);
    leg_->SetLineColor(1);
    leg_->SetLineStyle(1);
    leg_->SetLineWidth(1);
    leg_->SetFillColor(0);
    leg_->SetFillStyle(0);
    leg_->AddEntry(hist_[1],"Base","l");
    leg_->AddEntry(systHist_[4][1],"JERup","l");
    leg_->AddEntry(systHist_[5][1],"JERdown","l");

    hist_[1]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    hist_[1]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    systHist_[4][1]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    systHist_[4][1]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    systHist_[5][1]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    systHist_[5][1]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    
    systHist_[4][1]->GetXaxis()->SetTitle((fitVars_[i]->getFancyName()).c_str());
    hist_[1]->SetLineColor(1);
    systHist_[4][1]->SetLineColor(2);
    systHist_[5][1]->SetLineColor(3);
    systHist_[4][1]->Draw("hist");
    systHist_[5][1]->Draw("histsame");
    hist_[1]->Draw("histsame");
    leg_->Draw();
    canvas_->cd(); 
    // Order chosen to have good Y axis boundaries

    canvas_->SaveAs((outFolder_+outputFileName+string("_jer_sigVariationsWH.pdf")).c_str());
    canvas_->SaveAs((outFolder_+outputFileName+string("_jer_sigVariationsWH.png")).c_str());
    canvas_->Clear();
    hist_[1]->SetLineColor(sampleColour_[1]);
    
    hist_[2]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    hist_[2]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    systHist_[4][2]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    systHist_[4][2]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    systHist_[5][2]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    systHist_[5][2]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    

    systHist_[4][2]->GetXaxis()->SetTitle((fitVars_[i]->getFancyName()).c_str());
    hist_[2]->SetLineColor(1);
    systHist_[4][2]->SetLineColor(2);
    systHist_[5][2]->SetLineColor(3);
    systHist_[4][2]->Draw("hist");
    systHist_[5][2]->Draw("histsame");
    hist_[2]->Draw("histsame");
    leg_->Draw();
    canvas_->SaveAs((outFolder_+outputFileName+string("_jer_sigVariationsHH.pdf")).c_str());
    canvas_->SaveAs((outFolder_+outputFileName+string("_jer_sigVariationsHH.png")).c_str());
    canvas_->Clear();
    hist_[2]->SetLineColor(sampleColour_[2]);
    
    systHist_[4][3]->SetFillStyle(0);
    systHist_[5][3]->SetFillStyle(0);
    hist_[3]->SetFillStyle(0);

    hist_[3]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    hist_[3]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    systHist_[4][3]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    systHist_[4][3]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    systHist_[5][3]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    systHist_[5][3]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    
    systHist_[4][3]->GetXaxis()->SetTitle((fitVars_[i]->getFancyName()).c_str());
    hist_[3]->SetMarkerColor(1);
    systHist_[4][3]->SetLineColor(2);
    systHist_[5][3]->SetLineColor(3);
    systHist_[4][3]->Draw("hist");
    systHist_[5][3]->Draw("histsame");
    hist_[3]->Draw("same");
    ddbkgBands.Draw("2same");
    leg2_->Draw();
    canvas_->SaveAs((outFolder_+outputFileName+string("_jer_sigVariationsDD.pdf")).c_str());
    canvas_->SaveAs((outFolder_+outputFileName+string("_jer_sigVariationsDD.png")).c_str());
    canvas_->Clear();
    hist_[3]->SetLineColor(sampleColour_[3]);

    
    systHist_[4][3]->SetFillStyle(1001);
    systHist_[5][3]->SetFillStyle(1001);
    hist_[3]->SetFillStyle(1001);


    for(size_t f=4; f<nSamples_+2; f++){

      systHist_[4][f]->SetFillStyle(0);
      systHist_[5][f]->SetFillStyle(0);
      hist_[f]->SetFillStyle(0);

      hist_[f]->GetXaxis()->SetRange(displayMin_,displayMax_);    
      hist_[f]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
      systHist_[4][f]->GetXaxis()->SetRange(displayMin_,displayMax_);    
      systHist_[4][f]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
      systHist_[5][f]->GetXaxis()->SetRange(displayMin_,displayMax_);    
      systHist_[5][f]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
      
      systHist_[4][f]->GetXaxis()->SetTitle((fitVars_[i]->getFancyName()).c_str());
      hist_[f]->SetLineColor(1);
      systHist_[4][f]->SetLineColor(2);
      systHist_[5][f]->SetLineColor(3);
      systHist_[4][f]->Draw("hist");
      systHist_[5][f]->Draw("histsame");
      hist_[f]->Draw("histsame");
      leg_->Draw();
      canvas_->SaveAs((outFolder_+outputFileName+string("_jer_sigVariations")+sampleName_[f].c_str()+string(".pdf")).c_str());
      canvas_->SaveAs((outFolder_+outputFileName+string("_jer_sigVariations")+sampleName_[f].c_str()+string(".png")).c_str());
      canvas_->Clear();
      
      hist_[f]->SetLineColor(sampleColour_[f]);
      systHist_[4][f]->SetFillStyle(1001);
      systHist_[5][f]->SetFillStyle(1001);
      hist_[f]->SetFillStyle(1001);
      
    }
    
    //// END SYST

    //    canvas_->cd();
    //    TPad* p = (TPad*) canvas_->cd();
    
    //gPad->SetPad(0,0,1,1);


    // Without ratio:
    //    gPad->SetPad(0,0,1,1); 
    //    gPad->cd();
    //    gPad->Draw();
    //    canvas_->cd(); 

    // With ratio
    canvas_->Divide(1,2);
    canvas_->cd(1);
    gPad->SetBottomMargin(0.1);
    gPad->SetPad(0,0.2,1,1); 
    gPad->cd();
    gPad->Draw();


    // on top: leg_ = new TLegend(0.23,0.535,0.62,0.93,NULL,"brNDC");
    leg_ = new TLegend(0.845,0.2,0.99,0.99,NULL,"NDC"); // On the side <3
    //  leg_ = new TLegend(0.7147651,0.6346154,0.9446309,0.9353147,NULL,"brNDC");
    //leg_ = new TLegend(0.75,0.6346154,1.,1.,NULL,"brNDC");
    
    leg_->SetTextFont(62);
    leg_->SetBorderSize(0);
    leg_->SetLineColor(1);
    leg_->SetLineStyle(1);
    leg_->SetLineWidth(1);
    leg_->SetFillColor(0);
    leg_->SetFillStyle(0);
    leg_->AddEntry(hist_[0],(fancySampleName_[0]).c_str(),"lep");
    leg_->AddEntry(hist_[1],fancySampleName_[1].c_str(),"l");
    leg_->AddEntry(hist_[2],fancySampleName_[2].c_str(),"l");
    // Because colour printer is a nobrainer
    leg_->AddEntry(hist_[3],fancySampleName_[3].c_str(),"f");
    leg_->AddEntry(hist_[8],fancySampleName_[8].c_str(),"f");
    leg_->AddEntry(hist_[7],fancySampleName_[7].c_str(),"f");
    leg_->AddEntry(hist_[6],fancySampleName_[6].c_str(),"f");
    leg_->AddEntry(hist_[5],fancySampleName_[5].c_str(),"f");
    leg_->AddEntry(hist_[4],fancySampleName_[4].c_str(),"f");

    
    
    // Order chosen to have good Y axis boundaries
   
    perMassPointSignalShapesToCompareHH_.push_back((TH1*)hist_[2]->Clone(hist_[2]->GetName() +TString("comparison") + massPointName_[currentMassPoint_].c_str()) );
    perMassPointSignalShapesToCompareWH_.push_back((TH1*)hist_[1]->Clone(hist_[1]->GetName() +TString("comparison") + massPointName_[currentMassPoint_].c_str()) );
    
    TH1* higgsH_ = 0;
    TH1* higgsH2_ = 0;
    // must update when updating light chhiggs analysis //    double cHiggsBR_ = 0.05; // Perhaps move to cfg file.
    // must update when updating light chhiggs analysis //    double fhh(cHiggsBR_*cHiggsBR_) , fhw( 2*(1-cHiggsBR_)*cHiggsBR_), ftt(1-fhh-fhw);
    //  signalHistWH_->Scale(fhw/signalHistWH_->Integral());
    //  signalHistHH_->Scale(fhh/signalHistHH_->Integral());
    
    //  signalHistWH_->Add(signalHistHH_, fhh);
    higgsH_ = hist_[1];
    higgsH2_= hist_[2];
    // FIXME: fix for heavy/light in a non-hardcoded way
    //    higgsH_->Scale(fhw/higgsH_->Integral());
    //    higgsH_->Add(hist_[2],fhh);
    
    perMassPointSignalShapesToCompare_.push_back((TH1*)higgsH_->Clone(higgsH_->GetName() + TString("totComparison") + massPointName_[currentMassPoint_].c_str() ) );    
    // Stacked drawing -------------------------------------------
    
    
    
    hist_[0]->GetYaxis()->SetTitle("Events/bin");
    hist_[0]->GetYaxis()->SetTitleOffset(0.85);
    hist_[0]->GetXaxis()->SetTitle(fitVars_[i]->getFancyName().c_str());
    hist_[0]->GetXaxis()->SetTitleOffset(0.85);
    
    hist_[1]->GetYaxis()->SetTitle("Events/bin");
    hist_[1]->GetYaxis()->SetTitleOffset(0.85);
    hist_[1]->GetXaxis()->SetTitle(fitVars_[i]->getFancyName().c_str());
    hist_[1]->GetXaxis()->SetTitleOffset(0.85);
    
    

    THStack hs("hs","stacked");
    hist_[1]->DrawNormalized("histsame");
    hist_[2]->DrawNormalized("histsame");
    for(size_t f=3; f<nSamples_+2; f++){
      // for fig7 // for(size_t f=0; f<nMcSamples_; f++){
      hist_[f]->GetYaxis()->SetTitle("Events/bin");
      hist_[f]->GetYaxis()->SetTitleOffset(0.85);
      hist_[f]->GetXaxis()->SetTitle("p_{T}^{lead.track}/E^{#tau}");
      hist_[f]->GetXaxis()->SetTitleOffset(0.85);
      hist_[f]->GetXaxis()->SetTitleSize(5);
      hist_[f]->GetXaxis()->SetRange(displayMin_,displayMax_);    
      hist_[f]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
      if(!isDDbkg_[f]) hs.Add(hist_[f],"hist");
    
      cout << "I AM HISTOGRAM " << hist_[f]->GetName() << ", WITH NORMALIZATION: " << hist_[f]->Integral() << endl;
    }
    
    for(size_t f=3; f<nSamples_+2; f++){
      if(isDDbkg_[f])
	hs.Add(hist_[f],"hist");
      }
    
    //    cout << "dd integral: " << hist_[3]->Integral() << endl;
    //  normalize rc t
    if(fitVars_[i]->getToNorm()){
      normalize(hs, 1.);
      hs.SetMaximum(0.4);
    } else{
      // do not normalize btagmulti
      hs.SetMaximum(1.2); 
      hs.SetMaximum(20000);
      
      hs.SetMinimum(0.1);
    }
    hs.Draw("hist");
    hs.GetXaxis()->SetRange(displayMin_,displayMax_);    
    hs.GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    hs.GetXaxis()->SetTitleSize(0.05);
    hs.GetXaxis()->SetLabelSize(0.05);
    hs.GetYaxis()->SetTitle("Events/bin");
    hs.GetXaxis()->SetTitle((fitVars_[i]->getFancyName()).c_str());
    //  hs.GetXaxis()->SetTitleOffset(1.5);
    

    if(fitVars_[i]->getToNorm()) hist_[0]->Scale(1./hist_[0]->Integral());     // normalize to 1 rc_t
    // This is data, bitch! else                         hist_[0]->Scale( signalNormFactor_[currentMassPoint_] );  // normalize to cross section btagmulti
    
    hist_[0]->GetXaxis()->SetRange(displayMin_,displayMax_);    
    hist_[0]->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    


  
    hist_[0]->Draw("same");
    // do not normalize btagmulti 
    if(fitVars_[i]->getToNorm()){
      higgsH_->Scale(1./higgsH_->Integral());    /// ??? was signalHistWH_->Integral()); instead of higgsH->Integral());
      higgsH2_->Scale(1./higgsH2_->Integral());    /// ??? was signalHistWH_->Integral()); instead of higgsH->Integral());
    }
    else{
      higgsH_->Scale( signalNormFactor_[currentMassPoint_]);    /// ??? was signalHistWH_->Integral()); instead of higgsH->Integral());
      higgsH2_->Scale(signalNormFactor_[currentMassPoint_]);    /// ??? was signalHistWH_->Integral()); instead of higgsH->Integral());
    }

    higgsH_->GetXaxis()->SetRange(displayMin_,displayMax_);    
    higgsH_->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    higgsH_->Draw("histsame");

    higgsH2_->GetXaxis()->SetRange(displayMin_,displayMax_);    
    higgsH2_->GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    higgsH2_->Draw("histsame");


    TGraphErrors myBkgError;
    getErrorBands(hs, myBkgError);
    myBkgError.SetFillColor(1);
    myBkgError.SetFillStyle(3004);
    myBkgError.GetYaxis()->SetTitle("Events/bin");
    myBkgError.GetYaxis()->SetTitleOffset(0.85);
    myBkgError.GetXaxis()->SetTitle((fitVars_[i]->getFancyName()).c_str());
    myBkgError.GetXaxis()->SetTitleOffset(0.85);
    myBkgError.GetXaxis()->SetTitleSize(5);
    myBkgError.GetXaxis()->SetRange(displayMin_,displayMax_);    
    myBkgError.GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    myBkgError.Draw("2");
    
    TGraphErrors mySignalError;
    getErrorBands(*higgsH_, mySignalError);
    mySignalError.SetName("blahSignalError");
    mySignalError.SetFillColor(616);
    mySignalError.SetFillStyle(3005);
    mySignalError.GetYaxis()->SetTitle("Events/bin");
    mySignalError.GetYaxis()->SetTitleOffset(0.85);
    mySignalError.GetXaxis()->SetTitle((fitVars_[i]->getFancyName()).c_str());
    mySignalError.GetXaxis()->SetTitleOffset(0.85);
    mySignalError.GetXaxis()->SetTitleSize(5);
    mySignalError.GetXaxis()->SetRange(displayMin_,displayMax_);    

    mySignalError.GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    //    mySignalError.Draw("2");
    
    TGraphErrors mySignal2Error;
    getErrorBands(*higgsH2_, mySignalError);
    mySignalError.SetName("blahSignalError");
    mySignalError.SetFillColor(618);
    mySignalError.SetFillStyle(3005);
    mySignalError.GetYaxis()->SetTitle("Events/bin");
    mySignalError.GetYaxis()->SetTitleOffset(0.85);
    mySignalError.GetXaxis()->SetTitle((fitVars_[i]->getFancyName()).c_str());
    mySignalError.GetXaxis()->SetTitleOffset(0.85);
    mySignalError.GetXaxis()->SetTitleSize(5);
    mySignalError.GetXaxis()->SetRange(displayMin_,displayMax_);    
    mySignalError.GetXaxis()->SetRangeUser(displayMin_,displayMax_);    
    //  mySignalError.Draw("2");
    
    leg_->AddEntry(myBkgError.GetName(),"#splitline{bkg}{total unc.}","f");
//    leg_->AddEntry(mySignalError.GetName(),"#splitline{tb signal}{total unc.}","f");
//    leg_->AddEntry(mySignalError.GetName(),"#splitline{#tau#nu signal}{total unc.}","f");
    
    TPaveText *pt1 = new TPaveText(0.17,0.45,0.65,0.5, "brNDC");
    pt1->SetBorderSize(1);
    pt1->SetFillColor(19);
    pt1->SetFillStyle(0);
    pt1->SetLineColor(0);
    pt1->SetTextFont(132);
    pt1->SetTextSize(0.055);
    //  TText *text = pt1->AddText("#splitline{m_{H^{#pm}} = 120 GeV/c^{2},}{BR(t #rightarrow H^{+}b) = 0.05}");
    //	    TText *text = pt1->AddText("B(t #rightarrow H^{+}b) = 0.05");
    //	    text->SetTextAlign(11);
    //    pt1->Draw("same");
    
    TPaveText *pt = new TPaveText(0.15,0.93,0.9,1.0, "brNDC");
    pt->SetBorderSize(1);
    pt->SetFillColor(19);
    pt->SetFillStyle(0);
    pt->SetLineColor(0);
    pt->SetTextFont(42);
    pt->SetTextSize(0.045);
    TText *textPrel = pt->AddText("CMS Preliminary, #sqrt{s} = 8 TeV,  19.7 fb^{-1}");
    textPrel->SetTextAlign(11);
    pt->Draw("same");
    
    
    // End stacked drawing ---------------------------------------------
    
    // Normal drawing ----------------------------------------
    //  signalHistWH_->Draw("hist");
    //  //  signalHistHH_->DrawNormalized("histsame");
    //  
    //  ddbkgHist_->Draw("histsame");
    //  for(size_t f=0; f<nMcSamples_; f++)
    //    mcbkgHist_[f]->Draw("histsame");
    //  dataHist_->Draw("histsame");    
    // End normal drawing -----------------------------------
    
    leg_->Draw("same");

    if(fitVars_[i]->getDoLogy())
      gPad->SetLogy();
    
    
    canvas_->Modified();
    canvas_->Update();

    //    canvas_->cd();
    canvas_->cd(2);
    gPad->SetFillColor(0);
    gPad->SetPad(0,0,1,0.2);
    gPad->SetBottomMargin(0.4);
    gPad->SetGridx();
    gPad->SetGridy();
    
    // Build denominator
    TH1* denominator;
    for(size_t f=3; f<nSamples_+2; f++){
      if(f==3){
	denominator = (TH1*) hist_[f]->Clone(hist_[f]->GetName() + TString("_ratio"));
	cout << "I AM HISTOGRAM " << hist_[f]->GetName() << ", WITH NORMALIZATION: " << hist_[f]->Integral() << endl;
      }
      else if(f>3){
	denominator->Add(hist_[f]);
	cout << "I AM HISTOGRAM " << hist_[f]->GetName() << ", WITH NORMALIZATION: " << hist_[f]->Integral() << endl;
      }
    }
    // Build data clone
    TH1* dataClone = (TH1*) hist_[0]->Clone("dataclone");
    cout << "I AM NUMERATOR WITH NORMALIZATION: " << dataClone->Integral() << endl;
    cout << "I AM DENOMINATOR WITH NORMALIZATION: " << denominator->Integral() << endl;
      

    TH1* iRatio = dataClone;
    TGraphErrors myRelError = myBkgError;
    //    getErrorBands(myBkgError, myRelError);
    scaleErrorBands(myRelError, *iRatio, *denominator);
    iRatio->GetYaxis()->SetTitle("Data/#Sigma MC");
    iRatio->GetYaxis()->SetTitleSize(0.17);
    iRatio->GetYaxis()->SetTitleOffset(0.2);
    iRatio->GetYaxis()->SetLabelSize(0.1);
    iRatio->GetXaxis()->SetTitleSize(0.15);
    iRatio->GetXaxis()->SetLabelSize(0.1);
    iRatio->Divide(denominator);
    sumErrorBands(myRelError, *iRatio);
    iRatio->SetMarkerSize(1);
    iRatio->SetMarkerColor(1);
    iRatio->SetLineColor(1);
    iRatio->SetFillColor(0);
    iRatio->SetMarkerStyle(20);
    iRatio->GetYaxis()->SetRangeUser(0.1, 2.);
    iRatio->GetYaxis()->SetNdivisions(5);
    iRatio->Draw();
    myRelError.SetName("bl");
    myRelError.SetTitle("mmbm");
    myRelError.SetFillColor(kGray+2);
    myRelError.SetFillStyle(3001);
    myRelError.Draw("2same");
    
    gPad->Modified();
    gPad->Update();
    
    canvas_->Modified();
    canvas_->Update();
    

    canvas_->SaveAs((outFolder_+outputFileName+string(".pdf")).c_str());
    canvas_->SaveAs((outFolder_+outputFileName+string(".png")).c_str());
    canvas_->cd();
    canvas_->Clear();
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    
    
    
  } // End if !produceOnly_
  

  
  // Stat filling and histogram putting nonzeroes instead of zeroes (RooFit mojo)
  // Perhaps do a single loop with systs.
  
  
  
  for(size_t f=0; f<nSamples_+2; f++){
    for(int ibin=1; ibin<=hist_[f]->GetNbinsX(); ibin++){ // <= is for overflow
      
      hist_[nSamples_]->Scale(150./hist_[nSamples_]->Integral());      // Normalize to sample
      hist_[nSamples_+1]->Scale(12/hist_[nSamples_+1]->Integral()); // Normalize to sample
      
      if(ibin==1 && hist_[f]->GetBinContent(ibin) != 0)
	hist_[f]->SetBinError(ibin,  hist_[f]->GetBinContent(ibin) * hist_[f]->GetBinError(ibin+1)/hist_[f]->GetBinContent(ibin+1)  );
      
      
      if(hist_[f]->GetBinContent(ibin) == 0) // Protection from RooStat/Fit breaking in combine
	hist_[f]->SetBinContent(ibin, 1e-6);
      
      // Fill stat uncertainty histograms
      histStatUp_[f]  ->SetBinContent(ibin, std::max( hist_[f]->GetBinContent(ibin) + hist_[f]->GetBinError(ibin) , 1e-6 ) );
      histStatDown_[f]->SetBinContent(ibin, std::max( hist_[f]->GetBinContent(ibin) - hist_[f]->GetBinError(ibin) , 1e-6 ) );
    }
  }

  // Rescale stat plots - they must have the same integral as the base ones
  for(size_t f=0; f<nSamples_+2; f++){

    if(isSignal_[f]){
      hist_[f]->Scale( signalNormFactor_[currentMassPoint_] );
      for(size_t a=0; a<nSysts_; a++){ // Loop on syst components
	systHist_[a][f]->Scale( signalNormFactor_[currentMassPoint_] );
      }
    }
    string nonormName = histStatUp_[f]->GetName();
    nonormName += string("_noNorm");
    histStatNoNorm_.push_back( (TH1*) histStatUp_[f]->Clone( nonormName.c_str() ) );
    if(isSignal_[f])
      histStatNoNorm_[f]->Scale( signalNormFactor_[currentMassPoint_] );
    histStatUp_[f]->Scale(hist_[f]->Integral()/histStatUp_[f]->Integral() );
    histStatDown_[f]->Scale(hist_[f]->Integral()/histStatDown_[f]->Integral());
  }
  
  if(!unsplitUncertainties_){ // the correct thing to do
    for(size_t a=0; a<nSysts_; a++){ // Loop on syst components
      for(size_t f=0; f<nSamples_+2; f++){
	for(int ibin=1; ibin<=systHist_[a][f]->GetNbinsX(); ibin++){ // <= is for overflow
	  
	  if(ibin==1 && systHist_[a][f]->GetBinContent(ibin) != 0)
	    systHist_[a][f]->SetBinError(ibin,  systHist_[a][f]->GetBinContent(ibin) * systHist_[a][f]->GetBinError(ibin+1)/systHist_[a][f]->GetBinContent(ibin+1)  );
	  
	  
	  if(systHist_[a][f]->GetBinContent(ibin) == 0) // Protection from RooStat/Fit breaking in combine
	    systHist_[a][f]->SetBinContent(ibin, 1e-6);
	  
	}
      }
    } // end loop on syst components
  } // End of the correct way
  else{ // The finnish way: JES, JER, MET are stacked one over the other like if they were correlated
    
    // Stack them
    for(size_t f=0; f<nSamples_+2; f++){
      systHist_[0][f]->Add(systHist_[2][f],1);
      systHist_[0][f]->Add(systHist_[4][f],1);
      systHist_[0][f]->Add(hist_[f],-2);
      
      systHist_[1][f]->Add(systHist_[3][f],1);
      systHist_[1][f]->Add(systHist_[5][f],1);
      systHist_[1][f]->Add(hist_[f],-2);
      
      systHist_[0][f]->Scale(hist_[f]->Integral()/systHist_[0][f]->Integral());
      systHist_[1][f]->Scale(hist_[f]->Integral()/systHist_[1][f]->Integral());

    }

 
    for(size_t a=0; a<nSysts_; a++){ // Loop on syst components
      for(size_t f=0; f<nSamples_+2; f++){
	for(int ibin=1; ibin<=systHist_[a][f]->GetNbinsX(); ibin++){ // <= is for overflow
	  
	  if(ibin==1 && systHist_[a][f]->GetBinContent(ibin) != 0)
	    systHist_[a][f]->SetBinError(ibin,  systHist_[a][f]->GetBinContent(ibin) * systHist_[a][f]->GetBinError(ibin+1)/systHist_[a][f]->GetBinContent(ibin+1)  );
	}
      }
    } // end loop on syst components
    
    for(size_t f=0; f<nSamples_+2; f++){
      systHist_[0][f]->Add(systHist_[2][f], 1);
      systHist_[0][f]->Add(systHist_[4][f], 1);
      systHist_[0][f]->Add(hist_[f], -2);

      systHist_[1][f]->Add(systHist_[3][f], 1);
      systHist_[1][f]->Add(systHist_[5][f], 1);
      systHist_[1][f]->Add(hist_[f], -2);

      for(int ibin=1; ibin<=systHist_[0][f]->GetNbinsX(); ibin++){ // <= is for overflow
	if(systHist_[0][f]->GetBinContent(ibin) == 0){ // Protection from RooStat/Fit breaking in combine
	  systHist_[0][f]->SetBinContent(ibin, 1e-6);
	}
      }

      for(int ibin=1; ibin<=systHist_[1][f]->GetNbinsX(); ibin++){ // <= is for overflow
	if(systHist_[1][f]->GetBinContent(ibin) == 0){ // Protection from RooStat/Fit breaking in combine
	  systHist_[1][f]->SetBinContent(ibin, 1e-6);
	}
      }

    }    
    

  } // End of the finnish way 

  // Set names
  if(!doMultiDimensionalShapes_){
    
    for(size_t f=0; f<nSamples_+2; f++){
      hist_[f]->SetName(sampleName_[f].c_str());
      histStatUp_[f]->SetName(sampleName_[f].c_str()+TString("_")+sampleName_[f].c_str()+TString("_StatUp")); // Double name because of datacard syntax (indipendent lines for each stat)
      histStatDown_[f]->SetName(sampleName_[f].c_str()+TString("_")+sampleName_[f].c_str()+TString("_StatDown")); // Double name because of datacard syntax (indipendent lines for each stat)
    }
    
    // Syst case
    if(!unsplitUncertainties_){ // Correct way
      for(size_t a=0; a<nSysts_; a++){ // Loop on syst components
	for(size_t f=0; f<nSamples_+2; f++){
	  systHist_[a][f]->SetName(sampleName_[f].c_str()+systFancyComponents_[a]);
	}
      } // End syst loop
    } else{ // Finnish way (all stacked)
      for(size_t f=0; f<nSamples_+2; f++){
	systHist_[0][f]->SetName(sampleName_[f].c_str()+TString("_7Up"));
	systHist_[1][f]->SetName(sampleName_[f].c_str()+TString("_7Down"));
      }
    }
    // End syst case
  } // End if do multidimensional shapes

  if(produceOnly_ && !doMultiDimensionalShapes_)
    if(outputFile) outputFile->Write();
  
  if(doMultiDimensionalShapes_){
    vector<TH1*> tempMasterHist;
    vector<TString> tempMasterHistNames;
    tempMasterHist.clear();
    tempMasterHistNames.clear();
    // Store histograms
    for(size_t f=0; f<nSamples_+2; f++){
      tempMasterHist.push_back(  (TH1*) hist_[f]        ->Clone( TString( hist_[f]        ->GetName() + fitVars_[i]->getVarName())  ));
      tempMasterHist.push_back(  (TH1*) histStatUp_[f]  ->Clone( TString( histStatUp_[f]  ->GetName() + fitVars_[i]->getVarName())  ));
      tempMasterHist.push_back(  (TH1*) histStatDown_[f]->Clone( TString( histStatDown_[f]->GetName() + fitVars_[i]->getVarName())  ));
      
      tempMasterHistNames.push_back( sampleName_[f].c_str() 							     );
      tempMasterHistNames.push_back( sampleName_[f].c_str()+TString("_")+sampleName_[f].c_str()+TString("_StatUp")   );
      tempMasterHistNames.push_back( sampleName_[f].c_str()+TString("_")+sampleName_[f].c_str()+TString("_StatDown") );
    }
    
    // Syst case
    for(size_t a=0; a<nSysts_; a++){ // Loop on syst components
      for(size_t f=0; f<nSamples_+2; f++){
	tempMasterHist.push_back( (TH1*) systHist_[a][f]->Clone( TString( systHist_[a][f]->GetName() + fitVars_[i]->getVarName() ) ));
	tempMasterHistNames.push_back( sampleName_[f].c_str()+systFancyComponents_[a] );
      }
    }
    // End syst loop
    masterHist_.push_back( tempMasterHist );
    masterHistNames_.push_back( tempMasterHistNames );
  }
  // ---
  
  if(produceOnly_ && !doMultiDimensionalShapes_){
    ShapesToDatacard(); 

    if(outputFile) outputFile->Close();
  }
  
  cout << "File closed" << endl;
  
}

void LandSShapesProducer::UnrollMultiDimensionalShape(){
  
  // Is 2D. Must translate into genericD
  masterShapes_.clear(); // Just to be sure
  
  cout << "Unrolling shapes" << endl;

  // Open file for storing shapes
  string outputFileName = outputFileName_ + string("_") + massPointName_[currentMassPoint_] + string("_") /* + string("multiDimShape_")*/ + fitVars_[0]->getVarName();// + string("_") + fitVars_[1]->getVarName();
  TFile* outputFile(0);
  if(produceOnly_ && doMultiDimensionalShapes_)
    outputFile = new TFile((outFolder_+outputFileName+string(".root")).c_str(), "RECREATE");
  cout << "File opened" << endl;

  th2_.push_back( (TH2D*)th2_[nSamples_-1]->Clone( sampleName_[nSamples_].c_str()   + TString(th2_[nSamples_-1]->GetName() ))   ); // nSamples-1 Must always be di-bosons
  th2_.push_back( (TH2D*)th2_[nSamples_-1]->Clone( sampleName_[nSamples_+1].c_str() + TString(th2_[nSamples_-1]->GetName() )) ); // nSamples-1 Must always be di-bosons
  th2_[nSamples_]  ->Sumw2();
  th2_[nSamples_+1]->Sumw2();
  
  // Create unrolled stuff
  for(size_t f=0; f<nSamples_+2; f++){
    int nBins = th2_[f]->GetXaxis()->GetNbins() * th2_[f]->GetYaxis()->GetNbins();
    unrolled_.push_back( new TH1D(sampleName_[f].c_str(), sampleName_[f].c_str(),  nBins, 0 , nBins   ));
    unrolledStatUp_.push_back( new TH1D(sampleName_[f].c_str()+TString("_")+sampleName_[f].c_str()+TString("_StatUp"),sampleName_[f].c_str()+TString("_")+sampleName_[f].c_str()+TString("_StatUp"), nBins, 0 , nBins ) );
    unrolledStatDown_.push_back( new TH1D(sampleName_[f].c_str()+TString("_")+sampleName_[f].c_str()+TString("_StatDown"), sampleName_[f].c_str()+TString("_")+sampleName_[f].c_str()+TString("_StatDown"),  nBins, 0, nBins  ) );

    
    unrolled_[f]->Sumw2();
    unrolledStatUp_[f]->Sumw2();
    unrolledStatDown_[f]->Sumw2();
  }
  
  // Syst case
  for(size_t a=0; a<nSysts_; a++){ // Loop on syst components
    vector<TH1D*> unrolledSystTemp;
    unrolledSystTemp.clear();
    for(size_t f=0; f<nSamples_+2; f++){
      int nBins = th2_[f]->GetXaxis()->GetNbins() * th2_[f]->GetYaxis()->GetNbins();
      unrolledSystTemp.push_back( new TH1D(sampleName_[f].c_str()+systFancyComponents_[a], sampleName_[f].c_str()+systFancyComponents_[a], nBins, 0,  nBins  ));
      unrolledSystTemp[f]->Sumw2();

    }
    unrolledSyst_.push_back(unrolledSystTemp);
  } // End syst loop
    // End syst case
  
  // End create unrolled stuff
  
  // Fill statUp/Down

  // Stat filling and histogram putting nonzeroes instead of zeroes (RooFit mojo)
  // Perhaps do a single loop with systs.
  
 
  
  for(size_t f=0; f<nSamples_+2; f++){
    int ibin = 1;
    for(int xbin=1; xbin<th2_[f]->GetXaxis()->GetNbins(); xbin++){ // <= is for overflow
      for(int ybin=1; ybin<th2_[f]->GetYaxis()->GetNbins(); ybin++){ // <= is for overflow
	Double_t val =th2_[f]->GetBinContent(xbin,ybin);
	unrolled_[f]->SetBinContent(ibin, val);
	unrolled_[f]->SetBinError(ibin, th2_[f]->GetBinError(xbin,ybin));
	cout << "histogram " << unrolled_[f]->GetName() << ": th2 has bin " << ibin << " - ["<<xbin<<","<<ybin<<"] th2:" <<  val << ", th1: " << unrolled_[f]->GetBinContent(ibin) << endl;
	if(unrolled_[f]->GetBinContent(ibin) == 0)
	  unrolled_[f]->SetBinContent(ibin, 1e-6); // Protection from RooStat/Fit breaking in combine
	unrolledStatUp_[f]  ->SetBinContent(ibin, std::max( th2_[f]->GetBinContent(xbin,ybin) + th2_[f]->GetBinError(xbin,ybin), 1e-6 ) );
	unrolledStatDown_[f]->SetBinContent(ibin, std::max( th2_[f]->GetBinContent(xbin,ybin) - th2_[f]->GetBinError(xbin,ybin), 1e-6 ) );

	ibin++;
      }
    }
  }

    
  unrolled_[nSamples_]->Scale(150./unrolled_[nSamples_]->Integral());      // Normalize to sample
  unrolled_[nSamples_+1]->Scale(12/unrolled_[nSamples_+1]->Integral()); // Normalize to sample

  
  // Rescale stat plots - they must have the same integral as the base ones
  for(size_t f=0; f<nSamples_+2; f++){
    unrolledStatUp_[f]  ->Scale(unrolled_[f]->Integral()/unrolledStatUp_[f]->Integral());
    unrolledStatDown_[f]->Scale(unrolled_[f]->Integral()/unrolledStatDown_[f]->Integral());
  }
  
  
  for(size_t a=0; a<nSysts_; a++){ // Loop on syst components
    th2Syst_[a].push_back( (TH2D*)th2Syst_[a][nSamples_-1]->Clone( sampleName_[nSamples_].c_str()  + systComponents_[a])   ); // nSamples-1 Must always be di-bosons
    th2Syst_[a].push_back( (TH2D*)th2Syst_[a][nSamples_-1]->Clone( sampleName_[nSamples_+1].c_str()+ systComponents_[a]) ); // nSamples-1 Must always be di-bosons
    th2Syst_[a][nSamples_]  ->Sumw2();
    th2Syst_[a][nSamples_+1]->Sumw2();

    for(size_t f=0; f<nSamples_+2; f++){
      int ibin=1;
      for(int xbin=1; xbin<th2_[f]->GetXaxis()->GetNbins(); xbin++){ // <= is for overflow
	for(int ybin=1; ybin<th2_[f]->GetYaxis()->GetNbins(); ybin++){ // <= is for overflow
	  unrolledSyst_[a][f]->SetBinContent(ibin, th2Syst_[a][f]->GetBinContent(xbin,ybin));
	  if(unrolledSyst_[a][f]->GetBinContent(ibin) == 0) // Protection from RooStat/Fit breaking in combine
	    unrolledSyst_[a][f]->SetBinContent(ibin, 1e-6);
	  ibin++;
	}
      }
    }
    unrolledSyst_[a][nSamples_]->Scale(150./unrolledSyst_[a][nSamples_]->Integral());      // Normalize to sample
    unrolledSyst_[a][nSamples_+1]->Scale(12/unrolledSyst_[a][nSamples_+1]->Integral()); // Normalize to sample

  }


  if(produceOnly_ && doMultiDimensionalShapes_)
    outputFile->Write();
  
  outputFile->Close();
}

 void LandSShapesProducer::StorePerMassSignalShapes(){
  for(size_t i=0; i<perMassPointSignalShapesToCompare_.size(); i++){
    perMassPointSignalShapesToCompare_[i]  ->SetLineColor(currentMassPoint_);
    perMassPointSignalShapesToCompareHH_[i]->SetLineColor(currentMassPoint_);
    perMassPointSignalShapesToCompareWH_[i]->SetLineColor(currentMassPoint_);
  }
  signalShapesToCompare_  .push_back(perMassPointSignalShapesToCompare_  );
  signalShapesToCompareHH_.push_back(perMassPointSignalShapesToCompareHH_);
  signalShapesToCompareWH_.push_back(perMassPointSignalShapesToCompareWH_);

  cout << "Stored per mass signal shapes" << endl;
}

void LandSShapesProducer::DrawSignalShapesComparison(){
  cout << "Drawing signal shapes comparisons" << endl;
  canvas_->cd();
  
  leg_ = new TLegend(0.23,0.535,0.62,0.93,NULL,"brNDC");
  //  leg_ = new TLegend(0.7147651,0.6346154,0.9446309,0.9353147,NULL,"brNDC");
  //leg_ = new TLegend(0.75,0.6346154,1.,1.,NULL,"brNDC");
  leg_->SetTextFont(62);
  leg_->SetBorderSize(0);
  leg_->SetLineColor(1);
  leg_->SetLineStyle(1);
  leg_->SetLineWidth(1);
  leg_->SetFillColor(0);
  leg_->SetFillStyle(0);
  
  for(size_t s=0; s<signalShapesToCompare_.size(); s++)
    leg_->AddEntry(signalShapesToCompare_[s][0],(massPointName_[s]).c_str(),"l");
 
  
  // Draw them
  for(size_t i=0; i<nVars_; i++){
    canvas_->cd();
    for(size_t s=0; s<signalShapesToCompare_.size(); s++){     
      signalShapesToCompare_[s][i]->SetLineColor(s);
      if(s==0) signalShapesToCompare_[s][i]->Draw("hist");
      else signalShapesToCompare_[s][i]->Draw("histsame");
    }
    leg_->Draw();
    string outputFileName = outputFileName_ + string("_") + fitVars_[i]->getVarName();
    canvas_->SaveAs((outFolder_+outputFileName+string("_signalShapesComparison")+string(".pdf")).c_str());
    canvas_->SaveAs((outFolder_+outputFileName+string("_signalShapesComparison")+string(".png")).c_str());
    canvas_->Clear();
  }

  for(size_t i=0; i<nVars_; i++){
    canvas_->cd();
    for(size_t s=0; s<signalShapesToCompareHH_.size(); s++){     
      signalShapesToCompareHH_[s][i]->SetLineColor(s);
      if(s==0) signalShapesToCompareHH_[s][i]->Draw("hist");
      else signalShapesToCompareHH_[s][i]->Draw("histsame");
    }
    leg_->Draw();
    string outputFileName = outputFileName_ + string("_") + fitVars_[i]->getVarName();
    canvas_->SaveAs((outFolder_+outputFileName+string("_signalHHShapesComparison")+string(".pdf")).c_str());
    canvas_->SaveAs((outFolder_+outputFileName+string("_signalHHShapesComparison")+string(".png")).c_str());
    canvas_->Clear();
  }

  for(size_t i=0; i<nVars_; i++){
    canvas_->cd();
    for(size_t s=0; s<signalShapesToCompareWH_.size(); s++){     
      signalShapesToCompareWH_[s][i]->SetLineColor(s);
      if(s==0) signalShapesToCompareWH_[s][i]->Draw("hist");
      else signalShapesToCompareWH_[s][i]->Draw("histsame");
    }
    leg_->Draw();
    string outputFileName = outputFileName_ + string("_") + fitVars_[i]->getVarName();
    canvas_->SaveAs((outFolder_+outputFileName+string("_signalWHShapesComparison")+string(".pdf")).c_str());
    canvas_->SaveAs((outFolder_+outputFileName+string("_signalWHShapesComparison")+string(".png")).c_str());
    canvas_->Clear();
  }

    
}

void LandSShapesProducer::ShapesToDatacard(){

  // Create txt file
  time_t secs=time(0);
  tm *t=localtime(&secs);
  
  double testIntegral(0), testSumBins(0);

  for(int ibin=0; ibin<hist_[0]->GetNbinsX(); ++ibin){
    
    if(ibin==0){
    datacard_.open (   (outFolder_+datacardsBaseName_+string("_")+massPointName_[currentMassPoint_] +string("_mutau.txt") ).c_str()     ); 
    datacard_<< fixed << showpoint <<setprecision(3); // fixed point, show 3 decimals
    datacard_<<"Data: "<<t->tm_mday<<"/"<<t->tm_mon+1<<"/"<<t->tm_year+1900<<endl;
    datacard_<<"Description: H+, mu-tau, mass "<<massPointName_[currentMassPoint_]<<" GeV, lumi=" << commondefinitions::LUM_<<" pb-1"<<endl;
    datacard_<<"---------------------------------------------------------------------------------------------------------------------"<<endl;
    datacard_<<"imax   1  number of channels"<<endl;
    datacard_<<"jmax   *  number of backgrounds"<<endl;
    datacard_<<"kmax   *  number of nuisance parameters"<<endl;
    datacard_<<"---------------------------------------------------------------------------------------------------------------------"<<endl;
    //      outfile<<"shapes * * shapes_m"<<HMass[im]<<"_rc_t.root $PROCESS $PROCESS_$SYSTEMATIC"<<endl;                                                                             
    //    datacard_<<"shapes * * shapes_"<<massPointName_[currentMassPoint_]<<"_btagmultiplicity_j.root $PROCESS $PROCESS_$SYSTEMATIC"<<endl;
    // WARNING: fitVars_[0] is a temporary workaround that works only because the code is still supposed to run on one variable only. Woe me.
    datacard_<<"shapes * * " << outputFileName_ << "_" << massPointName_[currentMassPoint_] << "_" << fitVars_[0]->getVarName() << ".root $PROCESS $PROCESS_$SYSTEMATIC" << endl; 
    datacard_<<"---------------------------------------------------------------------------------------------------------------------"<<endl;
    datacard_<<"bin a"<<endl;
    datacard_<<"observation    "<<  hist_[0]->Integral() <<endl; // data yield
    testIntegral=hist_[0]->Integral(); cout << "Test Integral" << testIntegral << endl;
    datacard_<<"---------------------------------------------------------------------------------------------------------------------"<<endl;
    datacard_<<"bin              a          a          a           a           a           a           a         a      ";
    if(sampleName_.size() >9) datacard_<<"   a  ";
    datacard_<<endl;
    datacard_<<"process          "<<sampleName_[1]<<"   "<< sampleName_[2] << "      " << sampleName_[3]<<"   "<<sampleName_[4]<<"   "<<sampleName_[5]<<"   "<<sampleName_[6]<<"   "<<sampleName_[7]<<"   "<<sampleName_[8]<<"   "<< (sampleName_.size() > 9 ? sampleName_[9] : " " ) <<endl;
    datacard_<<"process          "<< (sampleName_.size() > 9 ? " -1   " : "" ) << "   0       1          2           3           4           5           6         7       "<<endl;
    datacard_<<"---------------------------------------------------------------------------------------------------------------------"<<endl;
    datacard_<<"rate          "<<
      hist_[1]->Integral()<<setw(10)<<
      hist_[2]->Integral()<<setw(10)<<
      hist_[3]->Integral()<<setw(10)<<
      hist_[4]->Integral()<<setw(10)<<
      hist_[5]->Integral()<<setw(10)<<
      hist_[6]->Integral()<<setw(10)<<
      hist_[7]->Integral()<<setw(10)<<
      hist_[8]->Integral()<<setw(10);
    if(sampleName_.size() > 9)  datacard_<<hist_[9]->Integral();
    datacard_ <<endl;
    datacard_<<"---------------------------------------------------------------------------------------------------------------------"<<endl;
    datacard_<<endl;
    datacard_<<" tauId  lnN   "<<setw(7)<<
      (sampleName_.size() > 9 ? "1.06      " : " " )<< 
      1.06<<setw(10)<<
      1.00<<setw(10)<<
      1.06<<setw(10)<<
      1.06<<setw(10)<<
      1.06<<setw(10)<<
      1.06<<setw(10)<<
      1.06<<setw(10)<<
      1.06<<endl;
    datacard_<<" jetTauMisId    lnN  "<<setw(7)<<
      (sampleName_.size() > 9 ? "1.00     " : " " )<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.15<<setw(10)<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.15<<endl;
    datacard_<<" fakesSyst      lnN   "<<setw(7)<<
      (sampleName_.size() > 9 ? "1.00      " : " " )<<
      1.00<<setw(10)<<
      1.11<<setw(10)<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.00<<endl; // Take from shapes?
    datacard_<<" tes      shape   "<<setw(7)<< 
      (sampleName_.size() > 9 ? "1       " : "" )<< 
      "1"<<setw(10)<<//systHist_[1][3]->Integral()  / hist_[3]->Integral()<<"/"<<  systHist_[0][3]->Integral()  / hist_[3]->Integral()  <<setw(7)<<
      "-"<<setw(10)<<//1.00<<setw(10)<<
      "1"<<setw(10)<<//systHist_[1][4]->Integral()  / hist_[4]->Integral()<<"/"<<  systHist_[0][4]->Integral()  / hist_[4]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[1][8]->Integral()  / hist_[8]->Integral()<<"/"<<  systHist_[0][8]->Integral()  / hist_[8]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[1][7]->Integral()  / hist_[7]->Integral()<<"/"<<  systHist_[0][7]->Integral()  / hist_[7]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[1][5]->Integral()  / hist_[5]->Integral()<<"/"<<  systHist_[0][5]->Integral()  / hist_[5]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[1][5]->Integral()  / hist_[5]->Integral()<<"/"<<  systHist_[0][5]->Integral()  / hist_[5]->Integral()  <<setw(7)<<
      "1"<<endl;//systHist_[1][6]->Integral()  / hist_[6]->Integral()<<"/"<<  systHist_[0][6]->Integral()  / hist_[6]->Integral()  <<endl;
    datacard_<<" topptunc      shape   "<<setw(7)<<  
      (sampleName_.size() > 9 ? "-      " : "" )<< //systHist_[1][1]->Integral()  / hist_[1]->Integral()<<"/"<<  systHist_[0][1]->Integral()  / hist_[1]->Integral()  <<setw(7)<<
      "-"<<setw(10)<<//systHist_[1][1]->Integral()  / hist_[1]->Integral()<<"/"<<  systHist_[0][1]->Integral()  / hist_[1]->Integral()  <<setw(7)<<
      "-"<<setw(10)<<//systHist_[1][1]->Integral()  / hist_[1]->Integral()<<"/"<<  systHist_[0][1]->Integral()  / hist_[1]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[1][3]->Integral()  / hist_[3]->Integral()<<"/"<<  systHist_[0][3]->Integral()  / hist_[3]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[1][4]->Integral()  / hist_[4]->Integral()<<"/"<<  systHist_[0][4]->Integral()  / hist_[4]->Integral()  <<setw(7)<<
      "-"<<setw(10)<<//1.00<<setw(10)<<
      "-"<<setw(10)<<//systHist_[1][8]->Integral()  / hist_[8]->Integral()<<"/"<<  systHist_[0][8]->Integral()  / hist_[8]->Integral()  <<setw(7)<<
      "-"<<setw(10)<<//systHist_[1][7]->Integral()  / hist_[7]->Integral()<<"/"<<  systHist_[0][7]->Integral()  / hist_[7]->Integral()  <<setw(7)<<
      "-"<<endl;//systHist_[1][6]->Integral()  / hist_[6]->Integral()<<"/"<<  systHist_[0][6]->Integral()  / hist_[6]->Integral()  <<endl;
    datacard_<<" jes      shape   "<<setw(7)<< 
      (sampleName_.size() > 9 ? "-       " : "" )<< 
      "1"<<setw(10)<<//systHist_[1][1]->Integral()  / hist_[1]->Integral()<<"/"<<  systHist_[0][1]->Integral()  / hist_[1]->Integral()  <<setw(7)<<
      "-"<<setw(10)<<//1.00<<setw(10)<<
      "1"<<setw(10)<<//systHist_[1][3]->Integral()  / hist_[3]->Integral()<<"/"<<  systHist_[0][3]->Integral()  / hist_[3]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[1][4]->Integral()  / hist_[4]->Integral()<<"/"<<  systHist_[0][4]->Integral()  / hist_[4]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[1][4]->Integral()  / hist_[4]->Integral()<<"/"<<  systHist_[0][4]->Integral()  / hist_[4]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[1][8]->Integral()  / hist_[8]->Integral()<<"/"<<  systHist_[0][8]->Integral()  / hist_[8]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[1][7]->Integral()  / hist_[7]->Integral()<<"/"<<  systHist_[0][7]->Integral()  / hist_[7]->Integral()  <<setw(7)<<
      "1"<<endl;//systHist_[1][6]->Integral()  / hist_[6]->Integral()<<"/"<<  systHist_[0][6]->Integral()  / hist_[6]->Integral()  <<endl;
    datacard_<<" jer      shape   "<<setw(7)<< 
      (sampleName_.size() > 9 ? "-       " : "" )<< 
      "1"<<setw(10)<<//systHist_[5][3]->Integral()  / hist_[3]->Integral()<<"/"<<  systHist_[4][3]->Integral()  / hist_[3]->Integral()  <<setw(7)<<
      "-"<<setw(10)<<//1.00<<setw(10)<<
      "1"<<setw(10)<<//systHist_[5][4]->Integral()  / hist_[4]->Integral()<<"/"<<  systHist_[4][4]->Integral()  / hist_[4]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[5][4]->Integral()  / hist_[4]->Integral()<<"/"<<  systHist_[4][4]->Integral()  / hist_[4]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[5][8]->Integral()  / hist_[8]->Integral()<<"/"<<  systHist_[4][8]->Integral()  / hist_[8]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[5][7]->Integral()  / hist_[7]->Integral()<<"/"<<  systHist_[4][7]->Integral()  / hist_[7]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[5][5]->Integral()  / hist_[5]->Integral()<<"/"<<  systHist_[4][5]->Integral()  / hist_[5]->Integral()  <<setw(7)<<
      "1"<<endl;//systHist_[5][6]->Integral()  / hist_[6]->Integral()<<"/"<<  systHist_[4][6]->Integral()  / hist_[6]->Integral()  <<endl;
    datacard_<<" umet      shape   "<<setw(7)<< 
      (sampleName_.size() > 9 ? "-      " : "" )<< 
      "1"<<setw(10)<<//systHist_[3][3]->Integral()  / hist_[3]->Integral()<<"/"<<  systHist_[2][3]->Integral()  / hist_[3]->Integral()  <<setw(7)<<
      "-"<<setw(10)<<//1.00<<setw(10)<<
      "1"<<setw(10)<<//systHist_[3][4]->Integral()  / hist_[4]->Integral()<<"/"<<  systHist_[2][4]->Integral()  / hist_[4]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[3][4]->Integral()  / hist_[4]->Integral()<<"/"<<  systHist_[2][4]->Integral()  / hist_[4]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[3][8]->Integral()  / hist_[8]->Integral()<<"/"<<  systHist_[2][8]->Integral()  / hist_[8]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[3][7]->Integral()  / hist_[7]->Integral()<<"/"<<  systHist_[2][7]->Integral()  / hist_[7]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[3][5]->Integral()  / hist_[5]->Integral()<<"/"<<  systHist_[2][5]->Integral()  / hist_[5]->Integral()  <<setw(7)<<
      "1"<<endl;//systHist_[3][6]->Integral()  / hist_[6]->Integral()<<"/"<<  systHist_[2][6]->Integral()  / hist_[6]->Integral()  <<endl;
    datacard_<<" leptEff        lnN   "<<setw(7)<<
      (sampleName_.size() > 9 ? "1.02        " : "" )<<
      1.02<<setw(10)<<
      1.00<<setw(10)<<
      1.02<<setw(10)<<
      1.02<<setw(10)<<
      1.02<<setw(10)<<
      1.02<<setw(10)<<
      1.02<<setw(10)<<
      1.02<<endl;
    datacard_<<" btag      shape   "<<setw(7)<< 
      (sampleName_.size() > 9 ? "1      " : "" )<<
      "1"<<setw(10)<<//systHist_[7][3]->Integral()  / hist_[3]->Integral()<<"/"<<  systHist_[6][3]->Integral()  / hist_[3]->Integral()  <<setw(7)<<
      "-"<<setw(10)<<//1.00<<setw(10)<<
      "1"<<setw(10)<<//systHist_[7][4]->Integral()  / hist_[4]->Integral()<<"/"<<  systHist_[6][4]->Integral()  / hist_[4]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[7][4]->Integral()  / hist_[4]->Integral()<<"/"<<  systHist_[6][4]->Integral()  / hist_[4]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[7][8]->Integral()  / hist_[8]->Integral()<<"/"<<  systHist_[6][8]->Integral()  / hist_[8]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[7][7]->Integral()  / hist_[7]->Integral()<<"/"<<  systHist_[6][7]->Integral()  / hist_[7]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[7][5]->Integral()  / hist_[5]->Integral()<<"/"<<  systHist_[6][5]->Integral()  / hist_[5]->Integral()  <<setw(7)<<
      "1"<<endl;//systHist_[7][6]->Integral()  / hist_[6]->Integral()<<"/"<<  systHist_[6][6]->Integral()  / hist_[6]->Integral()  <<endl;
    datacard_<<" unbtag      shape   "<<setw(7)<< 
      (sampleName_.size() > 9 ? "1         " : "" )<<
      "1"<<setw(10)<<//systHist_[9][3]->Integral()  / hist_[3]->Integral()<<"/"<<  systHist_[8][3]->Integral()  / hist_[3]->Integral()  <<setw(7)<<
      "-"<<setw(10)<<//1.00<<setw(10)<<
      "1"<<setw(10)<<//systHist_[9][4]->Integral()  / hist_[4]->Integral()<<"/"<<  systHist_[8][4]->Integral()  / hist_[4]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[9][4]->Integral()  / hist_[4]->Integral()<<"/"<<  systHist_[8][4]->Integral()  / hist_[4]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[9][8]->Integral()  / hist_[8]->Integral()<<"/"<<  systHist_[8][8]->Integral()  / hist_[8]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[9][7]->Integral()  / hist_[7]->Integral()<<"/"<<  systHist_[8][7]->Integral()  / hist_[7]->Integral()  <<setw(7)<<
      "1"<<setw(10)<<//systHist_[9][5]->Integral()  / hist_[5]->Integral()<<"/"<<  systHist_[8][5]->Integral()  / hist_[5]->Integral()  <<setw(7)<<
      "1"<<endl;//systHist_[9][6]->Integral()  / hist_[6]->Integral()<<"/"<<  systHist_[8][6]->Integral()  / hist_[6]->Integral()  <<endl;
    // FIXME: NoNorm??
    datacard_<<" "<<sampleName_[1]<<"_Stat     shape   "<<setw(7)<<
      "1"<<setw(10)<<//histStatNoNorm_[1]->Integral()  / hist_[1]->Integral()<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      (sampleName_.size() > 9 ? "-     " : "" )<<
      "-"<<endl;
    datacard_<<" "<<sampleName_[2]<<"_Stat     shape   "<<setw(7)<<
      "-"<<setw(10)<<//histStatNoNorm_[1]->Integral()  / hist_[1]->Integral()<<setw(10)<<
      "1"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      (sampleName_.size() > 9 ? "-     " : "" )<<
      "-"<<endl;
    datacard_<<sampleName_[3]<<"_Stat  shape   "<<setw(7)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<//histStatNoNorm_[3]->Integral()  / hist_[3]->Integral()<<setw(10)<<
      "1"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      (sampleName_.size() > 9 ? "-     " : "" )<<"-"<<setw(10)<<
      "-"<<endl;
    datacard_<<sampleName_[4]<<"_Stat    shape   "<<setw(7)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<//histStatNoNorm_[4]->Integral()  / hist_[4]->Integral()<<setw(10)<<
      "1"<<setw(10)<<//histStatNoNorm_[4]->Integral()  / hist_[4]->Integral()<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      (sampleName_.size() > 9 ? "-     " : "" )<<
      "-"<<endl;
    datacard_<<sampleName_[5]<<"_Stat     shape  "<<setw(7)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "1"<<setw(10)<<//histStatNoNorm_[8]->Integral()  / hist_[8]->Integral()<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      (sampleName_.size() > 9 ? "-     " : "" )<<
      "-"<<endl;
    datacard_<<sampleName_[6]<<"_Stat     shape"<<setw(7)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "1"<<setw(10)<<//histStatNoNorm_[7]->Integral()  / hist_[7]->Integral()<<setw(10)<<
      "-"<<setw(10)<<
      (sampleName_.size() > 9 ? "-     " : "" )<<
      "-"<<endl;
    datacard_<<sampleName_[7]<<"_Stat   shape   "<<setw(7)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "1"<<setw(10)<<
      (sampleName_.size() > 9 ? "-     " : "" )<<
      "-"<<endl;
//    datacard_<<sampleName_[7]<<"_embedded   lnN   "<<setw(7)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "1.99"<<setw(10)<<
//      (sampleName_.size() > 9 ? "-     " : "" )<<
//      "-"<<endl;
    datacard_<<sampleName_[8]<<"_Stat   shape   "<<setw(7)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "1"<<setw(10)<<
      (sampleName_.size() > 9 ? "-     " : "" )<<
      endl;
    if(sampleName_.size() >9){
      datacard_<<sampleName_[9]<<"_Stat     shape   "<<setw(7)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "-"<<setw(10)<<
      "1"<<endl; //histStatNoNorm_[6]->Integral()  / hist_[6]->Integral()<<endl;
    }
    datacard_<<" singletopCrossSection lnN   "<<setw(7)<<
      (sampleName_.size() > 9 ? "1.00     " : "" )<<\
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.08<<setw(10)<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<endl;
    datacard_<<" zllCrossSection       lnN   "<<setw(7)<<
      (sampleName_.size() > 9 ? "1.00     " : "" )<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.04<<endl;
    datacard_<<" dibosonCrossSection   lnN   "<<setw(7)<<
      (sampleName_.size() > 9 ? "1.00     " : "" )<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.00<<setw(10)<<
      1.04<<setw(10)<<
      1.00<<setw(10)<<
      1.00<<endl;
    datacard_<<" lumi_8TeV                lnN   "<<setw(7)<<
      (sampleName_.size() > 9 ? "1.026     " : "" )<<
      1.026<<setw(10)<<
      1.00<<setw(10)<<
      1.026<<setw(10)<<
      1.026<<setw(10)<<
      1.026<<setw(10)<<
      1.026<<setw(10)<<
      1.026<<setw(10)<<
      1.026<<endl;
    datacard_<<" pu               lnN   "<<setw(7)<<
      (sampleName_.size() > 9 ? "1.040     " : "" )<<
      1.04<<setw(10)<<
      1.00<<setw(10)<<
      1.02<<setw(10)<<
      1.08<<setw(10)<<
      1.02<<setw(10)<<
      1.02<<setw(10)<<
      1.04<<setw(10)<<
      1.25<<endl;//"    pileup"<<endl;
    // Write and close datacard_
    datacard_.close();
    //    break;
    }
 

//    if(ibin==0){
//    datacard_.open (   (outFolder_+datacardsBaseName_+string("_")+massPointName_[currentMassPoint_] +string("_mutau.txt") ).c_str()     ); 
//    datacard_<< fixed << showpoint <<setprecision(3); // fixed point, show 3 decimals
//    datacard_<<"Data: "<<t->tm_mday<<"/"<<t->tm_mon+1<<"/"<<t->tm_year+1900<<endl;
//    datacard_<<"Description: H+, mu-tau, mass "<<massPointName_[currentMassPoint_]<<" GeV, lumi=" << commondefinitions::LUM_<<" pb-1"<<endl;
//    datacard_<<"---------------------------------------------------------------------------------------------------------------------"<<endl;
//    datacard_<<"imax   1  number of channels"<<endl;
//    datacard_<<"jmax   *  number of backgrounds"<<endl;
//    datacard_<<"kmax   *  number of nuisance parameters"<<endl;
//    datacard_<<"---------------------------------------------------------------------------------------------------------------------"<<endl;
//    //      outfile<<"shapes * * shapes_m"<<HMass[im]<<"_rc_t.root $PROCESS $PROCESS_$SYSTEMATIC"<<endl;                                                                             
//    datacard_<<"shapes * * shapes_"<<massPointName_[currentMassPoint_]<<"_btagmultiplicity_j.root $PROCESS $PROCESS_$SYSTEMATIC"<<endl;
//    datacard_<<"---------------------------------------------------------------------------------------------------------------------"<<endl;
//    datacard_<<"bin a"<<endl;
//    datacard_<<"observation    "<<  hist_[0]->Integral() <<endl; // data yield
//    testIntegral=hist_[0]->Integral(); cout << "Test Integral" << testIntegral << endl;
//    datacard_<<"---------------------------------------------------------------------------------------------------------------------"<<endl;
//    datacard_<<"bin              a          a          a           a           a           a           a         a         "<<endl;
//    datacard_<<"process          "<<sampleName_[1]<<"   "<<sampleName_[3]<<"   "<<sampleName_[4]<<"   "<<sampleName_[2]<<"   "<<sampleName_[8]<<"   "<<sampleName_[7]<<"   "<<sampleName_[5]<<"   "<<sampleName_[6]<<endl; // Order matching values below
//    datacard_<<"process          0          1          2           3           4           5           6         7         "<<endl;
//    datacard_<<"---------------------------------------------------------------------------------------------------------------------"<<endl;
//    datacard_<<"rate          "<<
//      hist_[1]->Integral()<<setw(10)<<
//      hist_[3]->Integral()<<setw(10)<<
//      hist_[4]->Integral()<<setw(10)<<
//      hist_[2]->Integral()<<setw(10)<<
//      hist_[8]->Integral()<<setw(10)<<
//      hist_[7]->Integral()<<setw(10)<<
//      hist_[5]->Integral()<<setw(10)<<
//      hist_[6]->Integral()<<endl;
//    datacard_<<"---------------------------------------------------------------------------------------------------------------------"<<endl;
//    datacard_<<endl;
//    datacard_<<" tauId  lnN"<<setw(7)<<1.06<<setw(10)<<1.06<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.06<<setw(10)<<1.06<<setw(10)<<1.06<<endl;
//    datacard_<<" jetTauMisId    lnN"<<setw(7)<<1.00<<setw(10)<<1.00<<setw(10)<<1.15<<setw(10)<<1.00<<setw(10)<<1.15<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<endl;
//    datacard_<<" fakesSyst      lnN"<<setw(7)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.10<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<endl; // Take from shapes?
//    datacard_<<" fakesStat      lnN"<<setw(7)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<endl; // FIXME?
//    datacard_<<" tes      shape"<<setw(7)<< 
//      "1"<<setw(10)<<//systHist_[1][1]->Integral()  / hist_[1]->Integral()<<"/"<<  systHist_[0][1]->Integral()  / hist_[1]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[1][3]->Integral()  / hist_[3]->Integral()<<"/"<<  systHist_[0][3]->Integral()  / hist_[3]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[1][4]->Integral()  / hist_[4]->Integral()<<"/"<<  systHist_[0][4]->Integral()  / hist_[4]->Integral()  <<setw(7)<<
//      "-"<<setw(10)<<//1.00<<setw(10)<<
//      "1"<<setw(10)<<//systHist_[1][8]->Integral()  / hist_[8]->Integral()<<"/"<<  systHist_[0][8]->Integral()  / hist_[8]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[1][7]->Integral()  / hist_[7]->Integral()<<"/"<<  systHist_[0][7]->Integral()  / hist_[7]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[1][5]->Integral()  / hist_[5]->Integral()<<"/"<<  systHist_[0][5]->Integral()  / hist_[5]->Integral()  <<setw(7)<<
//      "1"<<endl;//systHist_[1][6]->Integral()  / hist_[6]->Integral()<<"/"<<  systHist_[0][6]->Integral()  / hist_[6]->Integral()  <<endl;
//    datacard_<<" topptunc      shape"<<setw(7)<< 
//      "-"<<setw(10)<<//systHist_[1][1]->Integral()  / hist_[1]->Integral()<<"/"<<  systHist_[0][1]->Integral()  / hist_[1]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[1][3]->Integral()  / hist_[3]->Integral()<<"/"<<  systHist_[0][3]->Integral()  / hist_[3]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[1][4]->Integral()  / hist_[4]->Integral()<<"/"<<  systHist_[0][4]->Integral()  / hist_[4]->Integral()  <<setw(7)<<
//      "-"<<setw(10)<<//1.00<<setw(10)<<
//      "-"<<setw(10)<<//systHist_[1][8]->Integral()  / hist_[8]->Integral()<<"/"<<  systHist_[0][8]->Integral()  / hist_[8]->Integral()  <<setw(7)<<
//      "-"<<setw(10)<<//systHist_[1][7]->Integral()  / hist_[7]->Integral()<<"/"<<  systHist_[0][7]->Integral()  / hist_[7]->Integral()  <<setw(7)<<
//      "-"<<setw(10)<<//systHist_[1][5]->Integral()  / hist_[5]->Integral()<<"/"<<  systHist_[0][5]->Integral()  / hist_[5]->Integral()  <<setw(7)<<
//      "-"<<endl;//systHist_[1][6]->Integral()  / hist_[6]->Integral()<<"/"<<  systHist_[0][6]->Integral()  / hist_[6]->Integral()  <<endl;
//    datacard_<<" jes      shape"<<setw(7)<< 
//      "1"<<setw(10)<<//systHist_[1][1]->Integral()  / hist_[1]->Integral()<<"/"<<  systHist_[0][1]->Integral()  / hist_[1]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[1][3]->Integral()  / hist_[3]->Integral()<<"/"<<  systHist_[0][3]->Integral()  / hist_[3]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[1][4]->Integral()  / hist_[4]->Integral()<<"/"<<  systHist_[0][4]->Integral()  / hist_[4]->Integral()  <<setw(7)<<
//      "-"<<setw(10)<<//1.00<<setw(10)<<
//      "1"<<setw(10)<<//systHist_[1][8]->Integral()  / hist_[8]->Integral()<<"/"<<  systHist_[0][8]->Integral()  / hist_[8]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[1][7]->Integral()  / hist_[7]->Integral()<<"/"<<  systHist_[0][7]->Integral()  / hist_[7]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[1][5]->Integral()  / hist_[5]->Integral()<<"/"<<  systHist_[0][5]->Integral()  / hist_[5]->Integral()  <<setw(7)<<
//      "1"<<endl;//systHist_[1][6]->Integral()  / hist_[6]->Integral()<<"/"<<  systHist_[0][6]->Integral()  / hist_[6]->Integral()  <<endl;
//    datacard_<<" jer      shape"<<setw(7)<< 
//      "1"<<setw(10)<<//systHist_[5][1]->Integral()  / hist_[1]->Integral()<<"/"<<  systHist_[4][1]->Integral()  / hist_[1]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[5][3]->Integral()  / hist_[3]->Integral()<<"/"<<  systHist_[4][3]->Integral()  / hist_[3]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[5][4]->Integral()  / hist_[4]->Integral()<<"/"<<  systHist_[4][4]->Integral()  / hist_[4]->Integral()  <<setw(7)<<
//      "-"<<setw(10)<<//1.00<<setw(10)<<
//      "1"<<setw(10)<<//systHist_[5][8]->Integral()  / hist_[8]->Integral()<<"/"<<  systHist_[4][8]->Integral()  / hist_[8]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[5][7]->Integral()  / hist_[7]->Integral()<<"/"<<  systHist_[4][7]->Integral()  / hist_[7]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[5][5]->Integral()  / hist_[5]->Integral()<<"/"<<  systHist_[4][5]->Integral()  / hist_[5]->Integral()  <<setw(7)<<
//      "1"<<endl;//systHist_[5][6]->Integral()  / hist_[6]->Integral()<<"/"<<  systHist_[4][6]->Integral()  / hist_[6]->Integral()  <<endl;
//    datacard_<<" umet      shape"<<setw(7)<< 
//      "1"<<setw(10)<<//systHist_[3][1]->Integral()  / hist_[1]->Integral()<<"/"<<  systHist_[2][1]->Integral()  / hist_[1]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[3][3]->Integral()  / hist_[3]->Integral()<<"/"<<  systHist_[2][3]->Integral()  / hist_[3]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[3][4]->Integral()  / hist_[4]->Integral()<<"/"<<  systHist_[2][4]->Integral()  / hist_[4]->Integral()  <<setw(7)<<
//      "-"<<setw(10)<<//1.00<<setw(10)<<
//      "1"<<setw(10)<<//systHist_[3][8]->Integral()  / hist_[8]->Integral()<<"/"<<  systHist_[2][8]->Integral()  / hist_[8]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[3][7]->Integral()  / hist_[7]->Integral()<<"/"<<  systHist_[2][7]->Integral()  / hist_[7]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[3][5]->Integral()  / hist_[5]->Integral()<<"/"<<  systHist_[2][5]->Integral()  / hist_[5]->Integral()  <<setw(7)<<
//      "1"<<endl;//systHist_[3][6]->Integral()  / hist_[6]->Integral()<<"/"<<  systHist_[2][6]->Integral()  / hist_[6]->Integral()  <<endl;
//    datacard_<<" leptEff        lnN"<<setw(7)<<1.02<<setw(10)<<1.02<<setw(10)<<1.02<<setw(10)<<1.00<<setw(10)<<1.02<<setw(10)<<1.02<<setw(10)<<1.02<<setw(10)<<1.02<<endl;
//    datacard_<<" btag      shape"<<setw(7)<< 
//      "1"<<setw(10)<<//systHist_[7][1]->Integral()  / hist_[1]->Integral()<<"/"<<  systHist_[6][1]->Integral()  / hist_[1]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[7][3]->Integral()  / hist_[3]->Integral()<<"/"<<  systHist_[6][3]->Integral()  / hist_[3]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[7][4]->Integral()  / hist_[4]->Integral()<<"/"<<  systHist_[6][4]->Integral()  / hist_[4]->Integral()  <<setw(7)<<
//      "-"<<setw(10)<<//1.00<<setw(10)<<
//      "1"<<setw(10)<<//systHist_[7][8]->Integral()  / hist_[8]->Integral()<<"/"<<  systHist_[6][8]->Integral()  / hist_[8]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[7][7]->Integral()  / hist_[7]->Integral()<<"/"<<  systHist_[6][7]->Integral()  / hist_[7]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[7][5]->Integral()  / hist_[5]->Integral()<<"/"<<  systHist_[6][5]->Integral()  / hist_[5]->Integral()  <<setw(7)<<
//      "1"<<endl;//systHist_[7][6]->Integral()  / hist_[6]->Integral()<<"/"<<  systHist_[6][6]->Integral()  / hist_[6]->Integral()  <<endl;
//    datacard_<<" unbtag      shape"<<setw(7)<< 
//      "1"<<setw(10)<<//systHist_[9][1]->Integral()  / hist_[1]->Integral()<<"/"<<  systHist_[8][1]->Integral()  / hist_[1]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[9][3]->Integral()  / hist_[3]->Integral()<<"/"<<  systHist_[8][3]->Integral()  / hist_[3]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[9][4]->Integral()  / hist_[4]->Integral()<<"/"<<  systHist_[8][4]->Integral()  / hist_[4]->Integral()  <<setw(7)<<
//      "-"<<setw(10)<<//1.00<<setw(10)<<
//      "1"<<setw(10)<<//systHist_[9][8]->Integral()  / hist_[8]->Integral()<<"/"<<  systHist_[8][8]->Integral()  / hist_[8]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[9][7]->Integral()  / hist_[7]->Integral()<<"/"<<  systHist_[8][7]->Integral()  / hist_[7]->Integral()  <<setw(7)<<
//      "1"<<setw(10)<<//systHist_[9][5]->Integral()  / hist_[5]->Integral()<<"/"<<  systHist_[8][5]->Integral()  / hist_[5]->Integral()  <<setw(7)<<
//      "1"<<endl;//systHist_[9][6]->Integral()  / hist_[6]->Integral()<<"/"<<  systHist_[8][6]->Integral()  / hist_[6]->Integral()  <<endl;
//    // FIXME: NoNorm??
//    datacard_<<" "<<sampleName_[1]<<"_Stat     shape"<<setw(7)<<
//      "1"<<setw(10)<<//histStatNoNorm_[1]->Integral()  / hist_[1]->Integral()<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<endl;
//    datacard_<<" tt_ltau_Stat  shape"<<setw(7)<<
//      "-"<<setw(10)<<
//      "1"<<setw(10)<<//histStatNoNorm_[3]->Integral()  / hist_[3]->Integral()<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<endl;
//    datacard_<<" tt_ll_Stat    shape"<<setw(7)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "1"<<setw(10)<<//histStatNoNorm_[4]->Integral()  / hist_[4]->Integral()<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<endl;
//    datacard_<<" Z_eemumu_Stat     shape"<<setw(7)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "1"<<setw(10)<<//histStatNoNorm_[8]->Integral()  / hist_[8]->Integral()<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<endl;
//    datacard_<<" Z_tautau_Stat     shape"<<setw(7)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "1"<<setw(10)<<//histStatNoNorm_[7]->Integral()  / hist_[7]->Integral()<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<endl;
//    datacard_<<" singleTop_Stat   shape"<<setw(7)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "1"<<setw(10)<<
//      "-"<<endl;
//    datacard_<<" di_boson_Stat     shape"<<setw(7)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "-"<<setw(10)<<
//      "1"<<endl; //histStatNoNorm_[6]->Integral()  / hist_[6]->Integral()<<endl;
//    datacard_<<" wjetsCrossSection     lnN"<<setw(7)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<endl;
//    datacard_<<" singletopCrossSection lnN"<<setw(7)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.08<<setw(10)<<1.00<<setw(10)<<endl;
//    datacard_<<" zllCrossSection       lnN"<<setw(7)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.04<<setw(10)<<1.04<<setw(10)<<1.00<<setw(10)<<1.00<<endl;
//    datacard_<<" dibosonCrossSection   lnN"<<setw(7)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.04<<endl;
//    datacard_<<" lumi_8TeV                lnN"<<setw(7)<<1.026<<setw(10)<<1.026<<setw(10)<<1.026<<setw(10)<<1.00<<setw(10)<<1.026<<setw(10)<<1.026<<setw(10)<<1.026<<setw(10)<<1.026<<endl;
//    
//    datacard_<<" pu               lnN"<<setw(7)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<endl;//"    pileup"<<endl;
//    
//    
//    // Write and close datacard_
//    datacard_.close();
//    //    break;
//    }
// 
   else
    {

      stringstream sidx;
      sidx<<ibin;
      string binlabel(string("bin")+sidx.str());
      


      cout << "Bin label: " << binlabel<<endl;
      testSumBins+=hist_[0]->GetBinContent(ibin); cout << "Testsumbins: " << testSumBins<<endl;
      datacard_.open (   (outFolder_+datacardsBaseName_+string("_")+massPointName_[currentMassPoint_] +string("_")+binlabel+string("_mutau.txt") ).c_str()     ); 
      datacard_<< fixed << showpoint <<setprecision(3); // fixed point, show 3 decimals
      datacard_<<"Data: "<<t->tm_mday<<"/"<<t->tm_mon+1<<"/"<<t->tm_year+1900<<endl;
      datacard_<<"Description: H+, mu-tau, btag multiplicity bin "<<binlabel<<", mass "<<massPointName_[currentMassPoint_]<<" GeV, lumi=" << commondefinitions::LUM_<<" pb-1"<<endl;
      datacard_<<"---------------------------------------------------------------------------------------------------------------------"<<endl;
      datacard_<<"imax   1  number of channels"<<endl;
      datacard_<<"jmax   *  number of backgrounds"<<endl;
      datacard_<<"kmax   *  number of nuisance parameters"<<endl;
      datacard_<<"---------------------------------------------------------------------------------------------------------------------"<<endl;
      datacard_<<"bin a"<<endl;
      datacard_<<"observation    "<<  hist_[0]->GetBinContent(ibin) <<endl; // data yield
      datacard_<<"---------------------------------------------------------------------------------------------------------------------"<<endl;
      datacard_<<"bin              a          a          a           a           a           a           a         a         "<<endl;
      datacard_<<"process          "<<sampleName_[1]<<"   "<<sampleName_[3]<<"   "<<sampleName_[4]<<"   "<<sampleName_[2]<<"   "<<sampleName_[8]<<"   "<<sampleName_[7]<<"   "<<sampleName_[5]<<"   "<<sampleName_[6]<<endl; // Order matching values below
      datacard_<<"process          0          1          2           3           4           5           6         7         "<<endl;
      datacard_<<"---------------------------------------------------------------------------------------------------------------------"<<endl;
      datacard_<<"rate          "<<
	hist_[1]->GetBinContent(ibin)<<setw(10)<<
	hist_[3]->GetBinContent(ibin)<<setw(10)<<
	hist_[4]->GetBinContent(ibin)<<setw(10)<<
	hist_[2]->GetBinContent(ibin)<<setw(10)<<
	hist_[8]->GetBinContent(ibin)<<setw(10)<<
	hist_[7]->GetBinContent(ibin)<<setw(10)<<
	hist_[5]->GetBinContent(ibin)<<setw(10)<<
	hist_[6]->GetBinContent(ibin)<<endl;
      datacard_<<"---------------------------------------------------------------------------------------------------------------------"<<endl;
      datacard_<<endl;
      datacard_<<" tauId  lnN"<<setw(7)<<1.06<<setw(10)<<1.06<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.06<<setw(10)<<1.06<<setw(10)<<1.06<<endl;
      datacard_<<" jetTauMisId    lnN"<<setw(7)<<1.00<<setw(10)<<1.00<<setw(10)<<1.15<<setw(10)<<1.00<<setw(10)<<1.15<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<endl;
      datacard_<<" fakesSyst      lnN"<<setw(7)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.10<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<endl; // Take from shapes?
      datacard_<<" fakesStat      lnN"<<setw(7)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<endl; // FIXME?
      datacard_<<" jes      lnN"<<setw(7)<< 
	systHist_[1][1]->GetBinContent(ibin)  / hist_[1]->GetBinContent(ibin)<<"/"<<  systHist_[0][1]->GetBinContent(ibin)  / hist_[1]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[1][3]->GetBinContent(ibin)  / hist_[3]->GetBinContent(ibin)<<"/"<<  systHist_[0][3]->GetBinContent(ibin)  / hist_[3]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[1][4]->GetBinContent(ibin)  / hist_[4]->GetBinContent(ibin)<<"/"<<  systHist_[0][4]->GetBinContent(ibin)  / hist_[4]->GetBinContent(ibin)  <<setw(7)<<
	1.00<<setw(10)<<
	systHist_[1][8]->GetBinContent(ibin)  / hist_[8]->GetBinContent(ibin)<<"/"<<  systHist_[0][8]->GetBinContent(ibin)  / hist_[8]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[1][7]->GetBinContent(ibin)  / hist_[7]->GetBinContent(ibin)<<"/"<<  systHist_[0][7]->GetBinContent(ibin)  / hist_[7]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[1][5]->GetBinContent(ibin)  / hist_[5]->GetBinContent(ibin)<<"/"<<  systHist_[0][5]->GetBinContent(ibin)  / hist_[5]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[1][6]->GetBinContent(ibin)  / hist_[6]->GetBinContent(ibin)<<"/"<<  systHist_[0][6]->GetBinContent(ibin)  / hist_[6]->GetBinContent(ibin)  <<endl;
      datacard_<<" jer      lnN"<<setw(7)<< 
	systHist_[5][1]->GetBinContent(ibin)  / hist_[1]->GetBinContent(ibin)<<"/"<<  systHist_[4][1]->GetBinContent(ibin)  / hist_[1]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[5][3]->GetBinContent(ibin)  / hist_[3]->GetBinContent(ibin)<<"/"<<  systHist_[4][3]->GetBinContent(ibin)  / hist_[3]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[5][4]->GetBinContent(ibin)  / hist_[4]->GetBinContent(ibin)<<"/"<<  systHist_[4][4]->GetBinContent(ibin)  / hist_[4]->GetBinContent(ibin)  <<setw(7)<<
	1.00<<setw(10)<<
	systHist_[5][8]->GetBinContent(ibin)  / hist_[8]->GetBinContent(ibin)<<"/"<<  systHist_[4][8]->GetBinContent(ibin)  / hist_[8]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[5][7]->GetBinContent(ibin)  / hist_[7]->GetBinContent(ibin)<<"/"<<  systHist_[4][7]->GetBinContent(ibin)  / hist_[7]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[5][5]->GetBinContent(ibin)  / hist_[5]->GetBinContent(ibin)<<"/"<<  systHist_[4][5]->GetBinContent(ibin)  / hist_[5]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[5][6]->GetBinContent(ibin)  / hist_[6]->GetBinContent(ibin)<<"/"<<  systHist_[4][6]->GetBinContent(ibin)  / hist_[6]->GetBinContent(ibin)  <<endl;
      datacard_<<" met      lnN"<<setw(7)<< 
	systHist_[3][1]->GetBinContent(ibin)  / hist_[1]->GetBinContent(ibin)<<"/"<<  systHist_[2][1]->GetBinContent(ibin)  / hist_[1]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[3][3]->GetBinContent(ibin)  / hist_[3]->GetBinContent(ibin)<<"/"<<  systHist_[2][3]->GetBinContent(ibin)  / hist_[3]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[3][4]->GetBinContent(ibin)  / hist_[4]->GetBinContent(ibin)<<"/"<<  systHist_[2][4]->GetBinContent(ibin)  / hist_[4]->GetBinContent(ibin)  <<setw(7)<<
	1.00<<setw(10)<<
	systHist_[3][8]->GetBinContent(ibin)  / hist_[8]->GetBinContent(ibin)<<"/"<<  systHist_[2][8]->GetBinContent(ibin)  / hist_[8]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[3][7]->GetBinContent(ibin)  / hist_[7]->GetBinContent(ibin)<<"/"<<  systHist_[2][7]->GetBinContent(ibin)  / hist_[7]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[3][5]->GetBinContent(ibin)  / hist_[5]->GetBinContent(ibin)<<"/"<<  systHist_[2][5]->GetBinContent(ibin)  / hist_[5]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[3][6]->GetBinContent(ibin)  / hist_[6]->GetBinContent(ibin)<<"/"<<  systHist_[2][6]->GetBinContent(ibin)  / hist_[6]->GetBinContent(ibin)  <<endl;
      datacard_<<" leptEff        lnN"<<setw(7)<<1.02<<setw(10)<<1.02<<setw(10)<<1.02<<setw(10)<<1.00<<setw(10)<<1.02<<setw(10)<<1.02<<setw(10)<<1.02<<setw(10)<<1.02<<endl;
      datacard_<<" btagging      lnN"<<setw(7)<< 
	systHist_[7][1]->GetBinContent(ibin)  / hist_[1]->GetBinContent(ibin)<<"/"<<  systHist_[6][1]->GetBinContent(ibin)  / hist_[1]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[7][3]->GetBinContent(ibin)  / hist_[3]->GetBinContent(ibin)<<"/"<<  systHist_[6][3]->GetBinContent(ibin)  / hist_[3]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[7][4]->GetBinContent(ibin)  / hist_[4]->GetBinContent(ibin)<<"/"<<  systHist_[6][4]->GetBinContent(ibin)  / hist_[4]->GetBinContent(ibin)  <<setw(7)<<
	1.00<<setw(10)<<
	systHist_[7][8]->GetBinContent(ibin)  / hist_[8]->GetBinContent(ibin)<<"/"<<  systHist_[6][8]->GetBinContent(ibin)  / hist_[8]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[7][7]->GetBinContent(ibin)  / hist_[7]->GetBinContent(ibin)<<"/"<<  systHist_[6][7]->GetBinContent(ibin)  / hist_[7]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[7][5]->GetBinContent(ibin)  / hist_[5]->GetBinContent(ibin)<<"/"<<  systHist_[6][5]->GetBinContent(ibin)  / hist_[5]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[7][6]->GetBinContent(ibin)  / hist_[6]->GetBinContent(ibin)<<"/"<<  systHist_[6][6]->GetBinContent(ibin)  / hist_[6]->GetBinContent(ibin)  <<endl;
      datacard_<<" bmistagging      lnN"<<setw(7)<< 
	systHist_[9][1]->GetBinContent(ibin)  / hist_[1]->GetBinContent(ibin)<<"/"<<  systHist_[8][1]->GetBinContent(ibin)  / hist_[1]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[9][3]->GetBinContent(ibin)  / hist_[3]->GetBinContent(ibin)<<"/"<<  systHist_[8][3]->GetBinContent(ibin)  / hist_[3]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[9][4]->GetBinContent(ibin)  / hist_[4]->GetBinContent(ibin)<<"/"<<  systHist_[8][4]->GetBinContent(ibin)  / hist_[4]->GetBinContent(ibin)  <<setw(7)<<
	1.00<<setw(10)<<
	systHist_[9][8]->GetBinContent(ibin)  / hist_[8]->GetBinContent(ibin)<<"/"<<  systHist_[8][8]->GetBinContent(ibin)  / hist_[8]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[9][7]->GetBinContent(ibin)  / hist_[7]->GetBinContent(ibin)<<"/"<<  systHist_[8][7]->GetBinContent(ibin)  / hist_[7]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[9][5]->GetBinContent(ibin)  / hist_[5]->GetBinContent(ibin)<<"/"<<  systHist_[8][5]->GetBinContent(ibin)  / hist_[5]->GetBinContent(ibin)  <<setw(7)<<
	systHist_[9][6]->GetBinContent(ibin)  / hist_[6]->GetBinContent(ibin)<<"/"<<  systHist_[8][6]->GetBinContent(ibin)  / hist_[6]->GetBinContent(ibin)  <<endl;
      // FIXME: NoNorm??
      datacard_<<" tbhStatistics     lnN"<<setw(7)<<
	histStatNoNorm_[1]->GetBinContent(ibin)  / hist_[1]->GetBinContent(ibin)<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<endl;
      datacard_<<" ttltauStatistics  lnN"<<setw(7)<<
	1.00<<setw(10)<<
	histStatNoNorm_[3]->GetBinContent(ibin)  / hist_[3]->GetBinContent(ibin)<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<endl;
      datacard_<<" ttllStatistics    lnN"<<setw(7)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	histStatNoNorm_[4]->GetBinContent(ibin)  / hist_[4]->GetBinContent(ibin)<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<endl;
      datacard_<<" zeemumuStatistics     lnN"<<setw(7)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	histStatNoNorm_[8]->GetBinContent(ibin)  / hist_[8]->GetBinContent(ibin)<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<endl;
      datacard_<<" ztautauStatistics     lnN"<<setw(7)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	histStatNoNorm_[7]->GetBinContent(ibin)  / hist_[7]->GetBinContent(ibin)<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<endl;
      datacard_<<" ztautauEmbeddedSimul     lnN"<<setw(7)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.50<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<endl;
      datacard_<<" singleTopStatistics   lnN"<<setw(7)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	histStatNoNorm_[5]->GetBinContent(ibin)  / hist_[5]->GetBinContent(ibin)<<setw(10)<<
	1.00<<endl;
      datacard_<<" dibosonStatistics     lnN"<<setw(7)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	1.00<<setw(10)<<
	histStatNoNorm_[6]->GetBinContent(ibin)  / hist_[6]->GetBinContent(ibin)<<endl;
      datacard_<<" wjetsCrossSection     lnN"<<setw(7)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<endl;
      datacard_<<" singletopCrossSection lnN"<<setw(7)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.08<<setw(10)<<1.00<<setw(10)<<endl;
      datacard_<<" zllCrossSection       lnN"<<setw(7)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.04<<setw(10)<<1.04<<setw(10)<<1.00<<setw(10)<<1.00<<endl;
      datacard_<<" dibosonCrossSection   lnN"<<setw(7)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.04<<endl;
      datacard_<<" lumiErr                lnN"<<setw(7)<<1.026<<setw(10)<<1.026<<setw(10)<<1.026<<setw(10)<<1.00<<setw(10)<<1.026<<setw(10)<<1.026<<setw(10)<<1.026<<setw(10)<<1.026<<endl;
      
      datacard_<<" pileupErr               lnN"<<setw(7)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<setw(10)<<1.00<<endl;//"    pileup"<<endl;
      
      datacard_.close();    // Write and close datacard_
    }
    
  } // End switch on bin to use
  
  cout<<"***********************************************"<<
    "*************** INT: "<<testIntegral<<"*************"<<endl<<
    "*************** SUM: "<<testSumBins<<"*************"<<endl;
  
  
}

void LandSShapesProducer::Produce(){
  
  for(size_t s=0; s< nMassPoints_; s++){
    
    InitMassPoint(s);
    
    cout << "=============================================================================" << endl;
    cout << "============================= MASS POINT: " << s << " ===============================" << endl;
    cout << "=============================================================================" << endl;
    for(size_t i = 0; i< nVars_; i++){
      cout << "============================= PROCESSING VARIABLE: " << fitVars_[i]->getVarName() << " ===============================" << endl;
      InitPerVariableAmbient(i);
      BuildDatasets(i);
      if(!doMultiDimensionalShapes_)
	DrawTemplates(i);
    }
    if(doMultiDimensionalShapes_)
      UnrollMultiDimensionalShape();
    cout << "============================= MASS POINT: " << s << " unrolled stuff ===============================" << endl;
    //    StorePerMassSignalShapes();
  }
  //  cout.rdbuf(old); // restore cout  
}
