#ifndef _MiniEvent_hh
#define _MiniEvent_hh

// FIXME: rationalize code
// FIXME: for next production rationalize classes (perhaps can be done right away for already existing trees, the format after all is the same)

/**
   @author psilva@cern.ch
   @version $version$
   @date $date$

   @short Compile me like this 
   
   //under CMSSW environment
   gSystem->AddIncludePath("-I${CMSSW_BASE}/src/lipcms");
   .L RunOverMiniEvents.C+

   //outside CMSSW environment
   python compile.py

   @short Run me like this
   event::Reader reader;
   TFile *f = TFile::Open("data.root");
   unsigned int nEntries = reader.AssignMiniEventTreeFrom(f);
   for(unsigned int i=0; i<nEntries; i++)
   {
      //analyze new mini event
      event::MiniEvent_t *ev = reader.GetNewMiniEvent(i);
      std::vector<PhysicsObject> jets = reader.GetPhysicsObjectsFrom(ev,event::JET);
      ...
   }
   f->Close();

*/



#if !defined(__CINT__) || defined(__MAKECINT__)

#include <string>
#include <iostream>
#include <map>
#include <algorithm>

#include "TSystem.h"
#include "TFile.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TObject.h"
#include "TString.h"
#include "TObjArray.h"
#include "TH2.h"
#include "TMath.h"
#include "TGraphAsymmErrors.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TVectorD.h"
#include "TFractionFitter.h"
//#include "lipcms/EventSelection/test/RunOverMiniEvents.h"

//#ifdef _STANDALONE  // FIXME: wtf is that???
//#include "LIP/TopTaus/interface/MiniTree.hh"
//#else
//#include "LIP/TopTaus/interface/MiniTree.hh"
//#endif

#include "MiniTree.hh"
#endif

using namespace std;

/**
   @class a physics object
   @short a Lorentz vector with an array of extra information   
*/

class PhysicsObject : public TLorentzVector
{
public:
  
  /**
     @short CTOR
  */
  PhysicsObject() : TLorentzVector(0,0,0,0), info_(0) { }
  PhysicsObject( TLorentzVector*, TVectorD*);


  /**
     @short CTOR (2)
  */
  PhysicsObject( TObject *p4, TObject *info)
    : TLorentzVector( *(TLorentzVector *)p4 ),
      info_((TVectorD *)info)
  { }
  
  /**
     @short returns the info associated to the object
     @param i the index where the info should be found
     @return the value of the info
  */
  Double_t operator[](int i) const
  {
    return (*info_)[i];
  }
  Double_t &operator[](int i)
  {
    return (*info_)[i];
  }
  
  /**
     @short can be used to sort a container of physics objects
     use as sort(myvec.begin(), myvec.end(), PhysicsObject::PtOrder);
     @param a physics object 
     @param b physics object
     @return true if p_T(a) > p_T(b)
  */
  static bool PtOrder(PhysicsObject a, PhysicsObject b)
    { 
      return (a.Pt()>b.Pt()); 
    }

  static bool BtagOrder(PhysicsObject a, PhysicsObject b)
  { 
    return (a[12]>b[12]); 
  }
  
  //the extra info
  TVectorD *GetInfo();
  TVectorD *info_;
  
  /**
     @short DTOR
  */
  ~PhysicsObject() {};
  
private: 
  
  //  ClassDef(PhysicsObject,4);    
};


/**
   @class PhysicsObjectPair
   @short groups two physics objects
*/
class PhysicsObjectPair : public TLorentzVector
{
 public:
  
  /**
     @short CTOR
  */
  PhysicsObjectPair();
  PhysicsObjectPair(PhysicsObject &a, PhysicsObject &b)
    :  TLorentzVector(a+b), physPair_(a,b), sumPt_(0), dR_(0), dilType_(""), dilCode_(0)
  {
        
    int id1=int(physPair_.first[0]);
    int id2=int(physPair_.second[0]);
    
    if(fabs(id1)==11 && fabs(id2)==11) { dilType_="ee"; dilCode_=0xbb; }
    else if( (fabs(id1)==11 && fabs(id2)==13)|| (fabs(id1)==13 && fabs(id2)==11) ) { dilType_="emu"; dilCode_=0xbd; }
    else if(fabs(id1)==13 && fabs(id2)==13) { dilType_="mumu"; dilCode_=0xdd; }
    else { dilType_ ="unk"; dilCode_=0; }
    
    if(id1*id2>0) isOS_=false;
    else isOS_=true;
    
    sumPt_ = physPair_.first.Pt()+physPair_.second.Pt(); 
    dR_ = physPair_.first.DeltaR(physPair_.second);
    dPhi_ = physPair_.first.DeltaPhi(physPair_.second);
    dEta_ = physPair_.first.Eta() - physPair_.second.Eta();
  }
  
  /**
   */
  PhysicsObject operator[](int i) const
  {
    if(i==1) return physPair_.first;
    return physPair_.second;
  }
  PhysicsObject &operator[](int i)
  {
    if(i==1) return physPair_.first;
    return physPair_.second;
  }
  
