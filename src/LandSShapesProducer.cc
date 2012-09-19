#include "LIP/TopTaus/interface/TauDileptonPDFBuilderFitter.hh"

// System includes
#include <sstream>

// CMSSW includes
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// ROOT includes
#include "RooMinuit.h"

using namespace std;
using namespace RooFit;

FitVar::FitVar(string varName, double min, double max, double bins, double hmin, double hmax, Int_t unbinned, Int_t smoothOrder):
  varName_(varName),
  min_(min),
  max_(max),
  bins_(bins),
  hmin_(hmin),
  hmax_(hmax),
  unbinned_(unbinned),
  smoothOrder_(smoothOrder)
{
  
}

string FitVar::getVarName(){
  return varName_;
}

double FitVar::getMin(){
  return min_;
}

double FitVar::getMax(){
  return max_;
}

int FitVar::getBins(){
  return bins_;
}

double FitVar::getHmin(){
  return hmin_;
}

double FitVar::getHmax(){
  return hmax_;
}

Int_t FitVar::getUnbinned(){
  return unbinned_;
}

Int_t FitVar::getSmoothOrder(){
  return smoothOrder_;
}


TauDileptonPDFBuilderFitter::TauDileptonPDFBuilderFitter(string parSet):
  parSet_(parSet)
{
  
  using namespace std; 
  using namespace RooFit ;
  
  Init();

  SetOptions();

}

TauDileptonPDFBuilderFitter::~TauDileptonPDFBuilderFitter(){
  
  delete myStyle_;
  delete signalFileWH_;
  delete signalFileHH_;
  delete ddBkgFile_;
  delete ttbarmcBkgFile_;
  delete mcBkgFile_;
  delete dataFile_;
  delete signalTreeWH_;
  delete signalTreeHH_; 
  delete ddBkgTree_ ;
  delete ttbarmcBkgTree_;
  delete mcBkgTree_;
  delete dataTree_;
  delete canvas_;
  delete sigVar_             ;              
  delete sigMeanVar_         ;              
  delete sigSigmaVar_        ;              
  delete ttbarmcbkgVar_      ;
  delete ttbarmcbkgMeanVar_  ;
  delete ttbarmcbkgSigmaVar_ ;
  delete mcbkgVar_           ;               
  delete mcbkgMeanVar_       ;         
  delete mcbkgSigmaVar_      ;         
  delete ddbkgVar_           ;              
  delete ddbkgMeanVar_       ;         
  delete ddbkgSigmaVar_      ;        
  delete myvar_         ;
  delete myvar_weights_ ;
  delete isOSvar_       ;
  delete mySignalDS_      ;
  delete unrMyDDBkgDS_      ; // Unreduced datasets for OS cut
  delete unrMyTTBARMCBkgDS_ ;
  delete unrMyMCBkgDS_      ; 
  delete unrMyDataDS_       ; 
  delete myDDBkgDS_      ; // Reduced datasets
  delete myTTBARMCBkgDS_ ;
  delete myMCBkgDS_      ; 
  delete myDataDS_       ; 
  delete signalHisto_    ;
  delete ttbarmcbkgHisto_;
  delete mcbkgHisto_     ;
  delete ddbkgHisto_     ;
  delete dataHisto_      ;
  delete b_signalModel_    ;
  delete b_ddbkgModel_     ;
  delete b_ttbarmcbkgModel_;  
  delete b_mcbkgModel_     ;  
  delete u_signalModel_    ; 
  delete u_ddbkgModel_     ;  
  delete u_ttbarmcbkgModel_;  
  delete u_mcbkgModel_     ;  
  delete signalHist_;
  delete ddbkgHist_;
  delete ttbarmcbkgHist_;
  delete mcbkgHist_;
  delete leg_;
  delete signalConstraint_;
  delete ttbarmcbkgConstraint_;
  delete ddbkgConstraint_;
  delete mcbkgConstraint_;
  delete sumModel_;
  delete model_;
  delete constrainedModelFit_;
  delete myFrame_;
  delete nll_;
  for(size_t i=0; i<nVars_;i++)
    delete likelihoodVector_[i];
  delete myNllFitResult_;
  delete contourPlot_;
  delete combll_;

}

  
void TauDileptonPDFBuilderFitter::Init(){

  // Clear vectors
  nVars_ = 0;
  fitVars_.clear();
  vars_.clear();
  mins_.clear(); 
  maxs_.clear();
  bins_.clear();
  hmin_.clear();
  hmax_.clear();
  unbinned_.clear(); 
  smoothOrder_.clear();

  likelihoodVector_.clear();
  // Init RooFit variables
  sigVar_             = 0;              
  sigMeanVar_         = 0;              
  sigSigmaVar_        = 0;              
  ttbarmcbkgVar_      = 0;
  ttbarmcbkgMeanVar_  = 0;
  ttbarmcbkgSigmaVar_ = 0;
  mcbkgVar_           = 0;               
  mcbkgMeanVar_       = 0;         
  mcbkgSigmaVar_      = 0;         
  ddbkgVar_           = 0;              
  ddbkgMeanVar_       = 0;         
  ddbkgSigmaVar_      = 0;        

  identifier_ = "";
  
  // Get ParameterSet from cfg file
  const edm::ParameterSet &mFitPars = edm::readPSetsFrom(parSet_)->getParameter<edm::ParameterSet>("mFitPars");

  outFolder_        = mFitPars.getParameter<std::string>("outFolder");
  resultsFileName_  = mFitPars.getParameter<std::string>("resultsFileName");
  baseMCDir_        = mFitPars.getParameter<std::string>("baseMCDir");
  baseDataDir_      = mFitPars.getParameter<std::string>("baseDataDir");
  
  signalFileNameWH_   = mFitPars.getParameter<std::string>("signalFileNameWH");
  signalFileNameHH_   = mFitPars.getParameter<std::string>("signalFileNameHH");
  dataFileName_       = mFitPars.getParameter<std::string>("dataFileName");
  ddBkgFileName_      = mFitPars.getParameter<std::string>("ddBkgFileName");
  ttbarmcBkgFileName_ = mFitPars.getParameter<std::string>("ttbarmcBkgFileName");
  mcBkgFileName_      = mFitPars.getParameter<std::string>("mcBkgFileName");
  
  minitreeSelected_   = mFitPars.getParameter<std::string>("minitreeSelected");
  minitreeDataDriven_ = mFitPars.getParameter<std::string>("minitreeDataDriven");
  
  vector<int> tempFitType;
  tempFitType.clear();
  tempFitType     = mFitPars.getParameter<vector<int> >("fitType");
  for(size_t k=0; k<tempFitType.size(); k++){
    if(tempFitType[k]==SM2BKG) fitType_.push_back(SM2BKG);
    if(tempFitType[k]==SM3BKG) fitType_.push_back(SM3BKG);
    if(tempFitType[k]==HIGGS2BKG) fitType_.push_back(HIGGS2BKG);
    if(tempFitType[k]==HIGGS3BKG) fitType_.push_back(HIGGS3BKG);
  }

  vars_        = mFitPars.getParameter<vector<string> >("vars");
  mins_        = mFitPars.getParameter<vector<double> >("mins");
  maxs_        = mFitPars.getParameter<vector<double> >("maxs");
  bins_        = mFitPars.getParameter<vector<int> >("bins");
  hmin_        = mFitPars.getParameter<vector<double> >("hmin");
  hmax_        = mFitPars.getParameter<vector<double> >("hmax");
  unbinned_    = mFitPars.getParameter<vector<Int_t> >("unbinned");
  smoothOrder_ = mFitPars.getParameter<vector<Int_t> >("smoothOrder");
  
  
  // Open files and get trees
  // ddBkg is the only to be taken from data driven estimation (tree)
  signalFileWH_   = TFile::Open(baseMCDir_   + signalFileNameWH_  ); signalTreeWH_   = (TTree*) signalFileWH_  ->Get(minitreeSelected_);
  signalFileHH_   = TFile::Open(baseMCDir_   + signalFileNameHH_  ); signalTreeHH_   = (TTree*) signalFileHH_  ->Get(minitreeSelected_);
  ddBkgFile_      = TFile::Open(baseDataDir_   + ddBkgFileName_     ); ddBkgTree_      = (TTree*) ddBkgFile_     ->Get(minitreeDataDriven_);
  ttbarmcBkgFile_ = TFile::Open(baseMCDir_   + ttbarmcBkgFileName_); ttbarmcBkgTree_ = (TTree*) ttbarmcBkgFile_->Get(minitreeSelected_);
  mcBkgFile_      = TFile::Open(baseMCDir_   + mcBkgFileName_     ); mcBkgTree_      = (TTree*) mcBkgFile_     ->Get(minitreeSelected_);
  dataFile_       = TFile::Open(baseDataDir_ + dataFileName_      ); dataTree_       = (TTree*) dataFile_      ->Get(minitreeSelected_);


  // Set variables
  //
  nVars_ = vars_.size();
  
  for(size_t i=0; i<nVars_; i++)
    {
      FitVar myVar(vars_[i], mins_[i], maxs_[i], bins_[i], hmin_[i], hmax_[i], unbinned_[i], smoothOrder_[i]);
      fitVars_.push_back(myVar);
    }
  
  // Set canvas
  canvas_ = new TCanvas("canvas","My plots ",0,0,1000,500);
  canvas_->cd();
  
  resultsFile_.open ((outFolder_+resultsFileName_).c_str());
 
  //  Uncomment following line in order to redirect stdout to file
  //  streamToFile_ = std::cout.rdbuf(resultsFile_.rdbuf());
  
  cout << "Init process complete" << endl;
}

