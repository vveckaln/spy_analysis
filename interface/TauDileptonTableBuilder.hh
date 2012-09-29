#ifndef _TauDileptonTableBuilder_hh
#define _TauDileptonTableBuilder_hh

#if !defined(__CINT__) || defined(__MAKECINT__)

// System headers
#include <string>
#include <iostream>
#include <iostream>
#include <iomanip>

#endif

#include "LIP/TopTaus/interface/Utilities.hh"

// FIXME: clean code
using namespace std;
using namespace utilities;

namespace tableutils{

  class TauDileptonTableBuilder : public StatUtils, EditorialUtils { // GenericUtils not needed apparently
    
  public :
    void mcTable(  int detailed, bool includeSoverB, bool printAllErrors, bool higgs, TString key, TString name, bool syst1, bool syst2, bool syst3 );
    void summaryTable( bool detailed, bool higgs,bool systset1, bool systset2, bool systset3);
    
  };
  
}
#endif // _TauDileptonTableBuilder_hh