  /**
     @short compute possible transverse mass combinations
     cf. http://cmslxr.fnal.gov/lxr/source/AnalysisDataFormats/EWK/src/WMuNuCandidate.cc
     @return MT(lepton1),MT(lepton2),MT(lepton1+lepton2)
   */
  std::vector<double> Mt(PhysicsObject &met)
    {
      std::vector<double> transverseMass;
      TLorentzVector theMET(met.Px(),met.Py(),0,met.Pt());
      TLorentzVector tLep1(physPair_.first.Px(),physPair_.first.Py(),0,physPair_.first.Pt());
      TLorentzVector tLep2(physPair_.second.Px(),physPair_.second.Py(),0,physPair_.second.Pt());
      TLorentzVector w1 = theMET+tLep1;         transverseMass.push_back(w1.Mt());
      TLorentzVector w2 = theMET+tLep2;         transverseMass.push_back(w2.Mt());
      TLorentzVector wsum = theMET+tLep1+tLep2; transverseMass.push_back(wsum.Mt());
      return transverseMass;
    }

  /**
     @short the sum pt/Delta R/Type of the pair
   */
  double SumPt() { return sumPt_; }
  double DR() { return dR_; }
  double DPhi() { return dPhi_; }
  double DEta() { return dEta_; }
  PhysicsObject &Leading() { return (physPair_.first.Pt()>physPair_.second.Pt() ? physPair_.first : physPair_.second); }
  PhysicsObject &NextLeading() { return (physPair_.first.Pt()>physPair_.second.Pt() ? physPair_.second : physPair_.first); }
  TString Type() { return dilType_; }
  unsigned int Code() { return dilCode_; }
  bool IsOS() { return isOS_; }


  /**
     @short can be used to sort a container of physics object pairs
     use as sort(myvec.begin(), myvec.end(), SumPtOrder);
     @param a physics object pair 
     @param b physics object pair
     @return true if Sum p_T(a) > Sum p_T(b)
  */
  static bool SumPtOrder(PhysicsObjectPair a, PhysicsObjectPair b)
    {  
      return (a.SumPt() > b.SumPt() );
    }
  
  /**
     @short DTOR
  */
  ~PhysicsObjectPair() { }
  
 private:
  
  //the pair
  std::pair<PhysicsObject,PhysicsObject> physPair_;

  //info
  double sumPt_,dR_,dEta_,dPhi_;
  bool isOS_;

  //the type
  TString dilType_;
  unsigned int dilCode_;

  //  ClassDef(PhysicsObjectPair,4);
};

typedef std::vector<PhysicsObject> PhysicsObjectCollection;
typedef std::vector<PhysicsObjectPair> PhysicsObjectPairCollection;

namespace event{

  /**
     @class event::Reader
     @short parses a mini event structure from a file

   */
  class Reader
    {    
    public:
      Reader() : spyEventsF_(0) { }
      ~Reader() { }

      /**
	 @short assigns the mini event structure variables to a tree stored in a ROOT file
	 @param f the pointer to the file
	 @param tag the shortname for the tree containing the events
	 @param path the path inside the file where to look for the tree
	 @return the number of events found
      */
      unsigned int AssignMiniEventTreeFrom(TFile *f, TString tag="data", TString path = "myEventSelector");

      /**
	 @short prepares a tree to store selected (spy) events
	 @param fout the name of the file which will contain the tree
	 @param tag the shortname for the tree containing the events                                                                                                                                 
	 @return the spy events tree
      */
      TTree *PrepareToSpyEvents(TString fout, TString tag="data",TString path = "myEventSelector")
	{
	  if(m_evHandler.miniTrees.find(tag)==m_evHandler.miniTrees.end()) return 0;
	  TTree *origt = m_evHandler.miniTrees[tag];
	  
	  //assign tree to output spy events file
	  TDirectory* outDir=0;
	  if(spyEventsF_==0) 
	    {
	      spyEventsF_ = TFile::Open(fout,"RECREATE");
	      outDir=spyEventsF_->mkdir(path);
	    }
	  else outDir = (TDirectory *) spyEventsF_->cd(path);
	  outDir->cd();
	  	 
	  //clone tree and assign the same addresses as the ones used by the original
	  TTree *t = origt->CloneTree(0);
	  t->SetDirectory(outDir);
	  t->AutoSave();
	  t->Print();
	  spyEventsTrees_[tag]=t;	
	  
	  return t;
	}


      /**
	 @short fills the spy events tree with the current entry
       */
      inline void SpyEvent(TString tag="data")
      {
	if(spyEventsTrees_.find(tag)==spyEventsTrees_.end()) return;
	spyEventsTrees_[tag]->Fill();
      }
      
      /**
	 @short saves the spy events tree to the output file
       */
      void EndSpyEvents()
      {       
	if(spyEventsF_==0) return;
	spyEventsF_->Write();
	spyEventsF_->Close();
	spyEventsF_->Delete();	
	spyEventsTrees_.clear();
      }