void TauDileptonPDFBuilderFitter::SetOptions(){
  myStyle_->SetOptStat(0);
}

void TauDileptonPDFBuilderFitter::InitFitSettings(size_t f){
  baseIdentifier_="";
  
  // FIXME: hardcoded
  signalStatError_=8.3; // 6.7; // 8.3 and 0.2 propagation // 6.7 (wh) and 0.2 (hh) propagation  
  ddbkgEstimate_ = 207.85; //165.4;
  ddbkgStatError_ = 49.52; //23.7;
  
  switch(fitType_[f]){
  case SM2BKG :
    includeSignal_=false;
    standaloneTTbar_=false;
    baseIdentifier_.append("SM2BKG");
    mcbkgStatError_ = 6.09; //3.48;
    break;
  case SM3BKG:
    includeSignal_=false;
    standaloneTTbar_=true;
    baseIdentifier_.append("SM3BKG");
    ttbarmcbkgStatError_ = 3.4; // 3.0
    mcbkgStatError_ = 5.05; // 1.76
    break;
  case HIGGS2BKG:
    includeSignal_=true;
    standaloneTTbar_=false;
    baseIdentifier_.append("HIGGS2BKG");
    mcbkgStatError_ = 6.09; // 3.48;
    break;
  case HIGGS3BKG:
    includeSignal_=true;
    standaloneTTbar_=true;
    baseIdentifier_.append("HIGGS3BKG");
    ttbarmcbkgStatError_ = 3.4; //3.0;
    mcbkgStatError_ = 5.05; //1.76;
    break;
  default : // Dummy - should never arrive here
    cout<<"Type of fit not available. Check your options motherfucker"<<endl;
  }

  likelihoodVector_.clear();
}

void TauDileptonPDFBuilderFitter::InitPerVariableAmbient(size_t i){
  
  // String identifiers
  switch(fitVars_[i].getUnbinned()){
  case 1 : // Unbinned
    identifier_=baseIdentifier_+string("_unbinned_")+fitVars_[i].getVarName();
    break;
  case 0:  // Binned (w/ or w/out smoothing)
    identifier_=baseIdentifier_+string("_binned_")+fitVars_[i].getVarName();
    break;
  default : // Dummy - should never arrive here
    cout<<"Neither binned not unbinned. Check your options motherfucker."<<endl;
  }
  
  // Binned fit variable
  myvar_           = new RooRealVar(fitVars_[i].getVarName().c_str(), fitVars_[i].getVarName().c_str(), fitVars_[i].getMin(), fitVars_[i].getMax());  myvar_->setBins(fitVars_[i].getBins()); 
  myvar_weights_   = new RooRealVar("weight","weight",0,1000);
  isOSvar_         = new RooRealVar("is_os","is_os",0,2);
  

  //Define data sets /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  mySignalDSName_= fitVars_[i].getVarName().c_str() + string("_mySignalDS");
  myDDBkgDSName_ = fitVars_[i].getVarName().c_str() + string("_myDDBkgDS");
  myTTBARMCBkgDSName_ = fitVars_[i].getVarName().c_str() + string("_myTTBARMCBkgDS");
  myMCBkgDSName_  = fitVars_[i].getVarName().c_str() + string("_myMCBkgDS");
  myDataDSName_   = fitVars_[i].getVarName().c_str() + string("_myDataDS");
  mySignalDS_        = 0; //= new RooDataSet(mySignalDSName.c_str(),mySignalDSName.c_str(), signalTree_, RooArgSet(*myvar,*myvar_weights),0,"weight" );
  unrMyDDBkgDS_      = 0; //= new RooDataSet(myDDBkgDSName.c_str(), myDDBkgDSName.c_str(),  ddBkgTree_,  RooArgSet(*myvar,*myvar_weights),0,"weight" );
  unrMyTTBARMCBkgDS_ = 0; //= new RooDataSet(myMCBkgDSName.c_str(), myMCBkgDSName.c_str(),  mcBkgTree_,  RooArgSet(*myvar,*myvar_weights),0,"weight" );
  unrMyMCBkgDS_      = 0; //= new RooDataSet(myMCBkgDSName.c_str(), myMCBkgDSName.c_str(),  mcBkgTree_,  RooArgSet(*myvar,*myvar_weights),0,"weight" );
  unrMyDataDS_       = 0; //= new RooDataSet(myDataDSName.c_str(),  myDataDSName.c_str(),   dataTree_,   *myvar );
  myDDBkgDS_      = 0; 
  myTTBARMCBkgDS_ = 0; 
  myMCBkgDS_      = 0; 
  myDataDS_       = 0; 

  // Define model names
  signalModelName_           = fitVars_[i].getVarName() + string("_SignalModel");
  signalConstrainedName_       = fitVars_[i].getVarName() + string("_signalConstrained");    signalConstraintName_ =fitVars_[i].getVarName()+string("_signalConstraint");
  ddbkgModelName_            = fitVars_[i].getVarName() + string("_ddbkgModel");  
  ddbkgConstrainedName_        = fitVars_[i].getVarName() + string("_ddbkgConstrained");    ddbkgConstraintName_ =fitVars_[i].getVarName()+string("_ddbkgConstraint");
  ttbarmcbkgModelName_       = fitVars_[i].getVarName() + string("_ttbarmcbkgModel");  
  ttbarmcbkgConstrainedName_   = fitVars_[i].getVarName() + string("_ttbarmcbkgConstrained");   ttbarmcbkgConstraintName_ =fitVars_[i].getVarName()+string("_ttbarmcbkgConstraint");
  mcbkgModelName_            = fitVars_[i].getVarName() + string("_mcbkgModel");  
  mcbkgConstrainedName_        = fitVars_[i].getVarName() + string("_mcbkgConstrained");   mcbkgConstraintName_ =fitVars_[i].getVarName()+string("_mcbkgConstraint");
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  // Define var names
  signalVarName_     =fitVars_[i].getVarName()+string("_signalVar");string signalMeanVarName  =fitVars_[i].getVarName()+string("_signalMeanVar");              string signalSigmaVarName     =fitVars_[i].getVarName()+string("_signalSigmaVar");
  ddbkgVarName_      =fitVars_[i].getVarName()+string("_ddbkgVar"); string ddbkgMeanVarName  =fitVars_[i].getVarName()+string("_ddbkgMeanVar");                string ddbkgSigmaVarName      =fitVars_[i].getVarName()+string("_ddbkgSigmaVar");
  ttbarmcbkgVarName_ =fitVars_[i].getVarName()+string("_ttbarmcbkgVar"); string ttbarmcbkgMeanVarName  =fitVars_[i].getVarName()+string("_ttbarmcbkgMeanVar"); string ttbarmcbkgSigmaVarName =fitVars_[i].getVarName()+string("_ttbarmcbkgSigmaVar");
  mcbkgVarName_      =fitVars_[i].getVarName()+string("_mcbkgVar"); string mcbkgMeanVarName  =fitVars_[i].getVarName()+string("_mcbkgMeanVar");                string mcbkgSigmaVarName      =fitVars_[i].getVarName()+string("_mcbkgSigmaVar");

  // PDF models
  b_signalModel_     = 0;
  b_ddbkgModel_      = 0;
  b_ttbarmcbkgModel_ = 0;  
  b_mcbkgModel_      = 0;  
  u_signalModel_     = 0; 
  u_ddbkgModel_      = 0;  
  u_ttbarmcbkgModel_ = 0;  
  u_mcbkgModel_      = 0;  

  signalHist_     = 0;
  ddbkgHist_      = 0;
  ttbarmcbkgHist_ = 0;
  mcbkgHist_      = 0;
  leg_            = 0;
  
  signalConstraint_     = 0;
  ttbarmcbkgConstraint_ = 0;
  ddbkgConstraint_      = 0;
  mcbkgConstraint_      = 0;

  sumWeights_ = 0;
  
  sumModel_            = 0;
  model_               = 0;
  constrainedModelFit_ = 0;
  
  myFrame_ = 0;

  nll_ = 0;
  myNllFitResult_ = 0;
  contourPlot_ = 0;

  combll_ = 0;
}

