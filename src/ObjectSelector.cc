#include "LIP/TopTaus/interface/ObjectSelector.hh"

#include "LIP/TopTaus/interface/CommonDefinitions.hh"
#include <iostream>
#include <iomanip>

#include "CondFormats/JetMETObjects/interface/JetResolution.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

using namespace std;
using namespace commondefinitions;


ObjectSelector::ObjectSelector( double tauPtCut ):
  TAU_PT_MIN_(tauPtCut)
{
  // Set Running conditions... /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  // b-tagging working points from https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagPerformanceOP#B_tagging_Operating_Points_for_3
  //      BTAGIND_=33; BTAG_CUT_ = 0.679;    // CSVM
  //      BTAGIND_=33; BTAG_CUT_ = 0.244;    // CSVL
  //      BTAGIND_=12; BTAG_CUT_ = 1.7;      // TCHEL
  
  // FIXME: move to cfg
  if( commondefinitions::run2012_ ) { commondefinitions::BTAGIND_=33; commondefinitions::BTAG_CUT_ = 0.679;} // CSVM 
  else { commondefinitions::BTAGIND_=33; commondefinitions::BTAG_CUT_=0.679 ;}  //  CSVM
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  // residual corrections in data ///////////////////////////////////////////////////////////////////////////////////////////////////////
  //resCorPar_      = new JetCorrectorParameters("/lustre/lip.pt/data/cmslocal/samples/CMSSW_4_2_X/mc/JEC/Jec11_V2_AK5PF_L2L3Residual.txt");
  //vector<JetCorrectorParameters> vParam; vParam.push_back(*resCorPar_);
  //resFactors_     = new FactorizedJetCorrector(vParam);  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //  cout << "[ObjectSelector::ObjectSelector] TAU PT MIN: " << TAU_PT_MIN_ << endl;
  SetLeptonPlusJetsSelection();

}

void ObjectSelector::SetLeptonPlusJetsSelection(){

  // by default apply lepton + jet selection
  leptonPlusJetsSelection_=true;
  
  //Vertex //////
  ZMAX_    = 0.5;
  ZTAUMAX_ = 0.5;
  ///////////////
  
  // Jets ///////////////////////////////////////////
  JET_PT_MIN_        = 30; // WARNING : Jet pt set to 20 for trigger studies
  //JET_PT_MIN_        = 30; 
  JET_ETA_MAX_       = 2.4;       // dilepton is 2.5 
  JET_EMF_MIN_       = 0.01;       
  JET_LTK_           = 5.;
  JET_LEPTON_DRMIN_  = 0.4;
  applyJetLtkCut_    = true; // TODAY: false
  //////////////////////////////////////////////////
  
  
  // electron //////////////////////////////////////
  // All values from TwikiTopRefEventSel#Electrons, lepton+jets selection
  // Signal
  E_RELISO_MAX_       = 0.1; // dilepton is 0.15
  E_ETA_MAX_          = 2.5;     
  E_ET_MIN_           = 35;      
  E_D0_MAX_           = 0.02; // dilepton is 0.04
  // Veto
  LOOSE_E_RELISO_MAX_ = 0.15;
  LOOSE_E_ETA_MAX_    = 2.5;
  LOOSE_E_ET_MIN_     = 20;
  RHO_AEFF_E_         = 0.24;
  //////////////////////////////////////////////////
  
  // muon //////////////////////////////////////////
  // All values from TwikiTopRefEventSel#Muons, lepton+jets selection
  // Signal
  M_RELISO_MAX_  = 0.12; // Tight WP    
  M_PT_MIN_      = 30; //30;  //WARNING M_PT_MIN_      = 20;               // dilepton is 20 // test on 70
  M_ETA_MAX_     = 2.1; // RunA trigger is still 2p1  -> 2.4 threshold means removing RunA. Trying that.
  M_D0_MAX_      = 0.2;                // dilepton is 0.20
  // Veto
  LOOSE_M_RELISO_MAX_ = 0.2; 
  LOOSE_M_ETA_MAX_    = 2.5; 
  LOOSE_M_PT_MIN_     = 10;
  RHO_AEFF_M_         = 0.112;
  ///////////////////////////////////////////////////
  
  
  // taus /////////////////////////////////////////////////////// 
  TAU_PF_LEADTRKPT_MIN_ = 5;  // WARNING 5;
  TAU_LEADPFPT_MIN_     = 10; // WARNING 10;  // lead track on pt
  TAU_ETA_MAX_          = 2.4; // TEST 2.3 (POG). But we usually use 2.4 for consistency with jet acceptance; 
  TAU_RELISO_           = 0.9;
  TAU_EOP_CUT_          = 0.5;
  TAU_EMF_CUT_          = 0.9;
  TAU_TC_LEADTRKPT_MIN_ = 10; // WARNING 10; 
  TAU_D0_MAX_           = 400;// was 200
  ISO_ECAL_1P_          = 1.5;// was 1
  ISO_ECAL_3P_          = 3;
  TAU_EMF_MAX_          = 2;  //0.95;
  ///////////////////////////////////////////////////////////////
  
}

void ObjectSelector::SetDileptonSelection(){
  
  //Vertex //////
  ZMAX_    = 1;
  ZTAUMAX_ = 0.2;
  ///////////////
  
  
  leptonPlusJetsSelection_=false;        
  
  // Jets///////////////////
  JET_ETA_MAX_       = 2.5;       //
  JET_EMF_MIN_       = 0.;        // which cut to be applied? 
  JET_LTK_           = 5.;       // warning
  applyJetLtkCut_    = true; // today: false
  JET_LEPTON_DRMIN_  = 0.4;
  
  
  // electron/////////////////
  E_RELISO_MAX_       = 0.15;  
  E_PT_MIN_           = 20;    
  E_D0_MAX_           = 0.04; 
  //////////////////////////// 
  
  // muon //////////////
  M_RELISO_MAX_  = 0.15; 
  M_ETA_MAX_     = 2.4;
  /////////////////////
  
}

bool ObjectSelector::LooseElectronVeto(event::Reader* reader, int selectedElectron, const vector<PhysicsObject>& vE){
  bool looseVeto(false);
  for( int i=0;i< (int) vE.size();++i ){
    if(i==selectedElectron){ continue; }
    
    const PhysicsObject * e = &vE[i];
    
    double eEta      = TMath::Abs(e->Eta());
    double eEt       = TMath::Abs(e->Et());
    double eRelIso   = (*e)[18];
    //double eD0       = fabs( (*e)[25] );
    bool   id        = reader->ElectronId( (PhysicsObject &)(*e),0);  // loose working point
    double minDR2mu  = (*e)[14];  
    double scEta    = (*e)[39];        
        
    if(id && eEt > LOOSE_E_ET_MIN_ && eEta < LOOSE_E_ETA_MAX_ && eRelIso < LOOSE_E_RELISO_MAX_ && minDR2mu >0.1){ looseVeto = true;}
  }
  return looseVeto;
}

