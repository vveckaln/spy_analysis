/**   
      \class	    physicsAnalysis physicsAnalysis.cc "UserCode/LIP/TopTaus/bin/physicsAnalysis.cc"                                                                     
      \brief    executable for performing the cutflow analysis and control plots for the various channels and data samples. Must expand to calling other classes
      
      \author   Pietro Vischia
      
      \version  $Id: physicsAnalysis.cc,v 1.22 2013/04/19 15:13:00 vischia Exp $                                                                                                       
*/
#include "LIP/TopTaus/include/gConfiguration.hh"
#include "LIP/TopTaus/include/CentralProcessor.hh"

#include "LIP/TopTaus/interface/CutflowAnalyzer.hh"
#include "LIP/TopTaus/interface/HistogramPlotter.hh"
#include "LIP/TopTaus/interface/SingleStepAnalyzer.hh"
#include "LIP/TopTaus/interface/TauDileptonTableBuilder.hh"
#include "LIP/TopTaus/interface/CommonDefinitions.hh"

// System includes	
#include <string>
#include <sstream>

// CMSSW includes
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
// ROOT includes
#include "TSystem.h"



/*
  
@run me like this

physicsAnalysis test/physicsAnalysisParSets_cfg.py sample --> analyze sample

physicsAnalysis test/physicsAnalysisParSets_cfg.py doPlots --> draw plots

physicsAnalysis test/physicsAnalysisParSets_cfg.py doTables --> draw tables

*/