void TauDileptonPDFBuilderFitter::BuildDatasets(size_t i){
  
  // Signal dataset from WH and HH
  if(includeSignal_){
    //  mySignalDS     = new RooDataSet(mySignalDSName.c_str(),mySignalDSName.c_str(), signalTree_, RooArgSet(*myvar,*myvar_weights),0,"weight" );
    mySignalDS_         = new RooDataSet(mySignalDSName_.c_str(),mySignalDSName_.c_str(),              RooArgSet(*myvar_,*myvar_weights_), "weight"); // This constructor does not accept the cut parameter
    
    // Temp variables for setting branch addresses
    double myVarAllocator, myVarWeightAllocator;
    double isOSsig;
    
    // Cross section
    // FIXME: hardcoded
    double fhh(0.05*0.05) , fhw( 2*(1-0.05)*0.05) ;      
    // Get WH events
    signalTreeWH_->SetBranchAddress(fitVars_[i].getVarName().c_str(), &myVarAllocator);
    signalTreeWH_->SetBranchAddress("weight", &myVarWeightAllocator);
    signalTreeWH_->SetBranchAddress("is_os", &isOSsig);
    for(unsigned int ev=0; ev<signalTreeWH_->GetEntries(); ev++){
      signalTreeWH_->GetEntry(ev);
      if(isOSsig<0.5) continue;
      myvar_->setVal(myVarAllocator);
      sumWeights_ += myVarWeightAllocator;
      myvar_weights_->setVal(fhw*myVarWeightAllocator);
      mySignalDS_->add(RooArgSet(*myvar_,*myvar_weights_),fhw*myVarWeightAllocator);
    }
    // Get HH events
    signalTreeHH_->SetBranchAddress(fitVars_[i].getVarName().c_str(), &myVarAllocator);
    signalTreeHH_->SetBranchAddress("weight", &myVarWeightAllocator);
    signalTreeHH_->SetBranchAddress("is_os", &isOSsig);
    //    cout << "getIsoS      ";
    for(unsigned int ev=0; ev<signalTreeHH_->GetEntries(); ev++){
      signalTreeHH_->GetEntry(ev);
      if(isOSsig < 0.5) continue;
      myvar_->setVal(myVarAllocator);
      sumWeights_ += myVarWeightAllocator;
      myvar_weights_->setVal(fhh*myVarWeightAllocator);
      mySignalDS_->add(RooArgSet(*myvar_,*myvar_weights_),fhh*myVarWeightAllocator);
    }
  }
  string myOsCut = "is_os>0.5";
  unrMyDDBkgDS_      = new RooDataSet(myDDBkgDSName_.c_str(), myDDBkgDSName_.c_str(),  ddBkgTree_,  RooArgSet(*myvar_,*myvar_weights_,*isOSvar_),myOsCut.c_str(),"weight" );
  myDDBkgDS_ = (RooDataSet*) unrMyDDBkgDS_->reduce(RooArgSet(*myvar_,*myvar_weights_));
  if(standaloneTTbar_){
    unrMyTTBARMCBkgDS_ = new RooDataSet(myTTBARMCBkgDSName_.c_str(), myTTBARMCBkgDSName_.c_str(),  ttbarmcBkgTree_,  RooArgSet(*myvar_,*myvar_weights_,*isOSvar_),myOsCut.c_str(),"weight" );
    myTTBARMCBkgDS_ = (RooDataSet*) unrMyTTBARMCBkgDS_->reduce(RooArgSet(*myvar_,*myvar_weights_));
  }
  unrMyMCBkgDS_      = new RooDataSet(myMCBkgDSName_.c_str(), myMCBkgDSName_.c_str(),  mcBkgTree_,  RooArgSet(*myvar_,*myvar_weights_,*isOSvar_),myOsCut.c_str(),"weight" );
  myMCBkgDS_ = (RooDataSet*) unrMyMCBkgDS_->reduce(RooArgSet(*myvar_,*myvar_weights_));
  unrMyDataDS_       = new RooDataSet(myDataDSName_.c_str(),  myDataDSName_.c_str(),   dataTree_,   RooArgSet(*myvar_,*isOSvar_), myOsCut.c_str() );
  myDataDS_ = (RooDataSet*) unrMyDataDS_->reduce(RooArgSet(*myvar_,*myvar_weights_));
  
  // Build binned clones
  if(includeSignal_)
    signalHisto_ = mySignalDS_->binnedClone();
  ddbkgHisto_ = myDDBkgDS_->binnedClone();
  if(standaloneTTbar_)
    ttbarmcbkgHisto_  = myTTBARMCBkgDS_->binnedClone(); 
  mcbkgHisto_ = myMCBkgDS_->binnedClone();
  dataHisto_  = myDataDS_ ->binnedClone();
  
}

