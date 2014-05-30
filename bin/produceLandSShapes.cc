/**                                                                                                                                                                              
  \class    produceLandSShapes produceLandSShapes.cc "UserCode/LIP/TopTaus/bin/produceLandSShapes.cc"                                                                     
  \brief    executable for producing the shapes for limit estimation with landS, following the instructions at http://cmsdoc.cern.ch/~anikiten/cms-higgs/LandsInstructions1.txt
  
  \author   Pietro Vischia

  \version  $Id: produceLandSShapes.cc,v 1.5 2013/04/19 14:46:45 vischia Exp $                                                                                                       
*/

#include "LIP/TopTaus/interface/LandSShapesProducer.hh"

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
  if ( argc < 4 ) {
    std::cout << "Usage : " << argv[0] << " parameters_cfg.py --produceOnly [true|*false*] --do [all|tb|taunu|mhmax|*plot*] --outFolder []" << std::endl;
    return 0;
  }
  
  string sProduceOnly("");
  bool produceOnly(false);
  string whatToDo("plot");
  string outputPrefix("");
  // Get input arguments
  for(int i=2;i<argc;i++){
    string arg(argv[i]);
    //    if(arg.find("--help")        !=string::npos) { printHelp(); return -1;}   
    if(arg.find("--produceOnly") !=string::npos) { sProduceOnly = argv[i+1];}
    if(arg.find("--do")          !=string::npos) { whatToDo     = argv[i+1];}
    if(arg.find("--outFolder")   !=string::npos) { outputPrefix = argv[i+1];}
    //check arguments // FIXME: implement --blah bih
  }
  
  if(sProduceOnly == "true")
    produceOnly=true;
  else if(sProduceOnly == "false")
    produceOnly=false;
  else{
    cout << "Error. ProduceOnly value not defined. Defaulting to true" << endl;
    produceOnly=true;
  }
  
  string parSet(argv[1]);
  
  if(whatToDo=="all"){

    cout << " ================================================================================================================================" << endl;
    cout << " =========================================================== DOING TB ===========================================================" << endl;
    cout << " ================================================================================================================================" << endl;

    LandSShapesProducer* producerTb = new LandSShapesProducer(parSet, "tb", outputPrefix, true);
    producerTb->Produce();
    delete producerTb;

    cout << " ================================================================================================================================" << endl;
    cout << " =========================================================== DOING TAUNU ========================================================" << endl;
    cout << " ================================================================================================================================" << endl;

    LandSShapesProducer* producerTaunu = new LandSShapesProducer(parSet, "taunu", outputPrefix, true);
    producerTaunu->Produce();
    delete producerTaunu;    
    
    //    LandSShapesProducer* producerMhmax = new LandSShapesProducer(parSet, "mhmax", outputPrefix, true);
    //    producerMhmax->Produce();
    //    delete producerMhmax;
    
    cout << " ================================================================================================================================" << endl;
    cout << " =========================================================== DOING PLOT==========================================================" << endl;
    cout << " ================================================================================================================================" << endl;

    LandSShapesProducer* producerPlot = new LandSShapesProducer(parSet, "plot", outputPrefix, false);
    producerPlot->Produce();
    delete producerPlot;

    cout << " ================================================================================================================================" << endl;
    cout << " =========================================================== DOING MHMODP5 ======================================================" << endl;
    cout << " ================================================================================================================================" << endl;

    LandSShapesProducer* producerMhmodp_tanb5 = new LandSShapesProducer(parSet, "mhmodp_tanb5", outputPrefix, true);
    producerMhmodp_tanb5->Produce();
    delete producerMhmodp_tanb5;

    cout << " ================================================================================================================================" << endl;
    cout << " =========================================================== DOING MHMODP30 =====================================================" << endl;
    cout << " ================================================================================================================================" << endl;

    LandSShapesProducer* producerMhmodp_tanb30 = new LandSShapesProducer(parSet, "mhmodp_tanb30", outputPrefix, true);
    producerMhmodp_tanb30->Produce();
    delete producerMhmodp_tanb30;


  }
  else{

    cout << " ================================================================================================================================" << endl;
    cout << " =========================================================== DOING "<<  whatToDo <<" ======================================================" << endl;
    cout << " ================================================================================================================================" << endl;

    LandSShapesProducer* myProducer = new LandSShapesProducer(parSet, whatToDo, outputPrefix, produceOnly);
    myProducer->Produce();

    delete myProducer;
  }
    cout << "Shapes producer reached its natural end" << endl;
  
  return 0;
  
}
