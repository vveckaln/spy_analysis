#include "LIP/TopTaus/interface/FitVar.hh"

// System includes

// CMSSW includes

// ROOT includes

using namespace std;

// FIXME: change of interface needed. ShapesProducer needs fancyName, will go in the constructor when I'll have time to modify TauDileptonPDFFitter as well. For now, initFancyName(string) method will do the deed, and default for fancyName is varname (the name in the tree)

FitVar::FitVar(string varName, double min, double max, int bins, double hmin, double hmax, Int_t unbinned, Int_t smoothOrder, bool toNorm, Int_t doLogy ):
  varName_(varName),
  fancyName_(varName),
  min_(min),
  max_(max),
  bins_(bins),
  hmin_(hmin),
  hmax_(hmax),
  unbinned_(unbinned),
  smoothOrder_(smoothOrder),
  toNorm_(toNorm),
  doLogy_(doLogy)
{
  binNames_.clear();
}

void FitVar::setFancyName(std::string fancyName){
  fancyName_ = fancyName;
}

void FitVar::setBinNames(std::vector<std::string> binNames){
  binNames_ = binNames;
}

bool FitVar::getToNorm(){
  return toNorm_;
}

Int_t FitVar::getDoLogy(){
  return doLogy_;
}

string FitVar::getVarName(){
  return varName_;
}

string FitVar::getFancyName(){
  return fancyName_;
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

std::string FitVar::getBinName(int ibin){
  return binNames_[ibin];
}

double FitVar::getBinStep(){
  return (max_-min_)/bins_;
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