bool ObjectSelector::LooseMuonVeto( int selectedMuon, const vector<PhysicsObject>& vM){

  bool looseVeto(false);
  
  for(int i=0;i< (int)vM.size();++i){
    
    if( i==selectedMuon ){continue;}
    
    const PhysicsObject * m = &vM[i];
    
    double mEta     = TMath::Abs(m->Eta());
    double mPt      = TMath::Abs(m->Pt());
    //double mD0      = fabs( (*m)[11]);
    double mRelIso  = (*m)[18];
    
    //see if this muon is glogal (how to see if it is only global?)
    bool isGlobalAndTracker ( (int( (*m)[3])>>1 & 0x3) == 3); // ( blah AND 0011 -> returns >0 only if both global and tracker (pos0 and pos1) are 1)
    
    if(! isGlobalAndTracker ) continue;
    
    if( mEta<LOOSE_M_ETA_MAX_  && mPt> LOOSE_M_PT_MIN_ && mRelIso < LOOSE_M_RELISO_MAX_ ){ looseVeto = true; }
    
  }
  
  return looseVeto;
  
}

bool ObjectSelector::UltraLooseMuonVeto( int selectedMuon, const vector<PhysicsObject>& vM){

  bool looseVeto(false);
  
  for(int i=0;i< (int)vM.size();++i){
    
    if( i==selectedMuon ){continue;}
    
    const PhysicsObject * m = &vM[i];
    
    double mEta     = TMath::Abs(m->Eta());
    double mPt      = TMath::Abs(m->Pt());
    //double mD0      = fabs( (*m)[11]);
    double mRelIso  = (*m)[18];
    
    //see if this muon is glogal (how to see if it is only global?)
    bool isGlobalAndTracker ( (int( (*m)[3])>>1 & 0x3) == 3); // ( blah AND 0011 -> returns >0 only if both global and tracker (pos0 and pos1) are 1)
    
    if(! isGlobalAndTracker ) continue;
    
    if( mEta<LOOSE_M_ETA_MAX_  && mPt> LOOSE_M_PT_MIN_ && mRelIso < 0.4 ){ looseVeto = true; }
    
  }
  
  return looseVeto;
  
}

void ObjectSelector::PreSelectElectrons( event::Reader* reader, vector<int>* e_i, const vector<PhysicsObject>& vE, PhysicsObject& vertex) {
  
  for(unsigned int i=0;i<vE.size();++i){
    
    const PhysicsObject * e = &vE[i]; 
    double scEta    = (*e)[39];  
    double eEta     = TMath::Abs(e->Eta()); 
    double eEt      = TMath::Abs(e->Et());
    double ePt      = TMath::Abs(e->Pt());
    double eRelIso  = (*e)[18];
    double minDR2mu = (*e)[14];
    double d0       = fabs((*e)[25]);
    bool   id       = reader->ElectronId((PhysicsObject &)(*e), 1);    //Tight working point
    //bool   id       = reader->ElectronId((PhysicsObject &)(*e), 0);  //Loose working point
    
    //Super TightId
    int v = (*e)[12];
    bool superTightId  =   (v >> 4) & 0x1 ; id = id && superTightId ;
    //cout<<endl<<" electron id : "<<v<<" very loose : "<<(v&0x1)<<" loose : "<<((v>>1)&0x1)<<" medium : "<<((v>>2)&0x1)<<" tight "<<((v>>3)&0x1)<<endl;
    
    
    double zvertex   = vertex[7];
    double zelectron = (*e)[29];
    bool   z(false); 
    if ( fabs(zvertex-zelectron) < ZMAX_ ){ z = true;}
    if(!z){ continue;}
    
    if( leptonPlusJetsSelection_  && ( fabs(scEta)> 1.4442 && fabs(scEta)<1.5660)  ){ continue; }
    if( leptonPlusJetsSelection_  && eEt < E_ET_MIN_ ){ continue; }
    if( !leptonPlusJetsSelection_ && ePt < E_PT_MIN_ ){ continue; }
    
    
    if(id && eEta < E_ETA_MAX_ && eRelIso < E_RELISO_MAX_ && minDR2mu >0.1){ e_i->push_back(i);}
    
  }
  
}

void ObjectSelector::PreSelectMuons( event::Reader* reader, vector<int>* m_i, const vector<PhysicsObject>& vM, PhysicsObject& vertex) {
  
  for( int i=0;i< (int) vM.size();++i){
    
    const PhysicsObject * m = &vM[i];
    
    double mEta     = TMath::Abs(m->Eta());
    double mPt      = TMath::Abs(m->Pt());
    double mD0      = fabs((*m)[11]);
    double mRelIso  = (*m)[18];
    
    bool id = reader->MuonId((PhysicsObject &) (*m));     
    
    
    double zvertex   = vertex[7];
    double zmuon     = (*m)[23];
    bool   z(false); 
    if ( fabs(zvertex-zmuon) < ZMAX_ ){ z = true;}
    if(!z){ continue;}
    
    
    if(id && mD0 < M_D0_MAX_&&  mPt > M_PT_MIN_ && mEta < M_ETA_MAX_ && mRelIso < M_RELISO_MAX_){ m_i->push_back(i);}        
    
  }
  
}

void ObjectSelector::PreSelectUltraLooseMuons( event::Reader* reader, vector<int>* m_i, const vector<PhysicsObject>& vM, PhysicsObject& vertex) {
  
  for( int i=0;i< (int) vM.size();++i){
    
    const PhysicsObject * m = &vM[i];
    
    double mEta     = TMath::Abs(m->Eta());
    double mPt      = TMath::Abs(m->Pt());
    double mD0      = fabs((*m)[11]);
    double mRelIso  = (*m)[18];
    
    bool id = reader->MuonId((PhysicsObject &) (*m));     
    
    
    double zvertex   = vertex[7];
    double zmuon     = (*m)[23];
    bool   z(false); 
    if ( fabs(zvertex-zmuon) < 1. ){ z = true;}
    if(!z){ continue;}
    
    
    if(id && mD0 < M_D0_MAX_&&  mPt > M_PT_MIN_ && mEta < M_ETA_MAX_ && mRelIso < 0.2){ m_i->push_back(i);}        
    
  }
  
}

