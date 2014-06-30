/**   
      \class	    plotLimits plotLimits.cc "UserCode/LIP/TopTaus/bin/plotLimits.cc"                                                                     
      \brief    executable for performing the cutflow analysis and control plots for the various channels and data samples. Must expand to calling other classes
      
      \author   Pietro Vischia
      
      \version  $Id: plotLimits.cc,v 1.22 2013/04/19 15:13:00 vischia Exp $                                                                                                       
*/


// System includes
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

// ROOT includes
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TColor.h>
#include <TH1F.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TLatex.h>
#include <TSystem.h>

// CMSSW includes                                                                                                                                                                       
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


/*
  
@run me like this

plotLimits.cc test/physicsAnalysisParSets_cfg.py sample --> analyze sample


*/

using namespace std;

void produceLimitCard(
		      TString outputDir,  // Output directory
		      TString inputDir,   // Input directory
		      string limitsName,
		      vector<TString> fNameInset, // Inset name of combine output 
		      vector<TString> legLabel, // Label for the limit
		      vector<Int_t> colours,
		      vector<Int_t> lineStyles,
		      TString fMethod,    // Fit method from combine (part of input filename)
		      TString lumi,        // Luminosity to be displayed [fb^{-1}]
		      bool expectedOnly,
		      bool logy
		      ){
  
  double myMasses[] = {180., 200., 220., 250., 300., 400., 500., 600.};
  double useMasses[] = {  1,   1,     1,    1,   1 ,   1,     1,    1,    1,    1};

 

  vector<double>massesN;      massesN.clear();
  vector<TString> masses;     masses.clear();
  
  for(size_t i=0; i<sizeof(myMasses)/sizeof(double); ++i)
    {
      if(inputDir.Contains("/tb/")) useMasses[i] = 1;
      if(inputDir.Contains("/taunu/") && myMasses[i] >300) useMasses[i]  =0;
      
      if(useMasses[i]) massesN.push_back(myMasses[i]);
      stringstream sMass;
      sMass<<myMasses[i];
      string mass=sMass.str();
      if(useMasses[i]) masses.push_back(TString(mass));


    } 
  
  
  
  // Limit matrix
  vector<vector<double> > limits; limits.clear();
  vector<vector<double> > limitErrs; limitErrs.clear();
  
  for(size_t m=0; m<fNameInset.size(); ++m){

    ofstream fStream;
    fStream.open ( outputDir+(limitsName+fNameInset[m].Data()+string(".json")).c_str());
    
    vector<TString> inputFiles; inputFiles.clear();
    for(size_t i=0; i<massesN.size(); ++i)
      {
	//	if(useMasses[i] == 0) continue;
//	stringstream sMass;
//	sMass<<myMasses[i];
//	string mass=sMass.str();
	inputFiles.push_back(inputDir+masses[i]+"/higgsCombine"+fNameInset[m]+"."+fMethod+".mH"+masses[i]+".root"); // FIXME: rerun the limits with -m XXX. Default is 120 unfortunately
      } 
    
    // One vector per each type of limit
    vector<double> myLimits; myLimits.clear();
    vector<double> myLimitErrs; myLimitErrs.clear();
    
    
    for(size_t i=0; i<inputFiles.size(); ++i){
      cout << "Mass point " << masses[i] << endl;
      TFile* inputFile = new TFile(inputFiles[i]); 
      TTree* inputTree = (TTree*) inputFile->Get("limit");
      // One limit for each mass point 
      double bufLimit, bufLimitErr;
      inputTree->SetBranchAddress("limit", &bufLimit);
      inputTree->SetBranchAddress("limitErr", &bufLimitErr);
      cout << "=============================="<<endl;
      for(unsigned int entry=0; entry<inputTree->GetEntries(); ++entry){
	inputTree->GetEntry(entry);
	if(entry==0) myLimits.push_back(bufLimit);// cout << "limit " << entry << ", value " << myLimits[entry] << ", buf " << bufLimit << endl;
	if(entry==3) myLimitErrs.push_back(bufLimitErr);

	cout << bufLimit << " +/- " << bufLimitErr << endl;
      }    
            cout << "=============================="<<endl;
      //    delete inputFile;
      //    delete inputTree;
    }
    
    // One array (containing all the masses) per each method
    limits.push_back(myLimits);
    limitErrs.push_back(myLimitErrs);
      
    // 0: asimov
    
    fStream << "{                              " << endl;
    fStream << "  \"luminosity\": \""<<lumi<<".\",  " << endl;
    fStream << "  \"masspoints\": {            " << endl; 
    for(size_t i=0; i<masses.size(); ++i){
      fStream << "    \"" << masses[i] << "\": {                       " << endl;
      fStream << "      \"expected\": {		 " << endl;
      fStream << "        \"median\": \""<<limits[m][i]<<"\", 	 " << endl;
      fStream << "        \"median_error\": \""<<limitErrs[m][i]<<"\"	 " << endl;
      fStream << "      }, 				 " << endl;
      fStream << "      \"mass\": \""<<masses[i] << "\", 		 " << endl;
      if(i==masses.size()-1) fStream << "    }                               " << endl;
      else                   fStream << "    },                              " << endl;
      
    }
    
    
    fStream << "  }" << endl;
    fStream << "}  " << endl; 
    
    fStream.close(); 

    
  }
  
  // Write table
///  ofstream fStreamTab;
///  fStreamTab.open ( outputDir+(limitsName+string(".tex")).c_str());
///  fStreamTab << "\\begin{tabular}{|c|c|c|c|c|c|c|c|c|c|c|}"<<endl;
///  fStreamTab << "\\hline" << endl;
///  fStreamTab << "    \\multicolumn{10}{|c|}{Signal injection test for $\\sigma\\times\\mathcal{B}$ [pb] } \\\\ ";
///  fStreamTab << "    \\hline"<<endl;
///  fStreamTab << "    \\hline"<<endl;
///  fStreamTab << "    $m_{H^{+}}$   &  Observed & Prefit & Prefit (sig:1x) & Prefit (sig:5x) & Prefit (sig:10x) & Postfit & Postfit (sig:1x) & Postfit (sig:5x) & Postfit (sig:10x)    \\\\"<<endl;
///  fStreamTab << "    \\hline"<<endl;
///  fStreamTab << "    \\hline    "<<endl; 
///  for(size_t i=0; i<masses.size(); ++i)
///    fStreamTab << setprecision(5) << "    "<< masses[i] << " & " << limits[0][i] << " & " << limits[1][i] << " & " << limits[2][i] << " & " << limits[3][i] << " & " << limits[4][i] << " & " << limits[5][i] << " & " << limits[6][i] << " & " << limits[7][i] << " & " << limits[8][i] << " \\\\" << endl; 
///  fStreamTab << "    \\hline" << endl;
///  fStreamTab << "\\end{tabular}" << endl;
///  fStreamTab.close();

  // Write table
  ofstream fStreamTab;
  fStreamTab.open ( outputDir+(limitsName+string("_vert.tex")).c_str());
  fStreamTab << "\\begin{tabular}{|c|c|c|c|c|c|c|c|c|c|c|}"<<endl;
  fStreamTab << "\\hline" << endl;
  fStreamTab << "    \\multicolumn{10}{|c|}{Signal injection test for signal strength } \\\\ ";
  fStreamTab << "    \\hline"<<endl;
  fStreamTab << "    \\hline"<<endl;
  fStreamTab << "  Type ";
  for(size_t i=0; i<masses.size(); ++i) fStreamTab << "&  $m_{H^{+}}$ = " << masses[i] << " ";
  fStreamTab << " \\\\" << endl;
  for(size_t k=0; k<limits.size(); ++k){ // Per each method
    std::string lineName = legLabel[k].Data();
    if(lineName.find("_")!=std::string::npos)lineName.replace(lineName.find("_"),1,"\\_");
    fStreamTab << " " << lineName << " " ;
    for(size_t i=0; i<masses.size(); ++i){ // Per each mass point
      fStreamTab << setprecision(3) << " & " << limits[k][i] << " $\\\pm$ " << limitErrs[k][i] << " " ;
    }
    fStreamTab << " \\\\" << endl;
  }
  fStreamTab << "    \\hline" << endl;
  fStreamTab << "\\end{tabular}" << endl;
  fStreamTab.close();
  
  // Draw
   TCanvas *limitsBr = new TCanvas("limitsBr", "",0,0,600,600);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   limitsBr->SetHighLightColor(2);
   limitsBr->Range(74.68355,-1.585366,732.9114,10.60976);
   limitsBr->SetFillColor(0);
   limitsBr->SetBorderMode(0);
   limitsBr->SetBorderSize(2);
   limitsBr->SetTickx(1);
   limitsBr->SetTicky(1);
   limitsBr->SetLeftMargin(0.16);
   limitsBr->SetRightMargin(0.05);
   limitsBr->SetTopMargin(0.05);
   limitsBr->SetBottomMargin(0.13);
   limitsBr->SetFrameFillStyle(0);
   limitsBr->SetFrameBorderMode(0);
   limitsBr->SetFrameFillStyle(0);
   limitsBr->SetFrameBorderMode(0);

 
   //TH1F *hframe__1 = new TH1F("hframe__1","",1000,massesN[0]-0.1,massesN[massesN.size()-1]+0.1);
   TH1F *hframe__1 = new TH1F("hframe__1","",1000,massesN[0]-20,massesN[massesN.size()-1]+20);
   hframe__1->SetMinimum(0.01); // For logy
   hframe__1->SetMaximum(500);
   if(inputDir.Contains("/tb/") && !logy){ hframe__1->SetMaximum(20); hframe__1->SetMinimum(0.);}
   if(inputDir.Contains("/tb/") && logy){ hframe__1->SetMaximum(200); hframe__1->SetMinimum(0.01);}
   if(inputDir.Contains("/taunu/") && !logy){ hframe__1->SetMaximum(50); hframe__1->SetMinimum(0.);}
   if(inputDir.Contains("/taunu/") && logy){ hframe__1->SetMaximum(200); hframe__1->SetMinimum(0.01);}
   if(inputDir.Contains("/mutau/taunu/")) hframe__1->SetMaximum(100);
   if(inputDir.Contains("/mutau/taunu/")) hframe__1->SetMinimum(4);
   hframe__1->SetDirectory(0);
   hframe__1->SetStats(0);
   hframe__1->SetLineStyle(0);
   hframe__1->SetMarkerStyle(20);
   hframe__1->GetXaxis()->SetTitle("m_{H^{+}} (GeV)");
   hframe__1->GetXaxis()->SetLabelFont(43);
   hframe__1->GetXaxis()->SetLabelOffset(0.007);
   hframe__1->GetXaxis()->SetLabelSize(20);
   hframe__1->GetXaxis()->SetTitleSize(33);
   hframe__1->GetXaxis()->SetTitleOffset(0.9);
   hframe__1->GetXaxis()->SetTitleFont(43);
   //   hframe__1->GetYaxis()->SetTitle("95% CL limit for #sigma#times#it{B}(#tau#nu,tb) [pb]");
   hframe__1->GetYaxis()->SetTitle("Signal strength (MaxLikelihoodFit)");//"95% CL limits for #sigma#times#it{B} [pb]");
   hframe__1->GetYaxis()->SetLabelFont(43);
   hframe__1->GetYaxis()->SetLabelOffset(0.007);
   hframe__1->GetYaxis()->SetLabelSize(27);
   hframe__1->GetYaxis()->SetTitleSize(33);
   hframe__1->GetYaxis()->SetTitleOffset(1.25);
   hframe__1->GetYaxis()->SetTitleFont(43);
   hframe__1->GetZaxis()->SetLabelFont(43);
   hframe__1->GetZaxis()->SetLabelOffset(0.007);
   hframe__1->GetZaxis()->SetLabelSize(27);
   hframe__1->GetZaxis()->SetTitleSize(33);
   hframe__1->GetZaxis()->SetTitleFont(43);
   hframe__1->Draw(" ");
   
   hframe__1->GetXaxis()->SetMoreLogLabels();
   hframe__1->GetXaxis()->SetNoExponent();
   gPad->SetLogx();
   if(logy) gPad->SetLogy();


   hframe__1->LabelsDeflate("X");
   gPad->Modified();
   gPad->Update();

   vector<TGraphErrors*> graph; graph.clear();
   for(size_t m=0; m<fNameInset.size(); ++m){
     graph.push_back(new TGraphErrors(massesN.size()) );
     graph[m]->SetName("valueFrom"+fNameInset[m]);
     graph[m]->SetTitle("Graph"+fNameInset[m]);
     graph[m]->SetFillColor(colours[m]);
     graph[m]->SetFillStyle(3001);
     graph[m]->SetLineColor(colours[m]);
     graph[m]->SetLineStyle(lineStyles[m]);
     graph[m]->SetLineWidth(3);
     graph[m]->SetMarkerStyle(20);
     for(size_t i=0; i<massesN.size(); ++i){
       graph[m]->SetPoint(i,massesN[i], limits[m][i]);
       graph[m]->SetPointError(i, (fNameInset.size() -m)*1.5 , limitErrs[m][i]);
     }
     //graph[m]->SetMinimum(myMasses[0]-0.1);
     //graph[m]->SetMaximum(myMasses[(sizeof(myMasses)/sizeof(double))-1]+0.1);
     
   }

   //   TGraph* graph = new TGraph(massesN.size());//12);
   //   graph->SetName("Expected");
   //   graph->SetTitle("Graph");
   //   graph->SetFillColor(1);
   //   graph->SetLineStyle(2);
   //   graph->SetLineWidth(3);
   //   graph->SetMarkerStyle(20);
   //   for(size_t i=0; i<massesN.size(); ++i){
   //     graph->SetPoint(i,massesN[i], limits[i][2]);
   //     cout << "i " << i << ", mass " << massesN[i] << ", exp median " << limits[i][2] << endl;
   //   }
   //   graph->SetMinimum(myMasses[0]-0.1);
   //   graph->SetMaximum(myMasses[(sizeof(myMasses)/sizeof(double))-1]+0.1);
   


   TH1F *Expected3__4 = new TH1F("Expected3__4","Graph",100,128,752);
//   Expected3__4->SetMinimum(0);
//   Expected3__4->SetMaximum(4.904986);
   Expected3__4->SetDirectory(0);
   Expected3__4->SetStats(0);
   Expected3__4->SetLineStyle(0);
   Expected3__4->SetMarkerStyle(20);
   Expected3__4->GetXaxis()->SetLabelFont(43);
   Expected3__4->GetXaxis()->SetLabelOffset(0.007);
   Expected3__4->GetXaxis()->SetLabelSize(5.4);
   Expected3__4->GetXaxis()->SetTitleSize(33);
   Expected3__4->GetXaxis()->SetTitleOffset(0.9);
   Expected3__4->GetXaxis()->SetTitleFont(43);
   Expected3__4->GetYaxis()->SetLabelFont(43);
   Expected3__4->GetYaxis()->SetLabelOffset(0.007);
   Expected3__4->GetYaxis()->SetLabelSize(27);
   Expected3__4->GetYaxis()->SetTitleSize(33);
   Expected3__4->GetYaxis()->SetTitleOffset(1.25);
   Expected3__4->GetYaxis()->SetTitleFont(43);
   Expected3__4->GetZaxis()->SetLabelFont(43);
   Expected3__4->GetZaxis()->SetLabelOffset(0.007);
   Expected3__4->GetZaxis()->SetLabelSize(27);
   Expected3__4->GetZaxis()->SetTitleSize(33);
   Expected3__4->GetZaxis()->SetTitleFont(43);
   //   Double_t x1, Double_t y1, Double_t x2, Double_t y2
   //TLegend *leg = new TLegend(0.48,0.75,0.85,0.92,NULL,"brNDC");
   TLegend *leg = new TLegend(0.58,0.75,0.95,0.92,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(62);
   leg->SetTextSize(0.03);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(4000);

   
   
   
   
   for(size_t m=0; m<fNameInset.size(); ++m){
     //size_t ma =m;
     //     if(inputDir.Contains("/taunu/")) m=fNameInset.size()-1-m;
     cout << " m is " << m << endl;
     
     graph[m]->SetHistogram(Expected3__4);
     graph[m]->Draw("2");//l ");
     graph[m]->Draw("p");//l ");
     TLegendEntry *entry=leg->AddEntry("Graph"+fNameInset[m],legLabel[m],"fl");   
     entry->SetLineColor(colours[m]);
     entry->SetLineStyle(lineStyles[m]);
     entry->SetLineWidth(3);
     entry->SetMarkerColor(m);
     entry->SetMarkerStyle(21);
     entry->SetMarkerSize(1);
     //m=ma;
   }
   
   

   leg->Draw();
   
   TH1F *hframe__5 = new TH1F("hframe__5","",1000,180,700);
//   hframe__5->SetMinimum(0);
//   hframe__5->SetMaximum(10);
   hframe__5->SetDirectory(0);
   hframe__5->SetStats(0);
   hframe__5->SetLineStyle(0);
   hframe__5->SetMarkerStyle(20);
   hframe__5->GetXaxis()->SetTitle("m_{H^{+}} (GeV)");
   hframe__5->GetXaxis()->SetLabelFont(43);
   hframe__5->GetXaxis()->SetLabelOffset(0.007);
   hframe__5->GetXaxis()->SetLabelSize(5.4);
   hframe__5->GetXaxis()->SetTitleSize(33);
   hframe__5->GetXaxis()->SetTitleOffset(0.9);
   hframe__5->GetXaxis()->SetTitleFont(43);
   hframe__5->GetYaxis()->SetTitle("Signal strength (MaxLikelihoodFit)");//"95% CL limit for #sigma#times#it{B}(#tau#nu,tb) [pb]");
   hframe__5->GetYaxis()->SetLabelFont(43);
   hframe__5->GetYaxis()->SetLabelOffset(0.007);
   hframe__5->GetYaxis()->SetLabelSize(27);
   hframe__5->GetYaxis()->SetTitleSize(33);
   hframe__5->GetYaxis()->SetTitleOffset(1.25);
   hframe__5->GetYaxis()->SetTitleFont(43);
   hframe__5->GetZaxis()->SetLabelFont(43);
   hframe__5->GetZaxis()->SetLabelOffset(0.007);
   hframe__5->GetZaxis()->SetLabelSize(27);
   hframe__5->GetZaxis()->SetTitleSize(33);
   hframe__5->GetZaxis()->SetTitleFont(43);
   hframe__5->Draw("sameaxis");

   TLatex *   tex = new TLatex(0.15,0.96,"CMS Preliminary");
tex->SetNDC();
   tex->SetTextFont(42);
   tex->SetTextSize(27);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.48,0.96,"#sqrt{s} = 8 TeV");
tex->SetNDC();
   tex->SetTextFont(42);
   tex->SetTextSize(27);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.71,0.96,"L="+lumi+" fb^{-1}");
