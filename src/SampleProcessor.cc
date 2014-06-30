#include "LIP/TopTaus/interface/SampleProcessor.hh"

// System headers
#include <iostream>
#include <iomanip>
#include <sstream>

// ROOT headers



SampleProcessor::SampleProcessor(double tauPtCut, TString inputArea, TString outputArea):
  sampleTauPtCut_(tauPtCut)
{
  
  commondefinitions::inputArea_ = inputArea;
  commondefinitions::outputArea_ = outputArea;
    
    outFile_=0;
    
    checkleptonsFromTauDecay_ = false; // true; // used to compute tables with leptons from tau decays (tt->WbWb->tauhtaul)

    pdfweights_=false; // this should allways be false, it will be overwritten by the process_ttbar

    jetpgid_=JETPGID_OLD;

    fast_=true;

    trigEffStudy_= false; // Trigger efficiency study for electron+jets trigger. Should always be false: obsolete!
   
    pu_ = NOPU; // PUPLUS PUMINUS now are dealt with by passing the pileup distributions computed with different minbias xsec

    keys_.push_back("PFlow"); // PF HPS taus 
    char data[10]; sprintf(data,"%dGeV",(int)sampleTauPtCut_);
    
    // I/O folders

    TString prepend( commondefinitions::eChONmuChOFF_ ? "eltau/" : "mutau/" );
    iSpyFolder_ = TString(commondefinitions::inputArea_ + prepend);
    oSpyFolder_ = TString(commondefinitions::outputArea_+ prepend);


    iFolder_ = TString(commondefinitions::inputArea_);
    oFolder_ = TString(commondefinitions::outputArea_);
 

    if( pu_ == PUPLUS  ) oFolder_ += TString("puplus-"); else if ( pu_ == PUMINUS ) oFolder_ += TString("puminus-");

    TString mtStr("nomt-");    if ( APPLY_MT_CUT_)    { mtStr  = TString("mt-");}
    TString lepStr("EL-");     if (!eChONmuChOFF_)    { lepStr = TString("MU-");}
    TString bwStr("");         if (!bwStr)            { bwStr  = TString("bw-");}
    TString vers("2011-V1-");

   
    if( run2012_ ) vers = TString("2012-V1-");

    
    
    iDFolder_ = TString(commondefinitions::inputArea_);  
    oDFolder_  = TString(commondefinitions::outputArea_);
    
    oFolder_ += mtStr+vers+TString("mc-")+lepStr+bwStr+TString(data)+TString("/");
    oDFolder_ += mtStr+vers+TString("data-")+lepStr+TString(data)+TString("/");
    
    // Summer11
    //http://cms.cern.ch/iCMS/prep/requestmanagement?status=Done&pwg=HIG&campid=Summer11_R1&dsn=
    //http://cms.cern.ch/iCMS/prep/requestmanagement?pwg=TOP&campid=Summer11_R1
    //https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections

    for (int i=0; i<200; i++ ){ defaultXSections_.push_back(0); }
    
    if( run2012_ ) {

      // xsections from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat8TeV
      defaultXSections_[DATA_URL]      = 0.;         
      defaultXSections_[TTBAR_URL]     = 245.8;///18.;  //Kidonakis - approx. NNLO - Inclusive - 234+10- - Top mass: m(top)=173GeV, Default scales: muF=muR=mt, PDF: MSTW2008 NNLO 
      
      defaultXSections_[W_ENU_URL]     = 12503.0;///50.; //NNLO 37509.0  36257.2/3 = 12085.7 - NNLO - W->lv, l=e,m,t - Inclusive W production, BR(W->lv) included, l=e,m,t, PDF error also includes alphas 
      defaultXSections_[W_MUNU_URL]    = 12503.0;///50.;
      defaultXSections_[W_TAUNU_URL]   = 12503.0;///50.; 

      defaultXSections_[WJMADGRAPH_URL]  = 37509.0;///18.; // 36257.2;// 30400; //36257.2;
      
      // for the _filter sample:     xsec: 11050.0 eff: 0.069 
      defaultXSections_[DY_10TO50_URL]          = 860.5;//P. 11050.0;///50.; // from http://cms.cern.ch/iCMS/prep/requestmanagement?dsn=DYJetsToLL_M-10To50filter_8TeV-madgraph&campid=Summer12_DR53X
      defaultXSections_[DY_FROM50_URL]          = 3503.71;//P. 2950.;///50.; // from http://cms.cern.ch/iCMS/prep/requestmanagement?dsn=DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball&campid=Summer12_DR53X

      defaultXSections_[S_URL]                  = 3.79;     
      defaultXSections_[T_URL]                  = 56.4;    
      defaultXSections_[W_URL]                  = 11.1;  
      
      
      defaultXSections_[A_S_URL]                = 1.76;///50.;    
      defaultXSections_[A_T_URL]                = 30.7;    
      defaultXSections_[A_W_URL]                = 11.1;

      defaultXSections_[QCD_EM30TO80_URL]       = 4615893.; // 74330000. eff 0.0621 http://cms.cern.ch/iCMS/prep/requestmanagement?dsn=QCD_Pt_30_80_EMEnriched_TuneZ2star_8TeV_pythia6&campid=Summer12
      defaultXSections_[QCD_EM80TO170_URL]      = 183294.9; // 1191000. eff 0.1539 http://cms.cern.ch/iCMS/prep/requestmanagement?dsn=QCD_Pt_80_170_EMEnriched_TuneZ2star_8TeV_pythia6&campid=Summer12
      
      defaultXSections_[QCD_BCTOE30TO80_URL]    = 167040.; // 74240000. eff 0.00225 http://cms.cern.ch/iCMS/prep/requestmanagement?dsn=QCD_Pt_30_80_BCtoE_TuneZ2star_8TeV_pythia6&campid=Summer12
      defaultXSections_[QCD_BCTOE80TO170_URL]   = 12981.9; //1191000. 0.0109 http://cms.cern.ch/iCMS/prep/requestmanagement?dsn=QCD_Pt_80_170*&campid=Summer12         
      
      defaultXSections_[QCD_MU_URL]             = 134680.; //364000000. eff 3.7E-4 http://cms.cern.ch/iCMS/prep/requestmanagement?dsn=QCD_Pt_20_MuEnrichedPt_15_TuneZ2star_8TeV_pythia6&campid=Summer12
      
      
      defaultXSections_[QCD_PHOTON30TO50_URL]   = 19931.62;        
      defaultXSections_[QCD_PHOTON50TO80_URL]   = 3322.309;         
      defaultXSections_[QCD_PHOTON80TO120_URL]  = 558.2865;        
      defaultXSections_[QCD_PHOTON120TO170_URL] = 108.0068;
      
      
      defaultXSections_[WW_URL]                 = 54.838;///50.;
      defaultXSections_[WZ_URL]                 = 33.72;///50.;
      defaultXSections_[ZZ_URL]                 = 17.627;///50.;
      
      defaultXSections_[CTRLWW_URL]             = 245.8; 
      defaultXSections_[WH80_URL]               = 245.8;            
      defaultXSections_[HH80_URL]               = 245.8;                                           
      defaultXSections_[WH100_URL]              = 245.8;                                          
      defaultXSections_[HH100_URL]              = 245.8;                                        
      defaultXSections_[WH120_URL]              = 245.8;                                          
      defaultXSections_[HH120_URL]              = 245.8;                                          
      defaultXSections_[WH140_URL]              = 245.8;                                          
      defaultXSections_[HH140_URL]              = 245.8; 
      defaultXSections_[WH150_URL]              = 245.8; 
      defaultXSections_[HH150_URL]              = 245.8; 
      defaultXSections_[WH155_URL]              = 245.8; 
      defaultXSections_[HH155_URL]              = 245.8; 
      defaultXSections_[WH160_URL]              = 245.8;                                           
      defaultXSections_[HH160_URL]              = 245.8;                                                                                 
      



      //	      // mhmax, tanb5
      //	      // This yields include branching ratio (which for the light charged higgs was identically 1 and thus not included)
      //	      defaultXSections_[TBH180_URL]             = 2 * 22.6975  * 0.831852  ; 
      //	      defaultXSections_[TBH190_URL]             = 2 * 20.4088  * 0.407966  ;   
      //	      defaultXSections_[TBH200_URL]             = 2 * 18.4811  * 0.228564  ;   
      //	      defaultXSections_[TBH220_URL]             = 2 * 15.4027  * 0.106446  ;  
      //	      defaultXSections_[TBH250_URL]             = 2 * 11.9413  * 0.04129783; 
      //	      defaultXSections_[TBH300_URL]             = 2 * 07.91902 * 0.02514242; 
      //	
      //	
      //	      defaultXSections_[HTB180_URL]             = 2 * 22.6975  * (1 - 0.831852   ) ;/// 30.; 
      //	      defaultXSections_[HTB190_URL]             = 2 * 20.4088  * (1 - 0.407966   ) ;/// 30.;   
      //	      defaultXSections_[HTB200_URL]             = 2 * 18.4811  * (1 - 0.228564   ) ;/// 30.;   
      //	      defaultXSections_[HTB220_URL]             = 2 * 15.4027  * (1 - 0.106446   ) ;/// 30.;  
      //	      defaultXSections_[HTB240_URL]             = 2 * 12.9842  * (1 - 0.04902523 ) ;/// 30.; 
      //	      defaultXSections_[HTB250_URL]             = 2 * 11.9413  * (1 - 0.04129783 ) ;/// 30.; 
      //	      defaultXSections_[HTB260_URL]             = 2 * 10.9879  * (1 - 0.0361988  ) ;/// 30.;  
      //	      defaultXSections_[HTB280_URL]             = 2 * 09.31608 * (1 - 0.02984434 ) ;/// 30.; 
      //	      defaultXSections_[HTB300_URL]             = 2 * 07.91902 * (1 - 0.02514242 ) ;/// 30.; 
      //	      defaultXSections_[HTB350_URL]             = 2 * 0.0536621 * (1.-0.01838561  ) ;/// 30.; 
      //	      defaultXSections_[HTB400_URL]             = 2 * 0.0371904 * (1.-0.01402549  ) ;/// 30.; 
      //	      defaultXSections_[HTB500_URL]             = 2 * 0.0187822 * (1.-0.006268037 ) ;/// 30.; 
      //	      defaultXSections_[HTB600_URL]             = 2 * 0.0100715 * (1.-0.004805924 ) ;/// 30.; 
      //	      defaultXSections_[HTB700_URL]             = 2 * 0.0056681 * (1.-0.004234392 ) ;/// 30.; 
      
 
//      // mhmodp, tanb5
//      // This yields include branching ratio only 
//      defaultXSections_[TBH180_URL]             = 2 * 0.731186; 
//      defaultXSections_[TBH190_URL]             = 2 * 1.; // Dummy   
//      defaultXSections_[TBH200_URL]             = 2 * 0.169792;   
//      defaultXSections_[TBH220_URL]             = 2 * 0.080715;  
//      defaultXSections_[TBH250_URL]             = 2 * 0.035701; 
//      defaultXSections_[TBH300_URL]             = 2 * 0.022798; 
//
//      defaultXSections_[HTB180_URL]             = 2  * 0.241657 ;/// 30.; 
//      defaultXSections_[HTB190_URL]             = 2  * 1. ; // Dummy
//      defaultXSections_[HTB200_URL]             = 2  * 0.817053 ;/// 30.;   
//      defaultXSections_[HTB220_URL]             = 2  * 0.869806 ;/// 30.;  
//      defaultXSections_[HTB240_URL]             = 2  * 0.671911 ;/// 30.; 
//      defaultXSections_[HTB250_URL]             = 2  * 0.664358 ;/// 30.; 
//      defaultXSections_[HTB260_URL]             = 2  * 0.661665 ;/// 30.;  
//      defaultXSections_[HTB280_URL]             = 2  * 0.661667 ;/// 30.; 
//      defaultXSections_[HTB300_URL]             = 2  * 0.641995 ;/// 30.; 
//      defaultXSections_[HTB350_URL]             = 2  * 0.583223 ;/// 30.; 
//      defaultXSections_[HTB400_URL]             = 2  * 0.504500 ;/// 30.; 
//      defaultXSections_[HTB500_URL]             = 2  * 0.253983 ;/// 30.; 
//      defaultXSections_[HTB600_URL]             = 2  * 0.202538 ;/// 30.; 
//      defaultXSections_[HTB700_URL]             = 2  * 0.180948 ;/// 30.; 
//

      // mhmodp, tanb30
      // This yields include branching ratio (which for the light charged higgs was identically 1 and thus not included)
     
      defaultXSections_[TBH180_URL]             = 2 * 0.901490; 
      defaultXSections_[TBH190_URL]             = 2 * 1.;   
      defaultXSections_[TBH200_URL]             = 2 * 0.534418;   
      defaultXSections_[TBH220_URL]             = 2 * 0.367483;  
      defaultXSections_[TBH250_URL]             = 2 * 0.257198; 
      defaultXSections_[TBH300_URL]             = 2 * 0.192875; 
      defaultXSections_[TBH400_URL]             = 2 * 0.151062; 
      defaultXSections_[TBH500_URL]             = 2 * 0.110921; 
      defaultXSections_[TBH600_URL]             = 2 * 0.095882; 


      defaultXSections_[HTB180_URL]             = 2 * 0.081662;/// 30.; 
      defaultXSections_[HTB190_URL]             = 2 * 1.;/// 30.;   
      defaultXSections_[HTB200_URL]             = 2 * 0.455760;/// 30.;   
      defaultXSections_[HTB220_URL]             = 2 * 0.625690;/// 30.;  
      defaultXSections_[HTB240_URL]             = 2 * 0.692009;/// 30.; 
      defaultXSections_[HTB250_URL]             = 2 * 0.705360;/// 30.; 
      defaultXSections_[HTB260_URL]             = 2 * 0.717428;/// 30.;  
      defaultXSections_[HTB280_URL]             = 2 * 0.733824;/// 30.; 
      defaultXSections_[HTB300_URL]             = 2 * 0.744171;/// 30.; 
      defaultXSections_[HTB350_URL]             = 2 * 0.750776;/// 30.; 
      defaultXSections_[HTB400_URL]             = 2 * 0.722967;/// 30.; 
      defaultXSections_[HTB500_URL]             = 2 * 0.563428;/// 30.; 
      defaultXSections_[HTB600_URL]             = 2 * 0.495196;/// 30.; 
      defaultXSections_[HTB700_URL]             = 2 * 0.459958;/// 30.; 

     
      defaultXSections_[HHHTAUTAUBB260_URL]             = 0.06 ;// 60 fb 
      defaultXSections_[HHHTAUTAUBB300_URL]             = 0.06 ;// 60 fb 
      defaultXSections_[HHHTAUTAUBB350_URL]             = 0.06 ;// 60 fb

      defaultXSections_[EMBEDDED_DATA_URL]      = 0.;               
      defaultXSections_[EMBEDDED_TTBAR_URL]     = defaultXSections_[TTBAR_URL];               
   
    } else {
      
      defaultXSections_[DATA_URL]      = 0.;         
      defaultXSections_[TTBAR_URL]     = 165;  //NNLO aproximation +- 10 old was 157.5 
      
      defaultXSections_[W_ENU_URL]     = 10438; //NNLO 31314/3 =10438 //NLO 28000/3 = 9333 // xsec 7899 WToENu_TuneZ2_7TeV-pythia6/Fall10-START38_V12-v1                 
      defaultXSections_[W_MUNU_URL]    = 10438;
      defaultXSections_[W_TAUNU_URL]   = 10438; 
      
      defaultXSections_[W0J_URL]         = 20030.*1.3;
      defaultXSections_[W1JA_URL]        = 3693.*1.3;
      defaultXSections_[W1JB_URL]        = 71.97*1.3;
      defaultXSections_[W2JA_URL]        = 943.4*1.3;
      defaultXSections_[W2JB_URL]        = 67.18*1.3;
      defaultXSections_[W3JA_URL]        = 208.7*1.3;
      defaultXSections_[W3JB_URL]        = 32.43*1.3;
      defaultXSections_[W4JA_URL]        = 44.46*1.3;
      defaultXSections_[W4JB_URL]        = 11.38*1.3;
      defaultXSections_[W5JA_URL]        = 11.11*1.3;
      defaultXSections_[W5JB_URL]        = 3.789*1.3;
      
      defaultXSections_[WJMADGRAPH_URL]  = 31314;
      
      defaultXSections_[TT0J_URL]        = 49.17*2.1768;
      defaultXSections_[TT1J_URL]        = 17.97*2.1768 ;
      defaultXSections_[TT2J_URL]        = 4.279*2.1768 ;
      defaultXSections_[TT3J_URL]        = 0.7951*2.1768 ;
      defaultXSections_[TT4J_URL]        = 0.1404*2.1768 ;
      
      // the number is LO cross section (3x310 pb) with a k-factor to correct for NLO (1.33). The actual source of this - I don't know.
      defaultXSections_[DY_10TO50_URL]          = 12369;
      defaultXSections_[DY_FROM50_URL]          = 3048; //NNLO 3048 //NLO 2800// xsec= 2321  eff=0.44 DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola 

      defaultXSections_[S_URL]                  = 2.72;     
      defaultXSections_[T_URL]                  = 42.6;    
      defaultXSections_[W_URL]                  = 7.87;  
      
      defaultXSections_[A_S_URL]                = 1.49;    
      defaultXSections_[A_T_URL]                = 22;    
      defaultXSections_[A_W_URL]                = 7.87;  
      

      defaultXSections_[QCD_EM20TO30_URL]       = 2454400.;     //xsec =236000000 eff=0.0104 QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6
      defaultXSections_[QCD_EM30TO80_URL]       = 3866200.;     //xsec =59480000  eff=0.065  QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia6 
      defaultXSections_[QCD_EM80TO170_URL]      = 139500.;      //xsec =900000    eff=0.155  QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6 
      defaultXSections_[QCD_EM170TO250_URL]      = 5503.824; //5962.476; //4586.52; 
      defaultXSections_[QCD_EM250TO350_URL]      = 779.45; //835.125; //556.75; 
      defaultXSections_[QCD_EM350TOINF_URL]      = 124.74; //133.65; //89.1; 

      
      defaultXSections_[QCD_BCTOE20TO30_URL]    = 132160;       //xsec = 236000000 eff=0.00056 QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6 
      defaultXSections_[QCD_BCTOE30TO80_URL]    = 136804;       //xsec = 59480000  eff=0.00230 QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6 
      defaultXSections_[QCD_BCTOE80TO170_URL]   = 9360;         //xsec = 900000    eff=0.0104  QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia6   
      defaultXSections_[QCD_BCTOE170TO250_URL]   = 758.4; //821.6; //632.; 
      defaultXSections_[QCD_BCTOE250TO350_URL]   = 144.62; //154.95; //103.3; 
      defaultXSections_[QCD_BCTOE350TOINF_URL]   = 33.6; //36.; //24.; 

      defaultXSections_[QCD_PHOTON15TO30_URL]   = 200061.7; //220067.87; //200061.7;        
      defaultXSections_[QCD_PHOTON30TO50_URL]   = 16690;        //xsec = 1.669e+04   G_Pt_30to50_TuneZ2_7TeV_pythia6                 
      defaultXSections_[QCD_PHOTON50TO80_URL]   = 2722;         //xsec = 2.722e+03   G_Pt_50to80_TuneZ2_7TeV_pythia6                         
      defaultXSections_[QCD_PHOTON80TO120_URL]  = 447.2;        //xsec = 4.472e+02   G_Pt_80to120_TuneZ2_7TeV_pythia6
      defaultXSections_[QCD_PHOTON120TO170_URL] = 84.1;	        //xsec = 8.417e+01   G_Pt_120to170_TuneZ2_7TeV_pythia6
      defaultXSections_[QCD_PHOTON170TO300_URL] = 48.195; //45.183;  //39.1586; //30.122;
      defaultXSections_[QCD_PHOTON300TO470_URL] = 3.42;  //3.21;  //2.14;



//      defaultXSections_[QCD_EM20TO30_URL]       = 2914860.;
//      defaultXSections_[QCD_EM30TO80_URL]       = 4615893.; //5077482.3; //4615893.; 
//      defaultXSections_[QCD_EM80TO170_URL]      = 277997.; //238283.37; //183294.9; 
//      defaultXSections_[QCD_EM170TO250_URL]      = 5503.824; //5962.476; //4586.52; 
//      defaultXSections_[QCD_EM250TO350_URL]      = 779.45; //835.125; //556.75; 
//      defaultXSections_[QCD_EM350TOINF_URL]      = 124.74; //133.65; //89.1; 
//      
//      defaultXSections_[QCD_BCTOE20TO30_URL]    = 167388.; 
//      defaultXSections_[QCD_BCTOE30TO80_URL]    = 167040.; //183744.; //167040.; 
//      defaultXSections_[QCD_BCTOE80TO170_URL]   =  19689.; //16876.47; //12981.9; 
//      defaultXSections_[QCD_BCTOE170TO250_URL]   = 758.4; //821.6; //632.; 
//      defaultXSections_[QCD_BCTOE250TO350_URL]   = 144.62; //154.95; //103.3; 
//      defaultXSections_[QCD_BCTOE350TOINF_URL]   = 33.6; //36.; //24.; 
//      
//      defaultXSections_[QCD_MU_URL]             = 134680.; //364000000. eff 3.7E-4 http://cms.cern.ch/iCMS/prep/requestmanagement?dsn=QCD_Pt_20_MuEnrichedPt_15_TuneZ2star_8TeV_pythia6&campid=Summer12
//      
//      defaultXSections_[QCD_PHOTON15TO30_URL]   = 200061.7; //220067.87; //200061.7;        
//      defaultXSections_[QCD_PHOTON30TO50_URL]   = 19931.62; //21924.78; //19931.62;        
//      defaultXSections_[QCD_PHOTON50TO80_URL]   = 3322.309;         
//      defaultXSections_[QCD_PHOTON80TO120_URL]  = 893.; //781.6; //614.115; //558.2865;        
//      defaultXSections_[QCD_PHOTON120TO170_URL] = 172.81; //151.209; //108.0068;
//      defaultXSections_[QCD_PHOTON170TO300_URL] = 48.195; //45.183;  //39.1586; //30.122;
//      defaultXSections_[QCD_PHOTON300TO470_URL] = 3.42;  //3.21;  //2.14;
      


      
      //defaultXSections_[QCD_MU_URL]           = 349988;       //xsec=296600000 	eff=0.00118  QCD_Pt-20_MuEnrichedPt-10_TuneZ2_7TeV-pythia6 //79688.;                                    
      defaultXSections_[QCD_MU_URL]             = 84679;        //xsec=296600000 	eff=0.002855 QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6      
      
      // xsections taken from CMS-AN-10-337
      defaultXSections_[WW_URL]                 = 43;
      defaultXSections_[WZ_URL]                 = 18.2;
      defaultXSections_[ZZ_URL]                 = 5.9;
      
      defaultXSections_[CTRLWW_URL]             = 165; 
      defaultXSections_[WH80_URL]               = 165;            
      defaultXSections_[HH80_URL]               = 165;                                           
      defaultXSections_[WH100_URL]              = 165;                                          
      defaultXSections_[HH100_URL]              = 165;                                        
      defaultXSections_[WH120_URL]              = 165;                                          
      defaultXSections_[HH120_URL]              = 165;                                          
      defaultXSections_[WH140_URL]              = 165;                                          
      defaultXSections_[HH140_URL]              = 165; 
      defaultXSections_[WH150_URL]              = 165; 
      defaultXSections_[HH150_URL]              = 165; 
      defaultXSections_[WH155_URL]              = 165; 
      defaultXSections_[HH155_URL]              = 165; 
      defaultXSections_[WH160_URL]              = 165;                                           
      defaultXSections_[HH160_URL]              = 165;                                                                                 
      
      defaultXSections_[TBH250_URL]                = 165;           
    }
}