//
int main(int argc, char* argv[]){
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  
  //check arguments
  if ( argc < 3 ) {
    std::cout << "Usage : " << argv[0] << " parameters_cfg.py action" << std::endl;
    return 0;
  }
  gConfiguration -> SetConfiguration(argv[1]);
  CentralProcessor * centralprocessor = new CentralProcessor();
  string parSet(argv[1]);
  string runOn(argv[2]);

  std::cout << "Requested to run on " << runOn << ". Cfg file is: " << parSet << endl;
  
  const edm::ParameterSet &pSet = edm::readPSetsFrom(parSet)->getParameter<edm::ParameterSet>("PhysicsAnalysisParSet");
  double tauPtCut = pSet.getParameter<double>("tauPtCut");
  bool eChONmuChOFF = pSet.getParameter<bool>("eChONmuChOFF");
 commondefinitions::eChONmuChOFF_  = eChONmuChOFF;

  bool noUncertainties = pSet.getParameter<bool>("noUncertainties");
  bool doWPlusJetsAnalysis = pSet.getParameter<bool>("doWPlusJetsAnalysis");
  bool computePDFWeights = pSet.getParameter<bool>("computePDFWeights");
  TString inputArea  = TString(pSet.getParameter<string>("inputArea"));
  TString outputArea = TString(pSet.getParameter<string>("outputArea"));
  TString spyInputArea  = TString(pSet.getParameter<string>("spyInputArea"));
  TString spyOutputArea = TString(pSet.getParameter<string>("spyOutputArea"));
  TString puFileName = TString(pSet.getParameter<string>("puFileName"));
  TString runRange   = TString(pSet.getParameter<string>("runRange"));
  std::vector<double> brHtaunu = pSet.getParameter<std::vector<double> >("brHtaunu");
  std::vector<double> brHtb    = pSet.getParameter<std::vector<double> >("brHtb");

  
  if(runOn == "spy_misidentifiedTau" || runOn == "spy_data" || runOn == "spy_ww" || runOn == "spy_wz" || runOn == "spy_zz" || runOn == "spy_dyvv" || runOn == "spy_singletop" || runOn == "spy_ttbar_mcbkg" || runOn == "spy_ttbar_Xtau" || runOn == "spy_zjets_from50" || runOn == "spy_jets_10to50" || runOn == "spyHadd" || runOn == "spyPlots"){ // Put it here in order to avoid loading a full instance of CutflowAnalyzer in memory
    // spy_data spy_ww spy_wz spy_zz spy_dyvv spy_singletop spy_ttbar_mcbkg spy_ttbar_Xtau spy_zjets_from50 spy-zjets_10to50

    SingleStepAnalyzer* analyzeThis = new SingleStepAnalyzer(noUncertainties, spyInputArea, spyOutputArea, puFileName, runRange, eChONmuChOFF); 
    
    if(runOn == "spy_misidentifiedTau") analyzeThis->process_spy_misidentifiedTau();
    if(runOn == "spy_data"	      ) analyzeThis->process_spy_data	         ();
    if(runOn == "spy_ww"	      ) analyzeThis->process_spy_dibosons	 (0);
    if(runOn == "spy_wz"	      ) analyzeThis->process_spy_dibosons	 (1);
    if(runOn == "spy_zz"	      ) analyzeThis->process_spy_dibosons	 (2);
    if(runOn == "spy_dyvv"	      ) analyzeThis->process_spy_dyvv	         (); // DAFUCK?
    if(runOn == "spy_singletop"	      ) analyzeThis->process_spy_singletop	 ();
    if(runOn == "spy_ttbar_mcbkg"     ) analyzeThis->process_spy_ttbar_mcbkg     ();
    if(runOn == "spy_ttbar_Xtau"      ) analyzeThis->process_spy_ttbar_Xtau      ();
    if(runOn == "spy_zjets_from50"    ) analyzeThis->process_spy_zjets_from50    ();
    if(runOn == "spy_zjets_10to50"    ) analyzeThis->process_spy_zjets_10to50    ();

    if(runOn == "spyHadd"             ){
      TString haddcmd = TString("sh ${CMSSW_BASE}/src/LIP/TopTaus/scripts/lip-batch/spyHadd.sh ") + spyOutputArea + TString(eChONmuChOFF ? " true" : " false" );
      gSystem->Exec(haddcmd);
    }
    if(runOn == "spyPlots"){
      TString samples(eChONmuChOFF ? "data/plotter/samples_spy_eltau.xml" : "data/plotter/samples_spy_mutau.xml");
      TString outFolder("spyplots/"); // move to input line
      TString cmd = "mkdir -p "+outFolder+"; cp data/plotter/index.html "+outFolder;
      gSystem->Exec(cmd);

      TString spyplots("data/plotter/spyplots.xml");

      HistogramPlotter a; // Move to input line or cfg file the choice of what to plot
      a.runOnSpy();
      a.runOn(eChONmuChOFF);
      a.parse(samples,spyplots,outFolder);
      
    }
    
    return 0;
  }
  if (runOn == "spyTables"){
    gConfiguration -> SetEnvironment_spy();  
    TauDileptonTableBuilder *tableBuilder = new TauDileptonTableBuilder();
    tableBuilder -> spy_table_number_events();
    return 0;
  }

  CutflowAnalyzer* analyzer = new CutflowAnalyzer( tauPtCut, noUncertainties, doWPlusJetsAnalysis, inputArea, outputArea, puFileName, runRange, brHtaunu, brHtb /*parSet*/, eChONmuChOFF, computePDFWeights );
  std::cout << "Analyzer has been set with a cut on tau pt of " << tauPtCut << " GeV/c " << std::endl;

  
  if(runOn == "testEmbedding") analyzer->process_embeddedData();


  for(int i=0; i<400; ++i){
    stringstream sidx;
    sidx<<i;
    string idx=sidx.str();
    // 50 from 0
    
    if      (runOn == "data_muonA_"+idx)  analyzer->process_data_RunA( i);
    else if (runOn == "data_muonB_"+idx)  analyzer->process_data_RunB( i);
    else if (runOn == "data_muonC1_"+idx) analyzer->process_data_RunC1(i);
    else if (runOn == "data_muonC2_"+idx) analyzer->process_data_RunC2(i);
    else if (runOn == "data_muonD1_"+idx) analyzer->process_data_RunD1(i);
    else if (runOn == "data_muonD2_"+idx) analyzer->process_data_RunD2(i);

    else if (runOn == "data_missingmuonB_"+idx)  analyzer->process_data_MissingRunB( i);
    else if (runOn == "data_missingmuonC1_"+idx) analyzer->process_data_MissingRunC1(i);
    else if (runOn == "data_missingmuonC2_"+idx) analyzer->process_data_MissingRunC2(i);
    else if (runOn == "data_missingmuonD1_"+idx) analyzer->process_data_MissingRunD1(i);
    else if (runOn == "data_missingmuonD2_"+idx) analyzer->process_data_MissingRunD2(i);

    else if(runOn == "ww_"+idx)            analyzer->process_diboson(0, i);
    else if(runOn == "wz_"+idx)            analyzer->process_diboson(1, i);
    else if(runOn == "zz_"+idx)            analyzer->process_diboson(2, i);
    else if(runOn == "ttbar_mutau_"+idx)         analyzer->process_ttbar_mutau(i)    ;
    else if(runOn == "ttbar_mumu_"+idx)          analyzer->process_ttbar_mumu(i)    ;
    else if(runOn == "ttbar_emu_"+idx)           analyzer->process_ttbar_emu(i)    ;
    else if(runOn == "ttbar_ddbkg_"+idx)         analyzer->process_ttbar_ddbkg(i)    ;
    else if(runOn == "ttbar_mcbkg_"+idx)         analyzer->process_ttbar_mcbkg(i)    ;
    else if(runOn == "ttbar_"+idx)               analyzer->process_ttbar(i)              ;
    else if(runOn == "qcd_1_"+idx)                 analyzer->process_qcd(1, i)                ;
    else if(runOn == "qcd_2_"+idx)                 analyzer->process_qcd(2, i)                ;
    else if(runOn == "qcd_3_"+idx)                 analyzer->process_qcd(3, i)                ;
    else if(runOn == "qcd_4_"+idx)                 analyzer->process_qcd(4, i)                ;
    else if(runOn == "qcd_5_"+idx)                 analyzer->process_qcd(5, i)                ;
    else if(runOn == "qcd_6_"+idx)                 analyzer->process_qcd(6, i)                ;
    else if(runOn == "qcd_7_"+idx)                 analyzer->process_qcd(7, i)                ;
    else if(runOn == "qcd_8_"+idx)                 analyzer->process_qcd(8, i)                ;
    else if(runOn == "qcd_9_"+idx)                 analyzer->process_qcd(9, i)                ;

    
    else if(runOn == "qcd_15_"+idx)                 analyzer->process_qcd(15, i)                ;
    else if(runOn == "qcd_16_"+idx)                 analyzer->process_qcd(16, i)                ;
    else if(runOn == "qcd_17_"+idx)                 analyzer->process_qcd(17, i)                ;
    else if(runOn == "qcd_18_"+idx)                 analyzer->process_qcd(18, i)                ;
    else if(runOn == "qcd_19_"+idx)                 analyzer->process_qcd(19, i)                ;
    else if(runOn == "qcd_20_"+idx)                 analyzer->process_qcd(20, i)                ;


    // 100 from 0
    else if(runOn == "qcd_11_"+idx)                 analyzer->process_qcd(11, i)                ;
    else if(runOn == "qcd_12_"+idx)                 analyzer->process_qcd(12, i)                ;
    else if(runOn == "qcd_13_"+idx)                 analyzer->process_qcd(13, i)                ;
    else if(runOn == "zjets_from50_"+idx)        analyzer->process_zjets_from50(i)       ;
    else if(runOn == "zjets_10to50_"+idx)        analyzer->process_zjets_10to50(i)       ;

    // 400 from 0
    else if(runOn == "wjets_"+idx)               analyzer->process_wjets(i)              ;

    // 20 from 0
    else if(runOn == "singletop_"+idx)           analyzer->process_singletop(i)          ;
    else if(runOn == "hhhtautaubb_higgs_260_"+idx)           analyzer->process_hhhtautaubb_higgs(260, i);
    else if(runOn == "hhhtautaubb_higgs_300_"+idx)           analyzer->process_hhhtautaubb_higgs(300, i);
    else if(runOn == "hhhtautaubb_higgs_350_"+idx)           analyzer->process_hhhtautaubb_higgs(350, i);


    // 10 from 0
    else if (runOn == "data_embedA_"+idx)  analyzer->process_data_EmbeddedRunA(i);
    else if (runOn == "data_embedB_"+idx)  analyzer->process_data_EmbeddedRunB(i);
    else if (runOn == "data_embedC1_"+idx) analyzer->process_data_EmbeddedRunC1(i);
    else if (runOn == "data_embedC2_"+idx) analyzer->process_data_EmbeddedRunC2(i);
    else if (runOn == "data_embedD1_"+idx) analyzer->process_data_EmbeddedRunD1(i);
    else if (runOn == "data_embedD2_"+idx) analyzer->process_data_EmbeddedRunD2(i);
    else if (runOn == "ttbar_embed_"+idx) analyzer->process_ttbar_Embedded(i);

    // 30 from 0
    else if(runOn == "qcd_10_"+idx)                 analyzer->process_qcd(10, i)                ;
    else if(runOn == "htb_higgs_bychannel_180_"+idx) analyzer->process_htb_higgs_bychannel(180, i) ;
    else if(runOn == "htb_higgs_bychannel_200_"+idx) analyzer->process_htb_higgs_bychannel(200, i) ;
    else if(runOn == "htb_higgs_bychannel_220_"+idx) analyzer->process_htb_higgs_bychannel(220, i) ;
    else if(runOn == "htb_higgs_bychannel_240_"+idx) analyzer->process_htb_higgs_bychannel(240, i) ;
    else if(runOn == "htb_higgs_bychannel_250_"+idx) analyzer->process_htb_higgs_bychannel(250, i) ;
    else if(runOn == "htb_higgs_bychannel_260_"+idx) analyzer->process_htb_higgs_bychannel(260, i) ;
    else if(runOn == "htb_higgs_bychannel_280_"+idx) analyzer->process_htb_higgs_bychannel(280, i) ;
    else if(runOn == "htb_higgs_bychannel_300_"+idx) analyzer->process_htb_higgs_bychannel(300, i) ;
    else if(runOn == "htb_higgs_bychannel_350_"+idx) analyzer->process_htb_higgs_bychannel(350, i) ;
    else if(runOn == "htb_higgs_bychannel_400_"+idx) analyzer->process_htb_higgs_bychannel(400, i) ;
    else if(runOn == "htb_higgs_bychannel_500_"+idx) analyzer->process_htb_higgs_bychannel(500, i) ;
    else if(runOn == "htb_higgs_bychannel_600_"+idx) analyzer->process_htb_higgs_bychannel(600, i) ;
    else if(runOn == "htb_higgs_bychannel_700_"+idx) analyzer->process_htb_higgs_bychannel(700, i) ;
    else if(runOn == "htb_higgs_180_"+idx)           analyzer->process_htb_higgs(180, i)           ;
    else if(runOn == "htb_higgs_200_"+idx)           analyzer->process_htb_higgs(200, i)           ;
    else if(runOn == "htb_higgs_220_"+idx)           analyzer->process_htb_higgs(220, i)           ;
    else if(runOn == "htb_higgs_240_"+idx)           analyzer->process_htb_higgs(240, i)           ;
    else if(runOn == "htb_higgs_250_"+idx)           analyzer->process_htb_higgs(250, i)           ;
    else if(runOn == "htb_higgs_260_"+idx)           analyzer->process_htb_higgs(260, i)           ;
    else if(runOn == "htb_higgs_280_"+idx)           analyzer->process_htb_higgs(280, i)           ;
    else if(runOn == "htb_higgs_300_"+idx)           analyzer->process_htb_higgs(300, i)           ;
    else if(runOn == "htb_higgs_350_"+idx)           analyzer->process_htb_higgs(350, i) ;
    else if(runOn == "htb_higgs_400_"+idx)           analyzer->process_htb_higgs(400, i) ;
    else if(runOn == "htb_higgs_500_"+idx)           analyzer->process_htb_higgs(500, i) ;
    else if(runOn == "htb_higgs_600_"+idx)           analyzer->process_htb_higgs(600, i) ;
    else if(runOn == "htb_higgs_700_"+idx)           analyzer->process_htb_higgs(700, i) ;

    // 1 from 0
    else if(runOn == "qcd_14_"+idx)                 analyzer->process_qcd(14, i)                ;


  }  

    
  if     (runOn == "hh_higgs_bychannel")  analyzer->process_hh_higgs_bychannel() ;
  else if(runOn == "hh_higgs")            analyzer->process_hh_higgs()           ;
  
  else if(runOn == "wh_higgs_bychannel")  analyzer->process_wh_higgs_bychannel() ;
  else if(runOn == "wh_higgs")            analyzer->process_wh_higgs()           ;
  else if(runOn == "tbh_higgs_bychannel") analyzer->process_tbh_higgs_bychannel() ;
  else if(runOn == "tbh_higgs")           analyzer->process_tbh_higgs()           ;



  // FIXME: manage with --blah for having parameters like "withHiggs" and so on
  else if(runOn == "doTables"){
    cout << "Doing tables" << endl;
    bool onlyhiggs(true), heavyhiggs(false), sm(false), doNotPrintAllErrors(false), printAllErrors(true), includeSoverB(true), doNotincludeSoverB(false), produceDatacards(false), withShapes(true), withStatShapes(false), unsplit(false);
    int myDetail(0), detailed(2), notDetailed(1), baseDetail(0);      
    myDetail = baseDetail;
    analyzer->mcTable(myDetail, includeSoverB, printAllErrors, heavyhiggs, sm, "PFlow", "yields-mc-", false, false, false); 
    cout << "Done SM table" << endl;
    analyzer->mcTable(myDetail, includeSoverB, printAllErrors, onlyhiggs, heavyhiggs, "PFlow", "yields-mc-", false, false, false); 
    cout << "Done TBH table" << endl;
    analyzer->summaryTable( myDetail, true, heavyhiggs, false, false, false, produceDatacards, withShapes, withStatShapes, unsplit);
    cout << "Done summary table" << endl;
  }
  else if(runOn == "doDatacards"){
    bool produceDatacards(true), withShapes(true), withStatShapes(false), unsplit(true), heavyhiggs(false);
    int detailed(2), notDetailed(1); 
    analyzer->summaryTable( notDetailed, true, heavyhiggs, false, false, false, produceDatacards, withShapes, withStatShapes, unsplit);
  }
  else if(runOn == "doPlots"){
//    PlotStyle sty();
//    sty.setTDRStyle();
      
    TString samples("data/plotter/samples.xml");
    TString samples_datadriven("data/plotter/samples_datadriven.xml"); 
    TString samples_embedded("data/plotter/samples_embedded.xml"); 
    TString samples_tautaubb("data/plotter/samples_embedded_hhhtautaubb.xml");
    TString outFolder("plots/"); // move to input line
    TString outFolderEmbedded("plotsEmbedded/"); // move to input line
    TString outFolderTautaubb("plotsTautaubb/");
    TString cmd = "mkdir -p "+outFolder+"; cp data/plotter/index.html "+outFolder;
    TString cmdEmbedded = "mkdir -p "+outFolderEmbedded;
    TString cmdTautaubb = "mkdir -p "+outFolderTautaubb;
    gSystem->Exec(cmd);
    gSystem->Exec(cmdEmbedded);
    gSystem->Exec(cmdTautaubb);
    TString limits("data/plotter/limits.xml");
    TString leptons("data/plotter/leptons.xml");
    TString met("data/plotter/met.xml");
    TString mt("data/plotter/mt.xml");
    TString wjets("data/plotter/wjets.xml");
    TString wtmass("data/plotter/wtmass.xml");
    TString jets("data/plotter/jets.xml");
    TString yields("data/plotter/yields.xml");
    TString taus("data/plotter/taus.xml");
    TString vertex("data/plotter/vertex.xml");
    TString afterR("data/plotter/afterR.xml");
    TString test("data/plotter/test.xml");
    TString debug("data/plotter/debug.xml");

    TString wplusjets("data/plotter/wplusjets.xml");
    printf("probe E\n");
    HistogramPlotter a; // Move to input line or cfg file the choice of what to plot
    printf("probe F\n");
    a.parse(samples,vertex,outFolder);
    printf("    a.parse(samples,vertex,outFolder);\n");
    a.parse(samples,met,outFolder);      
    a.parse(samples,leptons,outFolder);  
    a.parse(samples,mt,outFolder);      
    a.parse(samples,jets,outFolder); 
    a.parse(samples,yields,outFolder);
    a.parse(samples_datadriven,wplusjets,outFolder);
    printf("    a.parse(samples,vertex,outFolder);\n");


    a.parse(samples_embedded,vertex,outFolderEmbedded);
    a.parse(samples_embedded,met,outFolderEmbedded);      
    a.parse(samples_embedded,leptons,outFolderEmbedded);  
    a.parse(samples_embedded,mt,outFolderEmbedded);      
    a.parse(samples_embedded,jets,outFolderEmbedded); 
    a.parse(samples_embedded,yields,outFolderEmbedded);
    //    a.parse(samples_embedded,yields,outFolderEmbedded);

    a.parse(samples_tautaubb,vertex,outFolderTautaubb);
    a.parse(samples_tautaubb,met,outFolderTautaubb);      
    a.parse(samples_tautaubb,leptons,outFolderTautaubb);  
    a.parse(samples_tautaubb,mt,outFolderTautaubb);      
    a.parse(samples_tautaubb,jets,outFolderTautaubb); 
    a.parse(samples_tautaubb,yields,outFolderTautaubb);


    //a.parse(samples,limits,outFolder);      
    //a.parse(samples,debug,outFolder);  
    //a.parse(samples,afterR,outFolder);
    //
    //  //a.parse(samples,wjets,outFolder);
    //  //a.parse(samples,wtmass,outFolder);
    //  //a.parse(higgssamples,taus,outFolder);
    //  //a.parse(samples,taus,outFolder);
    //  //a.parse(samples,test,outFolder);
    
  }
  else
    cout << "Sample does not exist" << endl;
  
  cout << "physicsAnalysis reached its natural end" << endl;
  
  return 0;
  
}
//  // creates the physics analyzer
//  PhysicsAnalyzer analyzer(20);
//
//  
//  //analyzer.process_ttbar();  
//  //analyzer.process_data();
//  //analyzer.process_ttbar_bychannel();
//
//  // Table processing starts here
//  // include all errors -> (xx,xx,false,false)
//  bool onlyhiggs(true), sm(false), doNotPrintAllErrors(false), printAllErrors(true), includeSoverB(true), doNotincludeSoverB(false); 
//  int detailed(2), notDetailed(1);      
//analyzer.mcTable(notDetailed, includeSoverB, printAllErrors, sm, "PFlow", "yields-mc-", false, false, false); 
//analyzer.mcTable(notDetailed, includeSoverB, printAllErrors, onlyhiggs, "PFlow", "yields-mc-", false, false, false); 
////analyzer.mcTable(notDetailed, includeSoverB, doNotPrintAllErrors, onlyhiggs, "PFlow", "yields-mc-", false, false, false); 
//  //DEBUG (include all systematics )
//  //analyzer.mcTable(notDetailed, includeSoverB, doNotPrintAllErrors, sm, "PFlow", "yields-mc-", false, false, false);  
//  
//  //
// analyzer.summaryTable( notDetailed, false, false, false, false);
// analyzer.summaryTable( notDetailed, true, false, false, false);
//
// analyzer.summaryTable( notDetailed, false, true, false, false);
// analyzer.summaryTable( notDetailed, false, false, true, false);
// analyzer.summaryTable( notDetailed, false, true, true, false);
// analyzer.summaryTable( notDetailed, false, true, true, true);
//
// analyzer.summaryTable( notDetailed, true, true, false, false);
// analyzer.summaryTable( notDetailed, true, false, true, false);
// analyzer.summaryTable( notDetailed, true, true, true, false);
// analyzer.summaryTable( notDetailed, true, true, true, true);
//
//// analyzer.summaryTable( notDetailed, true, false, false, false);
//
//*/