tex->SetNDC();
   tex->SetTextFont(42);
   tex->SetTextSize(27);
   tex->SetLineWidth(2);
   tex->Draw();
   //      tex = new TLatex(0.2,0.84,"#splitline{e#mu+#mu#tau_{h} final state}{tan#beta=5}");
   if(inputDir.Contains("/emu/"))      tex = new TLatex(0.2,/*0.84*/0.90,"e#mu final state");
   if(inputDir.Contains("/mutau/"))    tex = new TLatex(0.2,/*0.84*/0.90,"#mu#tau_{h} final state");
   if(inputDir.Contains("/combined/")) tex = new TLatex(0.2,/*0.84*/0.90,"e#mu+ee+#mu#mu+#mu#tau_{h} final states");
   //tex = new TLatex(0.2,0.84,"e#mu final state");
tex->SetNDC();
   tex->SetTextFont(63);
   tex->SetTextSize(20);
   tex->SetLineWidth(2);
   tex->Draw();
   //      tex = new TLatex(0.2,0.64,"#splitline{m_{h}max scenario, tan#beta=5}{H^{+}#rightarrow#tau#nu and H^{+}#rightarrow tb decays}");
   if(expectedOnly){
     if(inputDir.Contains("/tb/"))       tex = new TLatex(0.2,/*0.64*/0.80,"#splitline{H^{+}#rightarrow tb decay}{Fitted signal strength}");
     if(inputDir.Contains("/taunu/"))    tex = new TLatex(0.2,/*0.64*/0.80,"#splitline{H^{+}#rightarrow#tau#nu decay}{Fitted signal strength}");
     if(inputDir.Contains("/combined/")) tex = new TLatex(0.2,/*0.64*/0.80,"#splitline{H^{+}#rightarrow#tau#nu and H^{+}#rightarrow tb decays}{Fitted signal strength}");
   }
     
   
   tex->SetNDC();
   tex->SetTextFont(63);
   tex->SetTextSize(20);
   tex->SetLineWidth(2);
   tex->Draw();

   gPad->RedrawAxis();

   limitsBr->Modified();
   limitsBr->Update();
   limitsBr->cd();
   limitsBr->SetSelected(limitsBr);
   
   limitsBr->SaveAs(outputDir+TString(limitsName)+".C");
   limitsBr->SaveAs(outputDir+TString(limitsName)+".png");
   limitsBr->SaveAs(outputDir+TString(limitsName)+".pdf");
   limitsBr->SaveAs(outputDir+TString(limitsName)+".root");
   


}