void SampleProcessor::init(vector<TString> listOfurls, vector<double> listOfXSections, bool fullStats, double lum, uint n_data){

  nToProcess_ = n_data;
 
  if(listOfurls.size()!= listOfXSections.size() ){ 
    cout<<endl<<"\n\n Number of files does not match number of xsections... \n\n\n"<<endl; 
    exit(0);
  }


  
  // Reset urls, xsections,files,histos, scales,reades,events ////////
  listOfurls_.clear();
  listOfXSections_.clear();
  listOfFiles_.clear();
  listOfHistos_.clear();
  listOfScales_.clear();
  listOfReaders_.clear();
  listOfReadersMC_.clear();
  //listOfSpies_.clear();

  listOfEventsToProcess_.clear();
  listOfEventsToProcessMC_.clear();

  listOfEvents_.clear();
  
  URLS_=listOfurls.size();

  for(unsigned int i=0; i< URLS_;i++){ 

    listOfurls_.push_back(listOfurls[i]); 
    listOfXSections_.push_back(listOfXSections[i]); 
    listOfFiles_.push_back(0); 
    listOfHistos_.push_back(0); 
    listOfScales_.push_back(0); 
    listOfReaders_.push_back(0);
    listOfReadersMC_.push_back(0);  
    //listOfSpies_.push_back(0);
    listOfEventsToProcess_.push_back(0); 
    listOfEventsToProcessMC_.push_back(0); 
    listOfEvents_.push_back(0); 
  }
  ///////////////////////////////////////////////////////////////////

  fullStats_ = fullStats;
  lum_       = lum;

  init();
  
}

void SampleProcessor::init_spy(vector<TString> listOfurls, vector<double> listOfXSections, bool fullStats, double lum, uint n_data){

  nToProcess_ = n_data;
 
  if(listOfurls.size()!= listOfXSections.size() ){ 
    cout<<endl<<"\n\n Number of files does not match number of xsections... \n\n\n"<<endl; 
    exit(0);
  }


  
  // Reset urls, xsections,files,histos, scales,reades,events ////////
  listOfurls_.clear();
  listOfXSections_.clear();
  listOfFiles_.clear();
  listOfHistos_.clear();
  listOfScales_.clear();
  listOfReaders_.clear();
  listOfReadersMC_.clear();
  //listOfSpies_.clear();

  listOfEventsToProcess_.clear();
  listOfEventsToProcessMC_.clear();

  listOfEvents_.clear();
  
  URLS_=listOfurls.size();

  for(unsigned int i=0; i< URLS_;i++){ 

    listOfurls_.push_back(listOfurls[i]); 
    listOfXSections_.push_back(listOfXSections[i]); 
    listOfFiles_.push_back(0); 
    listOfHistos_.push_back(0); 
    listOfScales_.push_back(0); 
    listOfReaders_.push_back(0);
    listOfReadersMC_.push_back(0);  
    //listOfSpies_.push_back(0);
    listOfEventsToProcess_.push_back(0); 
    listOfEventsToProcessMC_.push_back(0); 
    listOfEvents_.push_back(0); 
  }
  ///////////////////////////////////////////////////////////////////

  fullStats_ = fullStats;
  lum_       = lum;

  init_spy();
  
}