void TauDileptonPDFBuilderFitter::BuildPDFs(size_t i){
  
  switch(fitVars_[i].getUnbinned()){
  case 1 : // Unbinned
    if(includeSignal_) u_signalModel_ = new RooKeysPdf(signalModelName_.c_str(), signalModelName_.c_str(), *myvar_, *mySignalDS_, RooKeysPdf::MirrorBoth,2);
    u_ddbkgModel_  = new RooKeysPdf(ddbkgModelName_.c_str(),  ddbkgModelName_.c_str(),  *myvar_, *myDDBkgDS_ , RooKeysPdf::MirrorBoth,2);
    if(standaloneTTbar_) u_ttbarmcbkgModel_  = new RooKeysPdf(ttbarmcbkgModelName_.c_str(),  ttbarmcbkgModelName_.c_str(),  *myvar_, *myTTBARMCBkgDS_ , RooKeysPdf::MirrorBoth,2);
    u_mcbkgModel_  = new RooKeysPdf(mcbkgModelName_.c_str(),  mcbkgModelName_.c_str(),  *myvar_, *myMCBkgDS_ , RooKeysPdf::MirrorBoth,2);
    break;
  case 0:  // Binned (w/ or w/out smoothing)
    if(includeSignal_) b_signalModel_ = new RooHistPdf(signalModelName_.c_str(), signalModelName_.c_str(), *myvar_, *signalHisto_, fitVars_[i].getSmoothOrder());
    b_ddbkgModel_  = new RooHistPdf(ddbkgModelName_.c_str(),  ddbkgModelName_.c_str(), *myvar_, *ddbkgHisto_ , fitVars_[i].getSmoothOrder());
    if(standaloneTTbar_) b_ttbarmcbkgModel_ = new RooHistPdf(ttbarmcbkgModelName_.c_str(),  ttbarmcbkgModelName_.c_str(),  *myvar_, *ttbarmcbkgHisto_  , fitVars_[i].getSmoothOrder()); 
    b_mcbkgModel_  = new RooHistPdf(mcbkgModelName_.c_str(),  mcbkgModelName_.c_str(),  *myvar_, *mcbkgHisto_  , fitVars_[i].getSmoothOrder()); 
    break;
  default : // Dummy - should never arrive here
    cout<<"Neither binned not unbinned. Check your options motherfucker."<<endl;
  }
}

void TauDileptonPDFBuilderFitter::DrawTemplates(size_t i){
  canvas_->cd();
  canvas_->Clear();

  //signal histogram ////////////////////////////////////////////////////////////
  if(includeSignal_){
    signalHist_ = signalHisto_->createHistogram(fitVars_[i].getVarName().c_str(),fitVars_[i].getBins() );
    signalHist_->SetOption("0000");
    signalHist_->SetLineWidth(3);
    signalHist_->SetTitle("");
    signalHist_->GetYaxis()->SetTitle("a.u.");
    signalHist_->GetYaxis()->SetTitleOffset(1.5);
    signalHist_->SetLineColor(kGreen);
    signalHist_->SetFillColor(kGreen);
  }
  ///////////////////////////////////////////////////////////////////////////////
  
  
  // dd bkg histogram /////////////////////////////////////////////////
  ddbkgHist_ = ddbkgHisto_->createHistogram(fitVars_[i].getVarName().c_str(),fitVars_[i].getBins() );
  ddbkgHist_->SetLineColor(kRed);
  ddbkgHist_->SetFillColor(kRed);
  ddbkgHist_->SetLineWidth(3);
  ddbkgHist_->SetFillStyle(3017);
  /////////////////////////////////////////////////////////////////////
  
  // ttbar mc bkg histogram ////////////////////////////////////////////////
  if(standaloneTTbar_){
    ttbarmcbkgHist_ = ttbarmcbkgHisto_->createHistogram(fitVars_[i].getVarName().c_str(),fitVars_[i].getBins() );   
    ttbarmcbkgHist_->SetLineColor(kYellow);
    ttbarmcbkgHist_->SetFillColor(kYellow);
    ttbarmcbkgHist_->SetLineWidth(3);
    ttbarmcbkgHist_->SetFillStyle(3017);
  }
  ///////////////////////////////////////////////////////////////////
  
  // mc bkg histogram ////////////////////////////////////////////////
  mcbkgHist_ = mcbkgHisto_->createHistogram(fitVars_[i].getVarName().c_str(),fitVars_[i].getBins() );   
  mcbkgHist_->SetLineColor(kBlack);
  mcbkgHist_->SetFillColor(kBlack);
  mcbkgHist_->SetLineWidth(3);
  mcbkgHist_->SetFillStyle(3017);
  ///////////////////////////////////////////////////////////////////
  
  leg_ = new TLegend(0.3,0.665,0.85,0.86,NULL,"brNDC");
  leg_->SetTextFont(62);
  leg_->SetBorderSize(0);
  leg_->SetLineColor(1);
  leg_->SetLineStyle(1);
  leg_->SetLineWidth(1);
  leg_->SetFillColor(0);
  leg_->SetFillStyle(1001);
  if(includeSignal_) leg_->AddEntry(signalHist_,"signal template","f");
  leg_->AddEntry(ddbkgHist_,"dd bkg template","f");
  if(standaloneTTbar_) leg_->AddEntry(ttbarmcbkgHist_,"irr mc bkg template","f");
  leg_->AddEntry(mcbkgHist_,"mc bkg template","f");
  
  canvas_->cd(); 
  // Order chosen to have good Y axis boundaries
  if(standaloneTTbar_){
    if(fitVars_[i].getHmax()){ ttbarmcbkgHist_->SetMaximum(fitVars_[i].getHmax()); ttbarmcbkgHist_->SetMinimum(fitVars_[i].getHmin());} 
    ttbarmcbkgHist_->DrawNormalized("hist");
    mcbkgHist_     ->DrawNormalized("histsame");    
  }
  else {
    if(fitVars_[i].getHmax()){ mcbkgHist_->SetMaximum(fitVars_[i].getHmax()); mcbkgHist_->SetMinimum(fitVars_[i].getHmin());} 
    mcbkgHist_->DrawNormalized("hist");
  }
  
  if(includeSignal_)
    signalHist_->DrawNormalized("histsame");
  ddbkgHist_->DrawNormalized("histsame");
  
  if(standaloneTTbar_) ttbarmcbkgHist_->DrawNormalized("histsame"); // in order for it to be on top and thus viewable for discriminating it from higgs in rc_t plots
  
  leg_->Draw();
  canvas_->SaveAs((outFolder_+identifier_+string("_shapes_")+string(".pdf")).c_str());
  canvas_->SaveAs((outFolder_+identifier_+string("_shapes_")+string(".png")).c_str());
  canvas_->cd();
  canvas_->Clear();
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
}

