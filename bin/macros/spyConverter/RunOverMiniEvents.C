#include "RunOverMiniEvents.h"

using namespace std;

namespace event
{

  //
  unsigned int Reader::AssignMiniEventTreeFrom(TFile *f, TString tag, TString path)
  { 
    if(f==0) return 0;
  
    m_evHandler.miniTrees[tag] = (TTree *) f->Get( path + "/" + tag );
    if(m_evHandler.miniTrees[tag]==0) return 0;

    TTree *t = m_evHandler.miniTrees[tag];
    unsigned int nEntries = t->GetEntriesFast();
    if(nEntries==0) return 0;

    t->BuildIndex("RunNb","EventNb");

    //get the new mini event structure
    if(m_evHandler.miniEvents.find(tag) == m_evHandler.miniEvents.end()) m_evHandler.miniEvents[tag] = new MiniEvent_t;
    MiniEvent_t *ev = m_evHandler.miniEvents[tag];

    //assign the addresses to the tree  
    ev->iRun=0;               t->GetBranch("RunNb")->SetAddress(&ev->iRun);
    ev->iEvent=0;             t->GetBranch("EventNb")->SetAddress(&ev->iEvent);
    ev->iLumi=0;              t->GetBranch("Lumi")->SetAddress(&ev->iLumi);
    ev->eventClassif=0;       t->GetBranch("EventClassification")->SetAddress(&ev->eventClassif);
    ev->triggerColl=0;        t->GetBranch("Trigger")->SetAddress(&ev->triggerColl);
    ev->vertexColl=0;         t->GetBranch("Vertices")->SetAddress(&ev->vertexColl);
    ev->leptonsColl=0;        t->GetBranch("Leptons")->SetAddress(&ev->leptonsColl);
    ev->leptonsClassifColl=0; t->GetBranch("LeptonInfo")->SetAddress(&ev->leptonsClassifColl);
    ev->photonsColl=0;        t->GetBranch("Photons")->SetAddress(&ev->photonsColl);
    ev->photonsClassifColl=0; t->GetBranch("PhotonsInfo")->SetAddress(&ev->photonsClassifColl);
    ev->jetsColl=0;           t->GetBranch("Jets")->SetAddress(&ev->jetsColl);
    ev->jetsClassifColl=0;    t->GetBranch("JetsInfo")->SetAddress(&ev->jetsClassifColl);
    ev->metColl=0;            t->GetBranch("MET")->SetAddress(&ev->metColl);
    ev->metClassifColl=0;     t->GetBranch("METInfo")->SetAddress(&ev->metClassifColl);

    return nEntries;
  }

  //
  MiniEvent_t *Reader::GetNewMiniEvent(unsigned int ientry,TString tag)
  {
    //check if structures have been created
    if( m_evHandler.miniTrees.find(tag) == m_evHandler.miniTrees.end()
	|| m_evHandler.miniEvents.find(tag) == m_evHandler.miniEvents.end() ) return 0;

    TTree *t = m_evHandler.miniTrees[tag];
    MiniEvent_t *ev = m_evHandler.miniEvents[tag];
    if(t==0 || ev==0) return 0;

    //check number of entries in tree
    if(ientry >= t->GetEntriesFast()) return 0;

    //fill the mini event
    t->GetEntry(ientry);
    return ev;
  }

