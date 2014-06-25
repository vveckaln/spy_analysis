#include "LIP/TopTaus/interface/HistogramBuilder.hh"


void HistogramBuilder::buildHistograms( 
  TFile * outFile, 
  map< TString, SelectionMonitor * >  & stepsMons , 
  map< TString, SelectionMonitor * >  & yieldsMons , 
  map< TString, SelectionMonitor * >  & mcMons, 
  map< TString, SelectionMonitor * >  & bkgMons,
  map< TString, SelectionMonitor * >  & debugMons,
  map<TString,TDirectory *> & dirsMaps , vector<TString> & keys ){

  
  for(uint i=0;i<keys.size();i++){
    TString key = keys[i]; 
    dirsMaps[key]        = outFile->mkdir(key); 

    stepsMons[key]       = new SelectionMonitor(); 
    yieldsMons[key]      = new SelectionMonitor();
    debugMons[key]       = new SelectionMonitor();
    mcMons[key]          = new SelectionMonitor();
    bkgMons[key]         = new SelectionMonitor();
    newPhysicsMons_[key] = new SelectionMonitor();

  }


  buildRecoStepHistograms(   outFile, stepsMons,  dirsMaps , keys );
  buildYieldsHistograms(     outFile, yieldsMons, dirsMaps , keys );
  buildDebugHistograms(      outFile, debugMons,  dirsMaps , keys );
  buildMCYieldsHistograms(   outFile, mcMons,     dirsMaps , keys );
  buildBkgHistograms(        outFile, bkgMons,    dirsMaps , keys );
  buildNewPhysicsHistograms( outFile, dirsMaps , keys );


}


void HistogramBuilder::buildBkgHistograms(   TFile * outFile, map< TString, SelectionMonitor * > & bkgMons    , map<TString,TDirectory *> & dirsMaps , vector<TString> & keys ){


  TH1D *h;

  vector<TString> wplusjetsPlots; vector<TString> wplusjetsPlots_extra;

  wplusjetsPlots.push_back("wplusjets_pt_gluon");
  wplusjetsPlots.push_back("wplusjets_pt_quark");
  wplusjetsPlots.push_back("wplusjets_pt_bquark");
  wplusjetsPlots.push_back("wplusjets_pt_allquark");
  wplusjetsPlots.push_back("wplusjets_pt_unknown");

  wplusjetsPlots.push_back("wplusjets_pt");
  wplusjetsPlots.push_back("wplusjets_eta");


  wplusjetsPlots.push_back("recow_pt");

  vector<TString> wplusjetsPlots2; 
  wplusjetsPlots2.push_back("wplusjets_ptR");
  wplusjetsPlots2.push_back("wplusjets_etaR");


  


/*
  for(uint i=0;i<wplusjetsPlots.size();i++){ 
    TString plus  = wplusjetsPlots[i] + TString("_plus");  wplusjetsPlots_extra.push_back(plus);
    TString minus = wplusjetsPlots[i] + TString("_minus"); wplusjetsPlots_extra.push_back(minus);
  }

  for(uint i=0;i<wplusjetsPlots2.size();i++){ 
    TString plus  = wplusjetsPlots2[i] + TString("_plus");  wplusjetsPlots2_extra.push_back(plus);
    TString minus = wplusjetsPlots2[i] + TString("_minus"); wplusjetsPlots2_extra.push_back(minus);
  }
*/

  for(uint i=0;i<wplusjetsPlots_extra.size();i++ ){  wplusjetsPlots.push_back(wplusjetsPlots_extra[i]); } 

  
  for(uint i=0;i<wplusjetsPlots.size();i++){
    TString legend; double min(0), max(0); int bins(0);
    if( (wplusjetsPlots[i]).Contains("pt") ){ legend = TString("j;jet p_{T} [GeV/c]; Entries"); min=0;     max=250;  bins=25; }
    else                                    { legend = TString("j;#eta; N_{jets} ");            min=-3.05; max=3.05; bins=15; }
    h = new TH1D(wplusjetsPlots[i],legend,bins,min,max); 
    addHistoToMonitors(h,h->GetName(), bkgMons,keys);
  }


  h = new TH1D("wjets_jmult",   "j;N_{jets}; N_{events}",10,0,10);   addHistoToMonitors(h,h->GetName(), bkgMons,keys); 
  h = new TH1D("wjets_wjmult",  "j;N_{jets}; N_{events}",10,0,10);   addHistoToMonitors(h,h->GetName(), bkgMons,keys); 
 

  for(uint i=0;i<wplusjetsPlots2.size();i++){
    TString legend; double min(0), max(0); int bins(0);
    if( (wplusjetsPlots2[i]).Contains("pt") ){ legend = TString("j;jet p_{T} [GeV/c]; R ; Entries"); min=0;   max=250;  bins=25;}
    else                                     { legend = TString("j;#eta; N_{jets} ; R ; Entries");   min=-3.05; max=3.05; bins=15; }
    TH2D * hx = new TH2D(wplusjetsPlots2[i],legend,bins,min,max,15,0,0.3);
    addHistoToMonitors(hx,hx->GetName(), bkgMons,keys);
  }


  for(uint i = 0; i<keys.size();i++){
    TString key = keys[i]; TDirectory * d_temp = dirsMaps[key]; 
    TDirectory * d  = d_temp->mkdir("Bkg");
    TString id1 = key + TString(" lep_tau bkg"); (bkgMons[key])->initMonitorForStep(id1); dirsMaps[id1] = d->mkdir("lep_tau"); 
    TString id2 = key + TString(" e_tau bkg");   (bkgMons[key])->initMonitorForStep(id2); dirsMaps[id2] = d->mkdir("e_tau");
    TString id3 = key + TString(" m_tau bkg");   (bkgMons[key])->initMonitorForStep(id3); dirsMaps[id3] = d->mkdir("m_tau");   
  }

  for(uint i=0;i<keys.size();i++){
    TString key = keys[i];
    SelectionMonitor::StepMonitor_t & allMonitors = (bkgMons[key])->getAllMonitors();
    for(SelectionMonitor::StepMonitor_t::iterator sIt = allMonitors.begin(); sIt != allMonitors.end(); sIt++){
      for(SelectionMonitor::Monitor_t::iterator hIt = sIt->second.begin(); hIt != sIt->second.end(); hIt++){     
        TDirectory *d =dirsMaps[sIt->first]; hIt->second->SetDirectory(d); 
      }
    }	
  }

}


