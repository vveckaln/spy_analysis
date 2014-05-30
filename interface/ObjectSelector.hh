#ifndef _ObjectSelector_hh
#define _ObjectSelector_hh
/**                                                                                                                                                                              
																			 \class    ObjectSelector ObjectSelector.cc "UserCode/LIP/TopTaus/src/ObjectSelector.cc"                                                                     
																			 \brief    Class for selecting the physicsObjects present on the minitrees
  
  \author   Pietro Vischia, pietro.vischia@gmail.com

  \version  $Id: ObjectSelector.hh,v 1.1 2013/02/01 19:34:47 vischia Exp $                                                                                                       
*/

// System headers
#if !defined(__CINT__) || defined(__MAKECINT__)
#include <string>
#include <iostream>
#endif

// ROOT headers



#include "LIP/TopTaus/interface/MiniEvent.hh"

// OLD IMPORTANT NODE : in order to compile with these files one needs to remove JetCorrector Parameter from JetCorrectorParameters.h
//#include "/lip-sw/cmssw/users/nalmeida/new/CMSSW_4_2_4_patch1/src/CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
//#include "/lip-sw/cmssw/users/nalmeida/new/CMSSW_4_2_4_patch1/src/CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

// Forward declarations
class JetResolution;
class JetCorrectionUncertainty;


class ObjectSelector{
   
  public :

  ObjectSelector(double );

  inline double GetJetResidualPt(const PhysicsObject& j){
    return j.Pt();
  }
  
  void SetLeptonPlusJetsSelection();

  void SetDileptonSelection();

  // see if this event has other loose electron
  bool LooseElectronVeto(event::Reader*, int, const vector<PhysicsObject> &);
  
  // see if this event has other loose muons
  // note if selectedMuon is -1 it will run over all muons else it will skip the selectedMuon
  bool LooseMuonVeto( int, const vector<PhysicsObject> &);
  bool UltraLooseMuonVeto( int, const vector<PhysicsObject> &);

  // setting new cuts 
  inline void Emf_Jet( double emf )          { JET_EMF_MIN_ =  emf;    }
  inline void Pt_Jet(double cut)             { JET_PT_MIN_  =  cut;    }            
  inline void RelIso_Electron(double reliso ){ E_RELISO_MAX_=  reliso; }
  inline void RelIso_Muon(double reliso )    { M_RELISO_MAX_=  reliso; }
  
  //enable jet ltk cut
  inline void EnableLtkCutOnJets() { applyJetLtkCut_=true; }
  inline void DisableLtkCutOnJets(){ applyJetLtkCut_=false;}
  
  // preselection of objects
  void PreSelectElectrons(event::Reader*, vector<int>*, const vector<PhysicsObject>&, PhysicsObject&);
  void PreSelectMuons(    event::Reader*, vector<int>*, const vector<PhysicsObject>&, PhysicsObject&);
  void PreSelectUltraLooseMuons(    event::Reader*, vector<int>*, const vector<PhysicsObject>&, PhysicsObject&);
  void PreSelectJets( bool, vector<double>&,  double, JetCorrectionUncertainty*, int, vector<int>*, const vector<PhysicsObject>&);
  void PreSelectTaus( vector<int>*, const vector<PhysicsObject>&, int , TString&, PhysicsObject&, double);
  
  inline int GetNumbProngs( PhysicsObject& v){
    int numbProngs = v[3]; // check if this and decay mode give the same result for HPS
    return numbProngs;
  }
  
  inline int GetNumbProngsBasedOnDecayMode( PhysicsObject& v ){
    int numbProngs(0);
    int decayMode  = v[28];  
    if( decayMode == 0  || decayMode == 1  || decayMode == 2 ){ numbProngs=1;}
    if( decayMode == 10 || decayMode == 11 || decayMode == 12){ numbProngs=3;}
    return numbProngs;
  }
  
  // gets objects based on pt
  void GetObjectsBasedOnPt(
			   bool,
			   vector<double>&  ,
			   double,
			   JetCorrectionUncertainty*, 
			   int, vector<int>&, vector<int>&,
			   const vector<PhysicsObject>&, double, double 
			   );
  
  
  // object cleaning
  void ProcessSelfCleaning(  const vector<PhysicsObject>&, vector<int>*, vector<int>*, double );
  void ProcessCleaning( 
		       vector<int>*, vector<int>*, 
		       vector<int>*, vector<int>*,  
		       const vector<PhysicsObject>&, const vector<PhysicsObject>& , double 
		       );
  void ApplyCleaning( vector<int>*, vector<int>*, vector<int>*);

  // trigger efficiencies for muons
  void LeadMuonTriggerEfficiency(const vector<PhysicsObject>&, const int&, double&);
  
  // Data members
public:
  
  // Vertex
  double ZMAX_, ZTAUMAX_ ;
  
  // jet 
  double JET_PT_MIN_,JET_ETA_MAX_,JET_LTK_PT_MIN_, JET_BTAGGING_, JET_EMF_MIN_, JET_LTK_,JET_LEPTON_DRMIN_  ;
  
  // electron
  double E_RELISO_MAX_, E_ETA_MAX_, E_D0_MAX_, E_PT_MIN_, E_ET_MIN_,RHO_AEFF_E_;
  double LOOSE_E_RELISO_MAX_, LOOSE_E_ETA_MAX_, LOOSE_E_D0_MAX_, LOOSE_E_ET_MIN_;
  
  
  // muon
  double M_RELISO_MAX_, M_ETA_MAX_, M_D0_MAX_, M_PT_MIN_,RHO_AEFF_M_;  
  double LOOSE_M_RELISO_MAX_, LOOSE_M_ETA_MAX_, LOOSE_M_D0_MAX_, LOOSE_M_PT_MIN_;
  
  // taus
  double TAU_ETA_MAX_,TAU_RELISO_, TAU_EOP_CUT_,TAU_EMF_CUT_, TAU_D0_MAX_,ISO_ECAL_1P_ ,ISO_ECAL_3P_,TAU_EMF_MAX_;
  double TAU_PT_MIN_,TAU_TC_LEADTRKPT_MIN_,TAU_PF_LEADTRKPT_MIN_,TAU_LEADPFPT_MIN_ ;
  
  int TANC_FRDISCRIMINATOR_;
  int HPS_ISO_;
  
  
  bool applyJetLtkCut_;
  
  bool leptonPlusJetsSelection_;
  
  
  enum {JETS_TYPE};
  
  
  //JetCorrectorParameters * resCorPar_;
  //FactorizedJetCorrector * resFactors_;
  
  
};    


#endif
