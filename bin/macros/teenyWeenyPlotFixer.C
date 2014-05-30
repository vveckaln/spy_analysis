#include <TFile.h>
#include <TLine.h>
#include <TString.h>
#include <TCanvas.h>


void teenyWeenyPlotFixer(TString histName){

  TString
    iFileName(histName+".root"),
    oFileName(histName+".pdf");
  
  TFile* iFile = new TFile(iFileName, "READ");
  TCanvas* canva = (TCanvas*) iFile->Get(histName);
  canva->SaveAs(oFileName);
  
}
