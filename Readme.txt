----------------------------------
Repository for tauDilepton analysis
-----------------------------------
* This is the manual. Read the fucking manual +BEFORE+ running the code.

* For help: pietro.vischia atPlzNoSpam gmail.com / pietro.vischia atPlzNoSpam cern.ch

* Will work for ttbar cross section measurement as well as charged higgs search/tau polarization

* This started as a porting to a compiled binary approach like LIP/Top, in order
to remove levels of messiness in the original code to be compiled every time together with
its libraries - whoah running time is already 1/2 than before, LoL

* MOVED TO GITHUB.COM ON APRIL 30th, 2013 because of CERN CVS service shutdown

------------
Installation
------------
# 5311_patch6 (for analysis only)
setenv SCRAM_ARCH slc5_amd64_gcc462
scramv1 project CMSSW CMSSW_5_3_11_patch6
cd CMSSW_5_3_11_patch6/src/
cmsenv
# go then to the TAGS_2012.txt to its "CMSSW_5_3_11_patch6" section :)




# 539
setenv SCRAM_ARCH slc5_amd64_gcc462
scramv1 project CMSSW CMSSW_5_3_9
cd CMSSW_5_3_9/src/
cmsenv
wget -q -O - --no-check-certificate https://raw.github.com/vischia/TopTaus/master/TAGS_2012.txt | sh
scram b -j8


# 537
setenv SCRAM_ARCH slc5_amd64_gcc462
scramv1 project CMSSW CMSSW_5_3_7_patch4
cd CMSSW_5_3_7_patch4/src/
cmsenv
cvs co -p UserCode/LIP/TopTaus/TAGS_2012.txt | sh
scram b -j8

# Production version for Higgs Combination Tool
setenv SCRAM_ARCH slc5_amd64_gcc472 
cmsrel CMSSW_6_1_1 ### must be >= 6.1.1, as older versions have bugs
cd CMSSW_6_1_1/src 
cmsenv
addpkg HiggsAnalysis/CombinedLimit V03-01-08
scramv1 b

# 444
setenv SCRAM_ARCH slc5_amd64_gcc434
scramv1 project CMSSW CMSSW_4_4_4
cd CMSSW_4_4_4/src
cvs co -p UserCode/LIP/TopTaus/TAGS_2011.txt | sh

---------------------------------
Instructions for running analysis
---------------------------------

###### How to analyze spyfiles ######
bin/physicsAnalysis.cc is the base executable. It makes use of test/physicsAnalysisParSets_cfg.py in order to set up some needed parameters
Currently, the only things you must change in the cfg file are:
 - the "spyOutputArea" (and for good measure please change also "outputArea"). The input spyfiles are already in a shared folder, that you don't need to change.
 - the "eChOnMuChOff" variable: this is for choosing whether to analyze mutau or etau final states
You can run in parallel on all the spyfiles pertaining to a given set like this:
cd scripts/lip-batch
sh submit-jobs.sh spy
sh monitor.sh
When there are no more jobs running, you should have all the outputs in the spyOutputArea.
The stdout of the job will be in scripts/lip-batch/blahblah.sh.o123456 and the stderr in the corresponding .sh.e123456

Alternatively, you can run interactively by executing for example
physicsAnalysis test/physicsAnalysisParSets_cfg.py spy_zz
(in the bin sourcecode you can find the acceptable codes, or also in the scripts/lip-batch/job-spy_zz.sh and so on)

Currently, the plots get produced correctly at the OS selection step (Folder RecoSteps/[lep_tau|mu_tau]/OS/)

You then need to run 
physicsAnalysis test/physicsAnalysisParSets_cfg.py spyHadd
in order to merge the outputfiles as they should

and finally
physicsAnalysis test/physicsAnalysisParSets_cfg.py spyPlots
in order to produce the plots.

This works out of the box.


Coming to the code, the bin/physicsAnalysis.cc, when called for the spy files, creates an instance of the class SingleStepAnalyzer (interface/SingleStepAnalyzer.hh and 
src/SingleStepAnalyzer.cc) and runs.

The SingleStepAnalyzer::tauDileptonOSAnalysis(...) method selects the objects in the way they are selected in the paper.

You want to add your new computations and plot filling in the SingleStepAnalyzer::fillTauDileptonObjHistograms(...) method.

Plot declarations must go in src/HistogramBuilder.cc


