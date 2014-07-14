#ifndef _FileParser_hh
#define _FileParser_hh

#include <iomanip>
#include <iostream>
#include <fstream>
#include <map>
#include <TString.h>
#include "TXMLEngine.h" // needs '<flags LDFLAGS="-lXMLIO">' in the BuildFile
#include <TFile.h>


using namespace std;

class FileParser {
  
public: 
  
  FileParser();
  
  void clear();
  
  void parse( TString, TString);
  
  void process(       TXMLEngine*, XMLNodePointer_t  );
  void id(            TXMLEngine*, XMLNodePointer_t );
  void processFile(   TXMLEngine*, XMLNodePointer_t  );
  int    getAttrValue(       TXMLEngine*, XMLAttrPointer_t);
  float  getAttrFloatValue(  TXMLEngine*, XMLAttrPointer_t);
  
  bool getAttrBool(   TXMLEngine*, XMLAttrPointer_t);
  pair<float,float> getAttrRange( TXMLEngine*, XMLAttrPointer_t );
  
  
  // process/sample attributes ////
  map< int, TFile* >   mapFiles_;
  map< int, float  >   mapWeights_;
  map< int, int>       mapIsData_;
  map< int, int>       mapIsStack_;
  map< int, TString>   mapName_;
  map< int, int>       mapColor_;
  map< int, int>       mapFill_;
  map< int, int>       mapFillColor_;
  map< int, int>       mapLineColor_;
  map< int, int>       mapLine_;
  map< int, int>       mapLineStyle_;
  map< int, int>       mapLineWidth_;
  map< int, int>       mapMarker_;
  map< int, int>       mapMarkerColor_; 
  map< int, TString>   mapLegopt_;
  map< int, TString>   mapSampleOpt_;
  //////////////////////////////////
  
  // id attributes //////////////////////////////
  map< int, float  >              mapIdnorm_;
  map< int, TString  >            mapIdtag_;
  map< int, TString >             mapIdXtitle_;
  map< int, TString >             mapIdYtitle_;
  map< int, TString >             mapIdtitle_; 
  map< int, pair<float,float> >   mapIdXrange_;
  map< int, pair<float,float> >   mapIdYrange_;
  map< int, int >                 mapIdlogy_;
  map< int, int >                 mapIduflow_;
  map< int, int >                 mapIdoflow_;
  map< int, int >                 mapIddoRatio_;
  map< int, int >                 mapIdrelRatio_;
  map< int, TString >             mapIdFolder_; 
  map< int, TString >             mapIdHistoName_;    
  map< int, TString >             mapIdopt_;    
  map< int, int >                 mapIdXrebin_; 
  ////////////////////////////////////////////////
  
  TString process_,file_,weight_;
  
  // process/sample attributes
  TString isData_,isStack_,name_,color_,fill_,line_,linecolor_, marker_, width_,fillcolor_,markercolor_,legopt_,linestyle_;
  
  // id attributes
  TString id_,norm_,title_,xtitle_,ytitle_,xrange_,yrange_,doRatio_,tag_,logy_,uflow_,oflow_,opt_,xrebin_,relRatio_;
  
  int processNumber_;
  int idNumber_;
  
};

#endif // _FileParser_hh