  //
  PhysicsObjectCollection Reader::GetPhysicsObjectsFrom(MiniEvent_t *ev, unsigned int type, unsigned int algo, bool sortObjects)
  {
    PhysicsObjectCollection objColl;
    if(ev==0) return objColl;
  
    TClonesArray *coll = ev->leptonsColl, *info = ev->leptonsClassifColl;
    if(type==QUARK || type == JET) { coll = ev->jetsColl; info = ev->jetsClassifColl; }
    if(type==SUPERCLUSTER || type==PHOTON) { coll = ev->photonsColl; info = ev->photonsClassifColl; }
    if(type == MET) { coll = ev->metColl;  info = ev->metClassifColl; }
    if(coll==0) return objColl;

    //build the collection of physics objects
    for(int i=0; i< coll->GetEntriesFast(); i++)
      {
	if( (type==ELECTRON || type==MUON ) )
	  {
	    if( fabs((*(TVectorD *)info->At(i))[0]) != type ) continue;
	    unsigned int lepAlgo = int(fabs((*(TVectorD *)info->At(i))[3]));
	    lepAlgo = ((lepAlgo >> 8) & 0x1);
	    if(lepAlgo != algo) continue;
	  }
	
	if(type==TAU && fabs((*(TVectorD *)info->At(i))[0]) != type ) continue;      
	if( (type==PHOTON || type==SUPERCLUSTER) && fabs((*(TVectorD *)info->At(i))[0]) != type ) continue;      
	if( type==TRACK && fabs((*(TVectorD *)info->At(i))[0]) > 2 ) continue;
	else if(type==JET && fabs((*(TVectorD *)info->At(i))[3]) != algo) continue;
	else if(type==MET && fabs((*(TVectorD *)info->At(i))[0]) != algo) continue;
	objColl.push_back( PhysicsObject( coll->At(i), (info !=0 ? info->At(i) : 0) ) );
      }  
    
    if(sortObjects) sort(objColl.begin(),objColl.end(),PhysicsObject::PtOrder);
  
    return objColl;
  }

  
  //
  PhysicsObjectCollection Reader::GetVertexCollection(event::MiniEvent_t *ev, bool sortObjects)
  {
    PhysicsObjectCollection vtxColl;
    if(ev==0) return vtxColl;  
    if(ev->vertexColl==0) return vtxColl;
    
    //build the collection of physics objects
    for(int i=0; i< ev->vertexColl->GetEntriesFast(); i++)
      {
	TVectorD *info = (TVectorD *) ev->vertexColl->At(i);
	TLorentzVector vtxMom(0,0,0,0);
	vtxMom.SetPtEtaPhiE((*info)[1],0.,0.,(*info)[1]);
	vtxColl.push_back( PhysicsObject(&vtxMom, info) );
      }  
    if(sortObjects) sort(vtxColl.begin(),vtxColl.end(),PhysicsObject::PtOrder); 
    return vtxColl;
  }

  
  //
  PhysicsObjectPairCollection Reader::GetDileptonsWithArbitration(PhysicsObjectCollection &origElectrons,
								  PhysicsObjectCollection &origMuons,
								  unsigned int dilArbitration,
								  bool hasEGtrig, bool hasMuTrig)
    