void HistogramBuilder::buildDebugHistograms( TFile * outFile, map< TString, SelectionMonitor * > & debugMons , map<TString,TDirectory *> & dirsMaps , vector<TString> & keys ){

  TH1D * h;

  // Jet debug Histograms //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  h = new TH1D("jetChargedHadronEnergyFraction",            "j; E_{charged hadrons}/E; N_{Entries}",100,0,1);       addHistoToMonitors(h,h->GetName(), debugMons, keys);  
  h = new TH1D("jetNeutralHadronEnergyFraction",            "j; E_{neutral hadrons}/E; N_{Entries}",100,0,1);       addHistoToMonitors(h,h->GetName(), debugMons, keys);  
  h = new TH1D("jetNeutralHadronEnergyFractionIncludingHF", "j; (E_{neutral hadrons}+HF)/E; N_{Entries}",100,0,1);  addHistoToMonitors(h,h->GetName(), debugMons, keys);  
  h = new TH1D("jetChargedEmEnergyFraction",                "j; charged emf; N_{Entries}",100,0,1);                 addHistoToMonitors(h,h->GetName(), debugMons, keys);  
  h = new TH1D("jetNeutralEmEnergyFraction",                "j; neutral emf; N_{Entries}",100,0,1);                 addHistoToMonitors(h,h->GetName(), debugMons, keys);  
  h = new TH1D("jetChargedMultiplicity",                    "j; charge multiplicity; N_{Entries}",50,0,50);         addHistoToMonitors(h,h->GetName(), debugMons, keys);  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Jet debug Histograms ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  h = new TH1D("jetChargedHadronEnergyFractionMatched",            "j; E_{charged hadrons}/E; N_{Entries}",100,0,1);       addHistoToMonitors(h,h->GetName(), debugMons, keys);  
  h = new TH1D("jetNeutralHadronEnergyFractionMatched",            "j; E_{neutral hadrons}/E; N_{Entries}",100,0,1);       addHistoToMonitors(h,h->GetName(), debugMons, keys);  
  h = new TH1D("jetNeutralHadronEnergyFractionIncludingHFMatched", "j; (E_{neutral hadrons}+HF)/E; N_{Entries}",100,0,1);  addHistoToMonitors(h,h->GetName(), debugMons, keys);  
  h = new TH1D("jetChargedEmEnergyFractionMatched",                "j; charged emf; N_{Entries}",100,0,1);                 addHistoToMonitors(h,h->GetName(), debugMons, keys);  
  h = new TH1D("jetNeutralEmEnergyFractionMatched",                "j; neutral emf; N_{Entries}",100,0,1);                 addHistoToMonitors(h,h->GetName(), debugMons, keys);  
  h = new TH1D("jetChargedMultiplicityMatched",                    "j; charge multiplicity; N_{Entries}",50,0,50);         addHistoToMonitors(h,h->GetName(), debugMons, keys);  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  // lepton +jets trigger histograms //////////////////////////////////////////////////////////////////////////////////////////
  h = new TH1D("jmult_den",    "ljets trigger; jet multiplicity;N",4,3,7); addHistoToMonitors(h,h->GetName(), debugMons, keys); 
  h = new TH1D("jmult_num",    "ljets trigger; jet multiplicity;N",4,3,7); addHistoToMonitors(h,h->GetName(), debugMons, keys); 
  h = new TH1D("jmult_num_eta","ljets trigger; #eta;N",10,0,3.);        addHistoToMonitors(h,h->GetName(), debugMons, keys); 
  h = new TH1D("jmult_num_pt", "ljets trigger; pt;N",5 ,30,130);        addHistoToMonitors(h,h->GetName(), debugMons, keys); 
  h = new TH1D("jmult_den_eta","ljets trigger; #eta;N",10,0,3.);        addHistoToMonitors(h,h->GetName(), debugMons, keys); 
  h = new TH1D("jmult_den_pt", "ljets trigger; pt;N",5 ,30,130);        addHistoToMonitors(h,h->GetName(), debugMons, keys); 

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  //trigger efficiencies studies on data ////////////////////////////////////////////////////
  // TRIGGER on data electron(30GeV) + 2 Jets( 30 GeV) 
  // TRIGGER efficiency on electron(30GeV)+2Jets(30 GeV) + MHT
  h = new TH1D("den_eff_trigger_met","trigger; MET cut;N",145,5,150);            addHistoToMonitors(h,h->GetName(), debugMons, keys);
  h = new TH1D("num_eff_trigger_met","trigger; MET cut;N",145,5,150);            addHistoToMonitors(h,h->GetName(), debugMons, keys);  

  h = new TH1D("den_eff_trigger_met3j","trigger; MET cut;N",145,5,150);          addHistoToMonitors(h,h->GetName(), debugMons, keys);
  h = new TH1D("num_eff_trigger_met3j","trigger; MET cut;N",145,5,150);          addHistoToMonitors(h,h->GetName(), debugMons, keys);  


  h = new TH1D("den_eff_trigger_jetptcut","trigger; MET cut;N",80,20,100);       addHistoToMonitors(h,h->GetName(), debugMons, keys);
  h = new TH1D("num_eff_trigger_jetptcut","trigger; MET cut;N",80,20,100);       addHistoToMonitors(h,h->GetName(), debugMons, keys);  


  h = new TH1D("abs_den_eff_trigger_met","trigger; MET cut;N",145,5,150);        addHistoToMonitors(h,h->GetName(), debugMons, keys);
  h = new TH1D("abs_num_eff_trigger_met","trigger; MET cut;N",145,5,150);        addHistoToMonitors(h,h->GetName(), debugMons, keys);


  h = new TH1D("abs_den_eff_trigger_met3j","trigger; MET cut;N",145,5,150);        addHistoToMonitors(h,h->GetName(), debugMons, keys);
  h = new TH1D("abs_num_eff_trigger_met3j","trigger; MET cut;N",145,5,150);        addHistoToMonitors(h,h->GetName(), debugMons, keys);    


  h = new TH1D("abs_den_eff_trigger_jetptcut1","trigger; jet pt ;N",80,20,100);   addHistoToMonitors(h,h->GetName(), debugMons, keys);
  h = new TH1D("abs_den_eff_trigger_jetptcut2","trigger; jet pt ;N",80,20,100);   addHistoToMonitors(h,h->GetName(), debugMons, keys);

  h = new TH1D("abs_num_eff_trigger_jetptcut","trigger; jet pt ;N",80,20,100);   addHistoToMonitors(h,h->GetName(), debugMons, keys);  

  h = new TH1D("abs_num_eff_trigger_jetptcut1","trigger; jet pt;N",80,20,100);   addHistoToMonitors(h,h->GetName(), debugMons, keys);
  h = new TH1D("abs_num_eff_trigger_jetptcut2","trigger; jet pt;N",80,20,100);   addHistoToMonitors(h,h->GetName(), debugMons, keys);  


/*
   info[17] = j->chargedHadronEnergyFraction();
   info[18] = j->neutralHadronEnergyFraction();
   info[19] = ( j->neutralHadronEnergy() + j->HFHadronEnergy() ) / j->energy();
   info[20] = j->chargedEmEnergyFraction();
   info[21] = j->neutralEmEnergyFraction();
   info[22] = j->chargedMultiplicity();
   info[23] = j->numberOfDaughters();
*/
  
  // Electron debug Histograms ///////////////////////////////////////////////////////////////////////////////////////////////////////
  h = new TH1D("electronTrackIso",  "e; trk iso; N_{Entries}",100,0,5);          addHistoToMonitors(h,h->GetName(), debugMons, keys);  
  h = new TH1D("electronEcalIso",   "e; ecal iso; N_{Entries}",100,0,5);         addHistoToMonitors(h,h->GetName(), debugMons, keys);  
  h = new TH1D("electronCaloIso",   "e; calo iso; N_{Entries}",100,0,5);         addHistoToMonitors(h,h->GetName(), debugMons, keys);  
  h = new TH1D("electronRelIso",    "e; rel iso; N_{Entries}",50,0,0.2);          addHistoToMonitors(h,h->GetName(), debugMons, keys);  
  h = new TH1D("electronEOverP",    "e; E/P; N_{Entries}",100,0,5);              addHistoToMonitors(h,h->GetName(), debugMons, keys); 
  h = new TH1D("electronHOverE",    "e; H/E; N_{Entries}",25,0,0.1);              addHistoToMonitors(h,h->GetName(), debugMons, keys); 

  h = new TH1D("electronTrackIsoMatched",  "e; trk iso; N_{Entries}",100,0,5);   addHistoToMonitors(h,h->GetName(), debugMons, keys);  
  h = new TH1D("electronEcalIsoMatched",   "e; ecal iso; N_{Entries}",100,0,5);  addHistoToMonitors(h,h->GetName(), debugMons, keys);  
  h = new TH1D("electronCaloIsoMatched",   "e; calo iso; N_{Entries}",100,0,5);  addHistoToMonitors(h,h->GetName(), debugMons, keys);  
  h = new TH1D("electronRelIsoMatched",    "e; rel iso; N_{Entries}",50,0,0.2);   addHistoToMonitors(h,h->GetName(), debugMons, keys);  
  h = new TH1D("electronEOverPMatched",    "e; E/P; N_{Entries}",100,0,5);       addHistoToMonitors(h,h->GetName(), debugMons, keys); 
  h = new TH1D("electronHOverEMatched",    "e; H/E; N_{Entries}",25,0,0.1);       addHistoToMonitors(h,h->GetName(), debugMons, keys); 
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  h = new TH1D("optSelection", "v;N_{events}; optimization",50,0,50);   addHistoToMonitors(h,h->GetName(), debugMons,keys); 


/*
info[15]=isol[electron::Selector::TRKISO];
info[16]=isol[electron::Selector::ECALISO];
info[17]=isol[electron::Selector::CALOISO];
info[18]=isol[electron::Selector::RELISO];

info[5]=(*eIt)->eSuperClusterOverP();
info[6]=(*eIt)->hadronicOverEm();
*/


  for(uint i = 0; i<keys.size();i++){
    TString key = keys[i]; TDirectory * d_temp = dirsMaps[key]; TDirectory * d = d_temp->mkdir("Debug");
    TString id1 = key + TString(" lep_tau debug"); (debugMons[key])->initMonitorForStep(id1); dirsMaps[id1] = d->mkdir("lep_tau"); 
    TString id2 = key + TString(" e_tau debug");   (debugMons[key])->initMonitorForStep(id2); dirsMaps[id2] = d->mkdir("e_tau");
    TString id3 = key + TString(" m_tau debug");   (debugMons[key])->initMonitorForStep(id3); dirsMaps[id3] = d->mkdir("m_tau");
    //TString id4 = key + TString(" e_m debug");   (debugMons[key])->initMonitorForStep(id3); dirsMaps[id3] = d->mkdir("e_m");
  }

  for(uint i=0;i<keys.size();i++){
    TString key = keys[i];
    SelectionMonitor::StepMonitor_t & allMonitors = (debugMons[key])->getAllMonitors();
    for(SelectionMonitor::StepMonitor_t::iterator sIt = allMonitors.begin(); sIt != allMonitors.end(); sIt++){
      for(SelectionMonitor::Monitor_t::iterator hIt = sIt->second.begin(); hIt != sIt->second.end(); hIt++){     
        TDirectory *d =dirsMaps[sIt->first]; hIt->second->SetDirectory(d); 
      }
    }	
  }



}

