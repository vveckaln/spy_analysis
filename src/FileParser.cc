#include "LIP/TopTaus/interface/FileParser.hh"

#include "LIP/TopTaus/interface/CommonDefinitions.hh" // For output area

using namespace std;


FileParser::FileParser()
{ 
  
  clear();
  
  // sample /////////////////
  file_   =TString("file");
  weight_ =TString("weight");
  ////////////////////////////
  
  //process attributes /////////////
  process_     = TString("process");
  isData_      = TString("isdata");
  isStack_     = TString("isstack");
  name_        = TString("name");
  color_       = TString("color");
  fill_        = TString("fill");
  fillcolor_   = TString("fillcolor");
  line_        = TString("line");
  linestyle_   = TString("linestyle");
  width_       = TString("width");
  linecolor_   = TString("linecolor");
  marker_      = TString("marker");
  markercolor_ = TString("markercolor");
  legopt_      = TString("legopt");
  ///////////////////////////////////
  
  // id attributes ////////////
  id_       = TString("id");
  norm_     = TString("norm");
  title_    = TString("xtitle"); 
  xtitle_   = TString("xtitle"); 
  ytitle_   = TString("ytitle");
  xrange_   = TString("xrange");
  yrange_   = TString("yrange");
  doRatio_  = TString("doratio");
  relRatio_ = TString("relratio");
  tag_      = TString("tag");   
  logy_     = TString("logy");
  uflow_    = TString("uflow");
  oflow_    = TString("oflow");
  opt_      = TString("opt");
  xrebin_   = TString("xbin");
  /////////////////////////////
  
  
} 

void FileParser::clear(){
  processNumber_=-1;
  idNumber_  =-1;
  
  // samples attributes /////////////////////////////////////////////////////////////////////////////
  map<int , TFile *>::iterator it;
  for(it= mapFiles_.begin(); it!=mapFiles_.end(); it++){ if( (*it).second ) ((*it).second)->Close(); } 
  mapFiles_.clear();
  mapWeights_.clear();
  mapIsData_.clear();
  mapIsStack_.clear();
  mapName_.clear();
  mapColor_.clear();
  mapFill_.clear();
  mapFillColor_.clear();
  mapLineColor_.clear();
  mapLine_.clear();
  mapLineWidth_.clear();
  mapMarker_.clear(); 
  mapMarkerColor_.clear();
  mapLegopt_.clear();
  mapSampleOpt_.clear();
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  
  // id attributes ///////
  mapIdnorm_.clear();
  mapIdtag_.clear();
  mapIdXtitle_.clear();
  mapIdYtitle_.clear();
  mapIdtitle_.clear(); 
  mapIdXrebin_.clear();
  mapIdXrange_.clear();
  mapIdYrange_.clear();
  mapIdlogy_.clear();
  mapIduflow_.clear();
  mapIdoflow_.clear();
  mapIddoRatio_.clear();
  mapIdFolder_.clear(); 
  mapIdHistoName_.clear();    
  mapIdopt_.clear();    
  ////////////////////////
  
}


void FileParser::parse( TString samples, TString defs ){
  
  cout<<endl<<"\n parsing sample : "<<samples<<" with def : "<<defs<<endl;
  
  clear();
  
  // First create engine
  TXMLEngine * xml = new TXMLEngine;
  
  // parse samples //////////////////////////////////////////
  XMLDocPointer_t xmldoc = xml->ParseFile(samples);
  if (xmldoc==0) { delete xml; return; }
  XMLNodePointer_t mainnode = xml->DocGetRootElement(xmldoc);
  XMLNodePointer_t processNode = xml->GetChild(mainnode);
  while ( processNode != 0 ) {
    TString nodeName( xml->GetNodeName(processNode) );
    if( nodeName == process_ ){  process( xml, processNode ); }
    processNode = xml->GetNext(processNode);
  }
  ////////////////////////////////////////////////////////////
  
  
  // monitor to analyze ///////////////////////////////////////
  XMLDocPointer_t xmldoc2 = xml->ParseFile(defs);
  if ( xmldoc2 == 0 ) { delete xml; return; }
  XMLNodePointer_t mainnode2 = xml->DocGetRootElement(xmldoc2);
  XMLNodePointer_t idNode = xml->GetChild(mainnode2);
  while ( idNode != 0 ) {
    TString nodeName( xml->GetNodeName(idNode) );
    if( nodeName == id_ ){  id( xml, idNode ); }
    idNode = xml->GetNext(idNode);
  }
  ////////////////////////////////////////////////////////////
  
}