  {
    PhysicsObjectPairCollection dileptons;
    
    //filter the leptons according to the arbitration
    PhysicsObjectCollection electrons;
    for(PhysicsObjectCollection::iterator eIt = origElectrons.begin();
	eIt != origElectrons.end();
	eIt++)
      {
	if( HasArbitration(dilArbitration,event::Reader::ID) && !ElectronId( *eIt ) ) continue;
	if( HasArbitration(dilArbitration,event::Reader::ISO) && !ElectronIso( *eIt) ) continue;
	electrons.push_back( *eIt );
	
	//add ee dileptons
	PhysicsObjectCollection::iterator newEit = eIt; newEit++;
	for( ; newEit != origElectrons.end(); newEit++ )
	  {
	    if(eIt==newEit) continue;
	    bool sameCharge( ((*newEit)[0])*((*eIt)[0])>0 );	    
	    if(HasArbitration(dilArbitration,event::Reader::OS) && sameCharge) continue;
	    if(!hasEGtrig) continue;
	    dileptons.push_back( PhysicsObjectPair(*newEit, *eIt ) );
	  }
      }
    PhysicsObjectCollection muons;
    for(PhysicsObjectCollection::iterator mIt = origMuons.begin();
	mIt != origMuons.end();
	mIt++)
      {
	if(HasArbitration(dilArbitration,event::Reader::ID) && !MuonId( *mIt ) ) continue;
	if(HasArbitration(dilArbitration,event::Reader::ISO) && !MuonIso( *mIt) ) continue;
	muons.push_back( *mIt );
	
	//add mumu dileptons
	PhysicsObjectCollection::iterator newMit = mIt; newMit++;
	for( ; newMit != origMuons.end(); newMit++ )
	  {	 
	    bool sameCharge( ((*newMit)[0])*((*mIt)[0])>0 );
	    if(HasArbitration(dilArbitration,event::Reader::OS) && sameCharge) continue;
	    if(!hasMuTrig) continue;
	    dileptons.push_back( PhysicsObjectPair(*newMit, *mIt ) );
	  }
	
	//add emu dileptons
	for(PhysicsObjectCollection::iterator newEit = electrons.begin();
	    newEit != electrons.end();
	    newEit++)
	  {
	    bool sameCharge( ((*newEit)[0])*((*mIt)[0])>0 );
	    if(HasArbitration(dilArbitration,event::Reader::OS) && sameCharge) continue;
	    if( !hasEGtrig && !hasMuTrig) continue;
	    dileptons.push_back( PhysicsObjectPair(*newEit, *mIt ) );
	  }
      }

    //sort by sum pt
    sort( dileptons.begin(), dileptons.end(), PhysicsObjectPair::SumPtOrder);
    
    return dileptons;
  }
  
  //
  bool Reader::ElectronIsGood(PhysicsObject &electron)
  {
    if( electron.GetInfo()==0 ) return false;
    bool isPrompt( fabs(electron[25])<0.04);
    bool isECALdriven( (int(electron[3]) & 0x1) );
    int vbtf90=int(electron[13]);
    bool isNotFromConversion = ((vbtf90 >> 2) & 0x1);
    bool isawayFromMuons( fabs(electron[14])>0.1 );
    //return (isPrompt && isECALdriven && isNotFromConversion && isawayFromMuons);
    return (isPrompt && isNotFromConversion && isawayFromMuons);

  }

  //
  bool Reader::LeptonId(PhysicsObject &lepton)
  {
    if(fabs(lepton[0]) == 11 )  return ElectronId( lepton );
    else if( fabs(lepton[0]) == 13 ) return MuonId( lepton );
    else return false;
  }

  //
  bool Reader::LeptonIso(PhysicsObject &lepton)
  {
    if(fabs(lepton[0]) == 11 )  return ElectronIso( lepton );
    else if( fabs(lepton[0]) == 13 ) return MuonIso( lepton );
    else return false;
  }

  //
  bool Reader::ElectronId(PhysicsObject &electron,int wp)
  {
    return(true);
    if( electron.GetInfo()==0 ) return false;
    bool isGood( ElectronIsGood(electron) );
    size_t bitsel=7;
    if(wp==80) bitsel=6;
    else if(wp==70) bitsel=5;
    else if(wp==95) bitsel=8;
    else if(wp==85) bitsel=9;
    bool hasId( ( int(electron[12]) >> bitsel) & 0x1  );
    return (isGood && hasId);


//    if( electron.GetInfo()==0 ) return false;
//
//    unsigned int lepAlgo = (unsigned int)(electron[3]);
//    lepAlgo = ((lepAlgo >> 8) & 0x1);
//
//    bool isGood( ElectronIsGood(electron) );
//    size_t bitsel=7;
//    if(wp==80) bitsel=6;
//    else if(wp==70) bitsel=5;
//    else if(wp==95) bitsel=8;
//    else if(wp==85) bitsel=9;
//    // loose working points (CIC cut in categories ids)
//    else if(wp== 0) { if(lepAlgo==STDLEPTON){ bitsel=0;} else bitsel=1;}
//
//    // tight working points (CIC cut in categories ids)
//    else if(wp== 1) { if(lepAlgo==STDLEPTON){ bitsel=1;} else bitsel=3;}   
//
//    bool hasId( ( int(electron[12]) >> bitsel) & 0x1  );
//    return (isGood && hasId);
  }
  
