#ifndef _SingleStepAnalyzer_hh
#define _SingleStepAnalyzer_hh
/**                                                                                                                                                                              
  \class    SingleStepAnalyzer SingleStepAnalyzer.cc "UserCode/LIP/TopTaus/src/SingleStepAnalyzer.cc"                                                                     
  \brief    class for building and analyzing the cutflow of a physics analysis
  
  \author   Pietro Vischia, pietro.vischia@gmail.com

  \version  $Id: SingleStepAnalyzer.hh,v 1.7 2013/04/22 20:26:44 vischia Exp $                                                                                                       
*/

// System headers
#if !defined(__CINT__) || defined(__MAKECINT__)
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#endif

// ROOT headers



#include "LIP/TopTaus/interface/UncertaintyCalculator.hh"
#include "LIP/TopTaus/interface/AnalysisMonitoring.hh"
#include "LIP/TopTaus/interface/ObjectSelector.hh"
#include "LIP/TopTaus/interface/Utilities.hh"

// CMSSW headers
#include "PhysicsTools/Utilities/interface/LumiReweightingStandAlone.h"

using namespace std;



class SingleStepAnalyzer : public UncertaintyCalculator, public AnalysisMonitoring, public ObjectSelector, public GenericUtils, public PhysicsUtils {
public:
  
  SingleStepAnalyzer(bool, TString, TString, TString, TString, bool);
  
  void process(bool, urlCodes, TString, TString, vector<TString>&, uint);


  
private:

  
  void setSelectionParameters();
  
  void computeMHT( std::vector<PhysicsObject>&,  vector<int>&, PhysicsObject&);
  
  void computeMHTb( std::vector<PhysicsObject>&,  vector<int>& );
  
  pair<double,double> getEfficiencyAndError( double, double, double, double, double, double, double);
  pair<double,double> getEfficiencyAndError( std::vector<PhysicsObject>&,  vector<int>&, JetCorrectionUncertainty*, vector<double>& );
  
  
  //btag uncertainty//////////////////////////////////////////////////////////////////////////////////////////////////////
  void getBtagEfficiencies();
  pair<double, pair<double,double> > getNbtags( std::vector<PhysicsObject>&, vector<int>&); // unc, p0 and p1
  pair< double,double >             get2Nbtags( std::vector<PhysicsObject>&, vector<int>&); // unc, p2
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  TString myKey_;
  double JER_, JES_, UNC_, BTAGUNC_, UNBTAGUNC_, TES_, TOPPTUNC_; // uncertainties applied
  double jer_, jes_, unc_, btagunc_, unbtagunc_, tes_, topptunc_; // uncertainties applied
  bool sys_;
  void eventAnalysis(bool, TString, event::MiniEvent_t*, double, double, double, double, double, double, double);
  void tauDileptonOSAnalysis(
			    bool,
			    std::vector<PhysicsObject>&,
			    std::vector<PhysicsObject>&, vector<int>&,  
			    std::vector<PhysicsObject>&, vector<int>&,
			    std::vector<PhysicsObject>&, vector<int>&, 
			    std::vector<PhysicsObject>&, vector<int>&,
			    int, JetCorrectionUncertainty*, vector<double>&, 
			    TString, event::MiniEvent_t*,
			    PhysicsObjectCollection&
			    );
  
  void newPhysics(
		  vector<PhysicsObject>&,
		  std::vector<PhysicsObject>&, vector<int>&,  
		  std::vector<PhysicsObject>&, vector<int>&,
		  std::vector<PhysicsObject>&, vector<int>&, 
		  std::vector<PhysicsObject>&, vector<int>&, int, JetCorrectionUncertainty*, vector<double>&, 
		  TString,
		  event::MiniEvent_t*
		  );
  
  
  void fillTauDileptonObjHistograms(
				    JetCorrectionUncertainty*,
				    vector<PhysicsObject>&,
				    PhysicsObject&, int, TString, TString, 
				    vector<int>&, vector<PhysicsObject>&, 
				    vector<int>&, vector<PhysicsObject>&,
				    vector<int>&, vector<PhysicsObject>&,
				    vector<int>&, vector<PhysicsObject>&, vector<double>&, double, 
				    int btagscorrected =0
				    );
  
