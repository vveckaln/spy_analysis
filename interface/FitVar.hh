#ifndef _FitVar_hh
#define _FitVar_hh
/**                                                                                                                                                                              
																						 \class    likelihoodFitter::tauDileptonPDFBuilderFitter tauDileptonPDFBuilderFitter.cc "UserCode/LIP/TopTaus/tauDileptonPDFBuilderFitter.cc"                                                                     
  \brief    executable for performing multivariable likelihood fit in order to improve estimation of N_fakes
  
  \author   Pietro Vischia

  \version  $Id: FitVar.hh,v 1.2 2012/09/23 14:21:56 vischia Exp $                                                                                                       
*/

//#if !defined(__CINT__) || defined(__MAKECINT__)
//
//#ifndef __CINT__
////#include "RooGlobalFunc.h"
//#endif

// System headers
#include <string>
#include <vector>

// ROOT headers
#include "TString.h"

//#endif

class FitVar{
  
public:
  FitVar(std::string, double, double, int, double, double, Int_t, Int_t, bool, Int_t);
  void setFancyName(std::string);
  void setBinNames(std::vector<std::string>);
  bool getToNorm();
  Int_t getDoLogy();
  std::string getVarName();
  std::string getFancyName();
  double getMin();
  double getMax();
  int getBins();
  std::string getBinName(int);
  double getBinStep();
  double getHmin();
  double getHmax();
  Int_t getUnbinned();
  Int_t getSmoothOrder();
private:
  std::string varName_;
  std::string fancyName_;
  double min_;
  double max_;
  int bins_;
  std::vector<std::string> binNames_;
  double hmin_;
  double hmax_;
  Int_t unbinned_;
  Int_t smoothOrder_;
  bool toNorm_;
  Int_t doLogy_;
};

#endif //_FitVar_hh