void SampleProcessor::initProcessedFiles(){

  TString dataFolder  ("xxx"); 	 	 
  TString mcFolder    ("xxx");   

  listOfurls_.clear(); 
  listOfurls_.push_back(dataFolder+TString("out-data.root"));
  listOfurls_.push_back(mcFolder+TString("out-ttbar.root"));
  listOfurls_.push_back(mcFolder+TString("out-singletop.root"));  
  listOfurls_.push_back(mcFolder+TString("out-wjets.root"));  
  listOfurls_.push_back(mcFolder+TString("out-zjets.root"));  
  listOfurls_.push_back(mcFolder+TString("out-dibosons.root"));  
  listOfurls_.push_back(mcFolder+TString("out-qcd.root"));  

  listOfProcessedFiles_.clear();

  // open files ///////////////////////////////////////////////////////////////////////////////////////////////
  for(unsigned int i=0;i <listOfurls_.size();i++){
    listOfProcessedFiles_.push_back( TFile::Open(listOfurls_[i]) );
    if(! listOfProcessedFiles_[i] ){ cout <<endl<<"\n Can't find file : "<<(listOfurls_[i])<<endl; continue;}  
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////

}



void SampleProcessor::init(){


  // data location in the tree ///////////////////////
  TString cutflow("myEventSelector/Selection/cutflow");
  TString tag("data"); 
  TString id("myEventSelector");
  ////////////////////////////////////////////////////


  // open files ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  for(unsigned int i=0;i <URLS_;i++){
    listOfFiles_[i] = TFile::Open(listOfurls_[i]);  
    if(! listOfFiles_[i] ){ cout <<endl<<"\n Can't find file : "<<(listOfurls_[i])<<endl; continue;}

    listOfHistos_[i] = (TH1D *) (listOfFiles_[i])->Get(cutflow);
    listOfEvents_[i] = (listOfHistos_[i])->GetBinContent(1);

    //    double unsplitNumber(1.);
    if     (listOfurls_[i].Contains("ttbar_"              ))    listOfEvents_[i] =  8272517   ; // Works for embedded too (embedded ttbar must be normalized using the original number of events from the base sample)
    else if(listOfurls_[i].Contains("stop_s_"             ))    listOfEvents_[i] =  259960    ;
    else if(listOfurls_[i].Contains("stop_t_"             ))    listOfEvents_[i] =  3741874   ;
    else if(listOfurls_[i].Contains("stop_tW-DR_"         ))    listOfEvents_[i] =  497657    ;
    else if(listOfurls_[i].Contains("stopbar_s_"          ))    listOfEvents_[i] =  139974    ;
    else if(listOfurls_[i].Contains("stopbar_t_"          ))    listOfEvents_[i] =  1915066   ;
    else if(listOfurls_[i].Contains("stopbar_tW-DR_"      ))    listOfEvents_[i] =  493458    ;
    else if(listOfurls_[i].Contains("WJetsToLNu_"         ))    listOfEvents_[i] =  75352713  ;
    else if(listOfurls_[i].Contains("dy_from50_"          ))    listOfEvents_[i] =  30405425  ;
    else if(listOfurls_[i].Contains("dy_10_50_"           ))    listOfEvents_[i] =  9942910   ;
    else if(listOfurls_[i].Contains("WW_"                 ))    listOfEvents_[i] =  9974488   ;
    else if(listOfurls_[i].Contains("WZ_"                 ))    listOfEvents_[i] =  9924520   ;
    else if(listOfurls_[i].Contains("ZZ_"                 ))    listOfEvents_[i] =  9744891   ;
    else if(listOfurls_[i].Contains("qcd_EM_Pt30to80_"    ))    listOfEvents_[i] =  28619036  ;
    else if(listOfurls_[i].Contains("qcd_EM_Pt80to170_"   ))    listOfEvents_[i] =  19945948  ;
    else if(listOfurls_[i].Contains("qcd_30to80_BCtoE_"   ))    listOfEvents_[i] =  2043147   ;
    else if(listOfurls_[i].Contains("qcd_80to170_BCtoE_"  ))    listOfEvents_[i] =  1945523   ;
    else if(listOfurls_[i].Contains("PhotonJets_30to50_"  ))    listOfEvents_[i] =  1984322   ; //1939322   ;
    else if(listOfurls_[i].Contains("PhotonJets_50to80_"  ))    listOfEvents_[i] =  1986059   ;
    else if(listOfurls_[i].Contains("PhotonJets_80to120_" ))    listOfEvents_[i] =  1983625   ; //1938625   ;
    else if(listOfurls_[i].Contains("PhotonJets_120to170_"))    listOfEvents_[i] =  1960042   ;
    else if(listOfurls_[i].Contains("qcdmu15_20toinf_"    ))    listOfEvents_[i] =  21352566  ; //20920569  ;
    else if(listOfurls_[i].Contains("htb-pythia-m180_"  ))      listOfEvents_[i] = 300000;
    else if(listOfurls_[i].Contains("htb-pythia-m200_"  ))      listOfEvents_[i] = 299999;
    else if(listOfurls_[i].Contains("htb-pythia-m220_"  ))      listOfEvents_[i] = 299999;
    else if(listOfurls_[i].Contains("htb-pythia-m240_"  ))      listOfEvents_[i] = 300000;
    else if(listOfurls_[i].Contains("htb-pythia-m250_"  ))      listOfEvents_[i] = 299668;
    else if(listOfurls_[i].Contains("htb-pythia-m260_"  ))      listOfEvents_[i] = 299686;
    else if(listOfurls_[i].Contains("htb-pythia-m280_"  ))      listOfEvents_[i] = 300000;//290000;
    else if(listOfurls_[i].Contains("htb-pythia-m300_"  ))      listOfEvents_[i] = 300000;//290000;
    else if(listOfurls_[i].Contains("htb-pythia-m350_"  ))      listOfEvents_[i] = 286364;//290304;
    else if(listOfurls_[i].Contains("htb-pythia-m500_"  ))      listOfEvents_[i] = 288005;//290880;
    else if(listOfurls_[i].Contains("htb-pythia-m600_"  ))      listOfEvents_[i] = 286484;
    else if(listOfurls_[i].Contains("htb-pythia-m700_"  ))      listOfEvents_[i] = 290000;
    


    
    listOfScales_[i] = (lum_*listOfXSections_[i])/listOfEvents_[i];   

    if( listOfXSections_[i] == 0){ listOfScales_[i]=1;}  // Data

    listOfReaders_[i]     = new event::Reader(); 
    listOfReadersMC_[i]   = new event::Reader();

    listOfEventsToProcess_[i]   = (listOfReaders_[i])->AssignMiniEventTreeFrom(listOfFiles_[i],tag,id);
    listOfEventsToProcessMC_[i] = (listOfReadersMC_[i])->AssignMiniEventTreeFrom(listOfFiles_[i],"mc",id);


    //SPY ///////////////////////////////////////////////////////////
    TString extra; // FIXME CHANGE NAME TO SUBDIR OR SUCH
    if(eChONmuChOFF_) extra=TString("_spy_eltau.root");
    else              extra=TString("_spy_mutau.root");
    TString spyName = outFileName_ + extra;
    TTree * spyTree = listOfReaders_[i]->PrepareToSpyEvents(spyName);
    cout<<endl<<"Spy Events ... on file ... "<<spyName<<endl;
    //////////////////////////////////////////////////////////////////
    
    if(fullStats_){
      cout<<endl<<"\n processing file  : "<<(listOfurls_[i])
          <<"\n Tau pt cut was         : "<<setw(12)<<setiosflags(ios::fixed) << setprecision(8)<<sampleTauPtCut_
          <<"\n sample -> scale        : "<<setw(12)<<setiosflags(ios::fixed) << setprecision(8)<<(listOfScales_[i])
          <<"\n evt processed          : "<<setw(8)<<setprecision(1)<<(listOfEvents_[i])
          <<"\n evt in minitree        : "<<setw(8)<<setprecision(1)<<(listOfEventsToProcess_[i])
          <<"\n MC evt in minitree     : "<<setw(8)<<setprecision(1)<<(listOfEventsToProcessMC_[i])
          <<"\n Xsection employed      : "<<(listOfXSections_[i])
          <<"\n Normalized to lumi (pb): "<<LUM_<<endl;
      
      infoFile_<<endl<<"\n processing file : "<<(listOfurls_[i])
	       <<"\n Tau pt cut was      : "<<setw(12)<<setiosflags(ios::fixed) << setprecision(8)<<sampleTauPtCut_
	       <<"\n sample -> scale     : "<<setw(12)<<setiosflags(ios::fixed) << setprecision(8)<<(listOfScales_[i])
	       <<"\n evt processed       : "<<setw(8)<<setprecision(1)<<(listOfEvents_[i])
	       <<"\n evt in minitree     : "<<setw(8)<<setprecision(1)<<(listOfEventsToProcess_[i])
	       <<"\n MC evt in minitree  : "<<setw(8)<<setprecision(1)<<(listOfEventsToProcessMC_[i])
	       <<"\n Xsection employed : "<<(listOfXSections_[i])
	       <<"\n Normalized to lumi (pb): "<<LUM_<<endl;
      
      if(trigEffStudy_)   {  infoFile_<<" Trigger efficiency is ON  "; cout<<" Trigger efficiency is ON ";  }
      else                {  infoFile_<<" Trigger efficiency is OFF "; cout<<" Trigger efficiency is OFF "; }
      if(applybtagweight_){  infoFile_<<" BTAG weights are ON  ";      cout<<" BTAG weights are ON ";       }
      else                {  infoFile_<<" BTAG weights are OFF ";      cout<<" BTAG weights are OFF";       }



    }else{ 

      cout<<endl<<"\n processing file : "<<(listOfurls_[i])<<"\n evt in minitree     : "<<setw(8)<<setprecision(1)<<(listOfEventsToProcess_[i]);
      infoFile_<<"\n processing file : "<<(listOfurls_[i])<<"\n evt in minitree     : "<<setw(8)<<setprecision(1)<<(listOfEventsToProcess_[i]);
 
      if( nToProcess_ > listOfEventsToProcess_[i] ){ 
        cout<<endl<<" WARNING :: required number of events too large : "<<nToProcess_<<" sample will be fully processed..."<<endl;
        infoFile_<<"  WARNING :: required number of events too large : "<<nToProcess_<<" sample will be fully processed..."<<endl;
   
      }else {      
        cout<<" but only "<<nToProcess_<<" will be processed... "<<endl;
        infoFile_<<" but only "<<nToProcess_<<" will be processed... "<<endl;

        listOfScales_[i] = (lum_*listOfXSections_[i])/nToProcess_;   
        listOfEventsToProcess_[i] = nToProcess_;
      }

      if(trigEffStudy_)   {  infoFile_<<" Trigger efficiency is ON ";  cout<<" Trigger efficiency is ON ";  }
      else                {  infoFile_<<" Trigger efficiency is OFF "; cout<<" Trigger efficiency is OFF "; } 
      if(commondefinitions::applybtagweight_){  infoFile_<<" BTAG weights are ON  ";      cout<<" BTAG weights are ON ";       }
      else                {  infoFile_<<" BTAG weights are OFF ";      cout<<" BTAG weights are OFF";       }

    }
  }// finish processing urls /////////////////////////////////////////////////////////////////////////////////////////////////////////
  

}