void TauDileptonPDFBuilderFitter::BuildConstrainedModels(size_t i){

  // Get total entries from Histograms ////////////////////////////////////////////////
  cout<<endl<<endl<<"******************************************************************************************"<<endl;
  
  double sig_N   ;  double nsig   ;
  double ddbkg_N ;  double nddbkg ;
  double ttbarmcbkg_N ;  double nttbarmcbkg ; 
  double mcbkg_N ;  double nmcbkg ; 
  double data_N  ;   
  
  if(includeSignal_){ sig_N   = mySignalDS_->numEntries(); nsig   = signalHisto_->sum(kFALSE); }
  ddbkg_N = myDDBkgDS_->numEntries();  nddbkg = ddbkgHisto_->sum(kFALSE);
  if(standaloneTTbar_){ ttbarmcbkg_N = myTTBARMCBkgDS_->numEntries();  nttbarmcbkg = ttbarmcbkgHisto_->sum(kFALSE); }
  mcbkg_N = myMCBkgDS_->numEntries();  nmcbkg = mcbkgHisto_->sum(kFALSE); 
  data_N  = myDataDS_->numEntries();   
  
  /////////////////////////////////////////////////////////////////////////////////////
  cout<<endl<<"Identifier: "<<identifier_<<", meaning that includeSignal="<<includeSignal_<<", standaloneTTbar="<<standaloneTTbar_<<endl;
  cout<<endl<<" PDF data set Entries for fitVars_[i].getVarName()ribution : "<<fitVars_[i].getVarName();    
  cout<<endl<<"WARNING: values taken just from unbinned datasets"<<endl;
  cout<<endl<<"Sum weights for signal is " << sumWeights_ << endl;
  double separateSigEntries = signalTreeWH_->GetEntries() + signalTreeHH_->GetEntries();
  if(includeSignal_) cout<<endl<<" sig_N    = "<<sig_N<<  " , root entries : "<< separateSigEntries<<" weights : "<<nsig;
  cout<<endl<<" ddbkg_N  = "<<ddbkg_N<<" , root entries : "<<ddBkgTree_->GetEntries()<<" weights : "<<nddbkg;
  if(standaloneTTbar_) cout<<endl<<" ttbarmcbkg_N  = "<<ttbarmcbkg_N<<" , root entries : "<<ttbarmcBkgTree_->GetEntries()<<" weights : "<<nttbarmcbkg;
  cout<<endl<<" mcbkg_N  = "<<mcbkg_N<<" , root entries : "<<mcBkgTree_->GetEntries()<<" weights : "<<nmcbkg;
  cout<<endl<<" data_N   = "<<data_N<< " , root entries : "<<dataTree_->GetEntries()<<endl;
  cout<<endl<<endl<<" ******************************************************************************************* "<<endl;
  /////////////////////////////////////////////////////////////////////////////////////
  
  
  // Building the constrained models for signal mc bkg ///////////////////////////////////////////////////////////////////////////////////////////////
  
  double nsignalMean, nsignalSigma, nttbarmcbkgMean, nttbarmcbkgSigma;
  if(includeSignal_) nsignalMean=nsig; nsignalSigma=signalStatError_;
  double nddbkgMean(ddbkgEstimate_); double nddbkgSigma(ddbkgStatError_);
  nddbkg = ddbkgEstimate_;
  if(standaloneTTbar_) nttbarmcbkgMean=nttbarmcbkg; nttbarmcbkgSigma=ttbarmcbkgStatError_;
  double nmcbkgMean(nmcbkg); double nmcbkgSigma(mcbkgStatError_);
  
  if(includeSignal_){
    if( ! sigVar_         ) sigVar_         = new RooRealVar( "globalSignalVarName",       "globalSignalVarName",         nsig,   0, nsig*2.5);
    else{ sigVar_->setMin("",0);   sigVar_->setMax("",nsig*3.5);     sigVar_->setVal(nsig); }
    
    if( ! sigMeanVar_   ) sigMeanVar_   = new RooRealVar( "globalSignalMeanVarName",    "globalSignalMeanVarName",      nsignalMean); 
    else{ sigMeanVar_->setVal(nsignalMean);   }
    
    if( ! sigSigmaVar_  ) sigSigmaVar_  = new RooRealVar( "globalSignalSigmaVarName",   "globalSignalSigmaVarName",     nsignalSigma ); 
    else{ sigSigmaVar_->setVal(nsignalSigma); }
  }
  
  // FIXME?    
  //    if(! ddbkgVar_        ) ddbkgVar_       = new RooRealVar( "globalDDBkgVarName",        "globalDDBkgVarName",          nddbkgMean, 0, nddbkgMean*1.5); 
  //    else{ ddbkgVar_->setMin("",0);   ddbkgVar_->setMax("",nddbkgMean*3.5);     ddbkgVar_->setVal(nddbkgMean);}
  
  if(! ddbkgVar_        ) ddbkgVar_       = new RooRealVar( "globalDDBkgVarName",        "globalDDBkgVarName",          nddbkg, 0, nddbkg*1.5); 
  else{ ddbkgVar_->setMin("",0);   ddbkgVar_->setMax("",nddbkg*1.5);     ddbkgVar_->setVal(nddbkg);}
  
  if(! ddbkgMeanVar_)     ddbkgMeanVar_   = new RooRealVar( "globalDDBkgMeanVarName", "globalDDBkgMeanVarName",         nddbkgMean); 
  else{ ddbkgMeanVar_->setVal(nddbkgMean);   }
  
  if(! ddbkgSigmaVar_)    ddbkgSigmaVar_  = new RooRealVar( "globalDDBkgSigmaVarName","globalDDBkgSigmaVarName",        nddbkgSigma);  
  else{ ddbkgSigmaVar_->setVal(nddbkgSigma);   }
  
  if(standaloneTTbar_){
    if( ! ttbarmcbkgVar_       ) ttbarmcbkgVar_       = new RooRealVar( "globalTTbarMcBkgVarName",        "globalTTbarMcBkgVarName",          nttbarmcbkg, 0, nttbarmcbkg*1.5); 
    else{ ttbarmcbkgVar_->setMin("",0); ttbarmcbkgVar_->setMax("",nttbarmcbkg*3.5); ttbarmcbkgVar_->setVal(nttbarmcbkg); }
    
    if( ! ttbarmcbkgMeanVar_   ) ttbarmcbkgMeanVar_   = new RooRealVar( "globalTTbarMcBkgMeanVarName",    "globalTTbarMcBkgMeanVarName",      nttbarmcbkgMean); 
    else{ ttbarmcbkgMeanVar_->setVal(nttbarmcbkgMean);   }
    
    if( ! ttbarmcbkgSigmaVar_  ) ttbarmcbkgSigmaVar_  = new RooRealVar( "globalTTbarMcBkgSigmaVarName",   "globalTTbarMcBkgSigmaVarName",     nttbarmcbkgSigma ); 
    else{ ttbarmcbkgSigmaVar_->setVal(nttbarmcbkgSigma); }
  }
  
  if( ! mcbkgVar_       ) mcbkgVar_       = new RooRealVar( "globalMcBkgVarName",        "globalMcBkgVarName",          nmcbkg, 0, nmcbkg*1.5); 
  else{ mcbkgVar_->setMin("",0); mcbkgVar_->setMax("",nmcbkg*3.5); mcbkgVar_->setVal(nmcbkg); }
  
  if( ! mcbkgMeanVar_   ) mcbkgMeanVar_   = new RooRealVar( "globalMcBkgMeanVarName",    "globalMcBkgMeanVarName",      nmcbkgMean); 
  else{ mcbkgMeanVar_->setVal(nmcbkgMean);   }
  
  if( ! mcbkgSigmaVar_  ) mcbkgSigmaVar_  = new RooRealVar( "globalMcBkgSigmaVarName",   "globalMcBkgSigmaVarName",     nmcbkgSigma ); 
  else{ mcbkgSigmaVar_->setVal(nmcbkgSigma); }
  
  
  if(includeSignal_) signalConstraint_ = new RooGaussian( signalConstraintName_.c_str(), signalConstraintName_.c_str(), *sigVar_,*sigMeanVar_, *sigSigmaVar_);
  ddbkgConstraint_ = new RooGaussian( ddbkgConstraintName_.c_str(), ddbkgConstraintName_.c_str(), *ddbkgVar_, *ddbkgMeanVar_, *ddbkgSigmaVar_);   
  if(standaloneTTbar_) ttbarmcbkgConstraint_ = new RooGaussian( ttbarmcbkgConstraintName_.c_str(), ttbarmcbkgConstraintName_.c_str(), *ttbarmcbkgVar_,*ttbarmcbkgMeanVar_, *ttbarmcbkgSigmaVar_);
  mcbkgConstraint_ = new RooGaussian( mcbkgConstraintName_.c_str(), mcbkgConstraintName_.c_str(), *mcbkgVar_,*mcbkgMeanVar_, *mcbkgSigmaVar_);
    
  
  // build the sum model and model with constrains ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  string sumModelName = fitVars_[i].getVarName()+string("_sumModel");
  
  string sumModelExp = "";
  if(includeSignal_) sumModelExp.append(signalModelName_+string("+"));
  sumModelExp.append(ddbkgModelName_+string("+"));
  if(standaloneTTbar_) sumModelExp.append(ttbarmcbkgModelName_+string("+"));
  sumModelExp.append(mcbkgModelName_);
  
  string sumModelConstrainedName = fitVars_[i].getVarName()+string("_sumConstrainedModel");
  
  string sumModelConstrainedExp = sumModelName+string("*");
  if(includeSignal_) sumModelConstrainedExp.append(signalConstraintName_+string("*"));
  sumModelConstrainedExp.append(ddbkgConstraintName_+string("*"));
  if(standaloneTTbar_) sumModelConstrainedExp.append(ttbarmcbkgConstraintName_+string("*"));
  sumModelConstrainedExp.append(mcbkgConstraintName_);
  
  switch(fitVars_[i].getUnbinned()){
  case 1 : // Unbinned
    if(includeSignal_){
      if(standaloneTTbar_){
	sumModel_ = new RooAddPdf( sumModelName.c_str(),sumModelExp.c_str(), RooArgList( *u_signalModel_, *u_ddbkgModel_, *u_ttbarmcbkgModel_, *u_mcbkgModel_), RooArgList( *sigVar_, *ddbkgVar_, *ttbarmcbkgVar_, *mcbkgVar_) );
	model_ = new RooProdPdf(sumModelConstrainedName.c_str(),sumModelConstrainedExp.c_str(), RooArgSet(*sumModel_, *signalConstraint_, *ddbkgConstraint_, *ttbarmcbkgConstraint_, *mcbkgConstraint_));
      }
      else{
	sumModel_ = new RooAddPdf( sumModelName.c_str(),sumModelExp.c_str(), RooArgList( *u_signalModel_, *u_ddbkgModel_, *u_mcbkgModel_), RooArgList( *sigVar_, *ddbkgVar_, *mcbkgVar_) );
	model_ = new RooProdPdf(sumModelConstrainedName.c_str(),sumModelConstrainedExp.c_str(), RooArgSet(*sumModel_, *signalConstraint_, *ddbkgConstraint_, *mcbkgConstraint_));
      }
    } else{
      if(standaloneTTbar_){
	sumModel_ = new RooAddPdf( sumModelName.c_str(),sumModelExp.c_str(), RooArgList( *u_ddbkgModel_, *u_ttbarmcbkgModel_, *u_mcbkgModel_ ), RooArgList(*ddbkgVar_, *ttbarmcbkgVar_, *mcbkgVar_) );
	model_ = new RooProdPdf(sumModelConstrainedName.c_str(),sumModelConstrainedExp.c_str(), RooArgSet(*sumModel_, *ddbkgConstraint_, *ttbarmcbkgConstraint_, *mcbkgConstraint_));
      }
      else{
	sumModel_ = new RooAddPdf( sumModelName.c_str(),sumModelExp.c_str(), RooArgList( *u_ddbkgModel_, *u_mcbkgModel_), RooArgList( *ddbkgVar_, *mcbkgVar_) );
	model_ = new RooProdPdf(sumModelConstrainedName.c_str(),sumModelConstrainedExp.c_str(), RooArgSet(*sumModel_, *ddbkgConstraint_, *mcbkgConstraint_));
      }
    }
    break;
  case 0:  // Binned (w/ or w/out smoothing)
    if(includeSignal_){
      if(standaloneTTbar_){
	sumModel_ = new RooAddPdf( sumModelName.c_str(),sumModelExp.c_str(), RooArgList( *b_signalModel_, *b_ddbkgModel_, *b_ttbarmcbkgModel_, *b_mcbkgModel_), RooArgList( *sigVar_, *ddbkgVar_, *ttbarmcbkgVar_, *mcbkgVar_) );
	model_ = new RooProdPdf(sumModelConstrainedName.c_str(),sumModelConstrainedExp.c_str(), RooArgSet(*sumModel_, *signalConstraint_, *ddbkgConstraint_, *ttbarmcbkgConstraint_, *mcbkgConstraint_));
      }
      else{
	sumModel_ = new RooAddPdf( sumModelName.c_str(),sumModelExp.c_str(), RooArgList( *b_signalModel_, *b_ddbkgModel_, *b_mcbkgModel_ ), RooArgList( *sigVar_, *ddbkgVar_, *mcbkgVar_) );
	model_ = new RooProdPdf(sumModelConstrainedName.c_str(),sumModelConstrainedExp.c_str(), RooArgSet(*sumModel_, *signalConstraint_, *ddbkgConstraint_, *mcbkgConstraint_));
      }
    } else{
      if(standaloneTTbar_){
	sumModel_ = new RooAddPdf( sumModelName.c_str(),sumModelExp.c_str(), RooArgList( *b_ddbkgModel_, *b_ttbarmcbkgModel_, *b_mcbkgModel_ ), RooArgList(*ddbkgVar_, *ttbarmcbkgVar_, *mcbkgVar_) );
	model_ = new RooProdPdf(sumModelConstrainedName.c_str(),sumModelConstrainedExp.c_str(), RooArgSet(*sumModel_, *ddbkgConstraint_, *ttbarmcbkgConstraint_, *mcbkgConstraint_));
      }
      else{
	sumModel_ = new RooAddPdf( sumModelName.c_str(),sumModelExp.c_str(), RooArgList( *b_ddbkgModel_, *b_mcbkgModel_), RooArgList( *ddbkgVar_, *mcbkgVar_) );
	model_ = new RooProdPdf(sumModelConstrainedName.c_str(),sumModelConstrainedExp.c_str(), RooArgSet(*sumModel_, *ddbkgConstraint_, *mcbkgConstraint_));
      }
    }
    break;
  default : // Dummy - should never arrive here
    cout<<"Neither binned not unbinned. Check your options motherfucker."<<endl;
  }
}