void plotNormalLimits(
		      TString outputDir,  // Output directory
		      TString inputDir,   // Input directory
		      string limitsName,  // Output filename
		      TString fNameInset, // Inset name of combine output 
		      TString fMethod,    // Fit method from combine (part of input filename)
		      TString lumi        // Luminosity to be displayed [fb^{-1}]
		      ){
  ofstream fStream;
  fStream.open ( outputDir+(limitsName+string(".json")).c_str());
  
  double myMasses[] = {180., 200., 220., 250., 300.}; // FIXME: implement the tb-only ones with appropriate controls in the following code 
  
  vector<double>massesN;      massesN.clear();
  vector<TString> inputFiles; inputFiles.clear();
  vector<TString> masses;     masses.clear();

  for(size_t i=0; i<sizeof(myMasses)/sizeof(double); ++i)
    {
      massesN.push_back(myMasses[i]);
      stringstream sMass;
      sMass<<myMasses[i];
      string mass=sMass.str();
      masses.push_back(TString(mass));
      inputFiles.push_back(inputDir+mass+"/higgsCombine"+fNameInset+"."+fMethod+".mH120"+/*mass+*/".root"); // FIXME: rerun the limits with -m XXX. Default is 120 unfortunately
    } 
  
  
  vector<vector<double> > limits; limits.clear();
  vector<vector<double> > limitErrs; limitErrs.clear();
  
  for(size_t i=0; i<inputFiles.size(); ++i){
    cout << "Mass point " << masses[i] << endl;
    TFile* inputFile = new TFile(inputFiles[i]); 
    TTree* inputTree = (TTree*) inputFile->Get("limit");
    double bufLimit, bufLimitErr;
    vector<double> myLimits; myLimits.clear();
    vector<double> myLimitErrs; myLimitErrs.clear();
    inputTree->SetBranchAddress("limit", &bufLimit);
    inputTree->SetBranchAddress("limitErr", &bufLimitErr);
    for(unsigned int entry=0; entry<inputTree->GetEntries(); ++entry){
      inputTree->GetEntry(entry);
      myLimits.push_back(bufLimit);// cout << "limit " << entry << ", value " << myLimits[entry] << ", buf " << bufLimit << endl;
      myLimitErrs.push_back(bufLimitErr);
    }    
    limits.push_back(myLimits);
    limitErrs.push_back(myLimitErrs);
    
//    delete inputFile;
//    delete inputTree;
  }
  
  // 0: -2
  // 1: -1
  // 2: median
  // 3: +1
  // 4: +2
  // 5: obs
  
  fStream << "{                              " << endl;
  fStream << "  \"luminosity\": \""<<lumi<<".\",  " << endl;
  fStream << "  \"masspoints\": {            " << endl; 
  for(size_t i=0; i<masses.size(); ++i){
    fStream << "    \"" << masses[i] << "\": {                       " << endl;
    fStream << "      \"expected\": {		 " << endl;
    fStream << "        \"+1sigma\": \""<<limits[i][3]<<"\", 	 " << endl;
    fStream << "        \"+1sigma_error\": \""<<limitErrs[i][3]<<"\",  " << endl;
    fStream << "        \"+2sigma\": \""<<limits[i][4]<<"\", 	 " << endl;
    fStream << "        \"+2sigma_error\": \""<<limitErrs[i][4]<<"\",  " << endl;
    fStream << "        \"-1sigma\": \""<<limits[i][1]<<"\", 	 " << endl;
    fStream << "        \"-1sigma_error\": \""<<limitErrs[i][1]<<"\",  " << endl;
    fStream << "        \"-2sigma\": \""<<limits[i][0]<<"\", 	 " << endl;
    fStream << "        \"-2sigma_error\": \""<<limitErrs[i][0]<<"\",  " << endl;
    fStream << "        \"median\": \""<<limits[i][2]<<"\", 	 " << endl;
    fStream << "        \"median_error\": \""<<limitErrs[i][2]<<"\"	 " << endl;
    fStream << "      }, 				 " << endl;
    fStream << "      \"mass\": \""<<masses[i] << "\", 		 " << endl;
    fStream << "      \"observed\": "<<limits[i][5]<<"		 " << endl;
    if(i==masses.size()-1) fStream << "    }                               " << endl;
    else                   fStream << "    },                              " << endl;
    
  }


  fStream << "  }" << endl;
  fStream << "}  " << endl; 

  fStream.close(); 

  
  // Draw
   TCanvas *limitsBr = new TCanvas("limitsBr", "",0,0,600,600);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   limitsBr->SetHighLightColor(2);
   limitsBr->Range(74.68355,-1.585366,732.9114,10.60976);
   limitsBr->SetFillColor(0);
   limitsBr->SetBorderMode(0);
   limitsBr->SetBorderSize(2);
   limitsBr->SetTickx(1);
   limitsBr->SetTicky(1);
   limitsBr->SetLeftMargin(0.16);
   limitsBr->SetRightMargin(0.05);
   limitsBr->SetTopMargin(0.05);
   limitsBr->SetBottomMargin(0.13);
   limitsBr->SetFrameFillStyle(0);
   limitsBr->SetFrameBorderMode(0);
   limitsBr->SetFrameFillStyle(0);
   limitsBr->SetFrameBorderMode(0);
   
   TH1F *hframe__1 = new TH1F("hframe__1","",1000,myMasses[0]-0.1,myMasses[(sizeof(myMasses)/sizeof(double))-1]+0.1);
   hframe__1->SetMinimum(0.1); // For logy
   hframe__1->SetMaximum(100);
   hframe__1->SetDirectory(0);
   hframe__1->SetStats(0);
   hframe__1->SetLineStyle(0);
   hframe__1->SetMarkerStyle(20);
   hframe__1->GetXaxis()->SetTitle("m_{H^{+}} (GeV)");
   hframe__1->GetXaxis()->SetLabelFont(43);
   hframe__1->GetXaxis()->SetLabelOffset(0.007);
   hframe__1->GetXaxis()->SetLabelSize(20);
   hframe__1->GetXaxis()->SetTitleSize(33);
   hframe__1->GetXaxis()->SetTitleOffset(0.9);
   hframe__1->GetXaxis()->SetTitleFont(43);
   //   hframe__1->GetYaxis()->SetTitle("95% CL limit for #sigma#times#it{B}(#tau#nu,tb) [pb]");
   hframe__1->GetYaxis()->SetTitle("95% CL limits for #sigma#times#it{B} [pb]");
   hframe__1->GetYaxis()->SetLabelFont(43);
   hframe__1->GetYaxis()->SetLabelOffset(0.007);
   hframe__1->GetYaxis()->SetLabelSize(27);
   hframe__1->GetYaxis()->SetTitleSize(33);
   hframe__1->GetYaxis()->SetTitleOffset(1.25);
   hframe__1->GetYaxis()->SetTitleFont(43);
   hframe__1->GetZaxis()->SetLabelFont(43);
   hframe__1->GetZaxis()->SetLabelOffset(0.007);
   hframe__1->GetZaxis()->SetLabelSize(27);
   hframe__1->GetZaxis()->SetTitleSize(33);
   hframe__1->GetZaxis()->SetTitleFont(43);
   hframe__1->Draw(" ");
   
   hframe__1->GetXaxis()->SetMoreLogLabels();
   hframe__1->GetXaxis()->SetNoExponent();
   gPad->SetLogx();
   //   gPad->SetLogy();


   hframe__1->LabelsDeflate("X");
   gPad->Modified();
   gPad->Update();
   TGraph *graph = new TGraph(24);
   graph->SetName("Expected2Sigma");
   graph->SetTitle("Graph");

   
   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#ffff00");
   graph->SetFillColor(ci);
   graph->SetLineStyle(2);
   graph->SetLineWidth(3);
   graph->SetMarkerStyle(20);
   for(size_t i=0; i<massesN.size(); ++i)
     graph->SetPoint(i,massesN[i], limits[i][0]);
   for(size_t i=0; i<massesN.size(); ++i)
     graph->SetPoint(i+massesN.size(), massesN[massesN.size()-1-i], limits[massesN.size()-1-i][4]);


//   graph->SetPoint(0,180,2.42612);
//   graph->SetPoint(1,200,0.334644);
//   graph->SetPoint(2,220,0.211624);
//   graph->SetPoint(3,240,0.149488);
//   graph->SetPoint(4,250,0.174665);
//   graph->SetPoint(5,280,0.171518);
//   graph->SetPoint(6,300,0.174665);
//   graph->SetPoint(7,350,0.265224);
//   graph->SetPoint(8,400,0.322056);
//   graph->SetPoint(9,500,0.623436);
//   graph->SetPoint(10,600,1.05244);
//   graph->SetPoint(11,700,1.95541);
//   graph->SetPoint(12,700,6.90026);
//   graph->SetPoint(13,600,3.69789);
//   graph->SetPoint(14,500,2.16935);
//   graph->SetPoint(15,400,1.12879);
//   graph->SetPoint(16,350,0.930532);
//   graph->SetPoint(17,300,0.612197);
//   graph->SetPoint(18,280,0.601166);
//   graph->SetPoint(19,250,0.616359);
//   graph->SetPoint(20,240,0.527514);
//   graph->SetPoint(21,220,0.752343);
//   graph->SetPoint(22,200,1.1923);
//   graph->SetPoint(23,180,8.2756);

   
   TH1F *Expected2Sigma1__2 = new TH1F("Expected2Sigma1__2","Graph",100,128,752);
   Expected2Sigma1__2->SetMinimum(0);
   Expected2Sigma1__2->SetMaximum(9.088211);
   Expected2Sigma1__2->SetDirectory(0);
   Expected2Sigma1__2->SetStats(0);
   Expected2Sigma1__2->SetLineStyle(0);
   Expected2Sigma1__2->SetMarkerStyle(20);
   Expected2Sigma1__2->GetXaxis()->SetLabelFont(43);
   Expected2Sigma1__2->GetXaxis()->SetLabelOffset(0.007);
   Expected2Sigma1__2->GetXaxis()->SetLabelSize(5.4);
   Expected2Sigma1__2->GetXaxis()->SetTitleSize(33);
   Expected2Sigma1__2->GetXaxis()->SetTitleOffset(0.9);
   Expected2Sigma1__2->GetXaxis()->SetTitleFont(43);
   Expected2Sigma1__2->GetYaxis()->SetLabelFont(43);
   Expected2Sigma1__2->GetYaxis()->SetLabelOffset(0.007);
   Expected2Sigma1__2->GetYaxis()->SetLabelSize(27);
   Expected2Sigma1__2->GetYaxis()->SetTitleSize(33);
   Expected2Sigma1__2->GetYaxis()->SetTitleOffset(1.25);
   Expected2Sigma1__2->GetYaxis()->SetTitleFont(43);
   Expected2Sigma1__2->GetZaxis()->SetLabelFont(43);
   Expected2Sigma1__2->GetZaxis()->SetLabelOffset(0.007);
   Expected2Sigma1__2->GetZaxis()->SetLabelSize(27);
   Expected2Sigma1__2->GetZaxis()->SetTitleSize(33);
   Expected2Sigma1__2->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Expected2Sigma1__2);
   
   graph->Draw("f ");
   
   graph = new TGraph(24);
   graph->SetName("Expected1Sigma");
   graph->SetTitle("Graph");
   ci = TColor::GetColor("#33cc33");
   graph->SetFillColor(ci);
   graph->SetLineStyle(2);
   graph->SetLineWidth(3);
   graph->SetMarkerStyle(20);
   for(size_t i=0; i<massesN.size(); ++i)
     graph->SetPoint(i,massesN[i], limits[i][1]);
   for(size_t i=0; i<massesN.size(); ++i)
     graph->SetPoint(i+massesN.size(), massesN[massesN.size()-1-i], limits[massesN.size()-1-i][3]);
   //   graph->SetPoint(0,180,3.2462);
   //   graph->SetPoint(1,200,0.446175);
   //   graph->SetPoint(2,220,0.283723);
   //   graph->SetPoint(3,240,0.19931);
   //   graph->SetPoint(4,250,0.233466);
   //   graph->SetPoint(5,280,0.228682);
   //   graph->SetPoint(6,300,0.232878);
   //   graph->SetPoint(7,350,0.35224);
   //   graph->SetPoint(8,400,0.429391);
   //   graph->SetPoint(9,500,0.827975);
   //   graph->SetPoint(10,600,1.39773);
   //   graph->SetPoint(11,700,2.60712);
   //   graph->SetPoint(12,700,5.10631);
   //   graph->SetPoint(13,600,2.74398);
   //   graph->SetPoint(14,500,1.61627);
   //   graph->SetPoint(15,400,0.841006);
   //   graph->SetPoint(16,350,0.689552);
   //   graph->SetPoint(17,300,0.456116);
   //   graph->SetPoint(18,280,0.447897);
   //   graph->SetPoint(19,250,0.456116);
   //   graph->SetPoint(20,240,0.390369);
   //   graph->SetPoint(21,220,0.555321);
   //   graph->SetPoint(22,200,0.878847);
   //   graph->SetPoint(23,180,6.21824);
   
   TH1F *Expected1Sigma2__3 = new TH1F("Expected1Sigma2__3","Graph",100,128,752);
   Expected1Sigma2__3->SetMinimum(0);
   Expected1Sigma2__3->SetMaximum(6.820133);
   Expected1Sigma2__3->SetDirectory(0);
   Expected1Sigma2__3->SetStats(0);
   Expected1Sigma2__3->SetLineStyle(0);
   Expected1Sigma2__3->SetMarkerStyle(20);
   Expected1Sigma2__3->GetXaxis()->SetLabelFont(43);
   Expected1Sigma2__3->GetXaxis()->SetLabelOffset(0.007);
   Expected1Sigma2__3->GetXaxis()->SetLabelSize(5.4);
   Expected1Sigma2__3->GetXaxis()->SetTitleSize(33);
   Expected1Sigma2__3->GetXaxis()->SetTitleOffset(0.9);
   Expected1Sigma2__3->GetXaxis()->SetTitleFont(43);
   Expected1Sigma2__3->GetYaxis()->SetLabelFont(43);
   Expected1Sigma2__3->GetYaxis()->SetLabelOffset(0.007);
   Expected1Sigma2__3->GetYaxis()->SetLabelSize(27);
   Expected1Sigma2__3->GetYaxis()->SetTitleSize(33);
   Expected1Sigma2__3->GetYaxis()->SetTitleOffset(1.25);
   Expected1Sigma2__3->GetYaxis()->SetTitleFont(43);
   Expected1Sigma2__3->GetZaxis()->SetLabelFont(43);
   Expected1Sigma2__3->GetZaxis()->SetLabelOffset(0.007);
   Expected1Sigma2__3->GetZaxis()->SetLabelSize(27);
   Expected1Sigma2__3->GetZaxis()->SetTitleSize(33);
   Expected1Sigma2__3->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Expected1Sigma2__3);
   
   graph->Draw("f ");
   
   graph = new TGraph(massesN.size());//12);
   graph->SetName("Expected");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);
   graph->SetLineStyle(2);
   graph->SetLineWidth(3);
   graph->SetMarkerStyle(20);
   for(size_t i=0; i<massesN.size(); ++i){
     graph->SetPoint(i,massesN[i], limits[i][2]);
     cout << "i " << i << ", mass " << massesN[i] << ", exp median " << limits[i][2] << endl;
   }
   graph->SetMinimum(170.);
   graph->SetMaximum(710.);
   