void SampleProcessor::init_spy(){


  // data location in the tree ///////////////////////
  TString cutflow("myEventSelector/Selection/cutflow");
  TString tag("data"); 
  TString id("myEventSelector");
  ////////////////////////////////////////////////////


  // open files ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  for(unsigned int i=0;i <URLS_;i++){
    listOfFiles_[i] = TFile::Open(listOfurls_[i]);  
    if(! listOfFiles_[i] ){ cout <<endl<<"\n Can't find file : "<<(listOfurls_[i])<<endl; continue;}

    //    listOfHistos_[i] = (TH1D *) (listOfFiles_[i])->Get(cutflow);
    // listOfEvents_[i] = (listOfHistos_[i])->GetBinContent(1);

    listOfEvents_[i] = 1; // Dummy value for data samples

    //    double unsplitNumber(1.);
    if     (listOfurls_[i].Contains("ttbar_"              ))    listOfEvents_[i] =  8272517   ; // Works for embedded too (embedded ttbar must be normalized using the original number of events from the base sample)
    else if(listOfurls_[i].Contains("stop_s_"             ))    listOfEvents_[i] =  259960    ;
    else if(listOfurls_[i].Contains("stop_t_"             ))    listOfEvents_[i] =  3741874   ;
    else if(listOfurls_[i].Contains("stop_tW-DR_"         ))    listOfEvents_[i] =  497657    ;
    else if(listOfurls_[i].Contains("stopbar_s_"          ))    listOfEvents_[i] =  139974    ;
    else if(listOfurls_[i].Contains("stopbar_t_"          ))    listOfEvents_[i] =  1915066   ;
    else if(listOfurls_[i].Contains("stopbar_tW-DR_"      ))    listOfEvents_[i] =  493458    ;
    else if(listOfurls_[i].Contains("WJetsToLNu_"         ))    listOfEvents_[i] =  75352713  ;
    else if(listOfurls_[i].Contains("dy_from50_"          ))    listOfEvents_[i] =  30405425  ;
    else if(listOfurls_[i].Contains("dy_10_50_"           ))    listOfEvents_[i] =  9942910   ;
    else if(listOfurls_[i].Contains("WW_"                 ))    listOfEvents_[i] =  9974488   ;
    else if(listOfurls_[i].Contains("WZ_"                 ))    listOfEvents_[i] =  9924520   ;
    else if(listOfurls_[i].Contains("ZZ_"                 ))    listOfEvents_[i] =  9744891   ;
    else if(listOfurls_[i].Contains("qcd_EM_Pt30to80_"    ))    listOfEvents_[i] =  28619036  ;
    else if(listOfurls_[i].Contains("qcd_EM_Pt80to170_"   ))    listOfEvents_[i] =  19945948  ;
    else if(listOfurls_[i].Contains("qcd_30to80_BCtoE_"   ))    listOfEvents_[i] =  2043147   ;
    else if(listOfurls_[i].Contains("qcd_80to170_BCtoE_"  ))    listOfEvents_[i] =  1945523   ;
    else if(listOfurls_[i].Contains("PhotonJets_30to50_"  ))    listOfEvents_[i] =  1984322   ; //1939322   ;
    else if(listOfurls_[i].Contains("PhotonJets_50to80_"  ))    listOfEvents_[i] =  1986059   ;
    else if(listOfurls_[i].Contains("PhotonJets_80to120_" ))    listOfEvents_[i] =  1983625   ; //1938625   ;
    else if(listOfurls_[i].Contains("PhotonJets_120to170_"))    listOfEvents_[i] =  1960042   ;
    else if(listOfurls_[i].Contains("qcdmu15_20toinf_"    ))    listOfEvents_[i] =  21352566  ; //20920569  ;
    else if(listOfurls_[i].Contains("htb-pythia-m180_"  ))      listOfEvents_[i] = 300000;
    else if(listOfurls_[i].Contains("htb-pythia-m200_"  ))      listOfEvents_[i] = 299999;
    else if(listOfurls_[i].Contains("htb-pythia-m220_"  ))      listOfEvents_[i] = 299999;
    else if(listOfurls_[i].Contains("htb-pythia-m240_"  ))      listOfEvents_[i] = 300000;
    else if(listOfurls_[i].Contains("htb-pythia-m250_"  ))      listOfEvents_[i] = 299668;
    else if(listOfurls_[i].Contains("htb-pythia-m260_"  ))      listOfEvents_[i] = 299686;
    else if(listOfurls_[i].Contains("htb-pythia-m280_"  ))      listOfEvents_[i] = 300000;//290000;
    else if(listOfurls_[i].Contains("htb-pythia-m300_"  ))      listOfEvents_[i] = 300000;//290000;
    else if(listOfurls_[i].Contains("htb-pythia-m350_"  ))      listOfEvents_[i] = 286364;//290304;
    else if(listOfurls_[i].Contains("htb-pythia-m500_"  ))      listOfEvents_[i] = 288005;//290880;
    else if(listOfurls_[i].Contains("htb-pythia-m600_"  ))      listOfEvents_[i] = 286484;
    else if(listOfurls_[i].Contains("htb-pythia-m700_"  ))      listOfEvents_[i] = 290000;
    


    
    listOfScales_[i] = (lum_*listOfXSections_[i])/listOfEvents_[i];   

    if( listOfXSections_[i] == 0){ listOfScales_[i]=1;}  // Data

    listOfReaders_[i]     = new event::Reader(); 
    listOfReadersMC_[i]   = new event::Reader();

    listOfEventsToProcess_[i]   = (listOfReaders_[i])->AssignMiniEventTreeFrom(listOfFiles_[i],tag,id);
    listOfEventsToProcessMC_[i] = (listOfReadersMC_[i])->AssignMiniEventTreeFrom(listOfFiles_[i],"mc",id);


    //SPY ///////////////////////////////////////////////////////////
    TString extra; // FIXME CHANGE NAME TO SUBDIR OR SUCH
    if(eChONmuChOFF_) extra=TString("_spy_eltau.root");
    else              extra=TString("_spy_mutau.root");
    TString spyName = outFileName_ + extra;
    TTree * spyTree = listOfReaders_[i]->PrepareToSpyEvents(spyName);
    cout<<endl<<"Spy Events ... on file ... "<<spyName<<endl;
    //////////////////////////////////////////////////////////////////
    
    if(fullStats_){
      cout<<endl<<"\n processing file  : "<<(listOfurls_[i])
          <<"\n Tau pt cut was         : "<<setw(12)<<setiosflags(ios::fixed) << setprecision(8)<<sampleTauPtCut_
          <<"\n sample -> scale        : "<<setw(12)<<setiosflags(ios::fixed) << setprecision(8)<<(listOfScales_[i])
          <<"\n evt processed          : "<<setw(8)<<setprecision(1)<<(listOfEvents_[i])
          <<"\n evt in minitree        : "<<setw(8)<<setprecision(1)<<(listOfEventsToProcess_[i])
          <<"\n MC evt in minitree     : "<<setw(8)<<setprecision(1)<<(listOfEventsToProcessMC_[i])
          <<"\n Xsection employed      : "<<(listOfXSections_[i])
          <<"\n Normalized to lumi (pb): "<<LUM_<<endl;
      
      infoFile_<<endl<<"\n processing file : "<<(listOfurls_[i])
	       <<"\n Tau pt cut was      : "<<setw(12)<<setiosflags(ios::fixed) << setprecision(8)<<sampleTauPtCut_
	       <<"\n sample -> scale     : "<<setw(12)<<setiosflags(ios::fixed) << setprecision(8)<<(listOfScales_[i])
	       <<"\n evt processed       : "<<setw(8)<<setprecision(1)<<(listOfEvents_[i])
	       <<"\n evt in minitree     : "<<setw(8)<<setprecision(1)<<(listOfEventsToProcess_[i])
	       <<"\n MC evt in minitree  : "<<setw(8)<<setprecision(1)<<(listOfEventsToProcessMC_[i])
	       <<"\n Xsection employed : "<<(listOfXSections_[i])
	       <<"\n Normalized to lumi (pb): "<<LUM_<<endl;
      
      if(trigEffStudy_)   {  infoFile_<<" Trigger efficiency is ON  "; cout<<" Trigger efficiency is ON ";  }
      else                {  infoFile_<<" Trigger efficiency is OFF "; cout<<" Trigger efficiency is OFF "; }
      if(applybtagweight_){  infoFile_<<" BTAG weights are ON  ";      cout<<" BTAG weights are ON ";       }
      else                {  infoFile_<<" BTAG weights are OFF ";      cout<<" BTAG weights are OFF";       }



    }else{ 

      cout<<endl<<"\n processing file : "<<(listOfurls_[i])<<"\n evt in minitree     : "<<setw(8)<<setprecision(1)<<(listOfEventsToProcess_[i]);
      infoFile_<<"\n processing file : "<<(listOfurls_[i])<<"\n evt in minitree     : "<<setw(8)<<setprecision(1)<<(listOfEventsToProcess_[i]);
 
      if( nToProcess_ > listOfEventsToProcess_[i] ){ 
        cout<<endl<<" WARNING :: required number of events too large : "<<nToProcess_<<" sample will be fully processed..."<<endl;
        infoFile_<<"  WARNING :: required number of events too large : "<<nToProcess_<<" sample will be fully processed..."<<endl;
   
      }else {      
        cout<<" but only "<<nToProcess_<<" will be processed... "<<endl;
        infoFile_<<" but only "<<nToProcess_<<" will be processed... "<<endl;

        listOfScales_[i] = (lum_*listOfXSections_[i])/nToProcess_;   
        listOfEventsToProcess_[i] = nToProcess_;
      }

      if(trigEffStudy_)   {  infoFile_<<" Trigger efficiency is ON ";  cout<<" Trigger efficiency is ON ";  }
      else                {  infoFile_<<" Trigger efficiency is OFF "; cout<<" Trigger efficiency is OFF "; } 
      if(commondefinitions::applybtagweight_){  infoFile_<<" BTAG weights are ON  ";      cout<<" BTAG weights are ON ";       }
      else                {  infoFile_<<" BTAG weights are OFF ";      cout<<" BTAG weights are OFF";       }

    }
  }// finish processing urls /////////////////////////////////////////////////////////////////////////////////////////////////////////
  

}


int SampleProcessor::codeChannel(int i, int urlCode){

  int ret(666);
  if( 
    urlCode == WH80_URL  || urlCode == HH80_URL  ||
    urlCode == WH100_URL || urlCode == HH100_URL ||
    urlCode == WH120_URL || urlCode == HH120_URL || 
    urlCode == WH140_URL || urlCode == HH140_URL || 
    urlCode == WH150_URL || urlCode == HH150_URL || 
    urlCode == WH155_URL || urlCode == HH155_URL ||
    urlCode == WH160_URL || urlCode == HH160_URL 
  )                                          {  ret = HIGGS_CH;     }
  else if( 
	  urlCode == TBH180_URL || urlCode == HTB180_URL || 
	  urlCode == TBH190_URL || urlCode == HTB190_URL || 
	  urlCode == TBH200_URL || urlCode == HTB200_URL || 
	  urlCode == TBH220_URL || urlCode == HTB220_URL || 
	                           urlCode == HTB240_URL || 
	  urlCode == TBH250_URL || urlCode == HTB250_URL || 
	                           urlCode == HTB260_URL || 
                                   urlCode == HTB280_URL || 
	  urlCode == TBH300_URL	|| urlCode == HTB300_URL ||
	  urlCode == TBH400_URL	|| urlCode == HTB400_URL ||
	  urlCode == TBH500_URL	|| urlCode == HTB500_URL ||
	  urlCode == TBH600_URL	|| urlCode == HTB600_URL
	  	  ){  ret = TBH_CH;       }                   
  else if( i == ETAU                        ){  ret = ETAU_CH;      } 
  else if( i == MUTAU                       ){  ret = MUTAU_CH;     } 
  else if( i == EJETS                       ){  ret = EJETS_CH;     }
  else if( i == MUJETS                      ){  ret = MUJETS_CH;    }
  else if( i == EE                          ){  ret = EE_CH;        }
  else if( i == MUMU                        ){  ret = MUMU_CH;      }
  else if( i == EMU                         ){  ret = EMU_CH;       }
  else if( i == TAUJETS                     ){  ret = TAUJETS_CH;   }
  else if( i == TAUTAU                      ){  ret = TAUTAU_CH;    }
  else if( i == ALLJETS                     ){  ret = ALLJETS_CH;   }
  else if( i == WW || i == WZ || i== ZZ     ){  ret = EWKDI_CH;     }
  else if( i == WQQ                         ){  ret = WQQ_CH;       }
  else if( i == WENU                        ){  ret = WENU_CH;      }
  else if( i == WMUNU                       ){  ret = WMUNU_CH;     }
  else if( i == WTAUNU                      ){  ret = WTAUNU_CH;    }  
  else if( i == ZJETS                       ){  ret = ZJETS_CH;     } 
  else if( i == ZTAUTAU                     ){  ret = ZTAUTAU_CH;   } 
  else if( i == TOPS                        ){  ret = SINGLETOP_CH; } 
  else if( i == TOPT                        ){  ret = SINGLETOP_CH; } 
  else if( i == TOPW                        ){  ret = SINGLETOP_CH; } 
  else if( i == QCD                         ){  ret = QCD_CH;       } 

  return ret;

}



int SampleProcessor::tdChannel(int i){
  
  int ret(666);
  if     ( i == ETAU_CH                              ){  ret = ETAU_TDCH;      } 
  else if( i == MUTAU_CH                             ){  ret = MUTAU_TDCH;     } 
  else if( i == EJETS_CH || i == MUJETS_CH           ){  ret = LEPJETS_TDCH;   }
  //  else if( i == EE_CH || i == EMU_CH || i == MUMU_CH ){  ret = LEPLEP_TDCH;    }
  else if( i == EE_CH                                ){  ret = EE_TDCH;    }
  else if( i == EMU_CH                               ){  ret = EMU_TDCH;    }
  else if( i == MUMU_CH                              ){  ret = MUMU_TDCH;    }
  else if( i == TAUJETS_CH                           ){  ret = TAUJETS_TDCH;   }
  else if( i == TAUTAU_CH                            ){  ret = TAUTAU_TDCH;    }
  else if( i == ALLJETS_CH                           ){  ret = ALLJETS_TDCH;   }
  else if( i == EWKDI_CH                             ){  ret = EWKDI_TDCH;     }
  else if( i == WQQ_CH                               ){  ret = WQQ_TDCH;       }
  else if( i == WENU_CH                              ){  ret = WENU_TDCH;      }
  else if( i == WMUNU_CH                             ){  ret = WMUNU_TDCH;     }
  else if( i == WTAUNU_CH                            ){  ret = WTAUNU_TDCH;    }  
  else if( i == WJETS_CH                             ){  ret = WJETS_TDCH;     }
  else if( i == ZJETS_CH                             ){  ret = ZJETS_TDCH;     } 
  else if( i == ZTAUTAU_CH                           ){  ret = ZTAUTAU_TDCH;   } 
  else if( i == SINGLETOP_CH                         ){  ret = SINGLETOP_TDCH; } 
  else if( i == QCD_CH                               ){  ret = QCD_TDCH;       } 

  return ret;

}