void HistogramBuilder::buildRecoStepHistograms( TFile * outFile, map< TString, SelectionMonitor * > & stepsMons , map<TString,TDirectory *> & dirsMaps , vector<TString> & keys ){



  TH1D * h; TH2D *h2;

  // minimal separation between lepton and all jets//////////////////////////////////////////////////////////////////////////////
  // tt->      b+W---> b+e+neu   , b + W ---> tau + neu + b
  // distance between the lepton and the most energetic jet
  // distance between the lepton and the 2 most energetic jets
  // distance between the lepton and the 3 most energetic jets
  // minime from all jets
  h = new TH1D("DR1_lepton_jet",     "j;DR;      Entries",100,0,10);   addHistoToMonitors(h,h->GetName(),stepsMons, keys);
  h = new TH1D("DR2_lepton_jet",     "j;DR;      Entries",100,0,10);   addHistoToMonitors(h,h->GetName(),stepsMons, keys);
  h = new TH1D("DR3_lepton_jet",     "j;DR;      Entries",100,0,10);   addHistoToMonitors(h,h->GetName(),stepsMons, keys);
  h = new TH1D("DRALL_lepton_jet",   "j;DR;      Entries",100,0,10);   addHistoToMonitors(h,h->GetName(),stepsMons, keys);
  h = new TH1D("DEta1_lepton_jet",   "j;D#eta;   Entries",100,0,5);    addHistoToMonitors(h,h->GetName(),stepsMons, keys);
  h = new TH1D("DEta2_lepton_jet",   "j;D#eta;   Entries",100,0,5);    addHistoToMonitors(h,h->GetName(),stepsMons, keys);
  h = new TH1D("DEta3_lepton_jet",   "j;D#eta;   Entries",100,0,5);    addHistoToMonitors(h,h->GetName(),stepsMons, keys);
  h = new TH1D("DEtaALL_lepton_jet", "j;D#eta;   Entries",100,0,5);    addHistoToMonitors(h,h->GetName(),stepsMons, keys);
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  h = new TH1D("Dphi_lepton_met", "      l;D#phi;  Entries",100,0,4);  addHistoToMonitors(h,h->GetName(),stepsMons, keys);
  h = new TH1D("Dphi_tau_met", "         l;D#phi;  Entries",100,0,4);  addHistoToMonitors(h,h->GetName(),stepsMons, keys);
  h = new TH1D("dilarccosine",";#theta(l,l') [rad];Events",50,0,3.2);  addHistoToMonitors(h,h->GetName(),stepsMons, keys); 
  h = new TH1D("Dphi_lepton_tau", "      l;D#phi;  Entries",100,0,4);  addHistoToMonitors(h,h->GetName(),stepsMons, keys);
  h = new TH1D("DR_lepton_tau",          "j;DR;      Entries",100,0,10);   addHistoToMonitors(h,h->GetName(),stepsMons, keys);
 
  // vertex distributions //////////////////////////////////////////////////////////////////////////////////////////
  h = new TH1D("vertex","j;N_{vertex}; Entries",50,0,50);  addHistoToMonitors(h,h->GetName(),stepsMons, keys);
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////



  // vertex distributions //////////////////////////////////////////////////////////////////////////////////////////
  h = new TH1D("vertexUnweighted","j;N_{vertex}; Entries",50,0,50);  addHistoToMonitors(h,h->GetName(),stepsMons, keys);
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // weights distributions ///////////////////////////////////////////////////////////////////////////////////
  h = new TH1D("weights","w;weights; Entries",500,0,10);  addHistoToMonitors(h,h->GetName(),stepsMons, keys);
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////


  // Multiplicity histograms ////////////////////////////////////////////////////////////////////////////////////////////////////
  h = new TH1D("multiplicity_j",       "j;N_{jets}; Entries",6,0,6);   addHistoToMonitors(h,h->GetName(),stepsMons, keys);  
  h->GetXaxis()->SetBinLabel(1,"0"); h->GetXaxis()->SetBinLabel(2,"1"); h->GetXaxis()->SetBinLabel(3,"2");
  h->GetXaxis()->SetBinLabel(4,"3"); h->GetXaxis()->SetBinLabel(5,"4"); h->GetXaxis()->SetBinLabel(6,"#geq 5");
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  // Multiplicity histogram with MET requirement ///////////////////////////////////////////////////////////////////////////////
  h = new TH1D("multiplicity_jb",       "j;N_{jets}; Entries",6,0,6); addHistoToMonitors(h,h->GetName(),stepsMons, keys);  
  h->GetXaxis()->SetBinLabel(1,"0"); h->GetXaxis()->SetBinLabel(2,"1"); h->GetXaxis()->SetBinLabel(3,"2");
  h->GetXaxis()->SetBinLabel(4,"3"); h->GetXaxis()->SetBinLabel(5,"4"); h->GetXaxis()->SetBinLabel(6,"#geq 5");
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  // kinematic distributions ///////////////////////////////////////////////////////////////////////////////////////////////////
  h = new TH1D("pt_e",                "e;p_{T} [GeV/c]; Entries",300,0,750);    addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
  h = new TH1D("pt_m",                "m;p_{T} [GeV/c]; Entries",300,0,750);    addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("pt_j",                "j;p_{T} [GeV/c]; Entries",300,0,750);    addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("pt_bj",               "bj;p_{T} [GeV/c]; Entries",300,0,750);    addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("pt_j1",               "j1;p_{T} [GeV/c]; Entries",300,0,750);    addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("pt_j2",               "j2;p_{T} [GeV/c]; Entries",300,0,750);    addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
  h = new TH1D("pt_j3",               "j3;p_{T} [GeV/c]; Entries",300,0,750);    addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
  h = new TH1D("pt_t",                "t;p_{T} [GeV/c]; Entries",300,0,750);    addHistoToMonitors(h,h->GetName(), stepsMons, keys); 

  //eta phi of taus
  h2 = new TH2D("eta_phi_t","t; eta; phi; ",60,-3.05,3.05, 60, -3.15, 3.15 );       addHistoToMonitors(h2,h2->GetName(), stepsMons, keys);

  //eta phi of leptons
  h2 = new TH2D("eta_phi_lepton","l; eta; phi; ",60,-3.05,3.05, 60, -3.15, 3.15 );       addHistoToMonitors(h2,h2->GetName(), stepsMons, keys);

  //deltaphi tau-met and lepton-met
  h2 = new TH2D("dphiTauMET_dphiLeptonMET","l; dphi(tau-met); dphi(lepton-met);",60,0,3.15, 60, 0, 3.15 ); addHistoToMonitors(h2,h2->GetName(), stepsMons, keys);

  // mjj, mjjj
  h = new TH1D("mjj",              "mjj;M(j,j); Entries",400,0.,1600.);                   addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("mjjb",              "mjjb;M(j,j,b); Entries",400,0.,1600.);               addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h2 = new TH2D("mjjb_mjj","mjjb_mjj; M(j,j,b); M(j, j);",400,0.,1600., 400, 0., 1600. ); addHistoToMonitors(h2,h2->GetName(), stepsMons, keys);
  
  h = new TH1D("sumpt_taub",              "sumpt_taub;p_{T}^{\tau} + p_{T}^{b} [GeV/c]; Entries",600,0,1500);    addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("sumpt_jj",                "sumpt_jj;p_{T}^{jet1} + p_{T}^{jet2} [GeV/c]; Entries",600,0,1500);    addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("m_taub",              "m_taub;M(#tau,b); Entries",400,0.,1600.);                   addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  
  
  h = new TH1D("m_lepj_all",              "m_lepj_all;M(l,j); Entries",400,0.,1600.);                   addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("m_lepj_min",              "m_lepj_min;min M(l,j); Entries",400,0.,1600.);                   addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("m_lepj_max",              "m_lepj_max;max M(l,j); Entries",400,0.,1600.);                   addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("m_lepj_delta",            "m_lepj_delta;|M(#tau,j)-M(#mu,j)|; Entries",4000,0.,400.);                   addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h2 = new TH2D("m_lepj_mmj_mtj","m_lepj_mmj_mtj; M(#tau,j); M(#mu, j);",400,0.,1600., 400, 0., 1600. ); addHistoToMonitors(h2,h2->GetName(), stepsMons, keys);
  h2 = new TH2D("m_lepj_delta_mtj","m_lepj_delta_mtj; M(#tau,j);|M(#tau,j)-M(#mu,j)|;",400,0.,1600., 4000, 0., 400. ); addHistoToMonitors(h2,h2->GetName(), stepsMons, keys);
  
  // btag //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  h = new TH1D("btag_j",              "j;btag; Entries",200,-20,20);            addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("btagmultiplicity_j",  "j;N_{b-tags}; Entries",6,0,6);                 addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h->GetXaxis()->SetBinLabel(1,"0");
  h->GetXaxis()->SetBinLabel(2,"1");
  h->GetXaxis()->SetBinLabel(3,"2");
  h->GetXaxis()->SetBinLabel(4,"3");
  h->GetXaxis()->SetBinLabel(5,"4");
  h->GetXaxis()->SetBinLabel(6,"#geq5");

  h = new TH1D("btagmultiplicity0_j",  "j;N_{b-tags}; Entries",1,0,1);                 addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h->GetXaxis()->SetBinLabel(1,"0 btags");
  h = new TH1D("btagmultiplicity1_j",  "j;N_{b-tags}; Entries",1,0,1);                 addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h->GetXaxis()->SetBinLabel(1,"1 btags");
  h = new TH1D("btagmultiplicity2_j",  "j;N_{b-tags}; Entries",1,0,1);                 addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h->GetXaxis()->SetBinLabel(1,"2 btags");
  h = new TH1D("btagmultiplicity3_j",  "j;N_{b-tags}; Entries",1,0,1);                 addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h->GetXaxis()->SetBinLabel(1,"3 btags");
  h = new TH1D("btagmultiplicity4_j",  "j;N_{b-tags}; Entries",1,0,1);                 addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h->GetXaxis()->SetBinLabel(1,"4 btags");
  h = new TH1D("btagmultiplicity5_j",  "j;N_{b-tags}; Entries",1,0,1);                 addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h->GetXaxis()->SetBinLabel(1,"#geq5 btags");

  h = new TH1D("corrected_btagmultiplicity_j",  "j;N_{b-tags}; Entries",6,0,6);       addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h->GetXaxis()->SetBinLabel(1,"0");
  h->GetXaxis()->SetBinLabel(2,"1");
  h->GetXaxis()->SetBinLabel(3,"2");
  h->GetXaxis()->SetBinLabel(4,"3");
  h->GetXaxis()->SetBinLabel(5,"4");
  h->GetXaxis()->SetBinLabel(6,"#geq5");
  h = new TH1D("corrected_btagmultiplicity0_j",  "j;N_{b-tags}; Entries",1,0,1);                 addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h->GetXaxis()->SetBinLabel(1,"0 btags");
  h = new TH1D("corrected_btagmultiplicity1_j",  "j;N_{b-tags}; Entries",1,0,1);                 addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h->GetXaxis()->SetBinLabel(1,"1 btags");
  h = new TH1D("corrected_btagmultiplicity2_j",  "j;N_{b-tags}; Entries",1,0,1);                 addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h->GetXaxis()->SetBinLabel(1,"2 btags");
  h = new TH1D("corrected_btagmultiplicity3_j",  "j;N_{b-tags}; Entries",1,0,1);                 addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h->GetXaxis()->SetBinLabel(1,"3 btags");
  h = new TH1D("corrected_btagmultiplicity4_j",  "j;N_{b-tags}; Entries",1,0,1);                 addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h->GetXaxis()->SetBinLabel(1,"4 btags");
  h = new TH1D("corrected_btagmultiplicity5_j",  "j;N_{b-tags}; Entries",1,0,1);                 addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h->GetXaxis()->SetBinLabel(1,"#geq5 btags");


  // btag efficiency plots 
  h = new TH1D("btag_eff_realbs_num",  "j; p_{T}^{b-jets};     Entries",100,0,250);  addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("btag_eff_fakebs_num",  "j; p_{T}^{udscg-jets}; Entries",100,0,250);  addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("btag_eff_nomatch_num", "j; p_{T}^{nomatch};    Entries",100,0,250);  addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("btag_eff_fakebs2_num", "j; p_{T}^{udscg-jets}; Entries",100,0,250);  addHistoToMonitors(h,h->GetName(), stepsMons, keys);

  h = new TH1D("btag_eff_realbs_den",  "j; p_{T}^{b-jets};     Entries",100,0,250);  addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("btag_eff_fakebs_den",  "j; p_{T}^{udscg-jets}; Entries",100,0,250);  addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("btag_eff_nomatch_den", "j; p_{T}^{nomatch};    Entries",100,0,250);  addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("btag_eff_fakebs2_den", "j; p_{T}^{nomatch};    Entries",100,0,250);  addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  // btag uncertainty
  //h = new TH1D("btag_unc","j;0btags,1btags, uncertainty;     Entries",4,0,4);      addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  //h->GetXaxis()->SetBinLabel(1,"0"); h->GetXaxis()->SetBinLabel(2,"1"); h->GetXaxis()->SetBinLabel(3,"2"); h->GetXaxis()->SetBinLabel(4,"#geq3"); 
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
  // Mass between lepton and jet
  h = new TH1D("m_muj1","l; m_{ #mu j_{2} };",400,0,1600);        addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("m_muj2","l; m_{ #mu j_{2} };",400,0,1600);        addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("m_muj3","l; m_{ #mu j_{2} };",400,0,1600);        addHistoToMonitors(h,h->GetName(), stepsMons, keys);

  h = new TH1D("m_ej1","l; m_{ ej_{2} };",400,0,1600);        addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("m_ej2","l; m_{ ej_{2} };",400,0,1600);        addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("m_ej3","l; m_{ ej_{2} };",400,0,1600);        addHistoToMonitors(h,h->GetName(), stepsMons, keys);

  // Mass between lepton and b-jet
  h = new TH1D("m_mubj","l; m_{ #mu bjet };",400,0,1600);        addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("max_m_muj","l; max m_{ #mu j };",400,0,1600);        addHistoToMonitors(h,h->GetName(), stepsMons, keys);



  //debug ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  h = new TH1D("dphij1j2","l; #Delta#Phij_{1}j_{2};",60,0,3.15); addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("mj1j2","l; m_{ j_{1}j_{2} };",400,0,1600);        addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  //mt between lepton and met ("w_mass"), mt between tau and met  is ("w_mass_tau)
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





  // polarization with respect to tau pt
  h = new TH1D("r_t",                 "r;p_{T}^{ lead.track}/p_{T}^{jet}; Entries",100,0,1.2);  h->Rebin(4); addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
  // polarization with respect to tau jet
  h = new TH1D("rb_t",                 "r;p_{T}^{ lead.track}/p_{T}^{jet}; Entries",100,0,1.2);  h->Rebin(4); addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
  // polarization with respect to tau Energy
  h = new TH1D("rc_t",                 "r;p_{T}^{ lead.track}/p_{T}^{jet}; Entries",100,0,1.2);  h->Rebin(4); addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
 

  // polarization with respect to tau pt
  h = new TH1D("r_t1p",                 "r;p_{T}^{ lead.track}/p_{T}^{jet}; Entries",100,0,1.2);  h->Rebin(4); addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
  // polarization with respect to tau jet
  h = new TH1D("rb_t1p",                 "r;p_{T}^{ lead.track}/p_{T}^{jet}; Entries",100,0,1.2);  h->Rebin(4); addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
  // polarization with respect to tau Energy
  h = new TH1D("rc_t1p",                 "r;p_{T}^{ lead.track}/p_{T}^{jet}; Entries",100,0,1.2);  h->Rebin(4); addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
 
  // polarization with respect to tau pt
  h = new TH1D("r_t3p",                 "r;p_{T}^{ lead.track}/p_{T}^{jet}; Entries",100,0,1.2);  h->Rebin(4); addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
  // polarization with respect to tau jet
  h = new TH1D("rb_t3p",                 "r;p_{T}^{ lead.track}/p_{T}^{jet}; Entries",100,0,1.2);  h->Rebin(4); addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
  // polarization with respect to tau Energy
  h = new TH1D("rc_t3p",                 "r;p_{T}^{ lead.track}/p_{T}^{jet}; Entries",100,0,1.2);  h->Rebin(4); addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
 

  h = new TH1D("numbprongs_a", "n;numbprongs; Entries",10,0,10); addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
  h = new TH1D("numbprongs_b", "n;numbprongs; Entries",10,0,10); addHistoToMonitors(h,h->GetName(), stepsMons, keys); 


  h = new TH1D("dr_t",                "dr;dr tau-lepton; Entries",100,0,2);  addHistoToMonitors(h,h->GetName(), stepsMons, keys);
 
  h = new TH1D("pt_met" ,             "met;E_{T}^{miss} [GeV]; Entries",2000,0,2000);  addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
 


  h = new TH1D("st" ,                 "st; ST  [GeV/c^{2}]; Entries",1000,0,1000);  addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
  h = new TH1D("ht" ,                 "ht; HT  [GeV]; Entries",250,0,500);          addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
  h = new TH1D("mht" ,                "mht;  MHT [GeV]; Entries",250,0,500);        addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("mhtb" ,               "mhtb; MHT [GeV]; Entries",250,0,500);        addHistoToMonitors(h,h->GetName(), stepsMons, keys);  

 
  h2 = new TH2D("mhtVersusMET",        "mht;mht; met; met;",250,0,500,150,0,150);    addHistoToMonitors(h2,h2->GetName(), stepsMons, keys);
  h2 = new TH2D("mhtbVersusMET",       "mht;mht; met; met;",250,0,500,150,0,150);    addHistoToMonitors(h2,h2->GetName(), stepsMons, keys);


  // wT mass without met cut (note this plot is only filled at lepton level and met)
  h = new TH1D("w_mass" ,          "Mwt; m_{T}; Entries",2000,0,900);          addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
  h = new TH1D("w_mass2" ,         "Mwt; m_{T}; Entries",2000,0,900);          addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
  h = new TH1D("w_mass_tau" ,      "Mwt; m_{T}; Entries",2000,0,900);          addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("w_mass_min" ,      "Mwt; m_{T}; Entries",2000,0,900);          addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
  h = new TH1D("w_mass_max" ,      "Mwt; m_{T}; Entries",2000,0,900);          addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("w_mass_deltaphi" , "Mwt; #Delta_{#phi}; Entries",400,-4,4);   addHistoToMonitors(h,h->GetName(), stepsMons, keys); 



  TH2D * opt = new TH2D("optimization", "opt x;ltk; lpf; opt y;",10,0,10,10,0,10);  addHistoToMonitors(opt,opt->GetName(), stepsMons, keys); 
	
  h = new TH1D("eta_e",          "e; #eta ; Entries", 60,-3.05,3.05);   addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
  h = new TH1D("eta_m",          "m; #eta ; Entries", 60,-3.05,3.05);   addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
  h = new TH1D("eta_j",          "j; #eta ; Entries", 60,-3.05,3.05);   addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
  h = new TH1D("eta_bj",         "bj; #eta ; Entries", 60,-3.05,3.05);   addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
  h = new TH1D("eta_j1",         "j1;#eta ; Entries", 60,-3.05,3.05);   addHistoToMonitors(h,h->GetName(), stepsMons, keys);
  h = new TH1D("eta_j2",         "j2;#eta ; Entries", 60,-3.05,3.05);   addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
  h = new TH1D("eta_j3",         "j3;#eta ; Entries", 60,-3.05,3.05);   addHistoToMonitors(h,h->GetName(), stepsMons, keys); 
  h = new TH1D("eta_t",          "t; #eta ; Entries", 60,-3.05,3.05);   addHistoToMonitors(h,h->GetName(), stepsMons, keys); 




  // First Selection ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  vector<TString> objKey; 
  objKey.push_back("lep_tau"); objKey.push_back("e_tau"); objKey.push_back("m_tau");  //objKey.push_back("e_m");


  for(uint k=0;k<keys.size();k++){
    
    TString key= keys[k]; 
    TDirectory * dirObjTemp = dirsMaps[key]->mkdir("RecoSteps"); 

    for(uint i=0;i<objKey.size();i++){

      TString objString = objKey[i];

      TDirectory * dirObj = dirObjTemp->mkdir(objString);

      TString extra = key + TString(" ")+objString;

      TString step1  = extra+TString(" init");       stepsMons[key]->initMonitorForStep(step1);   dirsMaps[step1] = dirObj->mkdir("init");
      TString step2  = extra+TString(" #geq 1 jet"); stepsMons[key]->initMonitorForStep(step2);   dirsMaps[step2] = dirObj->mkdir("#geq 1 jet");
      TString step3  = extra+TString(" #geq 2 jet"); stepsMons[key]->initMonitorForStep(step3);   dirsMaps[step3] = dirObj->mkdir("#geq 2 jet");
      TString step4  = extra+TString(" #geq 3 jet"); stepsMons[key]->initMonitorForStep(step4);   dirsMaps[step4] = dirObj->mkdir("#geq 3 jet");
      TString step5  = extra+TString(" MET");        stepsMons[key]->initMonitorForStep(step5);   dirsMaps[step5] = dirObj->mkdir("MET");
      TString step6  = extra+TString(" MT");         stepsMons[key]->initMonitorForStep(step6);   dirsMaps[step6] = dirObj->mkdir("MT");
//      TString step6  = extra+TString(" 1 btag");     stepsMons[key]->initMonitorForStep(step6);   dirsMaps[step6] = dirObj->mkdir("1 btag");
//      TString step7  = extra+TString(" 1 Tau");      stepsMons[key]->initMonitorForStep(step7);   dirsMaps[step7] = dirObj->mkdir("1 Tau");
//      TString step8  = extra+TString(" OS");         stepsMons[key]->initMonitorForStep(step8);   dirsMaps[step8] = dirObj->mkdir("OS");
//      TString step9 = extra+TString(" 2 btag");     stepsMons[key]->initMonitorForStep(step9);  dirsMaps[step9]= dirObj->mkdir("2 btag");
//      TString step10 = extra+TString(" R");          stepsMons[key]->initMonitorForStep(step10);  dirsMaps[step10]= dirObj->mkdir("R");

      TString step7  = extra+TString(" 1 btag");     stepsMons[key]->initMonitorForStep(step7);   dirsMaps[step7] = dirObj->mkdir("1 btag");
      TString step8  = extra+TString(" 1 Tau");      stepsMons[key]->initMonitorForStep(step8);   dirsMaps[step8] = dirObj->mkdir("1 Tau");
      TString step9  = extra+TString(" OS");         stepsMons[key]->initMonitorForStep(step9);   dirsMaps[step9] = dirObj->mkdir("OS");
      TString step10 = extra+TString(" 2 btag");     stepsMons[key]->initMonitorForStep(step10);  dirsMaps[step10]= dirObj->mkdir("2 btag");
      TString step11 = extra+TString(" R");          stepsMons[key]->initMonitorForStep(step11);  dirsMaps[step11]= dirObj->mkdir("R");
      
    }

  }


  for(uint i=0;i<keys.size();i++){
    TString key = keys[i];
    SelectionMonitor::StepMonitor_t & allMonitors = (stepsMons[key])->getAllMonitors();
    for(SelectionMonitor::StepMonitor_t::iterator sIt = allMonitors.begin(); sIt != allMonitors.end(); sIt++){
      for(SelectionMonitor::Monitor_t::iterator hIt = sIt->second.begin(); hIt != sIt->second.end(); hIt++){     
        TDirectory *d =dirsMaps[sIt->first]; hIt->second->SetDirectory(d); 
      }
    }	
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




}


void HistogramBuilder::buildYieldsHistograms( TFile * outFile, map< TString, SelectionMonitor * > & yieldsMons , map<TString,TDirectory *> & dirsMaps , vector<TString> & keys ){


  // MET cut position replaced with 3rd jets
  //  TString steps[] = {"reco","trigger", "minitree","= 1 lepton","#geq 1 jet","#geq 2 jets","1l + #geq 2j + MET","1l + #geq 3j + MET", "#geq 1btag","1 #tau",         "OS", "#geq 2 btags", "R"};
  TString steps[] = {"reco","trigger", "minitree","= 1 lepton","#geq 1 jet","#geq 2 jets","1l + #geq 2j + MET","1l + #geq 3j + MET", "MT", "#geq 1btag","1 #tau",         "OS", "#geq 2 btags", "R"};

  if( commondefinitions::MODE_ == commondefinitions::STARTING_AT_LJETS_){
      steps[6]  = TString("1 lep + #geq 3 jets");  
      steps[7]  = TString("MET"); 
     

      if(!commondefinitions::APPLY_MT_CUT_){
	steps[8]  = TString("#geq 1btag");
	steps[9] = TString("1 #tau");
	steps[10] = TString("OS");
	steps[11] = TString("#geq 2 btags");
	steps[12] = TString("R");
      }
      else{
	steps[8]  = TString("MT");
	steps[9]  = TString("#geq 1btag");
	steps[10] = TString("1 #tau");
	steps[11] = TString("OS");
	steps[12] = TString("#geq 2 btags");
	steps[13] = TString("R");
      }
      
  }

  int nsteps = sizeof(steps)/sizeof(TString);
  TH1D * h0  = new TH1D("cutflow_triggErr",       "Cut Flow; Step;N",nsteps,0,nsteps); addHistoToMonitors(h0,h0->GetName(), yieldsMons, keys); 
  TH1D * h1  = new TH1D("cutflow_yields",         "Cut Flow; Step;N",nsteps,0,nsteps); addHistoToMonitors(h1,h1->GetName(), yieldsMons, keys); 
  TH1D * h2  = new TH1D("cutflow_yields_plus",    "Cut Flow; Step;N",nsteps,0,nsteps); addHistoToMonitors(h2,h2->GetName(), yieldsMons, keys); 
  TH1D * h3  = new TH1D("cutflow_yields_minus",   "Cut Flow; Step;N",nsteps,0,nsteps); addHistoToMonitors(h3,h3->GetName(), yieldsMons, keys); 
  TH1D * h4  = new TH1D("cutflow_yields_uncplus", "Cut Flow; Step;N",nsteps,0,nsteps); addHistoToMonitors(h4,h4->GetName(), yieldsMons, keys); 
  TH1D * h5  = new TH1D("cutflow_yields_uncminus","Cut Flow; Step;N",nsteps,0,nsteps); addHistoToMonitors(h5,h5->GetName(), yieldsMons, keys);
  TH1D * h6  = new TH1D("cutflow_yields_jerplus", "Cut Flow; Step;N",nsteps,0,nsteps); addHistoToMonitors(h6,h6->GetName(), yieldsMons, keys); 
  TH1D * h7  = new TH1D("cutflow_yields_jerminus","Cut Flow; Step;N",nsteps,0,nsteps); addHistoToMonitors(h7,h7->GetName(), yieldsMons, keys);  
  TH1D * h8  = new TH1D("cutflow_yields_bplus",   "Cut Flow; Step;N",nsteps,0,nsteps); addHistoToMonitors(h8,h8->GetName(), yieldsMons, keys); 
  TH1D * h9  = new TH1D("cutflow_yields_bminus",  "Cut Flow; Step;N",nsteps,0,nsteps); addHistoToMonitors(h9,h9->GetName(), yieldsMons, keys);  
  TH1D * h10 = new TH1D("cutflow_yields_unbplus", "Cut Flow; Step;N",nsteps,0,nsteps); addHistoToMonitors(h10,h10->GetName(), yieldsMons, keys); 
  TH1D * h11 = new TH1D("cutflow_yields_unbminus","Cut Flow; Step;N",nsteps,0,nsteps); addHistoToMonitors(h11,h11->GetName(), yieldsMons, keys);
  TH1D * h12 = new TH1D("cutflow_yields_tesplus", "Cut Flow; Step;N",nsteps,0,nsteps); addHistoToMonitors(h12,h12->GetName(), yieldsMons, keys);
  TH1D * h13 = new TH1D("cutflow_yields_tesminus","Cut Flow; Step;N",nsteps,0,nsteps); addHistoToMonitors(h13,h13->GetName(), yieldsMons, keys);
  TH1D * h14 = new TH1D("cutflow_yields_topptuncplus", "Cut Flow; Step;N",nsteps,0,nsteps); addHistoToMonitors(h14,h14->GetName(), yieldsMons, keys);
  TH1D * h15 = new TH1D("cutflow_yields_topptuncminus","Cut Flow; Step;N",nsteps,0,nsteps); addHistoToMonitors(h15,h15->GetName(), yieldsMons, keys);



  for(int ibin=1; ibin<=nsteps; ibin++) { 
    h0->GetXaxis()->SetBinLabel(ibin,steps[ibin-1]);
    h1->GetXaxis()->SetBinLabel(ibin,steps[ibin-1]);  h2->GetXaxis()->SetBinLabel(ibin,steps[ibin-1]); h3->GetXaxis()->SetBinLabel(ibin,steps[ibin-1]);
    h4->GetXaxis()->SetBinLabel(ibin,steps[ibin-1]);  h5->GetXaxis()->SetBinLabel(ibin,steps[ibin-1]); h6->GetXaxis()->SetBinLabel(ibin,steps[ibin-1]); 
    h7->GetXaxis()->SetBinLabel(ibin,steps[ibin-1]);  h8->GetXaxis()->SetBinLabel(ibin,steps[ibin-1]); h9->GetXaxis()->SetBinLabel(ibin,steps[ibin-1]);
    h10->GetXaxis()->SetBinLabel(ibin,steps[ibin-1]); h11->GetXaxis()->SetBinLabel(ibin,steps[ibin-1]);
    h12->GetXaxis()->SetBinLabel(ibin,steps[ibin-1]); h13->GetXaxis()->SetBinLabel(ibin,steps[ibin-1]);
  }


 for(uint i = 0; i<keys.size();i++){
    TString key = keys[i]; TDirectory * d_temp = dirsMaps[key]; 
    TDirectory * d       = d_temp->mkdir("Yields");
    TDirectory * trigger = d->mkdir("trigger");      
   
    TString id1 = key + TString(" lep_tau yields"); (yieldsMons[key])->initMonitorForStep(id1); dirsMaps[id1] = d->mkdir("lep_tau"); 
    TString id2 = key + TString(" e_tau yields");   (yieldsMons[key])->initMonitorForStep(id2); dirsMaps[id2] = d->mkdir("e_tau");
    TString id3 = key + TString(" m_tau yields");   (yieldsMons[key])->initMonitorForStep(id3); dirsMaps[id3] = d->mkdir("m_tau");

  }

  // change Name
  for(uint i=0;i<keys.size();i++){
    TString key = keys[i];
    SelectionMonitor::StepMonitor_t & allMonitors = (yieldsMons[key])->getAllMonitors();
    for(SelectionMonitor::StepMonitor_t::iterator sIt = allMonitors.begin(); sIt != allMonitors.end(); sIt++){
      for(SelectionMonitor::Monitor_t::iterator hIt = sIt->second.begin(); hIt != sIt->second.end(); hIt++){     
        TDirectory *d =dirsMaps[sIt->first]; hIt->second->SetDirectory(d); 
      }
    }	
  }

}




void HistogramBuilder::buildMCYieldsHistograms( TFile * outFile, map< TString, SelectionMonitor * >  & yieldsMons , map<TString,TDirectory *> & dirsMaps , vector<TString> & keys ){


  // MONITORING EVENT YIELDS and W+jets /////////////////////////////////////////////////////////////////////////////////////////////////////
  //  TString steps[] = {"reco","trigger", "minitree","= 1 lepton","#geq 1 jet","#geq 2 jets","1l + #geq 2j + MET","1l + #geq 3j + MET", "#geq 1btag","1 #tau",         "OS", "#geq 2 btags","R"};
  TString steps[] = {"reco","trigger", "minitree","= 1 lepton","#geq 1 jet","#geq 2 jets","1l + #geq 2j + MET","1l + #geq 3j + MET","MT", "#geq 1btag","1 #tau",         "OS", "#geq 2 btags","R"};


  if( commondefinitions::MODE_ == commondefinitions::STARTING_AT_LJETS_ ){
// fix attempt not gone well//    steps[6]  = TString("1 lep + #geq 3 jets");  
// fix attempt not gone well//    steps[7]  = TString("MET"); 
// fix attempt not gone well//    steps[8]  = TString("#geq 1btag");
// fix attempt not gone well//    steps[9] = TString("1 #tau");
// fix attempt not gone well//    steps[10] = TString("OS");
// fix attempt not gone well//    steps[11] = TString("#geq 2 btags");
// fix attempt not gone well//    steps[12] = TString("R");

    steps[6]  = TString("1 lep + #geq 3 jets");  
    steps[7]  = TString("MET"); 
    steps[8]  = TString("MT");
    steps[9]  = TString("#geq 1btag");
    steps[10] = TString("1 #tau");
    steps[11] = TString("OS");
    steps[12] = TString("#geq 2 btags");
    steps[13] = TString("R");

  }

  int nsteps = sizeof(steps)/sizeof(TString);

  // Add MC true for each reco channel 
  //  TString ttch[]= {"e#tau","#mu#tau","l(qq)","ll","#tau(qq)","#tau#tau","full had.","W+jets","Z+jets","Z(#tau#tau)","single top","qcd","diboson"};
  TString ttch[]= {"e#tau","#mu#tau","l(qq)","ee","e#mu","#mu#mu","#tau(qq)","#tau#tau","full had.","W+jets","Z+jets","Z(#tau#tau)","single top","qcd","diboson"};

  int  ysteps = sizeof(ttch)/sizeof(TString);
  TH2D * mc_h2d_0  = new TH2D("cutflow_mc_triggErr", "cut flow;Step;Event type;N_{events}",nsteps,0,nsteps,ysteps,0,ysteps); 
  addHistoToMonitors(mc_h2d_0,mc_h2d_0->GetName(), yieldsMons,keys);
  TH2D * mc_h2d_1  = new TH2D("cutflow_yields_mc", "cut flow;Step;Event type;N_{events}",nsteps,0,nsteps,ysteps,0,ysteps);  
  addHistoToMonitors(mc_h2d_1,mc_h2d_1->GetName(), yieldsMons,keys);
  TH2D * mc_h2d_2  = new TH2D("cutflow_yields_mc_plus", "cut flow;Step;Event type;N_{events}",nsteps,0,nsteps,ysteps,0,ysteps); 
  addHistoToMonitors(mc_h2d_2,mc_h2d_2->GetName(), yieldsMons,keys);
  TH2D * mc_h2d_3  = new TH2D("cutflow_yields_mc_minus", "cut flow;Step;Event type;N_{events}",nsteps,0,nsteps,ysteps,0,ysteps); 
  addHistoToMonitors(mc_h2d_3,mc_h2d_3->GetName(), yieldsMons,keys);
  TH2D * mc_h2d_4  = new TH2D("cutflow_yields_mc_uncplus", "cut flow;Step;Event type;N_{events}",nsteps,0,nsteps,ysteps,0,ysteps); 
  addHistoToMonitors(mc_h2d_4,mc_h2d_4->GetName(), yieldsMons,keys);
  TH2D * mc_h2d_5  = new TH2D("cutflow_yields_mc_uncminus", "cut flow;Step;Event type;N_{events}",nsteps,0,nsteps,ysteps,0,ysteps); 
  addHistoToMonitors(mc_h2d_5,mc_h2d_5->GetName(), yieldsMons,keys);
  TH2D * mc_h2d_6  = new TH2D("cutflow_yields_mc_jerplus", "cut flow;Step;Event type;N_{events}",nsteps,0,nsteps,ysteps,0,ysteps); 
  addHistoToMonitors(mc_h2d_6,mc_h2d_6->GetName(), yieldsMons,keys);
  TH2D * mc_h2d_7  = new TH2D("cutflow_yields_mc_jerminus", "cut flow;Step;Event type;N_{events}",nsteps,0,nsteps,ysteps,0,ysteps); 
  addHistoToMonitors(mc_h2d_7,mc_h2d_7->GetName(), yieldsMons,keys);
  TH2D * mc_h2d_8  = new TH2D("cutflow_yields_mc_bplus", "cut flow;Step;Event type;N_{events}",nsteps,0,nsteps,ysteps,0,ysteps); 
  addHistoToMonitors(mc_h2d_8,mc_h2d_8->GetName(), yieldsMons,keys);
  TH2D * mc_h2d_9  = new TH2D("cutflow_yields_mc_bminus", "cut flow;Step;Event type;N_{events}",nsteps,0,nsteps,ysteps,0,ysteps); 
  addHistoToMonitors(mc_h2d_9,mc_h2d_9->GetName(), yieldsMons,keys);
  TH2D * mc_h2d_10  = new TH2D("cutflow_yields_mc_unbplus", "cut flow;Step;Event type;N_{events}",nsteps,0,nsteps,ysteps,0,ysteps); 
  addHistoToMonitors(mc_h2d_10,mc_h2d_10->GetName(), yieldsMons,keys);
  TH2D * mc_h2d_11  = new TH2D("cutflow_yields_mc_unbminus", "cut flow;Step;Event type;N_{events}",nsteps,0,nsteps,ysteps,0,ysteps); 
  addHistoToMonitors(mc_h2d_11,mc_h2d_11->GetName(), yieldsMons,keys);
  TH2D * mc_h2d_12  = new TH2D("cutflow_yields_mc_tesplus", "cut flow;Step;Event type;N_{events}",nsteps,0,nsteps,ysteps,0,ysteps); 
  addHistoToMonitors(mc_h2d_12,mc_h2d_12->GetName(), yieldsMons,keys);
  TH2D * mc_h2d_13  = new TH2D("cutflow_yields_mc_tesminus", "cut flow;Step;Event type;N_{events}",nsteps,0,nsteps,ysteps,0,ysteps); 
  addHistoToMonitors(mc_h2d_13,mc_h2d_13->GetName(), yieldsMons,keys);
  TH2D * mc_h2d_14  = new TH2D("cutflow_yields_mc_topptuncplus", "cut flow;Step;Event type;N_{events}",nsteps,0,nsteps,ysteps,0,ysteps); 
  addHistoToMonitors(mc_h2d_14,mc_h2d_14->GetName(), yieldsMons,keys);
  TH2D * mc_h2d_15  = new TH2D("cutflow_yields_mc_topptuncminus", "cut flow;Step;Event type;N_{events}",nsteps,0,nsteps,ysteps,0,ysteps); 
  addHistoToMonitors(mc_h2d_15,mc_h2d_15->GetName(), yieldsMons,keys);


  for( int xsteps_i=1; xsteps_i <= nsteps; xsteps_i++ ){ 
    mc_h2d_0->GetXaxis()->SetBinLabel(xsteps_i, steps[xsteps_i-1]); mc_h2d_1->GetXaxis()->SetBinLabel(xsteps_i, steps[xsteps_i-1]); mc_h2d_2->GetXaxis()->SetBinLabel(xsteps_i, steps[xsteps_i-1]);
    mc_h2d_3->GetXaxis()->SetBinLabel(xsteps_i, steps[xsteps_i-1]); mc_h2d_4->GetXaxis()->SetBinLabel(xsteps_i, steps[xsteps_i-1]); mc_h2d_5->GetXaxis()->SetBinLabel(xsteps_i, steps[xsteps_i-1]);  
    mc_h2d_6->GetXaxis()->SetBinLabel(xsteps_i, steps[xsteps_i-1]); mc_h2d_7->GetXaxis()->SetBinLabel(xsteps_i, steps[xsteps_i-1]);
   }
  for( int ysteps_i=1; ysteps_i <= ysteps; ysteps_i++ ){ 
    mc_h2d_0->GetYaxis()->SetBinLabel(ysteps_i, ttch[ysteps_i-1]); mc_h2d_1->GetYaxis()->SetBinLabel(ysteps_i, ttch[ysteps_i-1]); mc_h2d_2->GetYaxis()->SetBinLabel(ysteps_i, ttch[ysteps_i-1]);
    mc_h2d_3->GetYaxis()->SetBinLabel(ysteps_i, ttch[ysteps_i-1]); mc_h2d_4->GetYaxis()->SetBinLabel(ysteps_i, ttch[ysteps_i-1]); mc_h2d_5->GetYaxis()->SetBinLabel(ysteps_i, ttch[ysteps_i-1]); 
    mc_h2d_6->GetYaxis()->SetBinLabel(ysteps_i, ttch[ysteps_i-1]); mc_h2d_7->GetYaxis()->SetBinLabel(ysteps_i, ttch[ysteps_i-1]);
  }


  for(uint i = 0; i<keys.size();i++){
    TString key = keys[i]; TDirectory * d_temp = dirsMaps[key]; 
    TDirectory * d = d_temp->mkdir("YieldsMC");
    TString id1 = key + TString(" yields mc");  (yieldsMons[key])->initMonitorForStep(id1); dirsMaps[id1]=d ;
  }


  for(uint i=0;i<keys.size();i++){
    TString key = keys[i];
    SelectionMonitor::StepMonitor_t & allMonitors = (yieldsMons[key])->getAllMonitors();
    for(SelectionMonitor::StepMonitor_t::iterator sIt = allMonitors.begin(); sIt != allMonitors.end(); sIt++){
      for(SelectionMonitor::Monitor_t::iterator hIt = sIt->second.begin(); hIt != sIt->second.end(); hIt++){     
        TDirectory *d =dirsMaps[sIt->first]; hIt->second->SetDirectory(d);
      }
    }	
  }

}




void HistogramBuilder::addHistoToMonitors( TH1 * h, TString name, map<TString, SelectionMonitor * > & v, vector<TString> & keys){
  for(uint k=0;k<keys.size();k++){ TString key=keys[k]; h->Sumw2(); (v[key])->addHistogram( h, name );}
}






void HistogramBuilder::buildNewPhysicsHistograms( TFile * outFile,  map<TString,TDirectory *> & dirsMaps , vector<TString> & keys ){

  TH1D * h;

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  h = new TH1D("inv_mass", "j;inv mass;  Entries",4000,-2000,2000);   addHistoToMonitors(h,h->GetName(),newPhysicsMons_, keys);
  h = new TH1D("inv_mass2", "j;inv mass;  Entries",4000,-2000,2000);   addHistoToMonitors(h,h->GetName(),newPhysicsMons_, keys);
  h = new TH1D("inv_mass3", "j;inv mass;  Entries",4000,-2000,2000);   addHistoToMonitors(h,h->GetName(),newPhysicsMons_, keys);
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // First Selection ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  vector<TString> objKey; 
  objKey.push_back("lep_lep"); objKey.push_back("e_e"); objKey.push_back("m_m"); objKey.push_back("tau_tau"); objKey.push_back("e_tau"); objKey.push_back("m_tau");  
  objKey.push_back("lep_tau");

  for(uint k=0;k<keys.size();k++){
    
    TString key= keys[k]; 
    TDirectory * dirObjTemp = dirsMaps[key]->mkdir("InvMassWithTaus"); 

    for(uint i=0;i<objKey.size();i++){

      TString objString = objKey[i];

      TDirectory * dirObj = dirObjTemp->mkdir(objString);

      TString extra = key + TString(" ")+objString;

      TString step1 = extra+TString(" invmass init");     newPhysicsMons_[key]->initMonitorForStep(step1);  dirsMaps[step1]= dirObj->mkdir("init");
      TString step2 = extra+TString(" invmass OS");       newPhysicsMons_[key]->initMonitorForStep(step2);  dirsMaps[step2]= dirObj->mkdir("OS");
      TString step3 = extra+TString(" invmass met");      newPhysicsMons_[key]->initMonitorForStep(step3);  dirsMaps[step3]= dirObj->mkdir("MET");
      TString step4 = extra+TString(" invmass deltaPhi"); newPhysicsMons_[key]->initMonitorForStep(step4);  dirsMaps[step4]= dirObj->mkdir("Separation");
      
    }
  }


  for(uint i=0;i<keys.size();i++){
    TString key = keys[i];
    SelectionMonitor::StepMonitor_t & allMonitors = (newPhysicsMons_[key])->getAllMonitors();
    for(SelectionMonitor::StepMonitor_t::iterator sIt = allMonitors.begin(); sIt != allMonitors.end(); sIt++){
      for(SelectionMonitor::Monitor_t::iterator hIt = sIt->second.begin(); hIt != sIt->second.end(); hIt++){     
        TDirectory *d =dirsMaps[sIt->first]; hIt->second->SetDirectory(d); 
      }
    }	
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




}