void TauDileptonPDFBuilderFitter::DoPerVariableFit(size_t i){
  switch(fitVars_[i].getUnbinned()){
  case 1 : // Unbinned
    if(includeSignal_){
      if(standaloneTTbar_){
	constrainedModelFit_ = model_->fitTo( *myDataDS_, Minos(), Constrain(RooArgSet(*sigVar_,*ddbkgVar_,*ttbarmcbkgVar_,*mcbkgVar_)), Save(kTRUE),PrintLevel(-1),Verbose(false),Range(fitVars_[i].getMin(),fitVars_[i].getMax()));
      }
      else{
	constrainedModelFit_ = model_->fitTo( *myDataDS_, Minos(), Constrain(RooArgSet(*sigVar_,*ddbkgVar_,*mcbkgVar_)), Save(kTRUE),PrintLevel(-1),Verbose(false),Range(fitVars_[i].getMin(),fitVars_[i].getMax()));
      }
    } else{
      if(standaloneTTbar_){
	constrainedModelFit_ = model_->fitTo( *myDataDS_, Minos(), Constrain(RooArgSet(*ddbkgVar_,*ttbarmcbkgVar_,*mcbkgVar_)), Save(kTRUE),PrintLevel(-1),Verbose(false),Range(fitVars_[i].getMin(),fitVars_[i].getMax()));
      }
      else{
	constrainedModelFit_ = model_->fitTo( *myDataDS_, Minos(), Constrain(RooArgSet(*ddbkgVar_,*mcbkgVar_)), Save(kTRUE),PrintLevel(-1),Verbose(false),Range(fitVars_[i].getMin(),fitVars_[i].getMax()));
      }
    }
    break;
  case 0:  // Binned (w/ or w/out smoothing)
    if(includeSignal_){
      if(standaloneTTbar_){
	constrainedModelFit_ = model_->fitTo( *dataHisto_, Minos(), Constrain(RooArgSet(*sigVar_,*ddbkgVar_,*ttbarmcbkgVar_,*mcbkgVar_)), Save(kTRUE),PrintLevel(-1),Verbose(false),Range(fitVars_[i].getMin(),fitVars_[i].getMax()));
      }
      else{
	constrainedModelFit_ = model_->fitTo( *dataHisto_, Minos(), Constrain(RooArgSet(*sigVar_,*ddbkgVar_,*mcbkgVar_)), Save(kTRUE),PrintLevel(-1),Verbose(false),Range(fitVars_[i].getMin(),fitVars_[i].getMax()));
      }
    } else{
      if(standaloneTTbar_){
	constrainedModelFit_ = model_->fitTo( *dataHisto_, Minos(), Constrain(RooArgSet(*ddbkgVar_,*ttbarmcbkgVar_,*mcbkgVar_)), Save(kTRUE),PrintLevel(-1),Verbose(false),Range(fitVars_[i].getMin(),fitVars_[i].getMax()));
      }
      else{
	constrainedModelFit_ = model_->fitTo( *dataHisto_, Minos(), Constrain(RooArgSet(*ddbkgVar_,*mcbkgVar_)), Save(kTRUE),PrintLevel(-1),Verbose(false),Range(fitVars_[i].getMin(),fitVars_[i].getMax()));
      }
    }
    break;
  default : // Dummy - should never arrive here
    cout<<"Neither binned not unbinned. Check your options motherfucker."<<endl;
  }
}