//   graph->SetPoint(0,180,4.48438);
//   graph->SetPoint(1,200,0.623047);
//   graph->SetPoint(2,220,0.392578);
//   graph->SetPoint(3,240,0.27832);
//   graph->SetPoint(4,250,0.325195);
//   graph->SetPoint(5,280,0.319336);
//   graph->SetPoint(6,300,0.325195);
//   graph->SetPoint(7,350,0.490234);
//   graph->SetPoint(8,400,0.599609);
//   graph->SetPoint(9,500,1.15234);
//   graph->SetPoint(10,600,1.94531);
//   graph->SetPoint(11,700,3.64062);

   TH1F *Expected3__4 = new TH1F("Expected3__4","Graph",100,128,752);
   Expected3__4->SetMinimum(0);
   Expected3__4->SetMaximum(4.904986);
   Expected3__4->SetDirectory(0);
   Expected3__4->SetStats(0);
   Expected3__4->SetLineStyle(0);
   Expected3__4->SetMarkerStyle(20);
   Expected3__4->GetXaxis()->SetLabelFont(43);
   Expected3__4->GetXaxis()->SetLabelOffset(0.007);
   Expected3__4->GetXaxis()->SetLabelSize(5.4);
   Expected3__4->GetXaxis()->SetTitleSize(33);
   Expected3__4->GetXaxis()->SetTitleOffset(0.9);
   Expected3__4->GetXaxis()->SetTitleFont(43);
   Expected3__4->GetYaxis()->SetLabelFont(43);
   Expected3__4->GetYaxis()->SetLabelOffset(0.007);
   Expected3__4->GetYaxis()->SetLabelSize(27);
   Expected3__4->GetYaxis()->SetTitleSize(33);
   Expected3__4->GetYaxis()->SetTitleOffset(1.25);
   Expected3__4->GetYaxis()->SetTitleFont(43);
   Expected3__4->GetZaxis()->SetLabelFont(43);
   Expected3__4->GetZaxis()->SetLabelOffset(0.007);
   Expected3__4->GetZaxis()->SetLabelSize(27);
   Expected3__4->GetZaxis()->SetTitleSize(33);
   Expected3__4->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Expected3__4);
   
   graph->Draw("l ");
   
   TLegend *leg = new TLegend(0.48,0.75,0.85,0.92,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(62);
   leg->SetTextSize(0.03);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(4000);
   TLegendEntry *entry=leg->AddEntry("Expected1Sigma","Expected median #pm 1#sigma","fl");

   ci = TColor::GetColor("#33cc33");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);
   entry->SetLineColor(1);
   entry->SetLineStyle(2);
   entry->SetLineWidth(3);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("Expected2Sigma","Expected median #pm 2#sigma","fl");

   ci = TColor::GetColor("#ffff00");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);
   entry->SetLineColor(1);
   entry->SetLineStyle(2);
   entry->SetLineWidth(3);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   leg->Draw();
   
   TH1F *hframe__5 = new TH1F("hframe__5","",1000,180,700);
   hframe__5->SetMinimum(0);
   hframe__5->SetMaximum(10);
   hframe__5->SetDirectory(0);
   hframe__5->SetStats(0);
   hframe__5->SetLineStyle(0);
   hframe__5->SetMarkerStyle(20);
   hframe__5->GetXaxis()->SetTitle("m_{H^{+}} (GeV)");
   hframe__5->GetXaxis()->SetLabelFont(43);
   hframe__5->GetXaxis()->SetLabelOffset(0.007);
   hframe__5->GetXaxis()->SetLabelSize(5.4);
   hframe__5->GetXaxis()->SetTitleSize(33);
   hframe__5->GetXaxis()->SetTitleOffset(0.9);
   hframe__5->GetXaxis()->SetTitleFont(43);
   hframe__5->GetYaxis()->SetTitle("95% CL limit for #sigma#times#it{B}(#tau#nu,tb) [pb]");
   hframe__5->GetYaxis()->SetLabelFont(43);
   hframe__5->GetYaxis()->SetLabelOffset(0.007);
   hframe__5->GetYaxis()->SetLabelSize(27);
   hframe__5->GetYaxis()->SetTitleSize(33);
   hframe__5->GetYaxis()->SetTitleOffset(1.25);
   hframe__5->GetYaxis()->SetTitleFont(43);
   hframe__5->GetZaxis()->SetLabelFont(43);
   hframe__5->GetZaxis()->SetLabelOffset(0.007);
   hframe__5->GetZaxis()->SetLabelSize(27);
   hframe__5->GetZaxis()->SetTitleSize(33);
   hframe__5->GetZaxis()->SetTitleFont(43);
   hframe__5->Draw("sameaxis");
   TLatex *   tex = new TLatex(0.2,0.96,"CMS Preliminary");