I have to fix the pileup reweighting (the spyfiles do not contain the needed histograms, and 
apparently is not possible to ask for a new version of the spyfiles soon, so I need to hack a bit to fetch the histograms from the original files and
configure the SingleStepAnalyzer to make it fetch the histos from there and not from the current file, basically.







###### End of how to analyze spyfiles ########

# Local pattuple test run (local data file at LIP)
# obsolete, must update: cmsRun LIP/TopTaus/test/createDataPattuple_cfg.py /lustre/data3/cmslocal/samples/CMSSW_5_2_5/test/Run2012B_SingleMu_AOD_PromptReco-v1_000_193_998_0C7DCC80-4E9D-E111-B22A-001D09F25267.root pattuple.root inclusive_mu


Finding HLT trigger on a per-run-range basis, together with the prescale
------------------------------------------------------------------------
ssh lxplus.cern.ch # needed for access to database
cvs co HLTrigger/Tools/python
scram b
cd HLTrigger/Tools/python
source $CMSSW_BASE/LIP/TopTaus/scripts/getTriggers.sh
This will fetch the list of triggers, and depending on what you comment will print or not the prescales table. (see TODO list)


Multiple likelihood fit for improving tau fakes uncertainty
-----------------------------------------------------------
runTauDileptonPDFBuilderFitter tauDileptonAnalysisParSets_cfg.py

Relevant parameters: straightforward cfg file. Must write doc here, though

Produce shapes:
---------------
produceLandSShapes tauDileptonAnalysisParSets_cfg.py --produceOnly (true|false [false])

Relevant parameters: straightforward cfg file.
Command options: 
- produceOnly: (necessary because in plots HH and WH are rescaled, in rootfile not)
               in TBH won't be necessary,  so fuck it.
	       -True: just produces rootfile with all the shapes, no plot.
	       -False: [default] just produces plots, no rootfile.

Run physicsAnalysis - this corresponds to the obsolete PhysicsAnalyzer code
------------------------------------------------------------------
- interactively:
  physicsAnalysis test/physicsAnalysisParSets_cfg.py sample
  (where "sample" is ttbar, wjets, etc. Strings can be found in bin/physicsAnalysis.cc)
- on the LIP batch:
  cd scripts/lip-batch/
  sh combineResults.sh path/to/dir clean --> this cleans the output directory in which you want to store the outputs	
  EDIT submit-jobs.sh in order to change the script path to your local installation.
  EDIT the *output* paths in test/physicsAnalysisParSets_cfg.py . The input are fixed (53X production)
  sh submit-jobs.sh
  qstat -u username --> this checks whether your jobs are running and if they did already finish
  sh combineResults.sh path/to/dir hadd [AB|ABC|ABCD] --> this hadds the relevant output files
  RUN physicsAnalysis test/physicsAnalysisParSets_cfg.py doPlots   ---> this produces plots
  RUN physicsAnalysis test/physicsAnalysisParSets_cfg.py doTables  ---> this produces tables and datacards (if "higgs" is turned on)
- have fun

Produce plots
-------------
cd LIP/TopTaus  # this is necessary, because at the moment bin/physicsAnalysis.cc is configured
                # in such a way as to fetch plots from "data/plotter/". Must change to configurable paths via cfg file
mkdir plots
physicsAnalysis test/physicsAnalysisParSets_cfg.py doPlots

Produce tables
--------------
physicsAnalysis test/physicsAnalysisParSets_cfg.py doTables 
                datacards are not produced, and tbh yields are rescaled to the production cross section of the tbh samples (1.1pb)
physicsAnalysis test/physicsAnalysisParSets_cfg.py doDatacards 
		datacards are produced, and tbh yields are rescaled to the ttbar production cross-section
		

Produce fakerate
----------------
for a list of all the options:
    doTauFakesStudy test/physicsAnalysisParSets_cfg.py --help 
for running everything:
    doTauFakesStudy test/physicsAnalysisParSets_cfg.py --do all

Plot signal cut efficiencies
----------------------------
root -l -b bin/macros/teenyWeenyHeavyChHiggsSignalCutsComparison.C


Datacards
---------
physicsAnalysis test/physicsAnalysisParSets_cfg.py doDatacards

For versioning them:
Install repository (CMS members only):
svn co svn+ssh://vischia@svn.cern.ch/reps/lipcms/Physics/datacards lipcms/Physics/datacards
chiggs datacards are in lipcms/Physics/datacards/chiggs/
xsec datacards are in lipcms/Physics/datacards/xsec/
See lipcms/Physics/datacards/Readme.txt for additional info


CHANGELOG for major updates:
2013-10-07: Added signal injection plotting (plotLimits.cc)
2013-09-19: Finally all the new electron ntuples are working with the already working splitting system
	    Bloody hell, 2 weeks of hadding and re-hadding to tune them!
2013-09-06: Cosmetics. Tag for preapproval of HIG-13-026 (V13-09-06)
2013-09-04: Towards the frozen format for publication
	    Empowered FitVar class for supporting fancy plot names
2013-08-31: Categorization per-btag-multiplicity for datacards
2013-08-30: Improved things for freezing.
	    Correted bug in jes/jer/met systematics shapes
2013-08-22: Split code runs in 15 minutes (previous: one hour)
	    Cleaned lip batch scripts feeding them arguments
	    Better pileup file for ReReco ntuples
	    Added top pt reweighting
2013-08-20: Start CMSSW_5_3_9 cycle. Last tag for CMSSW_5_3_7_patch4: V13-08-20-def
2013-08-07: Switcher for light/heavy H+ in tablebuilder
2013-07-XX: Fixes to datacards
	    b-tags categorization
2013-07-08: Tau energy scale
	    Plots legend in a proper place (fancy plots <3 )
2013-07-02: Removed obsolete syst components
	    Split doDatacards from doTables
2013-07-01: Code cleaning
	    New btagSF payload (will switch when ntuples will be ready)
2013-06-28: Added H+->tb M_H+ = 200 GeV processing
	    Fixed tables for AN (now with H+->tb yields too)
	    Switched to btagsmultiplicity shape for limits computation
2013-06-24: Fixed datacard production
	    Added both channels for mass points in which they are both available
	    Weighted MVA for data_rescaled
2013-06-17: Added plotting of signal cut efficiencies
2013-05-08: Added H+->tb yields to summary table, and fixed in a consistent way its xsec*BR
	    H+->taunu and H+->tb are now stacked in plots
	    Added H+->tb decay to datacards for mH+ = 250GeV/c^2
	    Committed revision 718 for chiggs datacards
2013-05-07: Reworked table builder for not having to run twice (tables, datacards)
	    Almost implemented decoupling of event analysis and application of a selection
	    Switched to text names for nuisance parameters (since february combine likes no numbers - for event counting this is not a problem, but for consistency I switched)
	    Removed unused nuisance parameters (full-hadronic ones)
2013-05-06: Fixed jer smearing/met propagation (condensed in single loop)
	    Fixed normalization for datacards and shapes
	    committed revision 716 for chiggs datacards - supersedes all previous revisions
2013-05-03: Enabled TDRStyle for kNN monitoring plots
	    Not show anymore ratio in R plot
2013-05-02: Enabled analsis of the new H+->tb (noH+->\tau\nu) samples
2013-05-01: Fixed bug in jer smearing propagation to met
2013-04-30: REPOSITORY MOVED TO https://github.com/vischia/TopTaus.git. No more updates will be committed to CERN CVS. After a week of testing at github, LIP/TopTaus/ files will be zeroed with the exception of the Readme.txt
	    plot fixes
	    jer smearing to tf
2013-04-29: committed revision 715 for chiggs datacards
	    cleanup and fix of shapes producer
	    jer and propagation to met fix
	    updated fakes values in summary table
2013-04-27: new kinematic variables added
	    plots ranges for eta tuned
	    some prefixes for optimization
	    metscaling testing
	    cleanup in plots
	    cleanup in todolist
2013-04-24: 2.41AM: added stat+syst bands in ratio plots
	    wJetsAnalysis was not using jerc (bugfix)
	    fixed btag bug for wJetsAnalysis
	    updated btag scale factors
2013-04-22: fixed bug in variables for OS shapes (it was fixed in 2011 and was never committed to the 2012 branch
	    added automatic calculation and table of fakes
2013-04-19: added index.html automatically put inside plots directory for easy and fancy propagation of the plots
	    configured LandsShapesProducer class for heavyChiggs
	    configured TauDileptonTableBuilder for producing datacards with shapes
	    committed revision 714 for chiggs datacards
2013-04-18: added mjj vs mjjb study (discrimination sig/ttbarbkg)
	    added residual mc shape subtraction from dd shape K-S test
	    added combine installation instructions and datacard automatic maker	
	    committed revision 702 for chiggs datacards
	    added doTauFakesStudy (complete tau fakes study)
2013-04-15: added chiggs BRs to TauDileptonTableBuilder, almost finished tau fakes software
2013-04-08: configured combineResults.sh script for different data hadding schemes 
2013-03-27: fixed jet smearing factors
	    fixed index of muon trigger eff muon, just for completeness
	    (it was already correct since the collection is already pt-ordered)
2013-03-22: muon trigger efficiencies scale factors added
	    pileup files splitted per run (data/pileup)
	    lumi per run added to CommonDefinitions.cc
2013-03-21: lepton-jet masses plots added
2013-03-20: fixed pileup reweighting
	    added table with the codes for antiLepton discriminators for taus
	    chosen for chiggs: antiMuonTight for highPt, againstElectronMVA3Medium
	    added class and executable for training distributions for fakes
2013-03-19: fixed tables for SM ttbar
	    added table for heavy charged higgs samples
2013-03-18: new data ntuples with improved splitting.
	    Modified combineResults.sh in order to support directory cleaning
	    New pileup files, and chosen value: 70300
	    data/plotter/samples.xml now has just final name path. Full path is read from cfg file (outputArea parameter)
2013-03-05: ported to NCG
	    improved lip-batch scripting system: now the user path is set once only in submit-jobs.sh
	    I/O and PU filenames moved to cfg files
2013-02-24: fixes: plotter perfectly working
2013-02-22: plotter and table added and integrated into physicsAnalysis.
2013-02-13: updated with 2012 cross sections - must check sample-specific ones
	    jet pt, btag, mlj added for leadingjet/nlj/nnlj
2013-02-11: updated JEC uncertainty sources
	    code is running on samples
	    lip-batch submission implemented
2013-02-08: ended base porting of the code. Now must move stuff to config file.
2013-02-07: added scripts/getTriggers.sh for easy trigger and prescales fetching
	    (will be crucial in particular for etau channel, later on)
2013-01-23: Likelihood fitter for tau fakes has RooDataset which will take into account correlations
            when performing the combined fit. Must just switch likelihoods to use it and to
	    product of likelihoods
2012-12-17: 2011 tagged (V12-12-17), start 2012 heavy developing and code porting
2012-09-23: memory problem solved. Now both lands and mlf are usable with multiple variables
2012-09-21: shapes producer for lands and multiple likelihood fit for fake rates are ready to use.

FIXED TAGS:
- V13-09-06: tag for preapproval of HIG-13-026
- V13-08-20-def: tag for use with first production (ABC1-rereco, C2D-promptReco) in CMSSW_5_3_7_patch4
- V13-08-20/V13-08-20_fix: forget.
- V13-03-07: full working heavy charged higgs tag
- V12-12-21 / V12-12-17: 2011 code.

CHANGELOG for datacards:
rev 718 (2013-05-08): cHiggs: H+->tb decay added to mH+=250GeV/c^2 datacard
rev 716 (2013-05-06): cHiggs: fixed yields and shapes. This version supersedes all the previous.
rev 715 (2012-04-29): cHiggs: moved to use pt_tau shapes. Fixed datacards with values after met correction.
rev 714 (2012-04-19): fixed first shapes files and datacards with support for shapes (R_tau) for heavy charged higgs		
rev 713 (2012-04-19): first shapes files and datacards with support for shapes (R_tau) for heavy charged higgs	
rev 702 (2012-04-18): first counting-only chiggs datacards for heavy charged higgs


TODO:
- clean and update TODO list ;)
- change the interface to FitVar class (constructor) in the whole code (for now, additional fancyName for the variable is set after calling costructor)
- add comp plots for fakes
- fix uncertainty band plotting in case the plot is normalized to 1 (Rtau)
- update tau ID uncertainties
- update uncertainties in HistogramPlotter
- move to txt file the cout of the fakes computation in TauFakesHelper.cc (latex table and everything)
- reweight MC distros con q/g fraction evaluated from MC
- From my 2013-04-22 presentation at TauPOG: recalculate fakes feeding the wmu sample the qcd mva and vice versa, try training with more kinematical variables
- move all 234.s in the code (TauDileptonTableBuilder in particular) to the value from SampleProcessor (or move the xsecs to CommonDefinitions, better) 
- vstrings in physicsAnalysisParSets_cfg.py for pileup syst
- move path of plots output directory to command line or cfg file.
- split JEC uncertainty sources into the 16 components.
- scripts/getTriggers.hs: implement running through sh getTriggers.sh PARS, in order to allow to choose
  at runtime whether to print the prescales table or just the triggers

- Make the unbinned fit work, caralho

- Apply single RooDataset per sample to LandSShapesProducer too

- Rework of LandSShapesProducer and PDFBuilderFitter
  --> New structure: -> single class for building datasets and models
                     -> one inheriting class for shapes production, storing and plotting (LandSShapesProducer)
		     -> one inheriting class for fitting, saving and plotting (PDFBuilderFitter)
		     -> perhaps one single executable with switch via ParSet in config file

- Pattuple producer 
  --> Acquire and improve version from 2013-01-23 - must upload from lipcms svn - in order to use
      multicrab in the future

- Ntuples producer --> Convert plugin

- Analysis code:
                 --> move hardcoded parameters to configuration file via edm::ParameterSet (minimize the need for recompiling)
		 --> import batch submission and modify process in order to run on a given subset of events.
		     (add s.th like void processEvents(uint firstEv, uint lastEv, string dataset){ if(string...) CutflowAnalyzer::process_ttbar(.. process(firstEv, lastEv) ..)  }
		     Doing that needs to redefine the total number of events in the event reweighting according to xsec and N_mc_evts
- Tau energy scale for main analysis code. 

- Split fake contribution into e and mu
  --> Evan/Monica 2013-01-23: "there are different scale factors for e and mu fakes to be applied,
      so you better split the two components in the cutflow yields table"

- Fix crash after natural end when running interactively (low priority)      
