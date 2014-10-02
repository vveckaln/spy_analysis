#include "LIP/TopTaus/include/HistogramPlotter.hh"
#include "LIP/TopTaus/include/ReadEvent.hh"
#include "LIP/TopTaus/include/FileReader.hh"
#include "LIP/TopTaus/include/EventDigester.hh"
#include "LIP/TopTaus/include/BTagger.hh"
#include "LIP/TopTaus/include/Selector.hh"
#include "LIP/TopTaus/include/HistogramFiller.hh"
#include "LIP/TopTaus/include/CPFileRegister.hh"
#include "LIP/TopTaus/include/CPHistogramPoolRegister.hh"
#include "LIP/TopTaus/include/Parser.hh"
#include "TLegend.h"
#include "TROOT.h"
#include "TPad.h"
using namespace cpFileRegister;
using namespace cpHistogramPoolRegister;

HistogramPlotter::HistogramPlotter(){
 
}

void HistogramPlotter::AddHistograms(const vector<TString> *run_options, const char * output_name){
  output_file_name = gspyOutputArea + gsubArea + "out_" + TString(output_name)+".root";
  output_file = new TFile(output_file_name, "recreate");
  //histogram_pool = new HistogramPool();
  input_file = (TFile*) malloc(sizeof(TFile[run_options -> size()]));
  HistogramPool * histogram_pool_other = new HistogramPool();
  for (unsigned int option_ind = 0; option_ind < run_options -> size(); option_ind ++){
    input_file_name = gspyOutputArea + gsubArea + "out_" + run_options -> at(option_ind) + ".root";
    new (input_file + option_ind) TFile(input_file_name, "read");
    if (input_file + option_ind == NULL) {
      printf(" no input file found\n");
      return;
    }
    histogram_pool_other -> GetFromFile(object_descriptors, input_file + option_ind);
    histogram_pool_other -> GetFromFile(selector_h_descriptors, input_file + option_ind);
    if (option_ind == 0){
      histogram_pool_other -> ls();
      printf("Cloning\n");
      histogram_pool = (HistogramPool*)histogram_pool_other -> Clone();
      histogram_pool -> Reset();
      histogram_pool -> ls();
    }
    histogram_pool -> Add(histogram_pool_other);
    
    
  }  
  output_file -> cd();
  histogram_pool -> ls();
  histogram_pool -> Write();
  
  histogram_pool -> Close();
  delete histogram_pool;
  for (unsigned short ind = 0; ind < run_options->size(); ind ++){
    input_file[ind] . Close();
    input_file[ind] . ~TFile();
  }
  free(input_file);
  output_file -> Close();
  delete output_file;
}

void HistogramPlotter::StackHistograms(const char * output_name){
  TLegend * legend = new TLegend(0.7, 0.7, 0.9, 0.9);

  histogram_pool = (HistogramPool*) malloc(sizeof(HistogramPool[MCdatasample_descriptors -> size()]));
  input_file = (TFile*) malloc(sizeof(TFile[MCdatasample_descriptors -> size()]));

  output_file_name = gspyOutputArea + gsubArea + "out_" + TString(output_name) + ".root";
  output_file = new TFile(output_file_name, "recreate");

  TString data_file_name = gspyOutputArea + gsubArea + "out_" + TString(datasample_descriptors -> at(0) . datasample_identifier) + ".root";
  data_file = new TFile(data_file_name, "read");
  data_histo_pool = new HistogramPool();
  data_histo_pool -> GetFromFile(object_descriptors, data_file);
  data_histo_pool -> GetFromFile(selector_h_descriptors, data_file);

  HistogramPool *sum_pool;// = new HistogramPool(object_descriptors, "temp");
  //sum_pool -> AddObjects(selector_h_descriptors);

  THStack_pool = new THStackPool(object_descriptors);
  THStack_pool -> AddObjects(selector_h_descriptors);

  TCanvas_pool = new TCanvasPool(object_descriptors);
  TCanvas_pool -> AddObjects(selector_h_descriptors);

  for (unsigned int opt_ind = 0; opt_ind < MCdatasample_descriptors -> size(); opt_ind ++){
    DataSampleDescriptor *MCdatasample_descr = & MCdatasample_descriptors -> at(opt_ind);
    input_file_name = gspyOutputArea + gsubArea + "out_" +  MCdatasample_descr -> datasample_identifier + ".root";
    new (histogram_pool + opt_ind) HistogramPool();

    new (input_file + opt_ind) TFile(input_file_name, "read");
    histogram_pool[opt_ind] . GetFromFile(object_descriptors, input_file + opt_ind);
    histogram_pool[opt_ind] . GetFromFile(selector_h_descriptors, input_file + opt_ind);
    if (opt_ind == 0){
      sum_pool = (HistogramPool*)histogram_pool[0].Clone();
      sum_pool -> Reset();
    }   
    sum_pool -> Add(histogram_pool + opt_ind);
    
    histogram_pool[opt_ind] . AddEntryToLegend(legend, MCdatasample_descr -> legend_entry, "f" );
    histogram_pool[opt_ind] . SetFillColor( MCdatasample_descr -> basic_hue + MCdatasample_descr -> hue_adjustment);
    THStack_pool -> Add(histogram_pool + opt_ind);
    
    
  }
  data_histo_pool -> Sumw2();
  data_histo_pool -> Normalise(sum_pool);
  data_histo_pool -> SetMarkerStyle(datasample_descriptors -> at(0) . marker_style);
  TGraph_pool = new TGraphPool();
  TGraph_pool -> ConstructRatioGraphs(sum_pool, data_histo_pool);
  sum_pool -> Close();

  delete sum_pool;
  TCanvas_pool -> Divide(1,2);
  TCanvas_pool -> SetPad(1, "pad1", "pad1", 0, 0.31, 1, 1);
  TCanvas_pool -> SetPad(2, "pad2", "pad2", 0, 0, 1, 0.3);
  TCanvas_pool -> cd(1);
  THStack_pool -> Draw();
  
  THStack_pool -> SetXaxesTitle(object_descriptors);
  THStack_pool -> SetYaxesTitle(object_descriptors);
  THStack_pool -> SetXaxesTitle(selector_h_descriptors);
  THStack_pool -> SetYaxesTitle(selector_h_descriptors);

  data_histo_pool -> AddEntryToLegend(legend, datasample_descriptors -> at(0) . legend_entry, "p" );
  data_histo_pool -> Draw("SAMEp");
  TCanvas_pool -> DrawOnAllCanvases(legend, "SAME");
 
  output_file -> cd();
  THStack_pool -> Write();
  TCanvas_pool -> cd(2);
  TGraph_pool -> SetMarkerStyle(21);
  TGraph_pool -> SetAllYaxesTitle("Data / #Sigma MC");
  TGraph_pool -> SetXaxesTitle(object_descriptors);
  TGraph_pool -> Draw("AP");
  TCanvas_pool -> Update();
  TCanvas_pool -> Save("pdf");
  TCanvas_pool -> Save("png");
  
}

HistogramPlotter::~HistogramPlotter(){
  printf("Deconstructing HistogramPlotter\n");
}
