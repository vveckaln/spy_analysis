#include "LIP/TopTaus/interface/CommonDefinitions.hh" // Include it for the initialization of "MODE", below. FIXME: check whether MODE can be initialized in the physicsAnalyzer or such


// FIXME: honestly, most of this stuff should be moved to cfg files (numerical parameters)
// Sample enums and such can (and should) remain here
// xsecs and their errors should remain here. Or in SampleProcessor.

namespace commondefinitions{
  
  double LUM_ERR            = 0.026; // HCP 2013 //ICHEP 2012 https://twiki.cern.ch/twiki/bin/view/CMS/TWikiLUM
  
  double TAU_ID_ERR         = 0.06;  
  
  double QCD_CS_ERR         = 0.50;
  
  double TTBAR_CS_ERR       = 0.10;   
  
  double SINGLETOP_CS_ERR   = 0.08;
  double OTHER_CS_ERR       = 0.04;
  
  double LEP_EFF_ERR        = 0.02;        // 3 electrons, 2 muons
  double TAU_MISSID_ERR     = 0.15;        // BULSHIT //0.15;
  
  double WJETS_CS_ERR       = 0.05;
  
  //  double BTAG_EFF_ERR       = 0.10;
  double BTAG_EFF_ERR       = 0.;
  
  double SIGNAL_EFF_ERR     = 0.04;  
  
  double BTAG_eff_R_ = 0.656; // 2013-08-24 ttbar.
  double BTAG_eff_F_ = 0.054;
  
  
  double XSEC_EXP_    = 245.8;//164.4;       //  was 158, 164.4 pm 2.8 (stat.) pm 11.9 (syst.)  pm 7.4
  double XSEC_EXP_ERR_= 16./245.8;  // FIXME: update error        // 14.29/164.4 => exp error, theoretical -> 165 +4/-9 +/-7
  
  
  //Payload:   https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFb-pt_WITHttbar_payload_EPS13.txt
  //  Tagger: CSVM within 20 < pt < 800 GeV, abs(eta) < 2.4, x = pt
  //						    SFb = (0.938887+(0.00017124*x))+(-2.76366e-07*(x*x));
  double SFb_error[] = {
    0.0415707,
    0.0204209,
    0.0223227,
    0.0206655,
    0.0199325,
    0.0174121,
    0.0202332,
    0.0182446,
    0.0159777,
    0.0218531,
    0.0204688,
    0.0265191,
    0.0313175,
    0.0415417,
    0.0740446,
    0.0596716 };
  
  
  // BR(H->tauNu) for the heavy tbh samples (from 180 to 300 GeV/c^2)
  std::vector<double> brHtaunu_; 
  std::vector<double> brHtb_;
  
//  double ftbh[] ={ // Selection  mu==200&&Xt==2000&&m2==200&&tanb==5
//    0.831852,
//    0.407966,
//    0.228564,
//    0.106446,
//    0.04129783,
//    0.02514242
//  };
    
  
  
  //RUNNING CONDITIONS ////////////////////////////////////////////////
  bool run2012_         = true;
  bool APPLY_MT_CUT_    = false;
  bool applybtagweight_ = true; // Finally
  bool MODE_ = STARTING_AT_LJETS_; //MODE_ = STARTING_AT_LJETSPLUSMET_;
  double LUM_ = 19696.; // Full 2012 ReReco 539
  bool eChONmuChOFF_; // Set through config file
   
  int BTAGIND_; double BTAG_CUT_;
  TString puFileName_;
  /////////////////////////////////////////////////////////////////////
  
  // I/O
  TString inputArea_;
  TString outputArea_;
  //////
 // namespace commondefinitions
  
}