void SampleProcessor::process_ttbar(int i){

  //MADGRAPH
  //process(false, TTBAR_URL, iFolder_ + TString("ttbar.root"), oFolder_+TString("out-ttbar-madgraph.root"),keys_);                          
  //process(false, TTBAR_URL, iFolder_ + TString("ttbar.root"), oFolder_+TString("out-ttbar-signal-madgraph.root"),keys_, TTBAR_SIGNAL_ );   
  //process(false, TTBAR_URL, iFolder_ + TString("ttbar.root"), oFolder_+TString("out-ttbar-other-madgraph.root"),keys_, TTBAR_OTHER_ );     

  url_= TTBAR_URL; 


  //NORMAL SAMPLE
  //  process(false, url_, iFolder_ + TString("ttbar_v1.root"), oFolder_+TString("out-ttbar_v1.root"),keys_);
  //  process(false, url_, iFolder_ + TString("ttbar_v2.root"), oFolder_+TString("out-ttbar_v2.root"),keys_);

  if(i>49){
    cout << "ERROR CODE - must be in the range [1,18]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  process(false, url_, iFolder_ + TString("ttbar_"+idx+".root"), oFolder_+TString("out-ttbar_"+idx+".root"),keys_);

  //SAMPLE WITH WEIGHTS
  //pdfweights_=true; process(false, url_, iFolder_ + TString("ttbar-pdf.root"), oFolder_+TString("out-ttbar-pdf.root"),keys_); pdfweights_=false;
  //pdfweights_=true; process(false, url_, iFolder_ + TString("ttbar-pdf-nofilter-B.root"), oFolder_+TString("out-ttbar-pdf.root"),keys_); pdfweights_=false;
  ///lustre/data3/cmslocal/samples/CMSSW_4_2_X/mc/mTrees-v3-B/ttbar-pdf-nofilter-B.root

  // Trigger efficiencies study ////////////////////////////////////////////////////////////////////////
  //iFolder_ = TString("/lustre/lip.pt/data/cmslocal/samples/CMSSW_4_2_X/mc/mTrees-v3/");
  //process(false, url_, iFolder_ + TString("trig-ttbar.root"), oFolder_+TString("out-ttbar.root"),keys_);
  //////////////////////////////////////////////////////////////////////////////////////////////////////    

}

void SampleProcessor::process_ttbar_Embedded(int i){

  url_= TTBAR_URL; 

  if(i>49){
    cout << "ERROR CODE - must be in the range [1,18]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

    process(true,url_,TString(),oDFolder_+TString("out-Embedded_RunA_"+idx+".root"),keys_);

  process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/tau_dilepton/goodEmbedYourAss/out/Embedded_ttbar_"+idx+".root"), oFolder_+TString("out-embedded-ttbar_"+idx+".root"),keys_);

  //SAMPLE WITH WEIGHTS
  //pdfweights_=true; process(false, url_, iFolder_ + TString("ttbar-pdf.root"), oFolder_+TString("out-ttbar-pdf.root"),keys_); pdfweights_=false;
  //pdfweights_=true; process(false, url_, iFolder_ + TString("ttbar-pdf-nofilter-B.root"), oFolder_+TString("out-ttbar-pdf.root"),keys_); pdfweights_=false;
  ///lustre/data3/cmslocal/samples/CMSSW_4_2_X/mc/mTrees-v3-B/ttbar-pdf-nofilter-B.root

  // Trigger efficiencies study ////////////////////////////////////////////////////////////////////////
  //iFolder_ = TString("/lustre/lip.pt/data/cmslocal/samples/CMSSW_4_2_X/mc/mTrees-v3/");
  //process(false, url_, iFolder_ + TString("trig-ttbar.root"), oFolder_+TString("out-ttbar.root"),keys_);
  //////////////////////////////////////////////////////////////////////////////////////////////////////    

}


void SampleProcessor::process_ttbar_scaleup(int i){

  url_= TTBAR_URL;

  if(i>49){
    cout << "ERROR CODE - must be in the range [1,18]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();
  //  process(false, url_, iFolder_ + TString("ttbar_scaleup.root"),   oFolder_+TString("out-ttbar_scaleup.root"),keys_);
  //  process(false, url_, iFolder_ + TString("ttbar_scaleup.root"), oFolder_+TString("out-ttbar-scaleup_mutau.root"),keys_, MUTAU_ ); 
  process(false, url_, iFolder_ + TString("ttbar_scaleup_"+idx+".root"),
	  oFolder_+TString("out-ttbar-scaleup_"+idx+".root"),keys_); 
}


void SampleProcessor::process_ttbar_scaledown(int i){

  url_= TTBAR_URL;

  if(i>49){
    cout << "ERROR CODE - must be in the range [1,18]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();
  //  process(false, url_, iFolder_ + TString("ttbar_scaledown.root"), oFolder_+TString("out-ttbar_scaledown.root"),keys_);
  //  process(false, url_, iFolder_ + TString("ttbar_scaledown.root"), oFolder_+TString("out-ttbar-scaledown_mutau.root"),keys_, MUTAU_ ); 
  process(false, url_, iFolder_ + TString("ttbar_scaledown_"+idx+".root"),
	  oFolder_+TString("out-ttbar-scaledown_"+idx+".root"),keys_); 
}


void SampleProcessor::process_ttbar_matchup(int i){

  url_= TTBAR_URL;

  if(i>49){
    cout << "ERROR CODE - must be in the range [1,18]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();
  //  process(false, url_, iFolder_ + TString("ttbar_matchup.root"),   oFolder_+TString("out-ttbar_matchup.root"),keys_);
  //  process(false, url_, iFolder_ + TString("ttbar_matchup.root"), oFolder_+TString("out-ttbar-matchup_mutau.root"),keys_, MUTAU_ ); 
  process(false, url_, iFolder_ + TString("ttbar_matchup_"+idx+".root"),
	  oFolder_+TString("out-ttbar-matchup_"+idx+".root"),keys_); 

}


void SampleProcessor::process_ttbar_matchdown(int i){

  url_= TTBAR_URL;

  if(i>49){
    cout << "ERROR CODE - must be in the range [1,18]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();
  //  process(false, url_, iFolder_ + TString("ttbar_matchdown.root"), oFolder_+TString("out-ttbar_matchdown.root"),keys_);
  //  process(false, url_, iFolder_ + TString("ttbar_matchdown.root"), oFolder_+TString("out-ttbar-matchdown_mutau.root"),keys_, MUTAU_ ); 
  process(false, url_, iFolder_ + TString("ttbar_matchdown_"+idx+".root"),
	  oFolder_+TString("out-ttbar-matchdown_"+idx+".root"),keys_); 

}


void SampleProcessor::process_ttbar_etau(int i){
  
  url_= TTBAR_URL;
  
  if(i>49){
    cout << "ERROR CODE - must be in the range [1,18]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();
  
  process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/samples/CMSSW_5_3_9_def/ttbar_"+idx+".root"), oFolder_+TString("out-ttbar-etau_"+idx+".root"),keys_, ETAU_ ); 

}



void SampleProcessor::process_ttbar_mutau(int i){

  url_= TTBAR_URL;

  if(i>49){
    cout << "ERROR CODE - must be in the range [1,18]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  //  process(false, url_, iFolder_ + TString("ttbar_v1.root"), oFolder_+TString("out-ttbar_v1_mutau.root"),keys_, MUTAU_ ); 
  //  process(false, url_, iFolder_ + TString("ttbar_v2.root"), oFolder_+TString("out-ttbar_v2_mutau.root"),keys_, MUTAU_ ); 
  process(false, url_, iFolder_ + TString("ttbar_"+idx+".root"), oFolder_+TString("out-ttbar-mutau_"+idx+".root"),keys_, MUTAU_ ); 
}

void SampleProcessor::process_ttbar_mumu(int i){

  url_= TTBAR_URL;
  if(i>49){
    cout << "ERROR CODE - must be in the range [1,18]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  process(false, url_, iFolder_ + TString("ttbar_"+idx+".root"), oFolder_+TString("out-ttbar-mumu_"+idx+".root"),keys_, MUMU_ ); 
}

void SampleProcessor::process_ttbar_emu(int i){

  url_= TTBAR_URL;
  if(i>49){
    cout << "ERROR CODE - must be in the range [1,18]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  process(false, url_, iFolder_ + TString("ttbar_"+idx+".root"), oFolder_+TString("out-ttbar-emu_"+idx+".root"),keys_, EMU_ ); 
}


void SampleProcessor::process_ttbar_ddbkg(int i){

  url_= TTBAR_URL;

  if(i>49){
    cout << "ERROR CODE - must be in the range [1,18]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  // dd bkg
  //  process(false, url_, iFolder_ + TString("ttbar_v1.root"), oFolder_+TString("out-ttbar_v1_ddbkg.root"),keys_, TTBAR_DDBKG_);   
  //  process(false, url_, iFolder_ + TString("ttbar_v2.root"), oFolder_+TString("out-ttbar_v2_ddbkg.root"),keys_, TTBAR_DDBKG_);   
  process(false, url_, iFolder_ + TString("ttbar_"+idx+".root"), oFolder_+TString("out-ttbar-ddbkg_"+idx+".root"),keys_, TTBAR_DDBKG_);   


}

void SampleProcessor::process_ttbar_mcbkg(int i){

  url_= TTBAR_URL;
  if(i>49){
    cout << "ERROR CODE - must be in the range [1,18]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  // mc bkg
  //  process(false, url_, iFolder_ + TString("ttbar_v1.root"), oFolder_+TString("out-ttbar_v1_mcbkg.root"),keys_, TTBAR_MCBKG_ );
  //  process(false, url_, iFolder_ + TString("ttbar_v2.root"), oFolder_+TString("out-ttbar_v2_mcbkg.root"),keys_, TTBAR_MCBKG_ );
  process(false, url_, iFolder_ + TString("ttbar_"+idx+".root"), oFolder_+TString("out-ttbar-mcbkg_"+idx+".root"),keys_, TTBAR_MCBKG_ );

}


void SampleProcessor::process_singletop(int i){

  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  if(i<20){
    //SINGLE TOP
    url_= S_URL; process(false, url_, iFolder_ + TString("stop_s_"+idx+".root"),     oFolder_ + TString("out-stop_s_"+idx+".root"),keys_);   
    url_= T_URL; process(false, url_, iFolder_ + TString("stop_t_"+idx+".root"),     oFolder_ + TString("out-stop_t_"+idx+".root"),keys_);  
    url_= W_URL; process(false, url_, iFolder_ + TString("stop_tW-DR_"+idx+".root"), oFolder_ + TString("out-stop-DR_tW_"+idx+".root"),keys_); 
    
    //ANTI SINGLE TOP
    url_= A_S_URL; process(false, url_, iFolder_ + TString("stopbar_s_"+idx+".root"),     oFolder_ + TString("out-stopbar_s_"+idx+".root"),keys_);   
    url_= A_W_URL; process(false, url_, iFolder_ + TString("stopbar_tW-DR_"+idx+".root"), oFolder_ + TString("out-stopbar-DR_tW_"+idx+".root"),keys_);
  }   
  url_= A_T_URL; process(false, url_, iFolder_ + TString("stopbar_t_"+idx+".root"),     oFolder_ + TString("out-stopbar_t_"+idx+".root"),keys_);  
}


void SampleProcessor::process_wjets(int i){

 

  // WARNING W+jets with new pgid info
  url_= WJMADGRAPH_URL;
  //  process(false, url_, iFolder_ + TString("WJetsToLNu_v1.root"), oFolder_+TString("out-wjets_v1.root"),keys_);
  //  process(false, url_, iFolder_ + TString("WJetsToLNu_v2.root"), oFolder_+TString("out-wjets_v2.root"),keys_);

  if(i>399){
    cout << "ERROR CODE - must be in the range [1,18]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();


  process(false, url_, iFolder_ + TString("WJetsToLNu_"+idx+".root"), oFolder_+TString("out-wjets_"+idx+".root"),keys_);


  // Trigger efficiencies study //////////////////////////////////////////////////////////////////////////
  // iFolder_ = TString("/lustre/lip.pt/data/cmslocal/samples/CMSSW_4_2_X/mc/mTrees-v3/");
  // process(false, url_, iFolder_ + TString("trig-WJetsToLNu.root"), oFolder_+TString("out-wjets.root"),keys_);
  //////////////////////////////////////////////////////////////////////////////////////////////////////////



}

void SampleProcessor::process_zjets_from50(int i){

  // MADGRAPH Z+JETS

  url_ = DY_FROM50_URL; 
  if(i>199){
    cout << "ERROR CODE - must be in the range [0,49]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  process(false, url_, iFolder_ + TString("dy_from50_"+idx+".root"), oFolder_+TString("out-dy_from50_"+idx+".root"),keys_); 

}

void SampleProcessor::process_zjets_10to50(int i){
  
  // MADGRAPH Z+JETS
  url_ = DY_10TO50_URL;
  if(i>99){
    cout << "ERROR CODE - must be in the range [0,49]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();
  process(false, url_, iFolder_ + TString("dy_10_50_"+idx+".root"), oFolder_+TString("out-dy_10to50_"+idx+".root"),keys_);    
}

void SampleProcessor::process_hh_higgs(){

  url_ = HH80_URL;  process(false, url_, iFolder_ + TString("hh-pythia-m80.root"),  oFolder_+TString("out-hh-pythia-m80.root"),keys_);  
  url_ = HH100_URL; process(false, url_, iFolder_ + TString("hh-pythia-m100.root"), oFolder_+TString("out-hh-pythia-m100.root"),keys_); 
  url_ = HH120_URL; process(false, url_, iFolder_ + TString("hh-pythia-m120.root"), oFolder_+TString("out-hh-pythia-m120.root"),keys_); 
  url_ = HH140_URL; process(false, url_, iFolder_ + TString("hh-pythia-m140.root"), oFolder_+TString("out-hh-pythia-m140.root"),keys_); 
  url_ = HH150_URL; process(false, url_, iFolder_ + TString("hh-pythia-m150.root"), oFolder_+TString("out-hh-pythia-m150.root"),keys_); 
  //  url_ = HH155_URL; process(false, url_, iFolder_ + TString("hh-pythia-m155.root"), oFolder_+TString("out-hh-pythia-m155.root"),keys_); 
  url_ = HH160_URL; process(false, url_, iFolder_ + TString("hh-pythia-m160.root"), oFolder_+TString("out-hh-pythia-m160.root"),keys_); 


}

void SampleProcessor::process_hh_higgs_bychannel(){

//  url_ = HH80_URL;  process(false, url_, iFolder_ + TString("hh-pythia-m80.root"),  oFolder_+TString("out-hh-pythia-m80_etau.root"), keys_, ETAU_);  
//  url_ = HH100_URL; process(false, url_, iFolder_ + TString("hh-pythia-m100.root"), oFolder_+TString("out-hh-pythia-m100_etau.root"),keys_, ETAU_); 
//  url_ = HH120_URL; process(false, url_, iFolder_ + TString("hh-pythia-m120.root"), oFolder_+TString("out-hh-pythia-m120_etau.root"),keys_, ETAU_); 
//  url_ = HH140_URL; process(false, url_, iFolder_ + TString("hh-pythia-m140.root"), oFolder_+TString("out-hh-pythia-m140_etau.root"),keys_, ETAU_); 
//  url_ = HH150_URL; process(false, url_, iFolder_ + TString("hh-pythia-m150.root"), oFolder_+TString("out-hh-pythia-m150_etau.root"),keys_, ETAU_); 
//  url_ = HH155_URL; process(false, url_, iFolder_ + TString("hh-pythia-m155.root"), oFolder_+TString("out-hh-pythia-m155_etau.root"),keys_, ETAU_); 
//  url_ = HH160_URL; process(false, url_, iFolder_ + TString("hh-pythia-m160.root"), oFolder_+TString("out-hh-pythia-m160_etau.root"),keys_, ETAU_); 

  
  url_ = HH80_URL;  process(false, url_, iFolder_ + TString("hh-pythia-m80.root"),  oFolder_+TString("out-hh-pythia-m80_mutau.root"), keys_, MUTAU_);  
  url_ = HH100_URL; process(false, url_, iFolder_ + TString("hh-pythia-m100.root"), oFolder_+TString("out-hh-pythia-m100_mutau.root"),keys_, MUTAU_); 
  url_ = HH120_URL; process(false, url_, iFolder_ + TString("hh-pythia-m120.root"), oFolder_+TString("out-hh-pythia-m120_mutau.root"),keys_, MUTAU_); 
  url_ = HH140_URL; process(false, url_, iFolder_ + TString("hh-pythia-m140.root"), oFolder_+TString("out-hh-pythia-m140_mutau.root"),keys_, MUTAU_); 
  url_ = HH150_URL; process(false, url_, iFolder_ + TString("hh-pythia-m150.root"), oFolder_+TString("out-hh-pythia-m150_mutau.root"),keys_, MUTAU_); 
  // url_ = HH155_URL; process(false, url_, iFolder_ + TString("hh-pythia-m155.root"), oFolder_+TString("out-hh-pythia-m155_mutau.root"),keys_, MUTAU_); 
  url_ = HH160_URL; process(false, url_, iFolder_ + TString("hh-pythia-m160.root"), oFolder_+TString("out-hh-pythia-m160_mutau.root"),keys_, MUTAU_); 

}


void SampleProcessor::process_wh_higgs(){

  url_ = WH80_URL;  process(false, url_, iFolder_ + TString("wh-pythia-m80.root"), oFolder_+TString("out-wh-pythia-m80.root"),keys_  ); 
  // For 90 must add url keys. Still, useless w/out hh, which does not exist.
  //url_ = WH90_URL;  process(false, url_, iFolder_ + TString("wh-pythia-m90.root"), oFolder_+TString("out-wh-pythia-m90.root"),keys_  ); 
  url_ = WH100_URL; process(false, url_, iFolder_ + TString("wh-pythia-m100.root"), oFolder_+TString("out-wh-pythia-m100.root"),keys_); 
  url_ = WH120_URL; process(false, url_, iFolder_ + TString("wh-pythia-m120.root"), oFolder_+TString("out-wh-pythia-m120.root"),keys_); 
  url_ = WH140_URL; process(false, url_, iFolder_ + TString("wh-pythia-m140.root"), oFolder_+TString("out-wh-pythia-m140.root"),keys_); 
  url_ = WH150_URL; process(false, url_, iFolder_ + TString("wh-pythia-m150.root"), oFolder_+TString("out-wh-pythia-m150.root"),keys_); 
  url_ = WH155_URL; process(false, url_, iFolder_ + TString("wh-pythia-m155.root"), oFolder_+TString("out-wh-pythia-m155.root"),keys_); 
  url_ = WH160_URL; process(false, url_, iFolder_ + TString("wh-pythia-m160.root"), oFolder_+TString("out-wh-pythia-m160.root"),keys_); 


}


void SampleProcessor::process_wh_higgs_bychannel(){

//  url_ = WH80_URL;  process(false, url_, iFolder_ + TString("wh-pythia-m80.root"),  oFolder_+TString("out-wh-pythia-m80_etau.root"),keys_,  ETAU_); 
//  url_ = WH100_URL; process(false, url_, iFolder_ + TString("wh-pythia-m100.root"), oFolder_+TString("out-wh-pythia-m100_etau.root"),keys_, ETAU_); 
//  url_ = WH120_URL; process(false, url_, iFolder_ + TString("wh-pythia-m120.root"), oFolder_+TString("out-wh-pythia-m120_etau.root"),keys_, ETAU_); 
//  url_ = WH140_URL; process(false, url_, iFolder_ + TString("wh-pythia-m140.root"), oFolder_+TString("out-wh-pythia-m140_etau.root"),keys_, ETAU_); 
//
//  url_ = WH150_URL; process(false, url_, iFolder_ + TString("wh-pythia-m150.root"), oFolder_+TString("out-wh-pythia-m150_etau.root"),keys_, ETAU_); 
//  url_ = WH155_URL; process(false, url_, iFolder_ + TString("wh-pythia-m155.root"), oFolder_+TString("out-wh-pythia-m155_etau.root"),keys_, ETAU_); 
//  url_ = WH160_URL; process(false, url_, iFolder_ + TString("wh-pythia-m160.root"), oFolder_+TString("out-wh-pythia-m160_etau.root"),keys_, ETAU_); 

  url_ = WH80_URL;  process(false, url_, iFolder_ + TString("wh-pythia-m80.root"),  oFolder_+TString("out-wh-pythia-m80_mutau.root"),keys_,  MUTAU_); 
  url_ = WH100_URL; process(false, url_, iFolder_ + TString("wh-pythia-m100.root"), oFolder_+TString("out-wh-pythia-m100_mutau.root"),keys_, MUTAU_); 
  url_ = WH120_URL; process(false, url_, iFolder_ + TString("wh-pythia-m120.root"), oFolder_+TString("out-wh-pythia-m120_mutau.root"),keys_, MUTAU_); 
  url_ = WH140_URL; process(false, url_, iFolder_ + TString("wh-pythia-m140.root"), oFolder_+TString("out-wh-pythia-m140_mutau.root"),keys_, MUTAU_); 
  url_ = WH150_URL; process(false, url_, iFolder_ + TString("wh-pythia-m150.root"), oFolder_+TString("out-wh-pythia-m150_mutau.root"),keys_, MUTAU_); 
  url_ = WH155_URL; process(false, url_, iFolder_ + TString("wh-pythia-m155.root"), oFolder_+TString("out-wh-pythia-m155_mutau.root"),keys_, MUTAU_); 
  url_ = WH160_URL; process(false, url_, iFolder_ + TString("wh-pythia-m160.root"), oFolder_+TString("out-wh-pythia-m160_mutau.root"),keys_, MUTAU_); 
  

}


void SampleProcessor::process_tbh_higgs_bychannel(){
  url_ = TBH180_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("tbh-pythia-m180.root"), oFolder_+TString("out-tbh-pythia-m180_mutau.root"),keys_, MUTAU_); 
  url_ = TBH190_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("tbh-pythia-m190.root"), oFolder_+TString("out-tbh-pythia-m190_mutau.root"),keys_, MUTAU_); 
  url_ = TBH200_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("tbh-pythia-m200.root"), oFolder_+TString("out-tbh-pythia-m200_mutau.root"),keys_, MUTAU_); 
  url_ = TBH220_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("tbh-pythia-m220.root"), oFolder_+TString("out-tbh-pythia-m220_mutau.root"),keys_, MUTAU_); 
  url_ = TBH250_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("tbh-pythia-m250.root"), oFolder_+TString("out-tbh-pythia-m250_mutau.root"),keys_, MUTAU_); 
  url_ = TBH300_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("tbh-pythia-m300.root"), oFolder_+TString("out-tbh-pythia-m300_mutau.root"),keys_, MUTAU_); 
  url_ = TBH400_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("tbh-pythia-m400.root"), oFolder_+TString("out-tbh-pythia-m400_mutau.root"),keys_, MUTAU_); 
  url_ = TBH500_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("tbh-pythia-m500.root"), oFolder_+TString("out-tbh-pythia-m500_mutau.root"),keys_, MUTAU_); 
  url_ = TBH600_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("tbh-pythia-m600.root"), oFolder_+TString("out-tbh-pythia-m600_mutau.root"),keys_, MUTAU_); 
  
}


void SampleProcessor::process_tbh_higgs(){
  url_ = TBH180_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("tbh-pythia-m180.root"), oFolder_+TString("out-tbh-pythia-m180.root"),keys_); 
  url_ = TBH190_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("tbh-pythia-m190.root"), oFolder_+TString("out-tbh-pythia-m190.root"),keys_); 
  url_ = TBH200_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("tbh-pythia-m200.root"), oFolder_+TString("out-tbh-pythia-m200.root"),keys_); 
  url_ = TBH220_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("tbh-pythia-m220.root"), oFolder_+TString("out-tbh-pythia-m220.root"),keys_); 
  url_ = TBH250_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("tbh-pythia-m250.root"), oFolder_+TString("out-tbh-pythia-m250.root"),keys_); 
  url_ = TBH300_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("tbh-pythia-m300.root"), oFolder_+TString("out-tbh-pythia-m300.root"),keys_); 
  url_ = TBH400_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("tbh-pythia-m400.root"), oFolder_+TString("out-tbh-pythia-m400.root"),keys_); 
  url_ = TBH500_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("tbh-pythia-m500.root"), oFolder_+TString("out-tbh-pythia-m500.root"),keys_); 
  url_ = TBH600_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("tbh-pythia-m600.root"), oFolder_+TString("out-tbh-pythia-m600.root"),keys_); 
}