  void fillTauDileptonObjTree(
			      event::MiniEvent_t*,
			      TString,                 // Selected or DataDriven
			      TString,
			      JetCorrectionUncertainty*,
			      PhysicsObject&, TString, TString,
			      PhysicsObject*,
			      double,
			      PhysicsObject*,
			      PhysicsObject*,
			      vector<int>&, vector<PhysicsObject>&, vector<double>&, double,
			      int nbtags_taus = 0     
			      );
  
  void fillDebugHistograms(
			   TString, 
			   vector<int>&, vector<PhysicsObject>&, 
			   vector<int>&, vector<PhysicsObject>&,
			   vector<int>&, vector<PhysicsObject>&
			   );
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  
  pair<double,double> btagSF(double);
  
  
  void evaluatePDFUncertainty();
  
  
  JetCorrectionUncertainty* jecUnc_ak5_pf_;
  JetCorrectionUncertainty* jecUnc_ak5_jpt_;
  JetCorrectionUncertainty* jecUnc_data_ak5_pf_;
  JetCorrectionUncertainty* jecUnc_data_ak5_jpt_;
  
  JetResolution* jerUnc_ak5_pf_pt_ ;
  JetResolution* jerUnc_ak5_jpt_pt_;
  

  bool noUncertainties_;   // Do not compute trees with systematic uncertainties (faster for preliminary checks)
  bool doWPlusJetsAnalysis_; // Do not perform WPlusJetsAnalysis (so no trees in it) (faster for preliminary and notau checks)
  bool    isData_;         // is this data ?
  int  ttbarLike_;         // see if we need to process (etau,mutau) (ee,emu,mumu)
  unsigned int i_;         // generic index
  event::Reader* evR_;    // Event Reader for the processed sample
  uint nevents_;           // number of events in minitree to be processed
  
  event::Reader* evRMC_;    // Event Reader for the MC processed sample
  uint neventsMC_;           // number of events in minitree to be processed
  
  int    TAUPRONGS_;       // number of prongs required in the tau dilepton analysis
  double MET_CUT_;         // MET cut that is applied
  double JET_PT_CUT_;
  double MT_CUT_;          // cut on transverse mass
  double R_CUT_;           // cut on polarization
  
  double DRMIN_JET_E_, DRMIN_JET_M_, DRMIN_T_E_, DRMIN_T_M_, DRMIN_T_J_; // Object spliting
  
  
  int urlCode_, evType_;   // url code of sample being processed and the caractherized event type
  int nVertex_;
  
  double electrontriggerefficiency_;
  double muontriggerefficiency_;
  double intimepuWeight_,outtimepuWeight_;
  double leptontriggerefficiency_;
  double w_;
  double is_os_;
  double tauR_;
  
  double scale_;
  double mht_, mhtb_;
  
  vector<int> jetsForTrigger_;
  vector<float> MCPUDist_, DataPUDist_;
  
  double A0_1_, errA0_1_, A1_1_, errA1_1_, A2_1_, errA2_1_;
  double A0_2_, errA0_2_, A1_2_, errA1_2_, A2_2_, errA2_2_;
  
  
  reweight::LumiReWeighting LumiWeights_;
  reweight::PoissonMeanShifter PShiftUp_;
  reweight::PoissonMeanShifter PShiftDown_;
  
  double testMe_; double testMe_Nev_;
  
  vector<double> weightedPDFSelectedEvents_;
  vector<double> weighted2PDFSelectedEvents_;
  vector<double> weightedPDFEvents_;
  
  vector<double> myWeights_;
  
  double originalPDFEvents_;
  double PDFSelectedEvents_;
  

  
  double BTagSF_;     double err_BTagSF_;
  double LightJetSF_; double err_LightJetSF_;
  
};


#endif