tex->SetNDC();
   tex->SetTextFont(43);
   tex->SetTextSize(27);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.53,0.96,"#sqrt{s} = 8 TeV");
tex->SetNDC();
   tex->SetTextFont(43);
   tex->SetTextSize(27);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.76,0.96,"L="+lumi+" fb^{-1}");
tex->SetNDC();
   tex->SetTextFont(43);
   tex->SetTextSize(27);
   tex->SetLineWidth(2);
   tex->Draw();
   //      tex = new TLatex(0.2,0.84,"#splitline{e#mu+#mu#tau_{h} final state}{tan#beta=5}");
   tex = new TLatex(0.2,0.84,"e#mu+#mu#tau_{h} final state");
   //tex = new TLatex(0.2,0.84,"e#mu final state");
tex->SetNDC();
   tex->SetTextFont(63);
   tex->SetTextSize(20);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.2,0.64,"#splitline{m_{h}max scenario, tan#beta=5}{H^{+}#rightarrow#tau#nu and H^{+}#rightarrow tb decays}");
tex->SetNDC();
   tex->SetTextFont(63);
   tex->SetTextSize(20);
   tex->SetLineWidth(2);
   tex->Draw();

   gPad->RedrawAxis();

   limitsBr->Modified();
   limitsBr->Update();
   limitsBr->cd();
   limitsBr->SetSelected(limitsBr);
   
   limitsBr->SaveAs(outputDir+TString(limitsName)+".C");
   limitsBr->SaveAs(outputDir+TString(limitsName)+".png");
   limitsBr->SaveAs(outputDir+TString(limitsName)+".pdf");
   limitsBr->SaveAs(outputDir+TString(limitsName)+".root");
   

   // Write table
   ofstream fStreamTab;
   fStreamTab.open ( outputDir+(limitsName+string(".tex")).c_str());
   fStreamTab << "\\begin{tabular}{|c|c|c|c|c|c|c|}"<<endl;
   fStreamTab << "\\hline" << endl;
   fStreamTab << "    \\multicolumn{7}{|c|}{95\\% CL upper limit on $\\sigma\\times\\mathcal{B}$ [pb] } \\\\ ";
   fStreamTab << "    \\hline"<<endl;
   fStreamTab << "    \\hline"<<endl;
   fStreamTab << "    $m_{\\PH^{+}}$   & \\multicolumn{5}{c|}{Expected limit}      & Observed  \\\\\\cline{2-6}  (\\GeVns)   & $-2\\sigma$  & $-1\\sigma$ & median & +1$\\sigma$ & +2$\\sigma$  & limit  \\\\"<<endl;
   fStreamTab << "    \\hline"<<endl;
   fStreamTab << "    \\hline    "<<endl;
   for(size_t i=0; i<masses.size(); ++i)
     fStreamTab << setprecision(5) << "    "<< masses[i] << " & " << limits[i][0] << " & " << limits[i][1] << " & " << limits[i][2] << " & " << limits[i][3] << " & " << limits[i][4] << " & " << limits[i][5] << " \\\\" << endl; 
   fStreamTab << "    \\hline" << endl;
   fStreamTab << "\\end{tabular}" << endl;
   
   fStreamTab.close();
  

}