void SampleProcessor::process_htb_higgs_bychannel(int sample, int i){

  if(i>30){
    cout << "ERROR CODE - must be in the range [0,29]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  switch(sample){
  case 180:
    url_ = HTB180_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m180_"+idx+".root"), oFolder_+TString("out-htb-pythia-m180-mutau_"+idx+".root"),keys_, MUTAU_); 
    break;
  case 200:
    url_ = HTB200_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m200_"+idx+".root"), oFolder_+TString("out-htb-pythia-m200-mutau_"+idx+".root"),keys_, MUTAU_); 
    break;
  case 220:
    url_ = HTB220_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m220_"+idx+".root"), oFolder_+TString("out-htb-pythia-m220-mutau_"+idx+".root"),keys_, MUTAU_); 
    break;
  case 240:
    url_ = HTB240_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m240_"+idx+".root"), oFolder_+TString("out-htb-pythia-m240-mutau_"+idx+".root"),keys_, MUTAU_); 
    break;
  case 250:
    url_ = HTB250_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m250_"+idx+".root"), oFolder_+TString("out-htb-pythia-m250-mutau_"+idx+".root"),keys_, MUTAU_); 
    break;
  case 260:
    url_ = HTB260_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m260_"+idx+".root"), oFolder_+TString("out-htb-pythia-m260-mutau_"+idx+".root"),keys_, MUTAU_); 
    break;
  case 280:
    url_ = HTB280_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m280_"+idx+".root"), oFolder_+TString("out-htb-pythia-m280-mutau_"+idx+".root"),keys_, MUTAU_); 
    break;
  case 300:
    url_ = HTB300_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m300_"+idx+".root"), oFolder_+TString("out-htb-pythia-m300-mutau_"+idx+".root"),keys_, MUTAU_); 
    break;
  case 350:
    url_ = HTB350_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m350_"+idx+".root"), oFolder_+TString("out-htb-pythia-m350-mutau_"+idx+".root"),keys_, MUTAU_); 
    break;
  case 400:
    url_ = HTB400_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m400_"+idx+".root"), oFolder_+TString("out-htb-pythia-m400-mutau_"+idx+".root"),keys_, MUTAU_); 
    break;
  case 500:
    url_ = HTB500_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m500_"+idx+".root"), oFolder_+TString("out-htb-pythia-m500-mutau_"+idx+".root"),keys_, MUTAU_); 
    break;
  case 600:
    url_ = HTB600_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m600_"+idx+".root"), oFolder_+TString("out-htb-pythia-m600-mutau_"+idx+".root"),keys_, MUTAU_); 
    break;
  case 700:
    url_ = HTB700_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m700_"+idx+".root"), oFolder_+TString("out-htb-pythia-m700-mutau_"+idx+".root"),keys_, MUTAU_); 
    break;
  default:
    cout << "ERROR CODE - must be among [180,200,220,240,250,260,280,300,350,400,500,600,700]" << endl;
    break;
  }
}


void SampleProcessor::process_htb_higgs(int sample, int i){
  
  if(i>30){
    cout << "ERROR CODE - must be in the range [0,29]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();
  
  switch(sample){
  case 180:
    url_ = HTB180_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m180_"+idx+".root"), oFolder_+TString("out-htb-pythia-m180_"+idx+".root"),keys_); 
    break;
  case 200:
    url_ = HTB200_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m200_"+idx+".root"), oFolder_+TString("out-htb-pythia-m200_"+idx+".root"),keys_); 
    break;
  case 220:
    url_ = HTB220_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m220_"+idx+".root"), oFolder_+TString("out-htb-pythia-m220_"+idx+".root"),keys_); 
    break;
  case 240:
    url_ = HTB240_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m240_"+idx+".root"), oFolder_+TString("out-htb-pythia-m240_"+idx+".root"),keys_); 
    break;
  case 250:
    url_ = HTB250_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m250_"+idx+".root"), oFolder_+TString("out-htb-pythia-m250_"+idx+".root"),keys_);
    break;
  case 260:
    url_ = HTB260_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m260_"+idx+".root"), oFolder_+TString("out-htb-pythia-m260_"+idx+".root"),keys_); 
    break;
  case 280:
    url_ = HTB280_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m280_"+idx+".root"), oFolder_+TString("out-htb-pythia-m280_"+idx+".root"),keys_);  
    break;
  case 300:
    url_ = HTB300_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m300_"+idx+".root"), oFolder_+TString("out-htb-pythia-m300_"+idx+".root"),keys_); 
    break;
  case 350:
    url_ = HTB350_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m350_"+idx+".root"), oFolder_+TString("out-htb-pythia-m350_"+idx+".root"),keys_); 
    break;
  case 400:
    url_ = HTB400_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m400_"+idx+".root"), oFolder_+TString("out-htb-pythia-m400_"+idx+".root"),keys_); 
    break;
  case 500:
    url_ = HTB500_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m500_"+idx+".root"), oFolder_+TString("out-htb-pythia-m500_"+idx+".root"),keys_); 
    break;
  case 600:
    url_ = HTB600_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m600_"+idx+".root"), oFolder_+TString("out-htb-pythia-m600_"+idx+".root"),keys_); 
    break;
  case 700:
    url_ = HTB700_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("htb-pythia-m700_"+idx+".root"), oFolder_+TString("out-htb-pythia-m700_"+idx+".root"),keys_); 
    break;
  default:
    cout << "ERROR CODE - must be among [180,200,220,240,250,260,280,300,350,400,500,600,700]" << endl;
    break;
  }
}


