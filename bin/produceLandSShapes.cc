/**                                                                                                                                                                              
  \class    produceLandSShapes produceLandSShapes.cc "UserCode/LIP/TopTaus/bin/produceLandSShapes.cc"                                                                     
  \brief    executable for producing the shapes for limit estimation with landS, following the instructions at http://cmsdoc.cern.ch/~anikiten/cms-higgs/LandsInstructions1.txt
  
  \author   Pietro Vischia

  \version  $Id: produceLandSShapes.cc,v 1.0 2012/09/19 13:03:33 vischia Exp $                                                                                                       
*/

#include "LIP/TopTaus/interface/TauDileptonPDFBuilderFitter.hh"

// System includes
#include <string>

// ROOT includes
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "TSystem.h"
/*

@run me like this

tauDileptonPDFBuilderFitter mutau HIGGS2BKG
tauDileptonPDFBuilderFitter mutau HIGGS2BKG

tauDileptonPDFBuilderFitter mutau HIGGS2BKG
tauDileptonPDFBuilderFitter mutau HIGGS3BKG

*/

//
int main(int argc, char* argv[])
{
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  //check arguments
  if ( argc < 2 ) {
    std::cout << "Usage : " << argv[0] << " parameters_cfg.py" << std::endl;
    return 0;
  }
  
  string parSet(argv[1]);

  TauDileptonPDFBuilderFitter* myFitter = new TauDileptonPDFBuilderFitter(parSet);
  myFitter->DoFit();
  cout << "Fitter reached natural end" << endl;

  return 0;
  
}
