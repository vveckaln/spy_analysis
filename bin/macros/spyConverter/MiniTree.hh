#ifndef _event_minitree_hh_
#define _event_minitree_hh_

#if !defined(__CINT__) || defined(__MAKECINT__)

#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "TString.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "TVectorD.h"

#endif

namespace event
{
  /**
     @short generic usage (pseudo-code)
     
     //start the tree
     CreateNewMiniTree( fs->make<TTree>("data","data"), "data" );
     
     //for each event event
     event::Handler ev;
     ev.StartNewMiniEvent("data");
     ev.FillMiniEventHeader(iRun, iEvent, iLumi, classInfoVector "data");
     ev.AddParticleToMiniEvent( jetP4, jetInfoVector, JET, "data");
     ...
     ev.EndNewMiniEvent("data");

     @author P. Silva (psilva@cern.ch)
  */

  /**
     @short the mini event structure
  */
  struct MiniEvent_t{
    Int_t iRun, iEvent, iLumi;
    TClonesArray *eventClassif;
    TClonesArray *triggerColl, *vertexColl;
    TClonesArray *leptonsColl,  *leptonsClassifColl;
    TClonesArray *photonsColl,  *photonsClassifColl;
    TClonesArray *jetsColl,     *jetsClassifColl;
    TClonesArray *metColl,      *metClassifColl;
  };
  
  
  /**
     @short adds a new particle to the mini event
  */
  enum ParticleType {MET, JET, QUARK, ELECTRON=11, MUON=13, TAU=15, CHLEPTON=17, TRACK=100, PHOTON=22, SUPERCLUSTER=2222, TOP=6};
  enum JetAlgo { AK5, SC5, IC5, AK5Track, AK5JPT, AK5PF, AK5PFLOW, UNKNOWNJETALGO };
  enum METAlgo { CALOAK5, CALOSC5, CALOIC5, TC, PF, PFLOWMET, UNKNOWNMETALGO };
  enum TauType { PFTAU, CALOTAU, HPSTAU, PFLOWTAU, HPSTANCTAU, UNKNOWNTAU };
  enum LeptonType { STDLEPTON=0, PFLOWLEPTON=1 };
  /**
     @class Handler
     @short the handler for booking and filling mini event trees
   */
  class Handler
  {
  public:
    
    Handler() { }
    ~Handler() {}
    
    //containers for different mini-event types
    std::map<TString, TTree *> miniTrees;
    std::map<TString, event::MiniEvent_t *> miniEvents;
    
    /**
       @short instantiates a new event
    */
    void CreateNewMiniTree(TTree *tree, TString tag="");
    
    /**
       @short prepares the storage of data from a new event
    */
    void StartNewMiniEvent(TString tag);
    
    /**
       Fills the event header
    */
    void FillMiniEventHeader(int run, int event, int lumi, std::vector<double> &info, TString tag);

    /**
       @short adds trigger bits info
    */
    void AddTriggerInfo(std::vector<int> &info,   TString tag);
    
    /**
       @short adds info about a event vertex
    */
    void AddVertexInfo(std::vector<double> &info,   TString tag);

    /**
       Adds particle to mini event
     */
    void AddParticleToMiniEvent(TLorentzVector &p, std::vector<double> &info, int pType, TString tag);
    
    /**
       @short Commands the filling of the tree
    */
    void EndNewMiniEvent(TString tag);
  };  
}


#endif