void SampleProcessor::process_qcd(int sample, int i){

  if(i>100){
    cout << "ERROR CODE - must be in the range [0,49]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();
  
  switch(sample){
  case 1:
    url_= QCD_EM30TO80_URL;     process(false,url_, iFolder_ + TString("qcd_EM_Pt30to80_"+idx+".root"),     oFolder_ + TString("out-qcd_EM_Pt30to80_"+idx+".root"),keys_); 
    break;
  case 2:
    url_= QCD_EM80TO170_URL;    process(false,url_, iFolder_ + TString("qcd_EM_Pt80to170_"+idx+".root"),    oFolder_ + TString("out-qcd_EM_Pt80to170_"+idx+".root"),keys_);    
    break;
  case 3:
    url_= QCD_BCTOE30TO80_URL;  process(false,url_, iFolder_ + TString("qcd_30to80_BCtoE_"+idx+".root"),    oFolder_ + TString("out-qcd_30to80_BCtoE_"+idx+".root"),keys_);  
    break;
  case 4:
    url_= QCD_BCTOE80TO170_URL; process(false,url_, iFolder_ + TString("qcd_80to170_BCtoE_"+idx+".root"),   oFolder_ + TString("out-qcd_80to170_BCtoE_"+idx+".root"),keys_);  
    break;
  case 5:
    url_= QCD_PHOTON30TO50_URL;   process(false,url_, iFolder_ + TString("PhotonJets_30to50_"+idx+".root"),   oFolder_ + TString("out-PhotonJets_30to50_"+idx+".root"),keys_);  
    break;
  case 6:
    url_= QCD_PHOTON50TO80_URL;   process(false,url_, iFolder_ + TString("PhotonJets_50to80_"+idx+".root"),   oFolder_ + TString("out-PhotonJets_50to80_"+idx+".root"),keys_);  
    break;
  case 7:
    url_= QCD_PHOTON80TO120_URL;  process(false,url_, iFolder_ + TString("PhotonJets_80to120_"+idx+".root"),  oFolder_ + TString("out-PhotonJets_80to120_"+idx+".root"),keys_); 
    break;
  case 8:
    url_= QCD_PHOTON120TO170_URL; process(false,url_, iFolder_ + TString("PhotonJets_120to170_"+idx+".root"), oFolder_ + TString("out-PhotonJets_120to170_"+idx+".root"),keys_); 
    break;
  case 9:
    // mu + jets specific samples
    url_= QCD_MU_URL; process(false, url_, iFolder_ + TString("qcdmu15_20toinf_"+idx+".root"), oFolder_ + TString("out-qcdmu15_20toinf_"+idx+".root"),keys_);   
    break;
  case 10:
    url_= QCD_EM20TO30_URL;     process(false,url_, iFolder_ + TString("qcd_EM_Pt20to30_"+idx+".root"),     oFolder_ + TString("out-qcd_EM_Pt20to30_"+idx+".root"),keys_); 
    break;
  case 11:
    url_= QCD_EM170TO250_URL;     process(false,url_, iFolder_ + TString("qcd_EM_Pt170to250_"+idx+".root"),     oFolder_ + TString("out-qcd_EM_Pt170to250_"+idx+".root"),keys_); 
    break;
  case 12:
    url_= QCD_EM250TO350_URL;     process(false,url_, iFolder_ + TString("qcd_EM_Pt250to350_"+idx+".root"),     oFolder_ + TString("out-qcd_EM_Pt250to350_"+idx+".root"),keys_);
    break;
  case 13:
    url_= QCD_EM350TOINF_URL;     process(false,url_, iFolder_ + TString("qcd_EM_Pt350toinf_"+idx+".root"),     oFolder_ + TString("out-qcd_EM_Pt350toinf_"+idx+".root"),keys_); 
    break;
  case 14:
    url_= QCD_BCTOE20TO30_URL;  process(false,url_, iFolder_ + TString("qcd_20to30_BCtoE.root"),    oFolder_ + TString("out-qcd_20to30_BCtoE.root"),keys_);  
    break;
  case 15:
    url_= QCD_BCTOE170TO250_URL;  process(false,url_, iFolder_ + TString("qcd_170to250_BCtoE_"+idx+".root"),    oFolder_ + TString("out-qcd_170to250_BCtoE_"+idx+".root"),keys_);  
    break;
  case 16:
    url_= QCD_BCTOE250TO350_URL;  process(false,url_, iFolder_ + TString("qcd_250to350_BCtoE_"+idx+".root"),    oFolder_ + TString("out-qcd_250to350_BCtoE_"+idx+".root"),keys_);  
    break;
  case 17:
    url_= QCD_BCTOE350TOINF_URL;  process(false,url_, iFolder_ + TString("qcd_350toinf_BCtoE_"+idx+".root"),    oFolder_ + TString("out-qcd_350toinf_BCtoE_"+idx+".root"),keys_);  
    break;
  case 18:
    url_= QCD_PHOTON15TO30_URL;   process(false,url_, iFolder_ + TString("PhotonJets_15to30_"+idx+".root"),   oFolder_ + TString("out-PhotonJets_15to30_"+idx+".root"),keys_);  
    break;
  case 19:
    url_= QCD_PHOTON170TO300_URL;   process(false,url_, iFolder_ + TString("PhotonJets_170to300_"+idx+".root"),   oFolder_ + TString("out-PhotonJets_170to300_"+idx+".root"),keys_);  
    break;
  case 20:
    url_= QCD_PHOTON300TO470_URL;   process(false,url_, iFolder_ + TString("PhotonJets_300to470_"+idx+".root"),   oFolder_ + TString("out-PhotonJets_300to470_"+idx+".root"),keys_);  
    break;
  default:
    cout << "No Sample" << endl;
    break;
  }
  
}


void SampleProcessor::process_diboson(int sample, int i){

  if(i>49){
    cout << "ERROR CODE - must be in the range [0,49]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();
  
  switch(sample){
  case 0:
    url_= WW_URL; process(false, url_, iFolder_ + TString("WW_"+idx+".root"), oFolder_+TString("out-ww_"+idx+".root"),keys_); 
    break;
  case 1:
    url_= WZ_URL; process(false, url_, iFolder_ + TString("WZ_"+idx+".root"), oFolder_+TString("out-wz_"+idx+".root"),keys_);  
    break;
  case 2:
    url_= ZZ_URL; process(false, url_, iFolder_ + TString("ZZ_"+idx+".root"), oFolder_+TString("out-zz_"+idx+".root"),keys_);
    break;
  default:
    cout << "No Sample" << endl;
    break;
  }
  
}


void SampleProcessor::process_embeddedData(){
  url_ = EMBEDDED_DATA_URL;
  
  if( !eChONmuChOFF_ && run2012_)
    process(true,url_,TString("/exper-sw/cmst3/cmssw/users/vischia/chiggs/CMSSW_5_3_9/src/lipcms/EventSelection/test/Monitor_data_Embedded.root"),oDFolder_+TString("out-embeddedData.root"), keys_);

}

void SampleProcessor::process_data_RunA(int i){

  url_ = DATA_URL;
  if(i>49){
    cout << "ERROR CODE - must be in the range [0,49]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  if( ! eChONmuChOFF_ && run2012_) 
    process(true,url_,iDFolder_+TString("Muon_RunA_"+idx+".root"),oDFolder_+TString("out-Muon_RunA_"+idx+".root"),keys_);
  else if( eChONmuChOFF_ && run2012_)
    {
      if(i>9) return;
      process(true,url_,iDFolder_+TString("Electron_RunA_"+idx+".root"),oDFolder_+TString("out-Electron_RunA_"+idx+".root"),keys_);
    }
}



void SampleProcessor::process_data_RunB(int i){

  url_ = DATA_URL;
  
  if(i>49){
    cout << "ERROR CODE - must be in the range [0,49]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  // DON't FORGET TO SET MUTAU ON
  //if( ! eChONmuChOFF_ && ! run2012_){ 
  //  //
  //}
  
  
  if( ! eChONmuChOFF_ && run2012_)
    process(true,url_,iDFolder_+TString("Muon_RunB_"+idx+".root"),oDFolder_+TString("out-Muon_RunB_"+idx+".root"),keys_);
  else if( eChONmuChOFF_ && run2012_)
    {
      if(i>39) return;
      process(true,url_,iDFolder_+TString("Electron_RunB_"+idx+".root"),oDFolder_+TString("out-Electron_RunB_"+idx+".root"),keys_);
    }
  
  
  /*
    else{ 
    // inclusive electron trigger
    process(true,url_,iDFolder_+TString("Electron_1.root"), oDFolder_+TString("out-Electron_1.root"),keys_);
    }
  */
  /*
    else if(! eChONmuChOFF_ && ! run2012_ ) {
    electron+2jets+mht samples
    process(true,url_,iDFolder_+TString("Ele_Met_eJTrigMatch_4.root"), oDFolder_+TString("out-eJTrigMatch_4.root"),keys_);
    }
  */  
}



void SampleProcessor::process_data_RunC1(int i){

  url_ = DATA_URL;

  if(i>49){
    cout << "ERROR CODE - must be in the range [0,49]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  // DON't FORGET TO SET MUTAU ON
  //if( ! eChONmuChOFF_ && ! run2012_){ 
  //  //
  //}
  
  
  if( ! eChONmuChOFF_ && run2012_)
    process(true,url_,iDFolder_+TString("Muon_RunC_1_"+idx+".root"),oDFolder_+TString("out-Muon_RunC_1_"+idx+".root"),keys_);
  else if( eChONmuChOFF_ && run2012_)
    {
      if(i>29) return;
      process(true,url_,iDFolder_+TString("Electron_RunC_leg1_"+idx+".root"),oDFolder_+TString("out-Electron_RunC_leg1_"+idx+".root"),keys_);
    }
  


}



void SampleProcessor::process_data_RunC2(int i){

  url_ = DATA_URL;

  if(i>49){
    cout << "ERROR CODE - must be in the range [0,49]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  // DON't FORGET TO SET MUTAU ON
  //if( ! eChONmuChOFF_ && ! run2012_){ 
  //  //
  //}
  
  
  if( ! eChONmuChOFF_ && run2012_)
    process(true,url_,iDFolder_+TString("Muon_RunC_2_"+idx+".root"),oDFolder_+TString("out-Muon_RunC_2_"+idx+".root"),keys_);
  else if( eChONmuChOFF_ && run2012_)
    {
      if(i>29) return;
      process(true,url_,iDFolder_+TString("Electron_RunC_leg2_"+idx+".root"),oDFolder_+TString("out-Electron_RunC_leg2_"+idx+".root"),keys_);
    }
  


}



void SampleProcessor::process_data_RunD1(int i){

  url_ = DATA_URL;

  if(i>49){
    cout << "ERROR CODE - must be in the range [0,49]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  // DON't FORGET TO SET MUTAU ON
  //if( ! eChONmuChOFF_ && ! run2012_){ 
  //  //
  //}
  
  
  if( ! eChONmuChOFF_ && run2012_)
    process(true,url_,iDFolder_+TString("Muon_RunD_1_"+idx+".root"),oDFolder_+TString("out-Muon_RunD_1_"+idx+".root"),keys_);
  else if( eChONmuChOFF_ && run2012_)
    {
      if(i>29) return;
      process(true,url_,iDFolder_+TString("Electron_RunD_leg1_"+idx+".root"),oDFolder_+TString("out-Electron_RunD_leg1_"+idx+".root"),keys_);
    }
  

}





void SampleProcessor::process_data_RunD2(int i){

  url_ = DATA_URL;

  if(i>49){
    cout << "ERROR CODE - must be in the range [0,49]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  // DON't FORGET TO SET MUTAU ON
  //if( ! eChONmuChOFF_ && ! run2012_){ 
  //  //
  //}
  
  
  if( ! eChONmuChOFF_ && run2012_)
    process(true,url_,iDFolder_+TString("Muon_RunD_2_"+idx+".root"),oDFolder_+TString("out-Muon_RunD_2_"+idx+".root"),keys_);
  else if( eChONmuChOFF_ && run2012_)
    {
      if(i>29) return;
      process(true,url_,iDFolder_+TString("Electron_RunD_leg2_"+idx+".root"),oDFolder_+TString("out-Electron_RunD_leg2_"+idx+".root"),keys_);
    }
  

}






void SampleProcessor::process_data_MissingRunB(int i){

  url_ = DATA_URL;
  if(i>49){
    cout << "ERROR CODE - must be in the range [0,49]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  if( ! eChONmuChOFF_ && run2012_) 
    process(true,url_,iDFolder_+TString("MuonMissing_RunB_"+idx+".root"),oDFolder_+TString("out-MuonMissing_RunB_"+idx+".root"),keys_);
  else if( eChONmuChOFF_ && run2012_)
    {
      if(i>9) return;
      process(true,url_,iDFolder_+TString("Electron_MisB_"+idx+".root"),oDFolder_+TString("out-Electron_MisB_"+idx+".root"),keys_);
    }

}
void SampleProcessor::process_data_MissingRunC1(int i){

  url_ = DATA_URL;
  if(i>49){
    cout << "ERROR CODE - must be in the range [0,49]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  if( ! eChONmuChOFF_ && run2012_) 
    process(true,url_,iDFolder_+TString("MuonMissing_RunC_1_"+idx+".root"),oDFolder_+TString("out-MuonMissing_RunC_1_"+idx+".root"),keys_);
  else if (eChONmuChOFF_ && run2012_){
    if(i>19) return;
    process(true,url_,iDFolder_+TString("Electron_RunC_mis1_"+idx+".root"),oDFolder_+TString("out-Electron_RunC_mis1_"+idx+".root"),keys_);
  }
}

void SampleProcessor::process_data_MissingRunC2(int i){

  url_ = DATA_URL;
  if(i>49){
    cout << "ERROR CODE - must be in the range [0,49]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  if( ! eChONmuChOFF_ && run2012_) 
    process(true,url_,iDFolder_+TString("MuonMissing_RunC_2_"+idx+".root"),oDFolder_+TString("out-MuonMissing_RunC_2_"+idx+".root"),keys_);
  else if (eChONmuChOFF_ && run2012_){
    if(i>19) return;
    process(true,url_,iDFolder_+TString("Electron_RunC_mis2_"+idx+".root"),oDFolder_+TString("out-Electron_RunC_mis2_"+idx+".root"),keys_);
  }

}

void SampleProcessor::process_data_MissingRunD1(int i){

  url_ = DATA_URL;
  if(i>49){
    cout << "ERROR CODE - must be in the range [0,49]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  if( ! eChONmuChOFF_ && run2012_) 
    process(true,url_,iDFolder_+TString("MuonMissing_RunD_1_"+idx+".root"),oDFolder_+TString("out-MuonMissing_RunD_1_"+idx+".root"),keys_);

}

void SampleProcessor::process_data_MissingRunD2(int i){

  url_ = DATA_URL;
  if(i>49){
    cout << "ERROR CODE - must be in the range [0,49]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  if( ! eChONmuChOFF_ && run2012_) 
    process(true,url_,iDFolder_+TString("MuonMissing_RunD_2_"+idx+".root"),oDFolder_+TString("out-MuonMissing_RunD_2_"+idx+".root"),keys_);
  else if (eChONmuChOFF_ && run2012_){
    if(i>19) return;
    process(true,url_,iDFolder_+TString("Electron_RunD_mis2_"+idx+".root"),oDFolder_+TString("out-Electron_RunD_mis2_"+idx+".root"),keys_);
  }

}


void SampleProcessor::process_data_EmbeddedRunA(int i){

  url_ = EMBEDDED_DATA_URL;
  if(i>49){
    cout << "ERROR CODE - must be in the range [0,49]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();


  if( ! eChONmuChOFF_ && run2012_) 
    process(true,url_,TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/tau_dilepton/goodEmbedYourAss/out/Embedded_RunA_"+idx+".root"),oDFolder_+TString("out-Embedded_RunA_"+idx+".root"),keys_);

}

void SampleProcessor::process_data_EmbeddedRunB(int i){

  url_ = EMBEDDED_DATA_URL;
  if(i>49){
    cout << "ERROR CODE - must be in the range [0,49]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  if( ! eChONmuChOFF_ && run2012_) 
    process(true,url_,TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/tau_dilepton/goodEmbedYourAss/out/Embedded_RunB_"+idx+".root"),oDFolder_+TString("out-Embedded_RunB_"+idx+".root"),keys_);

}

void SampleProcessor::process_data_EmbeddedRunC1(int i){

  url_ = EMBEDDED_DATA_URL;
  if(i>49){
    cout << "ERROR CODE - must be in the range [0,49]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  if( ! eChONmuChOFF_ && run2012_) 
    process(true,url_,TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/tau_dilepton/goodEmbedYourAss/out/Embedded_RunC_1_"+idx+".root"),oDFolder_+TString("out-Embedded_RunC_1_"+idx+".root"),keys_);

}

void SampleProcessor::process_data_EmbeddedRunC2(int i){

  url_ = EMBEDDED_DATA_URL;
  if(i>49){
    cout << "ERROR CODE - must be in the range [0,49]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  if( ! eChONmuChOFF_ && run2012_) 
    process(true,url_,TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/tau_dilepton/goodEmbedYourAss/out/Embedded_RunC_2_"+idx+".root"),oDFolder_+TString("out-Embedded_RunC_2_"+idx+".root"),keys_);
}

void SampleProcessor::process_data_EmbeddedRunD1(int i){

  url_ = EMBEDDED_DATA_URL;
  if(i>49){
    cout << "ERROR CODE - must be in the range [0,49]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  if( ! eChONmuChOFF_ && run2012_) 
    process(true,url_,TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/tau_dilepton/goodEmbedYourAss/out/Embedded_RunD_1_"+idx+".root"),oDFolder_+TString("out-Embedded_RunD_1_"+idx+".root"),keys_);

}

void SampleProcessor::process_data_EmbeddedRunD2(int i){

  url_ = EMBEDDED_DATA_URL;
  if(i>49){
    cout << "ERROR CODE - must be in the range [0,49]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();

  if( ! eChONmuChOFF_ && run2012_) 
    process(true,url_,TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/tau_dilepton/goodEmbedYourAss/out/Embedded_RunD_2_"+idx+".root"),oDFolder_+TString("out-Muon_RunD_2_"+idx+".root"),keys_);

}




void SampleProcessor::process_hhhtautaubb_higgs(int sample, int i){
 
  if(i>30){
    cout << "ERROR CODE - must be in the range [0,29]" << endl;
    return;
  }
  
  stringstream sidx;
  sidx<<i;
  string idx=sidx.str();
  
  switch(sample){
  case 260:
    url_ = HHHTAUTAUBB260_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("hhhtautaubb-pythia-m260_"+idx+".root"), oFolder_+TString("out-hhhtautaubb-pythia-m260_"+idx+".root"),keys_); 
    break;
  case 300:
    url_ = HHHTAUTAUBB300_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("hhhtautaubb-pythia-m300_"+idx+".root"), oFolder_+TString("out-hhhtautaubb-pythia-m300_"+idx+".root"),keys_); 
    break;
  case 350:
    url_ = HHHTAUTAUBB350_URL; process(false, url_, TString("/lustre/ncg.ingrid.pt/cmslocal/vischia/ttbarDileptonAnalysis/topMassAnalysis/tDilbh/") + TString("hhhtautaubb-pythia-m350_"+idx+".root"), oFolder_+TString("out-hhhtautaubb-pythia-m350_"+idx+".root"),keys_); 
    break;
  default:
    cout << "ERROR CODE - must be among [260,300,350]" << endl;
    break;
  }
}





























void SampleProcessor::process_triggersamples_mht(){

  url_ = DATA_URL;

  iDFolder_=TString("/lustre/data3/cmslocal/samples/CMSSW_4_2_X/data/mTrees-v3/");


  process(true,url_,iDFolder_+TString("Ele_MetTrig_4.root"),oDFolder_+TString("out-Ele_MetTrig_4.root"),keys_); 
  process(true,url_,iDFolder_+TString("Ele_MetTrig_5.root"),oDFolder_+TString("out-Ele_MetTrig_5.root"),keys_); 
  process(true,url_,iDFolder_+TString("Ele_MetTrig_6.root"),oDFolder_+TString("out-Ele_MetTrig_6.root"),keys_);
  process(true,url_,iDFolder_+TString("Ele_MetTrig_7.root"),oDFolder_+TString("out-Ele_MetTrig_7.root"),keys_); 
  process(true,url_,iDFolder_+TString("Ele_MetTrig_8.root"),oDFolder_+TString("out-Ele_MetTrig_8.root"),keys_);
  process(true,url_,iDFolder_+TString("Ele_MetTrig_9.root"),oDFolder_+TString("out-Ele_MetTrig_9.root"),keys_);
  process(true,url_,iDFolder_+TString("Ele_MetTrig_10.root"),oDFolder_+TString("out-Ele_MetTrig_10.root"),keys_);




}


void SampleProcessor::process_triggersamples_mhtjets(){

  url_ = DATA_URL;

  iDFolder_=TString("/lustre/data3/cmslocal/samples/CMSSW_4_2_X/data/mTrees-v3-B/");
/*
  process(true,url_,iDFolder_+TString("Ele_JetMetTrig_4.root"),oDFolder_+TString("out-Ele_JetMetTrig_4.root"),keys_); 
  process(true,url_,iDFolder_+TString("Ele_JetMetTrig_5.root"),oDFolder_+TString("out-Ele_JetMetTrig_5.root"),keys_); 
  process(true,url_,iDFolder_+TString("Ele_JetMetTrig_6.root"),oDFolder_+TString("out-Ele_JetMetTrig_6.root"),keys_);
  process(true,url_,iDFolder_+TString("Ele_JetMetTrig_7.root"),oDFolder_+TString("out-Ele_JetMetTrig_7.root"),keys_); 
  process(true,url_,iDFolder_+TString("Ele_JetMetTrig_8.root"),oDFolder_+TString("out-Ele_JetMetTrig_8.root"),keys_); 
  process(true,url_,iDFolder_+TString("Ele_JetMetTrig_9.root"),oDFolder_+TString("out-Ele_JetMetTrig_9.root"),keys_);
  process(true,url_,iDFolder_+TString("Ele_JetMetTrig_10.root"),oDFolder_+TString("out-Ele_JetMetTrig_10.root"),keys_);
*/

  process(true,url_,iDFolder_+TString("Ele_JetMetTrig_matched_4.root"),oDFolder_+TString("out-Ele_JetMetTrig_matched_4.root"),keys_); 
  process(true,url_,iDFolder_+TString("Ele_JetMetTrig_matched_5.root"),oDFolder_+TString("out-Ele_JetMetTrig_matched_5.root"),keys_); 
  process(true,url_,iDFolder_+TString("Ele_JetMetTrig_matched_6.root"),oDFolder_+TString("out-Ele_JetMetTrig_matched_6.root"),keys_);
  process(true,url_,iDFolder_+TString("Ele_JetMetTrig_matched_7.root"),oDFolder_+TString("out-Ele_JetMetTrig_matched_7.root"),keys_); 
  process(true,url_,iDFolder_+TString("Ele_JetMetTrig_matched_8.root"),oDFolder_+TString("out-Ele_JetMetTrig_matched_8.root"),keys_); 
  process(true,url_,iDFolder_+TString("Ele_JetMetTrig_matched_9.root"),oDFolder_+TString("out-Ele_JetMetTrig_matched_9.root"),keys_);
  process(true,url_,iDFolder_+TString("Ele_JetMetTrig_matched_10.root"),oDFolder_+TString("out-Ele_JetMetTrig_matched_10.root"),keys_);

}



//////////////////////// PROCESS SPY FILES ///////////////////////////

// FIXME: check input and output folders scheme


void SampleProcessor::process_spy_data(){

  url_ = DATA_URL;
  process(true,url_,iSpyFolder_+TString("spy_data.root"),oSpyFolder_+TString("out-data.root"),keys_);
    
}

void SampleProcessor::process_spy_misidentifiedTau(){

  url_ = DATA_URL; // THIS SHOULD COME FROM DATA_RESCALED, DAMMIT. IMPLEMENT WEIGHT FROM STUFF WITHIN THE OSANALYSIS CODE // DATA_RESCALED
  process(true,url_,iSpyFolder_+TString("spy_data.root"),oSpyFolder_+TString("out-data.root"),keys_);

}

void SampleProcessor::process_spy_dibosons(int sample){

  switch(sample){
  case 0:
    url_= WW_URL; process(false, url_, iSpyFolder_ + TString("spy_ww.root"), oSpyFolder_+TString("out-ww.root"),keys_); 
    break;
  case 1:
    url_= WZ_URL; process(false, url_, iSpyFolder_ + TString("spy_wz.root"), oSpyFolder_+TString("out-wz.root"),keys_);  
    break;
  case 2:
    url_= ZZ_URL; process(false, url_, iSpyFolder_ + TString("spy_zz.root"), oSpyFolder_+TString("out-zz.root"),keys_);
    break;
  default:
    cout << "No Sample, wrong samplecode chosen. Supported ones are: 0=ww, 1=wz, 2=zz" << endl;
    break;
  }
  
}

void SampleProcessor::process_spy_dyvv(){

  // FIXME: WHAT THE FUCK IS DYVV SUPPOSED TO MEAN?
  // MADGRAPH Z+JETS

  url_ = DY_FROM50_URL; // 10to50 FIXME

  process(false, url_, iSpyFolder_ + TString("spy_dy_from50.root"), oSpyFolder_+TString("out-dy_from50.root"),keys_); 

}


void SampleProcessor::process_spy_singletop(){

  // FIXME CROSSECTION

  //SINGLE TOP
  url_= S_URL; process(false, url_, iSpyFolder_ + TString("spy_stop_s.root"),     oSpyFolder_ + TString("out-stop_s.root"),keys_);   
  url_= T_URL; process(false, url_, iSpyFolder_ + TString("spy_stop_t.root"),     oSpyFolder_ + TString("out-stop_t.root"),keys_);  
  url_= W_URL; process(false, url_, iSpyFolder_ + TString("spy_stop_tW-DR.root"), oSpyFolder_ + TString("out-stop-DR_tW.root"),keys_); 
  
  //ANTI SINGLE TOP
  url_= A_S_URL; process(false, url_, iSpyFolder_ + TString("spy_stopbar_s.root"),     oSpyFolder_ + TString("out-stopbar_s.root"),keys_);   
  url_= A_W_URL; process(false, url_, iSpyFolder_ + TString("spy_stopbar_tW-DR.root"), oSpyFolder_ + TString("out-stopbar-DR_tW.root"),keys_);
  url_= A_T_URL; process(false, url_, iSpyFolder_ + TString("spy_stopbar_t.root"),     oSpyFolder_ + TString("out-stopbar_t.root"),keys_);  
}

void SampleProcessor::process_spy_ttbar_mcbkg(){

  url_= TTBAR_URL;
  process(false, url_, iSpyFolder_ + TString("spy_ttbar_mcbkg.root"), oSpyFolder_+TString("out-ttbar-mcbkg.root"),keys_, TTBAR_MCBKG_ );

}

void SampleProcessor::process_spy_ttbar_Xtau(){

  url_= TTBAR_URL;

  if(!eChONmuChOFF_) process(false, url_, iSpyFolder_ + TString("spy_ttbar_mutau.root"), oSpyFolder_+TString("out-ttbar-mutau.root"),keys_, MUTAU_ ); 
  else               process(false, url_, iSpyFolder_ + TString("spy_ttbar_etau.root"),  oSpyFolder_+TString("out-ttbar-etau.root"),keys_, ETAU_ ); 
}

void SampleProcessor::process_spy_zjets_from50(){

  // MADGRAPH Z+JETS

  url_ = DY_FROM50_URL; 

  process(false, url_, iSpyFolder_ + TString("spy_dy_from50,root"), oSpyFolder_+TString("out-dy_from50.root"),keys_); 

}

void SampleProcessor::process_spy_zjets_10to50(){
  
  // MADGRAPH Z+JETS
  url_ = DY_10TO50_URL;
  process(false, url_, iSpyFolder_ + TString("spy_dy_10_50.root"), oSpyFolder_+TString("out-dy_10to50.root"),keys_);    
}