void FileParser::process( TXMLEngine * xml, XMLNodePointer_t processNode ){
    printf("Begin FileParser::process\n");
  processNumber_++;
  XMLNodePointer_t child = xml->GetChild(processNode);
  
  TString childName( xml->GetNodeName(child) );
  printf("childName %s file_ %s\n", childName.Data(), file_.Data());
  if(childName == file_  ){ processFile(xml,child);   }
  
  mapName_[processNumber_]            = TString("unknown");  // sample name
  mapIsData_[processNumber_]          = 0;                   // by default we say that the sample is MC
  mapIsStack_[processNumber_]         = 0;                   // by default we use stack
  mapColor_[processNumber_]           = 0;                   // sample color  ?
  mapFill_[processNumber_]            = 0;                   // fill
  mapFillColor_[processNumber_]       = 0;                   //  
  mapLine_[processNumber_]            = 1;                   // line type
  mapLineWidth_[processNumber_]       = 1;                   // line width
  mapLineColor_[processNumber_]       = 1;                   // line color
  mapMarker_[processNumber_]          = 1;                   // marker
  mapMarkerColor_[processNumber_]     = 1;                   // marker color
  
  
  XMLAttrPointer_t attr = xml->GetFirstAttr(processNode);
  while (attr){
    TString attName( xml->GetAttrName(attr));
    
    
    cout<<endl<<" attribute : "<<attName<<endl;
    
    if( attName == name_        ){ mapName_[processNumber_]        = TString( xml->GetAttrValue(attr)); /*cout<<" value is : "<<mapName_[processNumber_]<<endl;  */}
    else if( attName == isData_      ){ mapIsData_[processNumber_]      = getAttrValue(xml,attr); /* cout<<" value is : "<<mapIsData_[processNumber_]<<endl;          */}
    else if( attName == isStack_     ){ mapIsStack_[processNumber_]     = getAttrValue(xml,attr); /* cout<<" value is : "<<mapIsStack_[processNumber_]<<endl;         */}
    else if( attName == color_       ){ mapColor_[processNumber_]       = getAttrValue(xml,attr); /* cout<<" value is : "<<mapColor_[processNumber_]<<endl;           */}
    else if( attName == fill_        ){ mapFill_[processNumber_]        = getAttrValue(xml,attr); /* cout<<" value is : "<<mapFill_[processNumber_]<<endl;            */}
    else if( attName == fillcolor_   ){ mapFillColor_[processNumber_]   = getAttrValue(xml,attr); /* cout<<" value is : "<<mapFillColor_[processNumber_]<<endl;       */}
    else if( attName == line_        ){ mapLine_[processNumber_]        = getAttrValue(xml,attr); /* cout<<" value is : "<<mapLine_[processNumber_]<<endl;            */}
    else if( attName == width_       ){ mapLineWidth_[processNumber_]   = getAttrValue(xml,attr); /* cout<<" value is : "<<mapLineWidth_[processNumber_]<<endl;       */}
    else if( attName == linecolor_   ){ mapLineColor_[processNumber_]   = getAttrValue(xml,attr); /* cout<<" value is : "<<mapLineColor_[processNumber_]<<endl;       */}
    else if( attName == linestyle_   ){ mapLineStyle_[processNumber_]   = getAttrValue(xml,attr); /*cout<<" value is : "<<mapLineStyle_[processNumber_]<<endl;*/} 
    else if( attName == marker_      ){ mapMarker_[processNumber_]      = getAttrValue(xml,attr); /* cout<<" value is : "<<mapMarker_[processNumber_]<<endl;          */}
    else if( attName == markercolor_ ){ mapMarkerColor_[processNumber_] = getAttrValue(xml,attr); /* cout<<" value is : "<<mapMarkerColor_[processNumber_]<<endl;     */}
    else if( attName == legopt_      ){ mapLegopt_[processNumber_]      = TString( xml->GetAttrValue(attr)); /*cout<<" value is : "<<mapLegopt_[processNumber_]<<endl;*/}
    else if( attName == opt_         ){ mapSampleOpt_[processNumber_]   = TString( xml->GetAttrValue(attr)); /*cout<<" value is : "<<mapLegopt_[processNumber_]<<endl;*/}
    
    
    
    
    attr = xml->GetNextAttr(attr);
  }
   printf("end FileParser::process\n");
}