      /**
	 @short retrieves a new mini event from the tree
	 @param ientry the index of the event (0,...nEntries(
	 @param tag the shortname of the tree
	 @return a pointer to the mini event (don't delete it!)
      */
      event::MiniEvent_t *GetNewMiniEvent(unsigned int ientry,TString tag="data");
      void GetNewMiniEvent_try(event::MiniEvent_t*& ev, unsigned int ientry,TString tag="data");

      /**
	 @short returns mini event for a given run/event/ls
      */
      event::MiniEvent_t *GetMiniEventFor(Int_t runNb, Int_t eventNb, Int_t ls, TString tag="data")
	{
	  if(m_evHandler.miniTrees.find(tag)== m_evHandler.miniTrees.end()) return 0;
	  Int_t serial=m_evHandler.miniTrees[tag]->GetEntryNumberWithIndex(runNb,eventNb);
	  if(serial<=0) return 0;
	  m_evHandler.miniTrees[tag]->GetEntry(serial);
	  if(runNb != m_evHandler.miniEvents[tag]->iRun) return 0;
	  if(ls != m_evHandler.miniEvents[tag]->iLumi) return 0;
	  if(eventNb != m_evHandler.miniEvents[tag]->iEvent) return 0;
	  return m_evHandler.miniEvents[tag];
	}
      
      /**
	 @short wraps up both p4 and information for objects in the event
	 @param ev the mini event
	 @param type particle type
	 @param sortObjects if true objects are sorted by pT
	 @return a collection of physics objects
      */
      PhysicsObjectCollection GetPhysicsObjectsFrom(event::MiniEvent_t *ev, unsigned int type, unsigned int algo=0,bool sortObjects=true);

      /**
	 @short retrieves the collection of vertices of a event
	 @param ev the mini event
	 @param sortObjects if true objects are sorted by pT
	 @return a collection of physics objects
       */
      PhysicsObjectCollection GetVertexCollection(event::MiniEvent_t *ev, bool sortObjects=true);


      /**
	 @short classifies the dileptons
      */
      enum DileptonArbitration { BASE=0, OS=1, ID=2, ISO=4};
      PhysicsObjectPairCollection GetDileptonsWithArbitration(PhysicsObjectCollection &electrons,
							      PhysicsObjectCollection &muons,
							      unsigned int dilArbitration=event::Reader::BASE,
							      bool hasEGtrig=true, bool hasMutrig=true);
      
      /**
	 @short retrieves a clean jet collection
       */
      enum JetArbitration { VTXCONSTRAINED=1, RESJECCORR=2, PARTONFLAVCORR=4 , BTAGORDERED=8 };
      PhysicsObjectCollection GetJetsWithArbitration( PhysicsObjectCollection &origJets, unsigned int jetArbitration=event::Reader::VTXCONSTRAINED);
					
      /**
	 @short cross cleans collections
	 @return a collection of clean objects
       */
      PhysicsObjectCollection PrunePhysicsObjectsFor(PhysicsObjectCollection &origObjects, PhysicsObjectCollection &otherObjects,double minDRallowed=0.4);	     
      
      /**
	 @short wrappers for lepton id and isolation
      */
      bool ElectronIsGood(PhysicsObject &electron);
      bool ElectronId(PhysicsObject &electron,int wp=90);
      bool ElectronIso(PhysicsObject &electron);
      bool MuonIsGood(PhysicsObject &muon);
      bool MuonId(PhysicsObject &muon);
      bool MuonIso(PhysicsObject &muon);
      bool LeptonId(PhysicsObject &lepton);
      bool LeptonIso(PhysicsObject &lepton);
      bool DileptonIsGood(PhysicsObjectPair &dilepton);

      TTree *GetMiniEventsTree(TString tag="data") { return m_evHandler.miniTrees[tag]; }
      event::MiniEvent_t *GetMiniEvent(TString tag="data") { return m_evHandler.miniEvents[tag]; }

    private:

      /**
	 @short returns true if arbitration is present
       */
      bool HasArbitration(unsigned int classif, unsigned int arbitration=event::Reader::BASE)
      {
	unsigned int maskedClassif = classif & arbitration;
	return (maskedClassif == arbitration);
      }

      //event handler
      event::Handler m_evHandler;

      //spy events
      TFile *spyEventsF_;
      std::map<TString, TTree* > spyEventsTrees_;
      
    };


  
//  class PhysicsEvent{
//  public:
//    PhysicsEvent(MiniEvent_t*);
//    TVectorD* trig;
//    TVectorD* classif; 
//    std::vector<PhysicsObject> jets_without_arbitration;
//    std::vector<PhysicsObject> vertices;
//    std::vector<PhysicsObject> jets;
//    std::vector<PhysicsObject> muons;
//    std::vector<PhysicsObject> electrons;
//    PhysicsObjectCollection mets;
//    // tau collection are needed for extracting the taus from the list of jets for trigger efficiencies considerations
//    std::vector<PhysicsObject> tausColl;
//    std::vector<PhysicsObject> taus; 
//
//  private:
//    
//    
//  };



}

#endif // _MiniEvent_hh
