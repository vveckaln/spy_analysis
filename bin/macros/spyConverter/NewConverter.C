/**
    .L RunOverMiniEvents.C+  
    gSystem->Load("libLIPTop.so");
    .L NewConverter.C+
*/

#include "LIP/Top/interface/EventSummaryHandler.h"
#include "RunOverMiniEvents.h"
#include "TDirectory.h"

//
void convert(TString url)
{
  bool debug(false);

  //For single top and dibosons we do not have PF objects /////////////////////////////////////////////////////
  bool pf(true);
  //  if( url.Contains("singletop") ){ pf = false;}  
  uint jetType, leptonType,tauType,metType;
  //  if( !pf ) { 
  //    jetType=event::AK5PF; leptonType=event::STDLEPTON; tauType = event::HPSTAU; metType =event::PF;
  //  }else{
  jetType=event::AK5PFLOW, leptonType=event::PFLOWLEPTON; tauType =event::PFLOWTAU; metType=event::PFLOWMET;
  //  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////



  top::EventSummaryHandler summaryHandler_;
  
  TString newurl(url);
  //  newurl.ReplaceAll("_","");
  newurl.ReplaceAll("root","");
  newurl.ReplaceAll(".","");
  //newurl.ReplaceAll("spy3","");
  newurl = "summaries/"+newurl+".root";
  TFile *fout = TFile::Open(newurl,"RECREATE");
  TDirectory *dir=fout->mkdir("evAnalyzer");
  dir->cd();
  TTree *tree = new TTree("data","Event Summary");
  summaryHandler_.initTree(tree);
  tree->SetDirectory(dir);
  
  event::Reader reader;                                                                                                                                                                                       
  TFile *f = TFile::Open(url);
  unsigned int nsel(0);
  unsigned int nEntries = reader.AssignMiniEventTreeFrom(f);                                                                                                                                                  
  for(unsigned int i=0; i<nEntries; i++)                                            
    {

  
      //analyze new mini event                                                                                                                                                      
      event::MiniEvent_t *ev = reader.GetNewMiniEvent(i);

      //the met
      //std::vector<PhysicsObject> mets = reader.GetPhysicsObjectsFrom(ev,event::MET,event::PF);  
      std::vector<PhysicsObject> mets = reader.GetPhysicsObjectsFrom(ev,event::MET,metType);
      std::vector<PhysicsObject> vertices                 = reader.GetVertexCollection(ev);

      if(vertices.size()==0) continue;
      if(debug) cout << "nvertices: " << vertices.size() << endl;
      PhysicsObject & vertex = vertices[0];
      
      //choose the leading lepton
      //std::vector<PhysicsObject> allelectrons = reader.GetPhysicsObjectsFrom(ev,event::ELECTRON,event::PFLOWLEPTON);  
      std::vector<PhysicsObject> allelectrons = reader.GetPhysicsObjectsFrom(ev,event::ELECTRON,leptonType);  
      std::vector<PhysicsObject>  electrons;
      for(std::vector<PhysicsObject>::iterator e = allelectrons.begin(); e!=allelectrons.end(); e++)
	{
	  double scEta    = (*e)[39];  
	  double eEta     = TMath::Abs(e->Eta()); 
	  double eEt      = TMath::Abs(e->Et());
	  double ePt      = TMath::Abs(e->Pt());
	  double eRelIso  = (*e)[18];
	  double minDR2mu = (*e)[14];
	  double d0       = fabs((*e)[25]);
	  //bool   id       = reader.ElectronId((PhysicsObject &)(*e));  

          // Check method at the end of file
          bool   id       = reader.ElectronId((PhysicsObject &)(*e), 1);

          if(pf){
            double zvertex   = vertex[7];
            double zelectron = (*e)[29];
            bool   z(false); 
            if ( fabs(zvertex-zelectron) < 1. ){ z = true;}
	    if(debug) cout << "electron z " << z  << endl;
            if(!z){ continue;}
          }else{
            id=reader.ElectronId((PhysicsObject &)(*e), 5);
            double rho      = (*e)[44];
            double ecalIso  = (*e)[16];
            double hcalIso  = (*e)[17] - ecalIso;
            if( fabs(scEta) < 1.4442 ){ if( (ecalIso-1)<0 ) ecalIso = 0; }
            double trackIso = (*e)[15];
	    //            if( eEt ){ eRelIso = (ecalIso+hcalIso+trackIso - 0.24*rho) /eEt; }
           
            // we apply W70 l+jets requirements ///////
            int id70( ( ((int)(*e)[12] ) >> 5) & 0x1 ); 
	    //            id = id70 && id;
            ////////////////////////////////////////////
          }
      


	  if( fabs(scEta)> 1.4442 && fabs(scEta)<1.5660 ){ continue; }
	  if(eEt < 15 ){ continue; }
	  if(ePt < 35) { continue; }

          // NOTE eRelIso was set to 0.17 but will move to 0.1 in next itetation
	  if(id && eEta <2.5 && eRelIso < 0.15 && minDR2mu >0.1){ electrons.push_back(*e);}
	  if(debug) cout << "electrons: " << electrons.size() << endl;
	  if(debug) cout << "id: " << id << ", eEta: " << eEta << ", eRelIso: " << eRelIso << ", minDR2mu: " << minDR2mu << endl;

	}
      
      //std::vector<PhysicsObject> allmuons = reader.GetPhysicsObjectsFrom(ev,event::MUON,event::PFLOWLEPTON);  
      std::vector<PhysicsObject> allmuons = reader.GetPhysicsObjectsFrom(ev,event::MUON,leptonType);  
      std::vector<PhysicsObject>  muons;
      for(std::vector<PhysicsObject>::iterator m = allmuons.begin(); m!=allmuons.end(); m++)
	{
    
          double mEta     = TMath::Abs(m->Eta());
          double mPt      = TMath::Abs(m->Pt());
          double mD0      = fabs((*m)[11]);
          double mRelIso  = (*m)[18];

          bool id = reader.MuonId((PhysicsObject &) (*m));     

          if(pf)
	    {
	      double zvertex   = vertex[7];
	      double zmuon     = (*m)[23];
	      bool   z(false); 
	      if ( fabs(zvertex-zmuon) < 1 ){ z = true;}
	      if(!z){ continue;}
	    }
	  else
	    {
	      double rho       = (*m)[26];
	      double ecalIso   = (*m)[16];
	      double hcalIso   = (*m)[17] - ecalIso;
	      double trackIso  = (*m)[15];
	      if( mPt )
		{
		  mRelIso = (ecalIso+hcalIso+trackIso -  0.112*rho) /mPt;
		}
	    }
	  
          if(id && mD0 < 0.02 &&  mPt > 30 && mEta < 2.1 && mRelIso < 0.2){ muons.push_back(*m);}        
	  if(debug) cout << "muons: " << muons.size() << endl;
        }
      
      
      //leading lepton
      if(electrons.size()+muons.size()==0) 
	{
	  cout << "no leptons.." << allelectrons.size() << " " << allmuons.size() << endl;
	  continue;
	}
      PhysicsObject leadLepton;
      if( muons.size() ) leadLepton=muons[0];
      else leadLepton=electrons[0];
      if( electrons.size() && muons.size() && electrons[0].Pt()>muons[0].Pt()) leadLepton=electrons[0];

      //leading tau candidate
      //std::vector<PhysicsObject> alltaus = reader.GetPhysicsObjectsFrom(ev,event::TAU);
      std::vector<PhysicsObject> alltaus = reader.GetPhysicsObjectsFrom(ev,event::TAU,tauType);
      std::vector<PhysicsObject> taus; 
      for(std::vector<PhysicsObject>::iterator t = alltaus.begin(); t != alltaus.end(); t++)
	{
	  double tauEta            = TMath::Abs(t->Eta());
	  double tauPt             = TMath::Abs(t->Pt());
	  double tauEmf            = (*t)[15];
	  int    type              = (int) (*t)[17]; 

	  if(type != int(tauType) ) continue;

	  double tau_dis_muon      = (*t)[21];
	  double tau_dis_electron  = (*t)[22];
	  
	  if(pf){
	    double zvertex   = vertex[7];
	    double ztau     = (*t)[30];
	    bool   z(false);
	    if ( fabs(zvertex-ztau) < 1 ){ z = true; }
	    if(!z){ continue;}
	    
	    int HPS_ISO_ = 26; //medium iso
	    int discByIsolation = ((int) (*t)[HPS_ISO_]) & 0x1;
	    
	    if( discByIsolation ==0){  continue; }}
	  	  
	  if( tau_dis_muon == 0 || tau_dis_electron < 2 || tauPt < 20 || fabs(tauEta) > 2.4 ) continue;
	  

	  taus.push_back(*t);
	}
      if(taus.size()==0) continue;


      //clean up the jets
      //std::vector<PhysicsObject> alljets = reader.GetPhysicsObjectsFrom(ev,event::JET,event::AK5PFLOW);  
      std::vector<PhysicsObject> alljets = reader.GetPhysicsObjectsFrom(ev,event::JET,jetType);  
      std::vector<PhysicsObject> jets;
      for(std::vector<PhysicsObject>::iterator j=alljets.begin(); j != alljets.end(); j++)
	{
	  double jetEta     = TMath::Abs(j->Eta());
	  double jetPt      = TMath::Abs(j->Pt());
	  double minDRToLep = TMath::Abs((*j)[15]);
	  if( minDRToLep < 0.3 ) continue;
	  if(jetPt > 30 && jetEta < 2.4 ){ jets.push_back(*j); }
	}
      if(jets.size()==0) continue;
      nsel++;

      //
      TVectorD *classif = (TVectorD *)ev->eventClassif->At(0);
      summaryHandler_.evSummary_.run=ev->iRun;
      summaryHandler_.evSummary_.lumi=ev->iLumi;
      summaryHandler_.evSummary_.event=ev->iEvent;
      summaryHandler_.evSummary_.cat=(electrons.size() ? 4 : 5);
      summaryHandler_.evSummary_.isSignal=false;
      if(url.Contains("ttbar") && int((*classif)[0])==9) summaryHandler_.evSummary_.isSignal=true; 
      summaryHandler_.evSummary_.weight=(*classif)[4];
      summaryHandler_.evSummary_.nvtx=1;
      summaryHandler_.evSummary_.ngenpu=0;
      summaryHandler_.evSummary_.ngenootpum1=0;
      summaryHandler_.evSummary_.ngenootpup1=0;
      summaryHandler_.evSummary_.rho=0;//rho;
      summaryHandler_.evSummary_.nparticles=electrons.size()+taus.size()+muons.size()+jets.size()+1;
      summaryHandler_.evSummary_.nmcparticles=0;
      summaryHandler_.evSummary_.nmeasurements=0;

      //save the jets
      int pidx(0);
      for(size_t ijet=0; ijet<jets.size(); ijet++,pidx++)
	{
	  summaryHandler_.evSummary_.px[pidx]    = jets[ijet].Px();
	  summaryHandler_.evSummary_.py[pidx]    = jets[ijet].Py();
	  summaryHandler_.evSummary_.pz[pidx]    = jets[ijet].Pz();
	  summaryHandler_.evSummary_.en[pidx]    = jets[ijet].E();
	  summaryHandler_.evSummary_.id[pidx]    = 1;
	  summaryHandler_.evSummary_.genid[pidx] = jets[ijet][1];
	  summaryHandler_.evSummary_.info1[pidx] = jets[ijet][12];
	  summaryHandler_.evSummary_.info2[pidx] = jets[ijet][13];
	  summaryHandler_.evSummary_.info3[pidx] = jets[ijet][14];
	}

      //save met
      summaryHandler_.evSummary_.px[pidx]    = mets[0].Px();
      summaryHandler_.evSummary_.py[pidx]    = mets[0].Py();
      summaryHandler_.evSummary_.pz[pidx]    = mets[0].Pz();
      summaryHandler_.evSummary_.en[pidx]    = mets[0].E();
      summaryHandler_.evSummary_.id[pidx]    = 0;
      summaryHandler_.evSummary_.genid[pidx] = -9999;
      pidx++;

      //save the leptons
      std::vector<std::vector<PhysicsObject> > allleptons;
      allleptons.push_back( electrons );
      allleptons.push_back( muons );
      allleptons.push_back( taus );
      for(std::vector< std::vector<PhysicsObject> >::iterator it=allleptons.begin(); it!=allleptons.end(); it++)
	{
	  for(std::vector<PhysicsObject>::iterator l = it->begin(); l!=it->end(); l++,pidx++)
	    {
	      summaryHandler_.evSummary_.px[pidx]=(*l).Px();
	      summaryHandler_.evSummary_.py[pidx]=(*l).Py();
	      summaryHandler_.evSummary_.pz[pidx]=(*l).Pz();
	      summaryHandler_.evSummary_.en[pidx]=(*l).E();
	      summaryHandler_.evSummary_.id[pidx] = (*l)[0];
	      summaryHandler_.evSummary_.genid[pidx] = (*l)[1];
	      summaryHandler_.evSummary_.info1[pidx]=(*l)[15];
	      summaryHandler_.evSummary_.info2[pidx]=(*l)[16];
	      summaryHandler_.evSummary_.info3[pidx]=(*l)[17];
	    }
	} 
      summaryHandler_.fillTree();
    }
  
      

  cout << "Selected " << nsel << "/" << nEntries << " from " << url << endl;

  f->Close();  
       
  tree->SetDirectory(dir);
  //  tree->Write();
  fout->Write();
  fout->Close();
}