  //
  bool Reader::ElectronIso(PhysicsObject &electron)
  {
    if( electron.GetInfo()==0 ) return false;
    //if( electron.GetInfo()->GetNoElements() < 19) return false;
    return (electron[18]<0.15 && electron[14]>0.1);
  }

  //
  bool Reader::MuonIsGood(PhysicsObject &muon)
  {
    bool isGlobalAndTracker( (int(muon[3])>>1 & 0x3) == 3);
    bool isPrompt( fabs(muon[11])<0.02 ); 
    return (isPrompt && isGlobalAndTracker);
  }

  //
  bool Reader::MuonId(PhysicsObject &muon)
  {
    if( muon.GetInfo()==0 ) return false;
    bool isGood( MuonIsGood(muon) );
    bool hasGoodTrack(muon[5]>10 && muon[4]<10); //chi2+nhits (inner track)    
    bool hasId( (int(muon[10])>>7) & 0x1 );           //global muon prompt tight
    return (isGood && hasGoodTrack && hasId);
  }

  //
  bool Reader::MuonIso(PhysicsObject &muon)
  {
    if( muon.GetInfo()==0 ) return false;
    return muon[18]<0.15;
  }

  //
  bool Reader::DileptonIsGood(PhysicsObjectPair &dilepton)
  {
    if( dilepton.M()<12 ) return false;
    if( dilepton.Type() == "emu" )
      {
	double mueta( fabs(dilepton[0][0])==13 ? dilepton[0].Eta() : dilepton[1].Eta());
	double elescet( fabs(dilepton[0][0])==13 ? dilepton[1][38] : dilepton[0][38] );
	if(fabs(mueta)>2.1 && elescet<17) return false;	
      }
    if( dilepton.Type() == "mumu" && fabs(dilepton[0].Eta())>2.1 && fabs(dilepton[1].Eta())>2.1) return false;
    return true;
  }  

  //
  PhysicsObjectCollection Reader::PrunePhysicsObjectsFor(PhysicsObjectCollection &origObjects, PhysicsObjectCollection &otherObjects,double minDRallowed)
  {
    PhysicsObjectCollection cleanObjects;
    for(size_t iobj=0; iobj<origObjects.size(); iobj++)
      {
	double minDR(1000);
	for(size_t jobj=0; jobj<otherObjects.size(); jobj++)
	  {
	    double dR = origObjects[iobj].DeltaR( otherObjects[jobj] );
	    if(dR<minDR) minDR=dR;
	  }
	if(minDR<minDRallowed) continue;
	cleanObjects.push_back(origObjects[iobj]);
      }
    return cleanObjects;
  }

  //
  PhysicsObjectCollection Reader::GetJetsWithArbitration( PhysicsObjectCollection &origJets, unsigned int jetArbitration)
  {
    PhysicsObjectCollection arbitratedJets;
    double fassocmin(0.);
    for(PhysicsObjectCollection::iterator jIt = origJets.begin(); jIt != origJets.end(); jIt++)
      {
	if( HasArbitration(jetArbitration,event::Reader::VTXCONSTRAINED) && fabs((*jIt)[16])<=fassocmin ) continue;
	TLorentzVector jetMom(*jIt);
	if( HasArbitration(jetArbitration,event::Reader::RESJECCORR)) jetMom *= (*jIt)[9];
	else if( HasArbitration(jetArbitration,event::Reader::PARTONFLAVCORR)) jetMom *= (*jIt)[10];
	arbitratedJets.push_back( PhysicsObject(&jetMom,jIt->GetInfo()) );
      }
    if(HasArbitration(jetArbitration,event::Reader::BTAGORDERED) ) sort(arbitratedJets.begin(),arbitratedJets.end(),PhysicsObject::BtagOrder);
    return arbitratedJets;
  }
}