void TauDileptonPDFBuilderFitter::DrawPerVariableFit(size_t i){
  canvas_->cd();  
  canvas_->Clear();  
  myFrame_ = myvar_->frame();
  myFrame_->SetTitle("");
  myFrame_->GetXaxis()->SetTitle(fitVars_[i].getVarName().c_str());
  myFrame_->GetYaxis()->SetTitle("Events");
  dataHisto_->plotOn(myFrame_);
  model_->plotOn(myFrame_);
  //    model->plotOn(myFrame, RooFit::LineStyle(kDashed), RooFit::Components(*signalModel), RooFit::LineColor(kGreen));   
  if(includeSignal_) 
    switch(fitVars_[i].getUnbinned()){
    case 1 : // Unbinned
      model_->plotOn(myFrame_, RooFit::LineStyle(kDashed), RooFit::Components(*u_signalModel_), RooFit::LineColor(kGreen));   
      break;
    case 0:  // Binned (w/ or w/out smoothing)
      model_->plotOn(myFrame_, RooFit::LineStyle(kDashed), RooFit::Components(*b_signalModel_), RooFit::LineColor(kGreen));   
      break;
    default : // Dummy - should never arrive here
      cout<<"Neither binned not unbinned. Check your options motherfucker."<<endl;
    }
  
  myFrame_->Draw();
  canvas_->SaveAs((outFolder_+identifier_+string("_modelFit_")+string(".pdf")).c_str());
  canvas_->SaveAs((outFolder_+identifier_+string("_modelFit_")+string(".png")).c_str());
  canvas_->cd();
  canvas_->Clear();
}