void FileParser::processFile( TXMLEngine * xml, XMLNodePointer_t fileNode){
  printf("begin processFile\n");
  TString fileName = commondefinitions::outputArea_ + TString( xml->GetNodeContent(fileNode) );
  
  cout << " DEBUGME: file is " << fileName << endl;
  TFile * file = TFile::Open( TString("file:")+fileName);
  if(file==0){ cout<<endl<<" Unknown file : "<<fileName<<endl;}

  mapFiles_[processNumber_] = file;

  cout<<endl<<" file : "<<fileName<<endl;
  XMLAttrPointer_t w = xml->GetFirstAttr(fileNode);
  float weight(1.);char* content;
  if(w){ content = (char*) xml-> GetAttrValue(w); sscanf(content,"%f",&weight);}
  mapWeights_[processNumber_] = weight;
  printf("end processFile\n");
}

bool FileParser::getAttrBool(TXMLEngine * xml, XMLAttrPointer_t attr){
  bool retValue;

  TString attrValue(xml->GetAttrValue(attr));
  if(      attrValue==TString("true") ){ retValue= true; }
  else if( attrValue==TString("false")){ retValue= false;}
  return attrValue;
}

int FileParser::getAttrValue(TXMLEngine * xml, XMLAttrPointer_t attr){
  int attrValue;
  char * value = (char*) xml->GetAttrValue(attr);
  sscanf(value,"%d",&attrValue);
  return attrValue;
}

float FileParser::getAttrFloatValue(TXMLEngine * xml, XMLAttrPointer_t attr){
  float attrValue;
  char * value = (char*) xml->GetAttrValue(attr);
  sscanf(value,"%f",&attrValue);
  return attrValue;
}


void FileParser::id( TXMLEngine * xml, XMLNodePointer_t idNode ){
  printf("begin FilerParser::id\n");
  idNumber_++;
  
  XMLAttrPointer_t attr = xml->GetFirstAttr(idNode);

  mapIdtitle_[idNumber_] = TString("");
  
  while (attr){
    TString attName( xml->GetAttrName(attr));
    if( attName == norm_       ){ mapIdnorm_[idNumber_]     = getAttrFloatValue(xml,attr);         }
    else if( attName == xtitle_     ){ mapIdXtitle_[idNumber_]   = TString( xml->GetAttrValue(attr));   }
    else if( attName == ytitle_     ){ mapIdYtitle_[idNumber_]   = TString( xml->GetAttrValue(attr));   }
    else if( attName == opt_        ){ mapIdopt_[idNumber_]      = TString( xml->GetAttrValue(attr));   }
    else if( attName == xrebin_     ){ mapIdXrebin_[idNumber_]   = getAttrValue(xml,attr);              }
    else if( attName == xrange_     ){ mapIdXrange_[idNumber_]   = getAttrRange(xml,attr);              }
    else if( attName == yrange_     ){ mapIdYrange_[idNumber_]   = getAttrRange(xml,attr);              }
    else if( attName == doRatio_    ){ mapIddoRatio_[idNumber_]  = getAttrValue(xml,attr);              }
    else if( attName == relRatio_   ){ mapIdrelRatio_[idNumber_] = getAttrValue(xml,attr);              }
    else if( attName == tag_        ){ mapIdtag_[idNumber_]      = TString( xml->GetAttrValue(attr));   }
    else if( attName == logy_       ){ mapIdlogy_[idNumber_]     = getAttrValue(xml,attr);              }
    else if( attName == title_      ){ mapIdtitle_[idNumber_]    = TString( xml->GetAttrValue(attr));   }
    else if( attName == uflow_      ){ mapIduflow_[idNumber_]    = getAttrValue(xml,attr);              }
    else if( attName == oflow_      ){ mapIdoflow_[idNumber_]    = getAttrValue(xml,attr);              }
    attr = xml->GetNextAttr(attr);
  }
  
  
  XMLNodePointer_t item = xml->GetChild(idNode);

  if(item){
    XMLAttrPointer_t folder= xml->GetFirstAttr(item);
    mapIdFolder_[idNumber_]    = TString( xml->GetAttrValue(folder));
    mapIdHistoName_[idNumber_] = TString( xml->GetNodeContent(item));    
  }
  printf("end FileParser::id\n");
  
}





pair<float,float> FileParser::getAttrRange( TXMLEngine * xml, XMLAttrPointer_t attr ){
  
  char * data = (char *) xml->GetAttrValue(attr);
  float d1,d2;
  sscanf(data,"%f,%f",&d1,&d2);
  pair<float,float> ret(d1,d2);
  return ret;
  
}