void ObjectSelector::PreSelectJets( bool isData, vector<double>& jerFactors, double jes, JetCorrectionUncertainty* junc, int jetAlgo, vector<int>* j_i, const vector<PhysicsObject>& vJ) {
  
  for(unsigned int i=0;i<vJ.size();++i){
    
    double jetEta     = TMath::Abs(vJ[i].Eta());
    double jetPt      = TMath::Abs(vJ[i].Pt());
    double minDRToLep = TMath::Abs(vJ[i][15]);
    
    if(jerFactors.size() !=0 ) jetPt = jerFactors[i]*jetPt;
    
    if(isData) jetPt = GetJetResidualPt(vJ[i]);
    
    double corr(0);
    
    // JES corrections ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //old proceadure
    if(junc==0){
      if( jes     ){ corr = sqrt( jes*jes +  ( (0.02)*jetEta *(0.02)*jetEta ) ) ;}
      if( jes >0  ){ jes = corr;} else if( jes <0 ){ jes =(-1)*corr;             }
    }
    
    //new proceadure
    else{
      if( jes     ){ junc->setJetEta(jetEta); junc->setJetPt(jetPt); corr=junc->getUncertainty(true); }
      if( jes >0  ){ jes = corr;} else if( jes <0 ){ jes =(-1)*corr;}
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    //apply jes correction
    jetPt = (1+jes)*jetPt;
    //////////////////////
    
    
    // emf is different for different types
    // NOTE jetEmf is computed differently according to jet Algo
    double jetLtk  = vJ[i][25]; 
    
    if( applyJetLtkCut_  &&  jetLtk<JET_LTK_ ) continue;  // WARNING !!
    if( minDRToLep < JET_LEPTON_DRMIN_       ) continue;
    
    
    if(jetPt > JET_PT_MIN_ && jetEta < JET_ETA_MAX_ /*&& jetEmf > JET_EMF_MIN*/ ){ j_i->push_back(i);}
    
  }
  
}

void ObjectSelector::PreSelectTaus( vector<int>* t_i, const vector<PhysicsObject>& vT, int requiredProngs, TString& myKey, PhysicsObject& vertex, double tes){
  
  // cout<<endl<<" tau collection size is : "<<vT.size()<<endl;
  
  bool applyTanc(false);
  bool applyHPS(false);

  if( myKey =="PFlow"   ){ applyHPS =true; HPS_ISO_ = HPS_MEDIUMISO; }  // MEDIUM WORKING POINT  
  else if(      myKey =="TaNC0"   ){ applyTanc=true; TANC_FRDISCRIMINATOR_ = TANCFR_ONE;    }
  else if( myKey =="TaNC1"   ){ applyTanc=true; TANC_FRDISCRIMINATOR_ = TANCFR_HALF;   }
  else if( myKey =="TaNC2"   ){ applyTanc=true; TANC_FRDISCRIMINATOR_ = TANCFR_QUARTER;} 
  else if( myKey =="TaNC3"   ){ applyTanc=true; TANC_FRDISCRIMINATOR_ = TANCFR_TENTH;  } 
  else if( myKey =="HPS0"    ){ applyHPS =true; HPS_ISO_ = HPS_LOOSEISO;   } 
  else if( myKey =="HPS1"    ){ applyHPS =true; HPS_ISO_ = HPS_MEDIUMISO;  } 
  else if( myKey =="HPS2"    ){ applyHPS =true; HPS_ISO_ = HPS_TIGHTISO;   } 
  //else if( myKey =="PFlow" ){ applyHPS =true; HPS_ISO_ = HPS_LOOSEISO;  }  // LOOSE WORKING POINT WARNING
  
  
  
  for(unsigned int i=0;i<vT.size();++i){
    
    
    double tauEta            = TMath::Abs(vT[i].Eta());
    double tauPt             = TMath::Abs(vT[i].Pt())*(1+tes);
    double tauLtk            = vT[i][4];   // LEAD track /charged hadron pt
    double tauLeadPF         = vT[i][5];   // LEAD PF PT
    double tauEmf            = vT[i][15];  // EMF
    int    numberOfProngs    = vT[i][3];
    //double absIso          = vT[i][6];  //ISO_ABSISOPT
    int    type              = vT[i][17]; 
    //double tauJetPt        = vT[i][7];
    //double relIso          = (tauJetPt-absIso)/tauJetPt;
    double d0                = fabs( vT[i][11] );
    int charge               = fabs( vT[i][0]  );
    
    // isobits
    
    // position 0 (default) , position 1 (delta beta corrections), position 2 (combined iso), position 3 (deltabeta 3hits)
    int isobits              = vT[i][20];  int tau_dis_iso =  isobits & 0x8; // deltabeta 3hits is binary 100
    
    double tau_dis_muon      = vT[i][21];
    double tau_dis_electron  = vT[i][22];
    int decayMode            = vT[i][28];  
    
    double zvertex   = vertex[7];
    double ztau     = vT[i][30];
    bool   z(false); 
    if ( fabs(zvertex-ztau) < ZTAUMAX_ ){ z = true; }
    if(!z){ continue;}
    
    
    //if(tau_dis_muon && tau_dis_electron && )
    //cout<<endl<<" tauLtk : "<<tauLtk<<" lead PF "<<tauLeadPF<<" number p. "<<" tau_dis_iso "<<tau_dis_iso<<" tau_dis_muon "<<tau_dis_muon<<" tau_dis_electron "<< tau_dis_electron<<endl;
    
    //cout<<endl<<" \n ========= ";
    //cout<<endl<<" tau dis iso = "<<tau_dis_iso<<" dis_muon = "<<tau_dis_electron<<" charge = "<<charge<<" numb prongs : "<<numberOfProngs<<endl;
    
    // The number of prongs is only checked for PF/TC Taus not for HPS or Tanc /////////////
    if( !applyTanc && ! applyHPS ){
      
      
      if     ( requiredProngs == 0 ){ if (numberOfProngs!=1 && numberOfProngs!=3) continue; }
      else if( requiredProngs == 1 ){ if (numberOfProngs!=1)                      continue; }
      else if( requiredProngs == 3 ){ if (numberOfProngs!=3)                      continue; }
      // require sign
      if( charge != 15 ){ continue; } 
    }
    ///////////////////////////////////////////////////////////////////////////////////////
    
    
    // In HPS we cannot access to signal tracks => not possible to compute d0
    if( ! applyHPS ){ if( d0>TAU_D0_MAX_) continue; }
    /////////////////////////////////////////////////////////////////////////
    
    
    // Require discriminators on leptons and tau pt and eta ///////////////////////////////////////////////////////////////////////
    /*
      Reference. Values for the discriminators to cut on:
    againstMuonLoose   : tau_dis_muon = 1;
    againstMuonMedium  : tau_dis_muon = 2;
    againstMuonTight   : tau_dis_muon = 3;
    againstMuonLoose2  : tau_dis_muon = 4;
    againstMuonMedium2 : tau_dis_muon = 5;
    againstMuonTight2  : tau_dis_muon = 6;

    againstElectronLoose      : tau_dis_electron= 1;
    againstElectronMedium     : tau_dis_electron= 2;
    againstElectronTight      : tau_dis_electron= 3;
    againstElectronLooseMVA3  : tau_dis_electron= 4;
    againstElectronMediumMVA3 : tau_dis_electron= 5;
    againstElectronTightMVA3  : tau_dis_electron= 6;
    */ 
    
    
    //if( tau_dis_muon == 0 || tau_dis_electron == 0 || tauPt < TAU_PT_MIN_ || tauEta > TAU_ETA_MAX_ ) continue;    // LooseLoose
    //    if( tau_dis_muon == 0 || tau_dis_electron < 2 || tauPt < TAU_PT_MIN_ || tauEta > TAU_ETA_MAX_ ) continue;        //LooseMedium
    //if( tau_dis_muon < 2 || tau_dis_electron < 3 || tauPt < TAU_PT_MIN_ || tauEta > TAU_ETA_MAX_ ) continue;       //TightTight

    // Only 4,5,6 work for electrons/muons, until next CRAB reprocessing.
    if( tau_dis_muon < 6 || tau_dis_electron < 5 || tauPt < TAU_PT_MIN_ || tauEta > TAU_ETA_MAX_ ) continue;        //MuonTight2 ElectronMedium
    //    if( tau_dis_muon < 5 || tau_dis_electron < 5 || tauPt < TAU_PT_MIN_ || tauEta > TAU_ETA_MAX_ ) continue;        //MuonMedium2 ElectronMedium

    // If putting electron descrimination against electron to tight no muons pass...  
    // if( tau_dis_muon == 0 || tau_dis_electron < 2.5 || tauPt < TAU_PT_MIN_ || tauEta > TAU_ETA_MAX_ ) continue;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    if( tauEmf >= TAU_EMF_MAX_ ) continue;
    
    
    if(type == PFTAU) {   
      
      // extra tanc variables
      if( applyTanc ){
	double discByFR   =vT[i][TANC_FRDISCRIMINATOR_]; 
        
	//WARNING 1): which value of TANC to be used ???
	//double discByTaNC =vT[i][23];
	//double TANC_BYTANC_=0.5; //?????????
	if( /*discByTaNC < TANC_BYTANC_ || */ discByFR < 1. ){ continue; }
	//WARNING 2): Shall we also use lead track and lead pf pf for tanc?
	//if( tauLtk > TAU_PF_LEADTRKPT_MIN_ && tauLeadPF > TAU_LEADPFPT_MIN_){t_i->push_back(i);}
	t_i->push_back(i);
      }
      else if(tau_dis_iso && tauLtk > TAU_PF_LEADTRKPT_MIN_ && tauLeadPF > TAU_LEADPFPT_MIN_ /*&& relIso > TAU_RELISO_*/){ t_i->push_back(i);}
      
      
    }
    else if(type == CALOTAU){
      
      double isoTkPt       = vT[i][13] ;     //iso tkpt sum
      double isoEcalSumPt  = vT[i][14] ;     //iso ecal et sum
      
      double eop(0);
      double e = TMath::Abs(vT[i].E());
      double p = vT[i][16]; // LEAD track P
      if(p) eop = e/p ;
      
      if( isoTkPt != 0 || (numberOfProngs == 1 && isoEcalSumPt > ISO_ECAL_1P_) || (numberOfProngs == 3 && isoEcalSumPt > ISO_ECAL_3P_) ){ continue; }
      
      
      // apply tau muon discrimination only since electron discrimination is applyied by default to tc taus
      if( tau_dis_iso && tauLtk > TAU_TC_LEADTRKPT_MIN_ /* && emf < TAU_EMF_CUT_ */ && eop >TAU_EOP_CUT_ ){ t_i->push_back(i);}
      
      
    }
    else if( type == HPSTAU  ){
      // WARNING DISCRIMINATOR BY DECAY MODE FINDING in 38x = LeadingTrackFinding > 0.5 (this is applied by default to all)
      /*double discByDecayModeFinding = vT[i][xxx]; */ 
      int discByIsolation = ((int) vT[i][HPS_ISO_]) & 0x1;
      
      if( /* discByDecayModeFinding ==0 ||*/  discByIsolation == 0){ continue; }     
      //// Delta Beta:
      //if( /* discByDecayModeFinding ==0 ||*/  discByIsolation < 2){ continue; }            
      
      //decayMode() == (N_charged - 1)*5 + N_piZeros
      //So:
      //0 = 1prong0pi0 ,1 = 1prong1pi0, 2 = 1prong2pi0 ... 10 = 3prong0pi0
      /*
        if( requiredProngs == 1 ){ if( decayMode != 0  && decayMode != 1  && decayMode != 2                                                           ) continue; }
        if( requiredProngs == 3 ){ if(                                                          decayMode != 10 && decayMode != 11 && decayMode != 12 ) continue; }
        if( requiredProngs == 0 ){ if( decayMode != 0  && decayMode != 1  && decayMode != 2  && decayMode != 10 && decayMode != 11 && decayMode != 12 ) continue; }
	
        // use tight electron discrimination
        if( tau_dis_electron < 3 ){ continue;}
	
        if     ( requiredProngs == 0 ){ if (numberOfProngs!=1 && numberOfProngs!=3) continue; }
        else if( requiredProngs == 1 ){ if (numberOfProngs!=1)                      continue; }
        else if( requiredProngs == 3 ){ if (numberOfProngs!=3)                      continue; }
      */
      t_i->push_back(i);
      //cout<<" decayMode "<<decayMode <<endl;
    }
    else if( type == PFLOWTAU  ){
      // WARNING DISCRIMINATOR BY DECAY MODE FINDING in 38x = LeadingTrackFinding > 0.5 (this is applied by default to all)
      /*double discByDecayModeFinding = vT[i][xxx]; */ 
      int discByIsolation = ((int) vT[i][HPS_ISO_] >> 2) & 0x1;
      
      
      
      if( /* discByDecayModeFinding ==0 ||*/  discByIsolation == 0){ continue; }            
      //	// Delta Beta:
      //        if( /* discByDecayModeFinding ==0 ||*/  discByIsolation < 2){ continue; }            
      
      //decayMode() == (N_charged - 1)*5 + N_piZeros
      //So:
      //0 = 1prong0pi0 ,1 = 1prong1pi0, 2 = 1prong2pi0 ... 10 = 3prong0pi0
      /*
        if( requiredProngs == 1 ){ if( decayMode != 0  && decayMode != 1  && decayMode != 2                                                           ) continue; }
        if( requiredProngs == 3 ){ if(                                                          decayMode != 10 && decayMode != 11 && decayMode != 12 ) continue; }
        if( requiredProngs == 0 ){ if( decayMode != 0  && decayMode != 1  && decayMode != 2  && decayMode != 10 && decayMode != 11 && decayMode != 12 ) continue; }
	
        // use tight electron discrimination
        if( tau_dis_electron < 3 ){ continue;}

        if     ( requiredProngs == 0 ){ if (numberOfProngs!=1 && numberOfProngs!=3) continue; }
        else if( requiredProngs == 1 ){ if (numberOfProngs!=1)                      continue; }
        else if( requiredProngs == 3 ){ if (numberOfProngs!=3)                      continue; }
      */
      t_i->push_back(i);
      //cout<<" decayMode "<<decayMode <<endl;
    }
  }
}

void ObjectSelector::GetObjectsBasedOnPt( 
					 bool isData,
					 vector<double>& jerFactors ,
					 double es,  JetCorrectionUncertainty* junc,
					 int type, vector<int>& obj_output, vector<int>& obj_input,
					 const vector<PhysicsObject>& v, double ptmin, double ptmax 
					 ){
  
  
  for(unsigned int i=0;i<obj_input.size();++i){
    int obj_ind = obj_input[i];
    double objPt  = TMath::Abs(v[obj_ind].Pt());
    
    // take into account jes(eta) with a 2% dependencie as recommended by the jes met group ///
    if( type == ObjectSelector::JETS_TYPE ){
      double corr(0);
      double jetEta = TMath::Abs(v[obj_ind].Eta());
      double jetPt = TMath::Abs(v[obj_ind].Pt());
      
      if(isData) jetPt = GetJetResidualPt( v[obj_ind]);
      
      if(jerFactors.size() !=0 ) jetPt = jetPt*jerFactors[obj_ind];
      
      
      if(junc==0){
	if( es ){    corr= sqrt( es*es +  ( (0.02)*jetEta *(0.02)*jetEta ) ) ;}
	if( es >0  ){ es = corr;} else if( es <0 ){ es =(-1)*corr;}
      }else{
	if( es     ){ junc->setJetEta(jetEta); junc->setJetPt(objPt); corr=junc->getUncertainty(true); }
	if( es >0  ){ es = corr;} else if( es <0 ){ es =(-1)*corr;}
      }
      
    }
    ////////////////////////////////////////////////////////////////////////////////////////////
    
    objPt = (1+es)*objPt;
    
    if(objPt<ptmin) continue;
    
    if     ( ptmax == 0    ){ obj_output.push_back(obj_ind); }
    else if( objPt < ptmax ){ obj_output.push_back(obj_ind); }
    
  }       
  
}

void ObjectSelector::ProcessSelfCleaning(  const vector<PhysicsObject>& vParticle, vector<int>* vi_old, vector<int>* vi_new, double DR ){

  vector<int>::iterator itv1, itv2;
  map< int, vector<int> * >::iterator itm1;
  
  // map of non isolated particles and vector of particles in their neighborhood 
  map< int, vector<int> * > nonIso;  
  
  //Build map of non isolated particules and fill new index of isolated ones //////////
  for(itv1 = vi_old->begin(); itv1 != vi_old->end(); ++itv1){
    
    vector<int> * p = new vector<int>;
    
    for(itv2 = vi_old->begin(); itv2 != vi_old->end(); ++itv2){ 
      if (  *itv1 != *itv2 ){
	double dr =vParticle[(*itv1)].DeltaR(vParticle[(*itv2)]);
	if(dr<DR){  p->push_back(*itv2); }
      }
    }
    if ( p->size() !=0 ) { nonIso[(*itv1)] = p;  }
    else{ delete p; vi_new->push_back(*itv1); }  
  }
  
}

void ObjectSelector::ProcessCleaning( 
				     vector<int>* va_old, vector<int>* a_remove, vector<int>* vb_old, vector<int>* b_remove,  
				     const vector<PhysicsObject>& aParticles, const vector<PhysicsObject>& bParticles, double DR ){  
  
  vector<int>::iterator itv1,itv2;
  
  for(itv1 = va_old->begin(); itv1 != va_old->end(); ++itv1){
    if( find(a_remove->begin(),a_remove->end(), *itv1 ) ==  a_remove->end() ){
      
      
      for( itv2 = vb_old->begin(); itv2 != vb_old->end(); ++itv2){
	
	if( find(b_remove->begin(),b_remove->end(), *itv2 ) ==  b_remove->end() ){
	  double dr =aParticles[(*itv1)].DeltaR(bParticles[(*itv2)]);
	  if(dr<DR){  a_remove->push_back(*itv1); }
	}
      }
    }
    
  }
  
}
 

void ObjectSelector::ApplyCleaning( vector<int>* i_old , vector<int>* i_remove, vector<int>* i_new){
  vector<int>::iterator itv1,itv2; 
  for(itv1 = i_old->begin(); itv1 != i_old->end(); ++itv1){
    if( find( i_remove->begin(), i_remove->end(), *itv1) == i_remove->end()){  i_new->push_back(*itv1); }
  }
}

void ObjectSelector::LeadMuonTriggerEfficiency(const vector<PhysicsObject>& muons, const int& ind, double& muontriggerefficiency){
  // Muon trigger/ID/Iso scale factors for efficiency are taken from https://twiki.cern.ch/twiki/bin/viewauth/CMS/MuonReferenceEffs
  // FIXME: implement non-pt dependent and implement non-fabs dependent
  PhysicsObject muon = muons[ind];

  double abseta = fabs( muon.Eta() );
  double pt = muon.Pt();

  // TOTAL // pt>500 assumed flat and uses 140.<pt<500. SF.
  if(abseta>=0. && abseta <0.9){ // ABCD
    if(pt>=140. /*&& pt<500.*/) muontriggerefficiency=0.98041749810533507;
    if(pt>=25.  && pt<30.)      muontriggerefficiency=0.98372524384334614;
    if(pt>=30.  && pt<35.)      muontriggerefficiency=0.98406344315477012;
    if(pt>=35.  && pt<40.)      muontriggerefficiency=0.98391658181685537;
    if(pt>=40.  && pt<50.)      muontriggerefficiency=0.98345252700570363;
    if(pt>=50.  && pt<60.)      muontriggerefficiency=0.98429177039157478;
    if(pt>=60.  && pt<90.)      muontriggerefficiency=0.98467201842489449;
    if(pt>=90.  && pt<140.)     muontriggerefficiency=0.98091711658069591;
  }  
  if(abseta>=0.9 && abseta <1.2){ // ABCD
    if(pt>=140. /*&& pt<500.*/) muontriggerefficiency=0.97127896196175556;
    if(pt>=25.  && pt<30.)      muontriggerefficiency=0.96838127559931908;
    if(pt>=30.  && pt<35.)      muontriggerefficiency=0.96538054889610103;
    if(pt>=35.  && pt<40.)      muontriggerefficiency=0.96696514151670487;
    if(pt>=40.  && pt<50.)      muontriggerefficiency=0.96667958160832501;
    if(pt>=50.  && pt<60.)      muontriggerefficiency=0.96273957552501865;
    if(pt>=60.  && pt<90.)      muontriggerefficiency=0.95952416834753307;
    if(pt>=90.  && pt<140.)     muontriggerefficiency=0.96444182461126438;
  }	  
  if(abseta>=1.2 && abseta <2.1){ // ABCD
    if(pt>=140. /*&& pt<500.*/) muontriggerefficiency=0.99416866829048334;
    if(pt>=25.  && pt<30.)      muontriggerefficiency=1.0051991254438037;
    if(pt>=30.  && pt<35.)      muontriggerefficiency=1.0013781590159485;
    if(pt>=35.  && pt<40.)      muontriggerefficiency=0.99616640424792002;
    if(pt>=40.  && pt<50.)      muontriggerefficiency=0.99425410141043047;
    if(pt>=50.  && pt<60.)      muontriggerefficiency=0.99054467301217797;
    if(pt>=60.  && pt<90.)      muontriggerefficiency=0.98829374192885855;
    if(pt>=90.  && pt<140.)     muontriggerefficiency=0.98187598993908232;
  }



    /* Naked values:
  In [19]: dict['IsoMu24']['TightID_IsodB']['PT_ABSETA_Barrel_0to0p9']       
    Out[19]: 
    {'140_500': {'data': {'PT_ABSETA_Barrel_0to0p9': 170.923905126562,
	    'efficiency': 0.93843876780571422,
	    'err_hi': 0.0050666130039955107,
	    'err_low': 0.0053491416097449064},
	  'data/mc': {'PT_ABSETA_Barrel_0to0p9': 174.76713166318004,
	    'efficiency_ratio': 0.98041749810533507,
	    'err_hi': 0.0081332613489531018,
	    'err_low': 0.0087466829592699382},
	  'mc': {'PT_ABSETA_Barrel_0to0p9': 174.76713166318004,
	    'efficiency': 0.95718280183621252,
	    'err_hi': 0.0060287203476393301,
	    'err_low': 0.0065691362155674664}},
	'25_30': {'data': {'PT_ABSETA_Barrel_0to0p9': 27.834995486240917,
	    'efficiency': 0.91820621634353716,
	    'err_hi': 0.00056495672179433592,
	    'err_low': 0.00056683536835262977},
	  'data/mc': {'PT_ABSETA_Barrel_0to0p9': 27.837963829313658,
	    'efficiency_ratio': 0.98372524384334614,
	    'err_hi': 0.0010031080858819209,
	    'err_low': 0.0010075240449770098},
	  'mc': {'PT_ABSETA_Barrel_0to0p9': 27.837963829313658,
	    'efficiency': 0.93339702532809798,
	    'err_hi': 0.00075899701248552463,
	    'err_low': 0.00076280593916992767}},
	'30_35': {'data': {'PT_ABSETA_Barrel_0to0p9': 32.746641896839911,
	    'efficiency': 0.92936382610783175,
	    'err_hi': 0.00036451752750855503,
	    'err_low': 0.00036538697765936491},
	  'data/mc': {'PT_ABSETA_Barrel_0to0p9': 32.749910446162517,
	    'efficiency_ratio': 0.98406344315477012,
	    'err_hi': 0.00063535205129759502,
	    'err_low': 0.0006375087089531662},
	  'mc': {'PT_ABSETA_Barrel_0to0p9': 32.749910446162517,
	    'efficiency': 0.94441454214417409,
	    'err_hi': 0.00048434211196648536,
	    'err_low': 0.0004862718563105615}},
	'35_40': {'data': {'PT_ABSETA_Barrel_0to0p9': 37.670601736313046,
	    'efficiency': 0.9355237458999156,
	    'err_hi': 0.00026020574031778375,
	    'err_low': 0.00026075425515741151},
	  'data/mc': {'PT_ABSETA_Barrel_0to0p9': 37.673364126749753,
	    'efficiency_ratio': 0.98391658181685537,
	    'err_hi': 0.00044422220647608669,
	    'err_low': 0.00044558990769502421},
	  'mc': {'PT_ABSETA_Barrel_0to0p9': 37.673364126749753,
	    'efficiency': 0.95081611915963471,
	    'err_hi': 0.00033814326314450581,
	    'err_low': 0.00033938500977268927}},
	'40_50': {'data': {'PT_ABSETA_Barrel_0to0p9': 44.340513019802621,
	    'efficiency': 0.94070359339739529,
	    'err_hi': 0.00020041245410647231,
	    'err_low': 0.00016124224017327959},
	  'data/mc': {'PT_ABSETA_Barrel_0to0p9': 44.342917008694222,
	    'efficiency_ratio': 0.98345252700570363,
	    'err_hi': 0.00029736820137356149,
	    'err_low': 0.00027066643447339175},
	  'mc': {'PT_ABSETA_Barrel_0to0p9': 44.342917008694222,
	    'efficiency': 0.95653177714793713,
	    'err_hi': 0.00020524316783116539,
	    'err_low': 0.00020596859995901333}},
	'50_60': {'data': {'PT_ABSETA_Barrel_0to0p9': 53.789342847075453,
	    'efficiency': 0.94402302093113344,
	    'err_hi': 0.00035464571076777938,
	    'err_low': 0.00035450976318700137},
	  'data/mc': {'PT_ABSETA_Barrel_0to0p9': 53.75578899867358,
	    'efficiency_ratio': 0.98429177039157478,
	    'err_hi': 0.00060283573195383719,
	    'err_low': 0.00060257389558970441},
	  'mc': {'PT_ABSETA_Barrel_0to0p9': 53.75578899867358,
	    'efficiency': 0.95908860495255233,
	    'err_hi': 0.0004639165894083157,
	    'err_low': 0.00046370081704151024}},
	'60_90': {'data': {'PT_ABSETA_Barrel_0to0p9': 68.730391710877825,
	    'efficiency': 0.94437317913039642,
	    'err_hi': 0.00055534840916684472,
	    'err_low': 0.00055764428023759923},
	  'data/mc': {'PT_ABSETA_Barrel_0to0p9': 68.736048944738855,
	    'efficiency_ratio': 0.98467201842489449,
	    'err_hi': 0.00092756084334772685,
	    'err_low': 0.00093371028168127751},
	  'mc': {'PT_ABSETA_Barrel_0to0p9': 68.736048944738855,
	    'efficiency': 0.95907384536125939,
	    'err_hi': 0.0007057823464877021,
	    'err_low': 0.00071158391170965895}},
	'90_140': {'data': {'PT_ABSETA_Barrel_0to0p9': 104.5256845587222,
	    'efficiency': 0.93898276097766487,
	    'err_hi': 0.0018674369359216896,
	    'err_low': 0.0018932808300815968},
	  'data/mc': {'PT_ABSETA_Barrel_0to0p9': 104.53990109043714,
	    'efficiency_ratio': 0.98091711658069591,
	    'err_hi': 0.0031287565107191648,
	    'err_low': 0.0032070455991960061},
	  'mc': {'PT_ABSETA_Barrel_0to0p9': 104.53990109043714,
	    'efficiency': 0.95724984823467363,
	    'err_hi': 0.0023870719016186688,
	    'err_low': 0.0024636313491742516}}}

    In [20]: 
      In [22]: dict['IsoMu24']['TightID_IsodB']['PT_ABSETA_Transition_0p9to1p2']       
      Out[22]: 
      {'140_500': {'data': {'PT_ABSETA_Transition_0p9to1p2': 171.02307488508285,
	      'efficiency': 0.86413759128998668,
	      'err_hi': 0.015269347301468728,
	      'err_low': 0.016122725659541096},
	    'data/mc': {'PT_ABSETA_Transition_0p9to1p2': 174.45408481156858,
	      'efficiency_ratio': 0.97127896196175556,
	      'err_hi': 0.025376053351416492,
	      'err_low': 0.02743437935083038},
	    'mc': {'PT_ABSETA_Transition_0p9to1p2': 174.45408481156858,
	      'efficiency': 0.88969042379403707,
	      'err_hi': 0.01712186169392001,
	      'err_low': 0.018867091511550003}},
	  '25_30': {'data': {'PT_ABSETA_Transition_0p9to1p2': 27.751889478949067,
	      'efficiency': 0.82194544122367552,
	      'err_hi': 0.0013039477018938418,
	      'err_low': 0.0013090479954735584},
	    'data/mc': {'PT_ABSETA_Transition_0p9to1p2': 27.755085504117098,
	      'efficiency_ratio': 0.96838127559931908,
	      'err_hi': 0.0025787575866262039,
	      'err_low': 0.002591557665753458},
	    'mc': {'PT_ABSETA_Transition_0p9to1p2': 27.755085504117098,
	      'efficiency': 0.84878287295980992,
	      'err_hi': 0.0018154079701705886,
	      'err_low': 0.0018254691006806723}},
	  '30_35': {'data': {'PT_ABSETA_Transition_0p9to1p2': 32.678887519886544,
	      'efficiency': 0.82914484921229503,
	      'err_hi': 0.00097776324221852126,
	      'err_low': 0.00098229607003779708},
	    'data/mc': {'PT_ABSETA_Transition_0p9to1p2': 32.686478674633101,
	      'efficiency_ratio': 0.96538054889610103,
	      'err_hi': 0.0018925540568696933,
	      'err_low': 0.001901806697793774},
	    'mc': {'PT_ABSETA_Transition_0p9to1p2': 32.686478674633101,
	      'efficiency': 0.85887876046436862,
	      'err_hi': 0.0013450830109056149,
	      'err_low': 0.0013518520345241827}},
	  '35_40': {'data': {'PT_ABSETA_Transition_0p9to1p2': 37.681183600885269,
	      'efficiency': 0.83613679390494133,
	      'err_hi': 0.00074921390039779884,
	      'err_low': 0.00075080864902723229},
	    'data/mc': {'PT_ABSETA_Transition_0p9to1p2': 37.691073306871196,
	      'efficiency_ratio': 0.96696514151670487,
	      'err_hi': 0.0014385864945490831,
	      'err_low': 0.001442964213476463},
	    'mc': {'PT_ABSETA_Transition_0p9to1p2': 37.691073306871196,
	      'efficiency': 0.86470210559342753,
	      'err_hi': 0.0010269441259473256,
	      'err_low': 0.0010306034078939811}},
	  '40_50': {'data': {'PT_ABSETA_Transition_0p9to1p2': 44.352429207734467,
	      'efficiency': 0.8446275610931202,
	      'err_hi': 0.00046265180835233164,
	      'err_low': 0.00046327640031021351},
	    'data/mc': {'PT_ABSETA_Transition_0p9to1p2': 44.355027480489127,
	      'efficiency_ratio': 0.96667958160832501,
	      'err_hi': 0.00052977221668667205,
	      'err_low': 0.00053048670702444901},
	    'mc': {'PT_ABSETA_Transition_0p9to1p2': 44.355027480489127,
	      'efficiency': 0.87374097597868028,
	      'err_hi': 1.5154813859663552e-05,
	      'err_low': 1.5154813859663552e-05}},
	  '50_60': {'data': {'PT_ABSETA_Transition_0p9to1p2': 53.759612006848243,
	      'efficiency': 0.84718518125856201,
	      'err_hi': 0.0010337855331709819,
	      'err_low': 0.0010357567140461829},
	    'data/mc': {'PT_ABSETA_Transition_0p9to1p2': 53.733448441803397,
	      'efficiency_ratio': 0.96273957552501865,
	      'err_hi': 0.0019297478815599588,
	      'err_low': 0.0019327403824020816},
	    'mc': {'PT_ABSETA_Transition_0p9to1p2': 53.733448441803397,
	      'efficiency': 0.87997336226316403,
	      'err_hi': 0.0013993299901930675,
	      'err_low': 0.0014012065047039721}},
	  '60_90': {'data': {'PT_ABSETA_Transition_0p9to1p2': 68.705285238787667,
	      'efficiency': 0.84357616695526649,
	      'err_hi': 0.0016473739496371941,
	      'err_low': 0.0016536505073870831},
	    'data/mc': {'PT_ABSETA_Transition_0p9to1p2': 68.72628644673695,
	      'efficiency_ratio': 0.95952416834753307,
	      'err_hi': 0.0030221102906236239,
	      'err_low': 0.0030421868554624618},
	    'mc': {'PT_ABSETA_Transition_0p9to1p2': 68.72628644673695,
	      'efficiency': 0.87916093703825182,
	      'err_hi': 0.0021724933679449282,
	      'err_low': 0.0021907610172522667}},
	  '90_140': {'data': {'PT_ABSETA_Transition_0p9to1p2': 104.50536913350477,
	      'efficiency': 0.84665919222547592,
	      'err_hi': 0.0054802608877182364,
	      'err_low': 0.0055789765024975143},
            'data/mc': {'PT_ABSETA_Transition_0p9to1p2': 104.51898738480921,
	      'efficiency_ratio': 0.96444182461126438,
	      'err_hi': 0.010055105850685283,
	      'err_low': 0.010200424767055799},
            'mc': {'PT_ABSETA_Transition_0p9to1p2': 104.51898738480921,
	      'efficiency': 0.87787481900915809,
	      'err_hi': 0.0071750195291853123,
	      'err_low': 0.0072626452059437133}}}

      In [23]: 
	In [24]: dict['IsoMu24']['TightID_IsodB']['PT_ABSETA_Endcaps_1p2to2p1']       
	Out[24]: 
	{'140_500': {'data': {'PT_ABSETA_Endcaps_1p2to2p1': 168.1525442137135,
		'efficiency': 0.82683947801809832,
		'err_hi': 0.011308756263186295,
		'err_low': 0.011707073177917663},
	      'data/mc': {'PT_ABSETA_Endcaps_1p2to2p1': 170.73449705514739,
		'efficiency_ratio': 0.99416866829048334,
		'err_hi': 0.027302993818728562,
		'err_low': 0.027846431972317096},
	      'mc': {'PT_ABSETA_Endcaps_1p2to2p1': 170.73449705514739,
		'efficiency': 0.83168933440628856,
		'err_hi': 0.019806805934334412,
		'err_low': 0.020099967276010291}},
	    '25_30': {'data': {'PT_ABSETA_Endcaps_1p2to2p1': 27.698281861092511,
		'efficiency': 0.79374186711322625,
		'err_hi': 0.00077954445660775569,
		'err_low': 0.00078049186065368961},
	      'data/mc': {'PT_ABSETA_Endcaps_1p2to2p1': 27.705955195620472,
		'efficiency_ratio': 1.0051991254438037,
		'err_hi': 0.0017782159910835391,
		'err_low': 0.0017823280400050144},
	      'mc': {'PT_ABSETA_Endcaps_1p2to2p1': 27.705955195620472,
		'efficiency': 0.78963644816422085,
		'err_hi': 0.0011618341550551762,
		'err_low': 0.001165088322305208}},
	    '30_35': {'data': {'PT_ABSETA_Endcaps_1p2to2p1': 32.636403834896512,
		'efficiency': 0.80848509506495958,
		'err_hi': 0.00062230595600791278,
		'err_low': 0.00062310196595774102},
	      'data/mc': {'PT_ABSETA_Endcaps_1p2to2p1': 32.630044552701698,
		'efficiency_ratio': 1.0013781590159485,
		'err_hi': 0.0013693380303715827,
		'err_low': 0.0013715807241176374},
	      'mc': {'PT_ABSETA_Endcaps_1p2to2p1': 32.630044552701698,
		'efficiency': 0.80737240750233219,
		'err_hi': 0.00091253169868429917,
		'err_low': 0.00091417799108474158}},
	    '35_40': {'data': {'PT_ABSETA_Endcaps_1p2to2p1': 37.606961140747615,
		'efficiency': 0.81418056279946283,
		'err_hi': 0.00051411825860703481,
		'err_low': 0.00051477939588037138},
	      'data/mc': {'PT_ABSETA_Endcaps_1p2to2p1': 37.613946100888114,
		'efficiency_ratio': 0.99616640424792002,
		'err_hi': 0.0011101094832906267,
		'err_low': 0.0011118453938292981},
	      'mc': {'PT_ABSETA_Endcaps_1p2to2p1': 37.613946100888114,
		'efficiency': 0.81731381356325516,
		'err_hi': 0.00075046636917388448,
		'err_low': 0.0007517384599005166}},
	    '40_50': {'data': {'PT_ABSETA_Endcaps_1p2to2p1': 44.338139026050378,
		'efficiency': 0.82445240820621812,
		'err_hi': 0.00033912400006564638,
		'err_low': 0.00033879478905920351},
	      'data/mc': {'PT_ABSETA_Endcaps_1p2to2p1': 44.34821082420018,
		'efficiency_ratio': 0.99425410141043047,
		'err_hi': 0.00071581456250963582,
		'err_low': 0.00071551911935136946},
	      'mc': {'PT_ABSETA_Endcaps_1p2to2p1': 44.34821082420018,
		'efficiency': 0.82921700502584317,
		'err_hi': 0.00048996520900932751,
		'err_low': 0.00048989542737746443}},
	    '50_60': {'data': {'PT_ABSETA_Endcaps_1p2to2p1': 53.75526471777529,
		'efficiency': 0.8288286854835083,
		'err_hi': 0.00076408064975267418,
		'err_low': 0.00076494547469196613},
	      'data/mc': {'PT_ABSETA_Endcaps_1p2to2p1': 53.740270521575724,
		'efficiency_ratio': 0.99054467301217797,
		'err_hi': 0.001581754019526234,
		'err_low': 0.0015867039726075091},
	      'mc': {'PT_ABSETA_Endcaps_1p2to2p1': 53.740270521575724,
		'efficiency': 0.83674033899258426,
		'err_hi': 0.0010910002949207547,
		'err_low': 0.0010955022975369145}},
	    '60_90': {'data': {'PT_ABSETA_Endcaps_1p2to2p1': 68.629114537732164,
		'efficiency': 0.82904551798424264,
		'err_hi': 0.0012258295642393971,
		'err_low': 0.0012288819777911159},
	      'data/mc': {'PT_ABSETA_Endcaps_1p2to2p1': 68.702092540415421,
		'efficiency_ratio': 0.98829374192885855,
		'err_hi': 0.0025335294460190152,
		'err_low': 0.0025430635002634187},
	      'mc': {'PT_ABSETA_Endcaps_1p2to2p1': 68.702092540415421,
		'efficiency': 0.83886549394331866,
		'err_hi': 0.0017567100282409724,
		'err_low': 0.0017644346471241503}},
	    '90_140': {'data': {'PT_ABSETA_Endcaps_1p2to2p1': 104.25616320155041,
		'efficiency': 0.82901945107830488,
		'err_hi': 0.0040657596366516146,
		'err_low': 0.0041029932811339157},
	      'data/mc': {'PT_ABSETA_Endcaps_1p2to2p1': 104.05685928928493,
		'efficiency_ratio': 0.98187598993908232,
		'err_hi': 0.0080605050273570671,
		'err_low': 0.0081573855229078087},
	      'mc': {'PT_ABSETA_Endcaps_1p2to2p1': 104.05685928928493,
		'efficiency': 0.84432195060573689,
		'err_hi': 0.0055584538777953621,
		'err_low': 0.0056340687750593994}}}




    */



}





