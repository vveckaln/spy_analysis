#include "lipcms/EventSelection/interface/MiniTree.hh"


namespace event
{

  //
  void Handler::CreateNewMiniTree(TTree *tree, TString tag)
  {
    if(tree==0) return;
      
    miniEvents[tag] = new event::MiniEvent_t;
    miniEvents[tag]->eventClassif = new TClonesArray("TVectorD", 5);
    miniEvents[tag]->triggerColl = new TClonesArray("TVectorD", 5);
    miniEvents[tag]->vertexColl = new TClonesArray("TVectorD", 10);
    miniEvents[tag]->leptonsColl = new TClonesArray("TLorentzVector", 25);
    miniEvents[tag]->leptonsClassifColl = new TClonesArray("TVectorD", 25);
    miniEvents[tag]->jetsColl = new TClonesArray("TLorentzVector", 30);
    miniEvents[tag]->jetsClassifColl = new TClonesArray("TVectorD", 30);
    miniEvents[tag]->metColl = new TClonesArray("TLorentzVector", 10);
    miniEvents[tag]->metClassifColl = new TClonesArray("TVectorD", 10);
    miniEvents[tag]->photonsColl = new TClonesArray("TLorentzVector", 10);
    miniEvents[tag]->photonsClassifColl = new TClonesArray("TVectorD", 10);
     
    miniTrees[tag] =  tree;
    miniTrees[tag]->Branch("RunNb",                     &miniEvents[tag]->iRun, "iRun/I");
    miniTrees[tag]->Branch("EventNb",                   &miniEvents[tag]->iEvent, "iEvent/I");
    miniTrees[tag]->Branch("Lumi",                      &miniEvents[tag]->iLumi, "iLumi/I");
    miniTrees[tag]->Branch("EventClassification",       &miniEvents[tag]->eventClassif, 128000,0);
    miniTrees[tag]->Branch("Trigger",       &miniEvents[tag]->triggerColl, 128000,0);
    miniTrees[tag]->Branch("Vertices",       &miniEvents[tag]->vertexColl, 128000,0);
    miniTrees[tag]->Branch("Leptons", "TClonesArray",   &miniEvents[tag]->leptonsColl, 128000, 0);
    miniTrees[tag]->Branch("LeptonInfo", "TClonesArray",&miniEvents[tag]->leptonsClassifColl, 128000,0);
    miniTrees[tag]->Branch("Photons", "TClonesArray",   &miniEvents[tag]->photonsColl, 128000, 0);
    miniTrees[tag]->Branch("PhotonsInfo", "TClonesArray",&miniEvents[tag]->photonsClassifColl, 128000,0);
    miniTrees[tag]->Branch("Jets", "TClonesArray",      &miniEvents[tag]->jetsColl, 128000, 0);
    miniTrees[tag]->Branch("JetsInfo", "TClonesArray",  &miniEvents[tag]->jetsClassifColl, 128000, 0);
    miniTrees[tag]->Branch("MET", "TClonesArray",       &miniEvents[tag]->metColl, 128000, 0);
    miniTrees[tag]->Branch("METInfo", "TClonesArray",   &miniEvents[tag]->metClassifColl, 128000, 0);
  }

  //
  void Handler::StartNewMiniEvent(TString tag)
  {
    if(miniEvents.find(tag) == miniEvents.end() ) return;
    miniEvents[tag]->eventClassif->Delete();     
    miniEvents[tag]->triggerColl->Delete();
    miniEvents[tag]->vertexColl->Delete();
    miniEvents[tag]->leptonsColl->Delete();     
    miniEvents[tag]->leptonsClassifColl->Delete();
    miniEvents[tag]->photonsColl->Delete();     
    miniEvents[tag]->photonsClassifColl->Delete();
    miniEvents[tag]->jetsColl->Delete();     
    miniEvents[tag]->jetsClassifColl->Delete();
    miniEvents[tag]->metColl->Delete();
    miniEvents[tag]->metClassifColl->Delete();
  }

  //
  void Handler::FillMiniEventHeader(int run, int event, int lumi, std::vector<double> &info,   TString tag)
  {
    if(miniEvents.find(tag)==miniEvents.end()) return;
    miniEvents[tag]->iRun=run;
    miniEvents[tag]->iEvent=event;
    miniEvents[tag]->iLumi=lumi;
    
    TVectorD infoVec(info.size());
    for(unsigned int i=0; i<info.size(); i++) infoVec[i] = info[i];
    new( (*miniEvents[tag]->eventClassif)[0] ) TVectorD( infoVec );
  }

  //
  void Handler::AddTriggerInfo(std::vector<int> &info,   TString tag)
  {
    TVectorD infoVec(info.size());
    for(unsigned int i=0; i<info.size(); i++) infoVec[i] = info[i];
    new( (*miniEvents[tag]->triggerColl)[0] ) TVectorD( infoVec );
  }

  //
  void Handler::AddVertexInfo(std::vector<double> &info,   TString tag)
  {
    int ivtx = miniEvents[tag]->vertexColl->GetEntriesFast();
    TVectorD infoVec(info.size());
    for(unsigned int i=0; i<info.size(); i++) infoVec[i] = info[i];
    new( (*miniEvents[tag]->vertexColl)[ivtx] ) TVectorD( infoVec );
  }


  //
  void Handler::AddParticleToMiniEvent(TLorentzVector &p, std::vector<double> &info, int pType, TString tag)
  {
    if(miniEvents.find(tag)== miniEvents.end()) return;

    TClonesArray *pColl=0;
    TClonesArray *pInfoColl=0;
    if(pType==CHLEPTON || pType==ELECTRON || pType==MUON || pType==TAU || pType==TRACK) 
      { pColl= miniEvents[tag]->leptonsColl;  pInfoColl = miniEvents[tag]->leptonsClassifColl; }
    if(pType==PHOTON || pType==SUPERCLUSTER)
      { pColl = miniEvents[tag]->photonsColl; pInfoColl = miniEvents[tag]->photonsClassifColl; }
    if(pType==JET)      { pColl= miniEvents[tag]->jetsColl; pInfoColl = miniEvents[tag]->jetsClassifColl; }
    if(pType==MET)      { pColl= miniEvents[tag]->metColl; pInfoColl = miniEvents[tag]->metClassifColl; }
    
    if(pColl==0 || pInfoColl==0) return;

    int ipart = pColl->GetEntriesFast();
    new( (*pColl)[ipart] ) TLorentzVector(p);
    TVectorD infoVec(info.size());
    for(unsigned int i=0; i<info.size(); i++) infoVec[i] = info[i];
    new( (*pInfoColl)[ipart] ) TVectorD( infoVec );
  }

  //
  void Handler::EndNewMiniEvent(TString tag)
  {
    if(miniTrees.find(tag)== miniTrees.end()) return;
    miniTrees[tag]->Fill();
  }

}