void TauDileptonPDFBuilderFitter::DoPerVariableLikelihoodFit(size_t i){
  // Test on the fit ////////////////////////////////////////////////////////////
  //    RooNLLVar * nll = (RooNLLVar *) model->createNLL(
  //						     *dataHisto_,
  //						     //*myDataDS_,
  //      RooFit::CloneData(kTRUE),
  //      Extended(kTRUE),
  //      Constrain(RooArgSet(*ddbkgVar_,*mcbkgVar_)),
  //      Range(min,max)
  //    );
  
  switch(fitVars_[i].getUnbinned()){
  case 1: // Unbinned
    if(includeSignal_){
      if(standaloneTTbar_) nll_ = (RooNLLVar*) model_->createNLL( *myDataDS_, RooFit::CloneData(kTRUE), Extended(kTRUE), Constrain(RooArgSet(*sigVar_,*ddbkgVar_,*ttbarmcbkgVar_,*mcbkgVar_)), Range(fitVars_[i].getMin(),fitVars_[i].getMax()));
      else nll_ = (RooNLLVar*) model_->createNLL( *myDataDS_, RooFit::CloneData(kTRUE), Extended(kTRUE), Constrain(RooArgSet(*sigVar_,*ddbkgVar_,*mcbkgVar_)), Range(fitVars_[i].getMin(),fitVars_[i].getMax()));
    } else{
      if(standaloneTTbar_) nll_ = (RooNLLVar *) model_->createNLL( *myDataDS_, RooFit::CloneData(kTRUE), Extended(kTRUE), Constrain(RooArgSet(*ddbkgVar_,*ttbarmcbkgVar_,*mcbkgVar_)), Range(fitVars_[i].getMin(),fitVars_[i].getMax()));
      else nll_ = (RooNLLVar*) model_->createNLL( *myDataDS_, RooFit::CloneData(kTRUE), Extended(kTRUE), Constrain(RooArgSet(*ddbkgVar_,*mcbkgVar_)), Range(fitVars_[i].getMin(),fitVars_[i].getMax()));
    }
    break;
  case 0:  // Binned (w/ or w/out smoothing)
    if(includeSignal_){
      if(standaloneTTbar_) nll_ = (RooNLLVar*) model_->createNLL( *dataHisto_, RooFit::CloneData(kTRUE), Extended(kTRUE), Constrain(RooArgSet(*sigVar_,*ddbkgVar_,*ttbarmcbkgVar_,*mcbkgVar_)), Range(fitVars_[i].getMin(),fitVars_[i].getMax()));
      else nll_ = (RooNLLVar*) model_->createNLL( *dataHisto_, RooFit::CloneData(kTRUE), Extended(kTRUE), Constrain(RooArgSet(*sigVar_,*ddbkgVar_,*mcbkgVar_)), Range(fitVars_[i].getMin(),fitVars_[i].getMax()));
    } else{
      if(standaloneTTbar_) nll_ = (RooNLLVar*) model_->createNLL( *dataHisto_, RooFit::CloneData(kTRUE), Extended(kTRUE), Constrain(RooArgSet(*ddbkgVar_,*ttbarmcbkgVar_,*mcbkgVar_)), Range(fitVars_[i].getMin(),fitVars_[i].getMax()));
      else nll_ = (RooNLLVar*) model_->createNLL( *dataHisto_, RooFit::CloneData(kTRUE), Extended(kTRUE), Constrain(RooArgSet(*ddbkgVar_,*mcbkgVar_)), Range(fitVars_[i].getMin(),fitVars_[i].getMax()));
    }
    break;
  default : // Dummy - should never arrive here
    cout<<"Neither binned not unbinned. Check your options motherfucker."<<endl;
  }
  
  likelihoodVector_.push_back(nll_); //.push_back(nll); 
  
  RooMinuit minuit(*nll_);
  minuit.migrad();
  minuit.setErrorLevel(0.5);
  minuit.hesse();
  
  myNllFitResult_ = minuit.save();
  cout<<endl<<"*******************************************************************************"<<endl;
  cout<<      "*******************************************************************************"<<endl;
  cout<<      "************* IDENTIFIER: "<< identifier_ << "****************************"<<endl;
  cout<<endl<<"FIT RESULTS for variable"<<fitVars_[i].getVarName()<<endl<<endl;
  myNllFitResult_->Print("v");
  cout<<endl<<"*******************************************************************************"<<endl;
  cout<<      "*******************************************************************************"<<endl;
  
  canvas_->cd();
  canvas_->Clear();
  
  if(includeSignal_){
    contourPlot_ = minuit.contour( *ddbkgVar_, *sigVar_,1,2,3);
    contourPlot_->GetYaxis()->SetTitle("N(H+), m_{H} = 120 GeV/c2");
    contourPlot_->GetYaxis()->SetRangeUser(0,600);
  }
  else 
    if(standaloneTTbar_){
      contourPlot_ = minuit.contour( *ddbkgVar_, *ttbarmcbkgVar_,1,2,3);
      contourPlot_->GetYaxis()->SetTitle("N(t#bar{t}#rightarrow l#tau)");
      contourPlot_->GetYaxis()->SetRangeUser(0,400);
    }
    else{
      contourPlot_ = minuit.contour( *ddbkgVar_, *mcbkgVar_,1,2,3);
      contourPlot_->GetYaxis()->SetTitle("N^{MCdriven}_{Bkg}");
      contourPlot_->GetYaxis()->SetRangeUser(0,200);
    }
  contourPlot_->SetTitle("");
  contourPlot_->GetXaxis()->SetTitle("N^{DD}_{Bkg}");
  contourPlot_->GetXaxis()->SetRange(0,400);
  contourPlot_->Draw();
  canvas_->SaveAs((outFolder_+identifier_+string("_contour_")+string(".pdf")).c_str());
  canvas_->SaveAs((outFolder_+identifier_+string("_contour_")+string(".png")).c_str());
  canvas_->cd();
  canvas_->Clear();
  ///////////////////////////////////////////////////////////
}

void TauDileptonPDFBuilderFitter::DoCombinedLikelihoodFit(){
  //  RooAddition* combll;
  //  RooFitResult* myRes;
  //  RooPlot* contourPlot;
  //  for(int dim=0; dim<NVARS; dim++){
  RooArgSet llSet;
  cout<<"FINAL FIT NUMBER " << nVars_ << ", likelihoodVector with dim: " << likelihoodVector_.size() << endl;    
  // fill arg set with a
  for(size_t i=0; i<nVars_;i++){ llSet.add( *(likelihoodVector_[i]) ); }
  //  for(int i=0; i<likelihoodVector.size();i++){ llSet.add( *(likelihoodVector[i]) ); }
  
  //create a rooAddition
  combll_ = new RooAddition("combll","combll",llSet);
  
  //minimize
  RooMinuit minuit(*combll_);
  minuit.setVerbose(false);
  minuit.setErrorLevel(0.5); //otherwise RooFit seems to assume the chi^2 default
  minuit.hesse();
  minuit.migrad();
  minuit.setErrorLevel(0.5);
  minuit.hesse();
  minuit.minos();
  minuit.save();
  myNllFitResult_ = minuit.save();
  cout<<endl<<"*******************************************************************************"<<endl;
  cout<<      "*******************************************************************************"<<endl;
  cout<<      "********************** IDENTIFIER: " << identifier_ << "**************************" << endl;
  cout<<endl<<"FINAL FIT RESULTS"<<endl<<endl;
  myNllFitResult_->Print("v");//myRes->printToStream(resultsFile);
  cout<<endl<<"*******************************************************************************"<<endl;
  cout<<      "*******************************************************************************"<<endl;
  
  canvas_->Clear();
  
  if(includeSignal_){
    contourPlot_ = minuit.contour( *ddbkgVar_ , *sigVar_,1,2,3);
    contourPlot_->GetYaxis()->SetTitle("N(t#bar{t}#rightarrow l#tau)");
    contourPlot_->GetYaxis()->SetRangeUser(0,600);
  }
  else{
    if(standaloneTTbar_){
      cout << "Here I am" << endl;
      contourPlot_ = minuit.contour( *ddbkgVar_ , *ttbarmcbkgVar_,1,2,3);
      cout << "Here I am. contour generation set to some other stuff" << endl;
      myNllFitResult_->Print("v");
      contourPlot_->GetYaxis()->SetTitle("N(t#bar{t}#rightarrow l#tau)");
      cout << "Here I am not. contour access crashed because of lack of component" << endl;
      contourPlot_->GetYaxis()->SetRangeUser(0,400);
    }
    else{
      contourPlot_ = minuit.contour( *ddbkgVar_ , *mcbkgVar_,1,2,3);
      contourPlot_->GetYaxis()->SetTitle("N(t#bar{t}#rightarrow l#tau)");
      contourPlot_->GetYaxis()->SetRangeUser(0,200);
    }
  }
  contourPlot_->SetTitle("");
  contourPlot_->GetXaxis()->SetTitle("N^{DD}_{Bkg}");
  contourPlot_->GetXaxis()->SetRangeUser(0,400);
  contourPlot_->Draw();
  
  
  std::ostringstream oss;
  oss << nVars_;//dim;
  
  canvas_->SaveAs((outFolder_+baseIdentifier_+string("_contour_final_")+oss.str()+string("vars.pdf")).c_str());
  canvas_->SaveAs((outFolder_+baseIdentifier_+string("_contour_final_")+oss.str()+string("vars.png")).c_str());
  ///////////////////////////////////////////////////////////

  minuit.cleanup();
}

void TauDileptonPDFBuilderFitter::DoFit(){
  
  //cout << "INIT: signal tree entries:"<<signalTree_->GetEntries()<<endl;
  
  for(size_t f=0; f<fitType_.size(); f++){
    
    // Set fit settings
    InitFitSettings(f);
    
    for(size_t i = 0; i< nVars_; i++){
      
      InitPerVariableAmbient(i);
      
      BuildDatasets(i);
      
      BuildPDFs(i);
      
      DrawTemplates(i);
      
      BuildConstrainedModels(i);
      
      DoPerVariableFit(i);
      
      DrawPerVariableFit(i);
      
      DoPerVariableLikelihoodFit(i);
      
    }
    
    DoCombinedLikelihoodFit();
    
  }
  
  
  //}
  //  cout.rdbuf(old); // restore   
}