int main(int argc, char* argv[])
{
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  
  //check arguments
  if ( argc < 3 ) {
    std::cout << "Usage : " << argv[0] << " --logy [true|false] --expectedOnly [true|false]" << std::endl;
    return 0;
  }
  
  string sLogy("");
  bool logy(false);
  
  string sExpectedOnly("");
  bool expectedOnly(false);
  
  for(int i=1;i<argc;i++){
    string arg(argv[i]);
    //    if(arg.find("--help")        !=string::npos) { printHelp(); return -1;} 
    if(arg.find("--logy") !=string::npos && i+1<argc) { sLogy = argv[i+1]; printf("logy = %s  [TEMPORARILY DISABLED to plot with true and false quickly for everything]\n", sLogy.c_str()); }
    if(arg.find("--expectedOnly") !=string::npos && i+1<argc) { sExpectedOnly = argv[i+1]; printf("expectedOnly = %s\n", sExpectedOnly.c_str()); }
  }

  if(sLogy=="true")
    logy=true;
  else if(sLogy=="")
    cout << "No Logy specified. Defaulting to false" << endl;

  if(sExpectedOnly=="true")
    expectedOnly=true;
  else if(sExpectedOnly=="")
    cout << "No ExpectedOnly specified. Defaulting to false" << endl;
  
  //string parSet(argv[1]);
  //string runOn(argv[2]);
  
  //  const edm::ParameterSet &pSet = edm::readPSetsFrom(parSet)->getParameter<edm::ParameterSet>("PhysicsAnalysisParSet");
  //  double tauPtCut = pSet.getParameter<double>("tauPtCut");
  
  
  
  // Plot signal injection results
  vector<TString> insets; insets.clear();
  vector<Int_t> colours; colours.clear();
  vector<Int_t> lineStyles; lineStyles.clear();
  vector<TString> labels; labels.clear();
//  if(!expectedOnly){ insets.push_back("HybridObserved"             ); colours.push_back(kBlack);    lineStyles.push_back(2); labels.push_back("Observed"); }
//  insets.push_back("HybridAsimovPrefit"         );                   colours.push_back(kRed);      lineStyles.push_back(2); labels.push_back("No injection");
//  insets.push_back("HybridAsimovPrefit_sig1x"   );                   colours.push_back(kOrange);   lineStyles.push_back(4); labels.push_back("1#times signal injection");
//  insets.push_back("HybridAsimovPrefit_sig5x"   );                   colours.push_back(kOrange+2); lineStyles.push_back(4); labels.push_back("5#times signal injection");
//  insets.push_back("HybridAsimovPrefit_sig10x"  );                   colours.push_back(kOrange+4); lineStyles.push_back(4); labels.push_back("10#times signal injection");
//  if(!expectedOnly){ insets.push_back("HybridAsimovPostfit"        ); colours.push_back(kBlack);   lineStyles.push_back(2); labels.push_back("No injection");                      }
  if(!expectedOnly){ insets.push_back("HybridAsimovPostfit_sig1x"  ); colours.push_back(kRed);     lineStyles.push_back(4); labels.push_back("1#times signal injection");  }
  if(!expectedOnly){ insets.push_back("HybridAsimovPostfit_sig5x"  ); colours.push_back(kBlue);    lineStyles.push_back(4);labels.push_back("5#times signal injection");	       }
  if(!expectedOnly){ insets.push_back("HybridAsimovPostfit_sig10x" ); colours.push_back(kGreen-4); lineStyles.push_back(4);labels.push_back("10#times signal injection");        } 


 
  vector<TString> inDir; inDir.clear();
  //  inDir.push_back("LIMITS/signalInjection_emu/tb/");
  //  inDir.push_back("LIMITS/signalInjection_emu/taunu/");
  
  vector<TString> outDir; outDir.clear();
  //  outDir.push_back("out_signalInjection/tb/");
  //  outDir.push_back("out_signalInjection/taunu/");
 
//  inDir.push_back("LIMITS/emu/tb/");         outDir.push_back("results_signalInjection/emu/tb/");        
//  //  inDir.push_back("LIMITS/emu/taunu/");      outDir.push_back("results_signalInjection/emu/taunu/");     
//  inDir.push_back("LIMITS/mutau/tb/");       outDir.push_back("results_signalInjection/mutau/tb/");      
//  //  inDir.push_back("LIMITS/mutau/taunu/");    outDir.push_back("results_signalInjection/mutau/taunu/");   
  inDir.push_back("LIMITS/combined/tb/");    outDir.push_back("results_signalInjection/combined/tb/");   
  inDir.push_back("LIMITS/combined/taunu/"); outDir.push_back("results_signalInjection/combined/taunu/");



  for(size_t j=0; j<inDir.size(); ++j){
    gSystem->Exec("mkdir -p "+outDir[j]);
    logy=false;
    //produceLimitCard(outDir[j], inDir[j] , "signalInjectionTest", insets, labels, colours, lineStyles, "HybridNew", "19.7", expectedOnly, logy );
    produceLimitCard(outDir[j], inDir[j] , "signalInjectionTest", insets, labels, colours, lineStyles, "MaxLikelihoodFit", "19.7", expectedOnly, logy );
  }

  inDir.clear();
  outDir.clear();

//  inDir.push_back("LIMITS/emu/tb/");         outDir.push_back("results_signalInjection/emu/tb/log/");        
//  //  inDir.push_back("LIMITS/emu/taunu/");      outDir.push_back("results_signalInjection/emu/taunu/log/");     
//  inDir.push_back("LIMITS/mutau/tb/");       outDir.push_back("results_signalInjection/mutau/tb/log/");      
//  //  inDir.push_back("LIMITS/mutau/taunu/");    outDir.push_back("results_signalInjection/mutau/taunu/log/");   
  inDir.push_back("LIMITS/combined/tb/");    outDir.push_back("results_signalInjection/combined/tb/log/");   
  inDir.push_back("LIMITS/combined/taunu/"); outDir.push_back("results_signalInjection/combined/taunu/log/");

  for(size_t j=0; j<inDir.size(); ++j){
    gSystem->Exec("mkdir -p "+outDir[j]);
    logy=true;
    //produceLimitCard(outDir[j], inDir[j] , "signalInjectionTest", insets, labels, colours, lineStyles, "HybridNew", "19.7", expectedOnly, logy );
    produceLimitCard(outDir[j], inDir[j] , "signalInjectionTest", insets, labels, colours, lineStyles, "MaxLikelihoodFit", "19.7", expectedOnly, logy );
  }


  
  // Plotting normal limits
  // plotNormalLimits(outDir[j], inDir[j] ,insets[i].Data(), insets[i], "HybridNew", "19.3/19.7" );

  return 0;
}