//
void convertAll()
{
//   TString files[]={
//     "spyevents-mutau/out-Muon_spy.root",
//     "spyevents-mutau/out-dy_from50.root_spy_mutau.root",
//     "spyevents-mutau/out-stop-DR_tW.root_spy_mutau.root",   
//     "spyevents-mutau/out-stopbar-DR_tW.root_spy_mutau.root",    
//     "spyevents-mutau/out-stop_s.root_spy_mutau.root",           
//     "spyevents-mutau/out-stopbar_s.root_spy_mutau.root",        
//     "spyevents-mutau/out-stop_t.root_spy_mutau.root",           
//     "spyevents-mutau/out-stopbar_t.root_spy_mutau.root",        
//     "spyevents-mutau/out-ttbar_etau.root_spy_mutau.root",       
//     "spyevents-mutau/out-ttbar_mutau.root_spy_mutau.root",
//     "spyevents-mutau/out-ttbar_other.root_spy_mutau.root",        
//     "spyevents-mutau/out-wjets.root_spy_mutau.root",
//     "spyevents-mutau/out-ww.root_spy_mutau.root",
//     "spyevents-mutau/out-wz.root_spy_mutau.root",
//     "spyevents-mutau/out-zz.root_spy_mutau.root"
//   };

//   TString files[]={
//     "spyevents-etau/out-Ele_spy.root", 
//     "spyevents-etau/out-dy_from50.root_spy_eltau.root",      
//     "spyevents-etau/out-stop-DR_tW.root_spy_eltau.root",     
//     "spyevents-etau/out-stop_s.root_spy_eltau.root",         
//     "spyevents-etau/out-stop_t.root_spy_eltau.root",         
//     "spyevents-etau/out-stopbar-DR_tW.root_spy_eltau.root",  
//     "spyevents-etau/out-stopbar_s.root_spy_eltau.root",      
//     "spyevents-etau/out-stopbar_t.root_spy_eltau.root",      
//     "spyevents-etau/out-ttbar.root_spy_eltau.root",          
//     "spyevents-etau/out-ttbar_etau.root_spy_eltau.root",     
//     "spyevents-etau/out-ttbar_mutau.root_spy_eltau.root",    
//     "spyevents-etau/out-wjets.root_spy_eltau.root",
//     "spyevents-etau/out-ttbar_other.root_spy_eltau.root",           
//     "spyevents-etau/out-ww.root_spy_eltau.root",
//     "spyevents-etau/out-wz.root_spy_eltau.root",
//     "spyevents-etau/out-zz.root_spy_eltau.root"
//   };

//  TString files[]={
//    "etau_kin2_Jan2012/DY_M50.root",   
//    "etau_kin2_Jan2012/SingleElectron.root", 
//    "etau_kin2_Jan2012/SingleT_t.root",
//    "etau_kin2_Jan2012/SingleTbar_t.root",  
//    "etau_kin2_Jan2012/TTJets_161p5.root", 
//    "etau_kin2_Jan2012/TTJets_169p5.root", 
//    "etau_kin2_Jan2012/TTJets_178p5.root",  
//    "etau_kin2_Jan2012/TTJets_other.root",   
//    "etau_kin2_Jan2012/WJets.root",  
//    "etau_kin2_Jan2012/WZ.root",
//    "etau_kin2_Jan2012/SingleTbar_s.root",  
//    "etau_kin2_Jan2012/SingleT_s.root", 
//    "etau_kin2_Jan2012/SingleT_tW.root",  
//    "etau_kin2_Jan2012/SingleTbar_tW.root",  
//    "etau_kin2_Jan2012/TTJets_166p5.root", 
//    "etau_kin2_Jan2012/TTJets_175p5.root",  
//    "etau_kin2_Jan2012/TTJets_184p5.root",  
//    "etau_kin2_Jan2012/TTJets_signal.root",  
//    "etau_kin2_Jan2012/WW.root",     
//    "etau_kin2_Jan2012/ZZ.root"
//  };
  TString files[]={
    "conversion/spyDir/spy-Electron.root",   
    "conversion/spyDir/spy-singletop.root", 
    "conversion/spyDir/spy-ttbar-ddbkg.root",
    "conversion/spyDir/spy-ttbar-etau.root",  
    "conversion/spyDir/spy-ttbar-mcbkg.root", 
    "conversion/spyDir/spy-zjets.root", 
  };
  
//   TString files[]={
//     "kin_Jan2012/out-Muon_spy.root",                          
//     "kin_Jan2012/out-stop-DR_tW.root_spy_mutau.root",    
//     "kin_Jan2012/out-stop_s.root_spy_mutau.root",
//     "kin_Jan2012/out-stop_t.root_spy_mutau.root",
//     "kin_Jan2012/out-ttbar.root_spy_mutau.root",
//     "kin_Jan2012/out-stopbar-DR_tW.root_spy_mutau.root", 
//     "kin_Jan2012/out-ttbar_etau.root_spy_mutau.root",
//     "kin_Jan2012/out-stopbar_s.root_spy_mutau.root",    
//     "kin_Jan2012/out-ttbar_mutau.root_spy_mutau.root",
//     "kin_Jan2012/out-dy_from50.root_spy_mutau.root",
//     "kin_Jan2012/out-stopbar_t.root_spy_mutau.root",      
//     "kin_Jan2012/out-ttbar_other.root_spy_mutau.root",
//     "kin_Jan2012/out-ttbar-m161p5.root_spy_mutau.root",
//     "kin_Jan2012/out-ttbar-m161p5_etau.root_spy_mutau.root",  
//     "kin_Jan2012/out-ttbar-m161p5_mutau.root_spy_mutau.root", 
//     "kin_Jan2012/out-ttbar-m161p5_other.root_spy_mutau.root", 
//     "kin_Jan2012/out-ttbar-m166p5.root_spy_mutau.root",
//     "kin_Jan2012/out-ttbar-m166p5_etau.root_spy_mutau.root",  
//     "kin_Jan2012/out-ttbar-m166p5_mutau.root_spy_mutau.root", 
//     "kin_Jan2012/out-ttbar-m166p5_other.root_spy_mutau.root", 
//     "kin_Jan2012/out-ttbar-m169p5.root_spy_mutau.root",       
//     "kin_Jan2012/out-ttbar-m169p5_etau.root_spy_mutau.root",  
//     "kin_Jan2012/out-ttbar-m169p5_mutau.root_spy_mutau.root", 
//     "kin_Jan2012/out-ttbar-m169p5_other.root_spy_mutau.root", 
//     "kin_Jan2012/out-ttbar-m175p5.root_spy_mutau.root",       
//     "kin_Jan2012/out-ttbar-m175p5_etau.root_spy_mutau.root",  
//     "kin_Jan2012/out-ttbar-m175p5_mutau.root_spy_mutau.root", 
//     "kin_Jan2012/out-ttbar-m175p5_other.root_spy_mutau.root", 
//     "kin_Jan2012/out-ttbar-m178p5.root_spy_mutau.root",       
//     "kin_Jan2012/out-ttbar-m178p5_etau.root_spy_mutau.root",  
//     "kin_Jan2012/out-ttbar-m178p5_mutau.root_spy_mutau.root", 
//     "kin_Jan2012/out-ttbar-m178p5_other.root_spy_mutau.root", 
//     "kin_Jan2012/out-ttbar-m184p5.root_spy_mutau.root",       
//     "kin_Jan2012/out-ttbar-m184p5_etau.root_spy_mutau.root",
//     "kin_Jan2012/out-ttbar-m184p5_mutau.root_spy_mutau.root",
//     "kin_Jan2012/out-ttbar-m184p5_other.root_spy_mutau.root", 
//     "kin_Jan2012/out-qcd_30to80_BCtoE.root_spy_mutau.root",   
//     "kin_Jan2012/out-wjets.root_spy_mutau.root",
//     "kin_Jan2012/out-qcd_80to170_BCtoE.root_spy_mutau.root",   
//     "kin_Jan2012/out-ww.root_spy_mutau.root",
//     "kin_Jan2012/out-wz.root_spy_mutau.root",
//     "kin_Jan2012/out-qcd_EM_Pt30to80.root_spy_mutau.root",    
//     "kin_Jan2012/out-qcd_EM_Pt80to170.root_spy_mutau.root",  
//     "kin_Jan2012/out-zz.root_spy_mutau.root",
//     "kin_Jan2012/out-qcdmu15_20toinf.root_spy_mutau.root"};


  const size_t nfiles=sizeof(files)/sizeof(TString);
  for(size_t ifile=0; ifile<nfiles; ifile++)   convert(files[ifile]);
}

