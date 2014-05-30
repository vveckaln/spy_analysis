#ifndef _TauDileptonTableBuilder_hh
#define _TauDileptonTableBuilder_hh

#include "LIP/TopTaus/interface/Utilities.hh"

#if !defined(__CINT__) || defined(__MAKECINT__)

// System headers
#include <string>
#include <iostream>
#include <iostream>
#include <iomanip>

#endif

// ROOT headers
#include "TString.h"

// FIXME: clean code
using namespace std;
using namespace utilities;

namespace tableutils{

  class TauDileptonTableBuilder : protected StatUtils, private EditorialUtils { // GenericUtils not needed apparently
    
  public :
    void mcTable(  int detailed, bool includeSoverB, bool printAllErrors, bool higgs, bool lightH, TString key, TString name, bool syst1, bool syst2, bool syst3 );
    void summaryTable( int detailed, bool higgs, bool lightH, bool systset1, bool systset2, bool systset3, bool produceDatacards, bool withShapes, bool withStatShapes, bool unsplit);
    void doDatacards(vector<double>, vector<vector<double> >, vector<vector<double> >, vector<vector<double> >, vector<double>, bool, bool, bool, string);
    void doDatacardInput();
    void doDatacard();

    
  };
  
}
#endif // _TauDileptonTableBuilder_hh
