#!/bin/bash

# usage: sh combineResults.sh outputArea [hadd|clean] [AB|ABC|ABCD]

if [ "${2}" = "hadd" ]; then


    if [ "${3}" = "data" ]; then
	DATAPLACE="${1}/nomt-2012-V1-data-MU-20GeV/"
     #DATAPLACE="/lustre/data3/cmslocal/vischia/tau_dilepton/outputFiles444_3/mt-2011-V1-data-MU-20GeV/"
	cd $DATAPLACE
#    if [ "${3}" = "AB" ]; then
#    	hadd -f out-data.root out-Muon_RunA.root out-Muon_RunA_06AugRecover.root out-Muon_RunB_*.root
#    elif ["${3}" = "ABC" ]; then
#        hadd -f out-data.root out-Muon_RunA.root out-Muon_RunA_06AugRecover.root out-Muon_RunB_*.root out-Muon_RunC1.root out-Muon_RunC2_*.root
#    elif ["${3}" = "ABCD" ]; then
 #   hadd -f out-data.root out-Muon_RunA_*.root out-Muon_RunB_*.root out-Muon_RunC_1_*.root out-Muon_RunC_2_*.root out-Muon_RunD_1_*.root out-Muon_RunD_2_*.root &
	hadd -f out-data.root out-Muon_RunA_*.root out-Muon_RunB_*.root out-Muon_RunC_1_*.root out-Muon_RunC_2_*.root out-Muon_RunD_1_*.root out-Muon_RunD_2_*.root out-MuonMissing_RunB_*.root out-MuonMissing_RunC_1_*.root out-MuonMissing_RunC_2_*.root out-MuonMissing_RunD_1_*.root out-MuonMissing_RunD_2_*.root 
	if [ "${4}" = "muon" ]; then
	    hadd -f out-data.root out-Muon_RunA_*.root out-Muon_RunB_*.root out-Muon_RunC_1_*.root out-Muon_RunC_2_*.root out-Muon_RunD_1_*.root out-Muon_RunD_2_*.root out-MuonMissing_RunB_*.root out-MuonMissing_RunC_1_*.root out-MuonMissing_RunC_2_*.root out-MuonMissing_RunD_1_*.root out-MuonMissing_RunD_2_*.root 
	elif [ "${4}" = "electron" ]; then
	    hadd -f out-data.root out-Electron_RunA_*.root out-Electron_RunB_*.root out-Electron_RunC_leg1_*.root out-Electron_RunC_leg2_*.root out-Electron_RunD_leg1_*.root out-Electron_RunD_leg2_*.root out-Electron_MisB_*.root out-Electron_RunC_mis1_*.root out-Electron_RunC_mis2_*.root out-Electron_RunD_mis1_*.root out-Electron_RunD_mis2_*.root   
	fi
	    
#    fi
	
    fi

    if [ "${3}" = "embed" ]; then
	DATAPLACE="${1}/nomt-2012-V1-data-MU-20GeV/"
     #DATAPLACE="/lustre/data3/cmslocal/vischia/tau_dilepton/outputFiles444_3/mt-2011-V1-data-MU-20GeV/"
	cd $DATAPLACE
#    if [ "${3}" = "AB" ]; then
#    	hadd -f out-data.root out-Muon_RunA.root out-Muon_RunA_06AugRecover.root out-Muon_RunB_*.root
#    elif ["${3}" = "ABC" ]; then
#        hadd -f out-data.root out-Muon_RunA.root out-Muon_RunA_06AugRecover.root out-Muon_RunB_*.root out-Muon_RunC1.root out-Muon_RunC2_*.root
#    elif ["${3}" = "ABCD" ]; then
 #   hadd -f out-data.root out-Muon_RunA_*.root out-Muon_RunB_*.root out-Muon_RunC_1_*.root out-Muon_RunC_2_*.root out-Muon_RunD_1_*.root out-Muon_RunD_2_*.root &
	
	hadd -f out-embedded-data.root out-Embedded_RunA_*.root out-Embedded_RunB_*.root out-Embedded_RunC_1_*.root out-Embedded_RunC_2_*.root out-Embedded_RunD_1_*.root out-Embedded_RunD_2_*.root 	
#    fi

	DATAPLACE="${1}/nomt-2012-V1-mc-MU-20GeV/"
	cd $DATAPLACE
        hadd -f out-embedded-ttbar.root out-embedded-ttbar_*.root
    fi

    DATAPLACE="${1}/nomt-2012-V1-mc-MU-20GeV/"
    
    cd $DATAPLACE
    if [ "${3}" = "mc1_1" ]; then
	
    # QCD
	hadd -f $DATAPLACE/out-qcd.root $DATAPLACE/out-qcd_*root $DATAPLACE/out-qcdmu15*.root $DATAPLACE/out-PhotonJets*root
    elif [ "${3}" = "mc1_2" ]; then
	    # zjets
	hadd -f $DATAPLACE/out-zjets.root $DATAPLACE/out-dy_from50_*root $DATAPLACE/out-dy_10to50_*.root
    elif [ "${3}" = "mc1_3" ]; then
    # single top
	hadd -f $DATAPLACE/out-singletop.root $DATAPLACE/out-stop*.root
	
    # diboson
	hadd -f $DATAPLACE/out-dibosons.root $DATAPLACE/out-ww_*.root $DATAPLACE/out-wz_*.root $DATAPLACE/out-zz_*.root
    elif [ "${3}" = "mc1_4" ]; then
    # wjets
	hadd -f $DATAPLACE/out-wjets.root $DATAPLACE/out-wjets_*.root
    elif [ "${3}" = "mc1_5" ]; then
	hadd -f $DATAPLACE/out-ttbar.root $DATAPLACE/out-ttbar_*.root
    elif [ "${3}" = "mc1_6" ]; then
	hadd -f $DATAPLACE/out-ttbar-ddbkg.root $DATAPLACE/out-ttbar-ddbkg_*.root
    elif [ "${3}" = "mc1_7" ]; then
	hadd -f $DATAPLACE/out-ttbar-mcbkg.root $DATAPLACE/out-ttbar-mcbkg_*.root
    elif [ "${3}" = "mc1_8" ]; then
	hadd -f $DATAPLACE/out-ttbar-mutau.root $DATAPLACE/out-ttbar-mutau_*.root
    elif [ "${3}" = "mc1_9" ]; then
	hadd -f $DATAPLACE/out-ttbar-mumu.root $DATAPLACE/out-ttbar-mumu_*.root
    elif [ "${3}" = "mc1_10" ]; then
	hadd -f $DATAPLACE/out-ttbar-emu.root $DATAPLACE/out-ttbar-emu_*.root

    elif [ "${3}" = "mc2_1" ]; then
    # misidentified tau_h component from MC
    hadd -f $DATAPLACE/out-misidbkg.root $DATAPLACE/out-wjets.root $DATAPLACE/out-ttbar-ddbkg.root $DATAPLACE/out-qcd.root

    elif [ "${3}" = "mc2_2" ]; then
    # mc total
    hadd -f $DATAPLACE/out-mc.root $DATAPLACE/out-qcd.root $DATAPLACE/out-wjets.root $DATAPLACE/out-zjets.root $DATAPLACE/out-dibosons.root $DATAPLACE/out-ttbar.root $DATAPLACE/out-singletop.root 
    elif [ "${3}" = "mc2_3" ]; then
   # data driven bkg, using mc
    hadd -f $DATAPLACE/out-ddbkg.root $DATAPLACE/out-qcd.root $DATAPLACE/out-wjets.root $DATAPLACE/out-ttbar-ddbkg.root
    
    ## no ##    # ttbar mc for shapes?
    ## no ##    hadd -f $DATAPLACE/out-ttbar_mcbkg_shapes.root $DATAPLACE/out-ttbar_mcbkg.root $DATAPLACE/out-ttbar_emu.root $DATAPLACE/out-ttbar_mumu.root

    elif [ "${3}" = "mc2_4" ]; then
# mc bkg (w.r.t. top xsec measurement: ttbar is signal for this)
	hadd -f $DATAPLACE/out-mcbkg.root $DATAPLACE/out-zjets.root $DATAPLACE/out-singletop.root $DATAPLACE/out-dibosons.root $DATAPLACE/out-ttbar-mcbkg.root
    ## no ## $DATAPLACE/out-ttbar_emu.root $DATAPLACE/out-ttbar_mumu.root

    # other cont
    elif [ "${3}" = "mc2_5" ]; then
	hadd -f $DATAPLACE/out-ttbar-other.root  $DATAPLACE/out-ttbar-ddbkg.root $DATAPLACE/out-ttbar-mcbkg.root 
	
    elif [ "${3}" = "mc3_1" ]; then
    # allmcmkg (w.r.t. charged higgs search: ttbar is includes as irreducible background)
    hadd -f $DATAPLACE/out-allmcbkg.root $DATAPLACE/out-ttbar.root $DATAPLACE/out-mcbkg.root 
    
    elif [ "${3}" = "mc1_11" ]; then
	hadd -f $DATAPLACE/out-htb-pythia-m180.root $DATAPLACE/out-htb-pythia-m180_*.root 
	hadd -f $DATAPLACE/out-htb-pythia-m200.root $DATAPLACE/out-htb-pythia-m200_*.root 
	hadd -f $DATAPLACE/out-htb-pythia-m220.root $DATAPLACE/out-htb-pythia-m220_*.root 
	hadd -f $DATAPLACE/out-htb-pythia-m240.root $DATAPLACE/out-htb-pythia-m240_*.root 
    elif [ "${3}" = "mc1_12" ]; then
	hadd -f $DATAPLACE/out-htb-pythia-m250.root $DATAPLACE/out-htb-pythia-m250_*.root 
	hadd -f $DATAPLACE/out-htb-pythia-m260.root $DATAPLACE/out-htb-pythia-m260_*.root 
	hadd -f $DATAPLACE/out-htb-pythia-m280.root $DATAPLACE/out-htb-pythia-m280_*.root 
	hadd -f $DATAPLACE/out-htb-pythia-m300.root $DATAPLACE/out-htb-pythia-m300_*.root 
	hadd -f $DATAPLACE/out-htb-pythia-m350.root $DATAPLACE/out-htb-pythia-m350_*.root 
    elif [ "${3}" = "mc1_13" ]; then
	hadd -f $DATAPLACE/out-htb-pythia-m400.root $DATAPLACE/out-htb-pythia-m400_*.root 
	hadd -f $DATAPLACE/out-htb-pythia-m500.root $DATAPLACE/out-htb-pythia-m500_*.root 
	hadd -f $DATAPLACE/out-htb-pythia-m600.root $DATAPLACE/out-htb-pythia-m600_*.root 
	hadd -f $DATAPLACE/out-htb-pythia-m700.root $DATAPLACE/out-htb-pythia-m700_*.root 

    elif [ "${3}" = "mc1_14" ]; then
    hadd -f $DATAPLACE/out-hhhtautaubb-pythia-m260.root $DATAPLACE/out-hhhtautaubb-pythia-m260_*.root 
    hadd -f $DATAPLACE/out-hhhtautaubb-pythia-m300.root $DATAPLACE/out-hhhtautaubb-pythia-m300_*.root 
    hadd -f $DATAPLACE/out-hhhtautaubb-pythia-m350.root $DATAPLACE/out-hhhtautaubb-pythia-m350_*.root 

    elif [ "${3}" = "mc2_6" ]; then
    # higgs  mass points
	hadd -f $DATAPLACE/out-heavyHiggs-pythia-m180.root $DATAPLACE/out-tbh-pythia-m180.root $DATAPLACE/out-htb-pythia-m180.root 
	cp  $DATAPLACE/out-tbh-pythia-m190.root $DATAPLACE/out-heavyHiggs-pythia-m190.root                                          
	hadd -f $DATAPLACE/out-heavyHiggs-pythia-m200.root $DATAPLACE/out-tbh-pythia-m200.root $DATAPLACE/out-htb-pythia-m200.root 
	hadd -f $DATAPLACE/out-heavyHiggs-pythia-m220.root $DATAPLACE/out-tbh-pythia-m220.root $DATAPLACE/out-htb-pythia-m220.root 
	cp  $DATAPLACE/out-htb-pythia-m240.root $DATAPLACE/out-heavyHiggs-pythia-m240.root                                           
    elif [ "${3}" = "mc2_7" ]; then
	hadd -f $DATAPLACE/out-heavyHiggs-pythia-m250.root $DATAPLACE/out-tbh-pythia-m250.root $DATAPLACE/out-htb-pythia-m250.root 
	cp  $DATAPLACE/out-htb-pythia-m260.root $DATAPLACE/out-heavyHiggs-pythia-m260.root                                         
	cp  $DATAPLACE/out-htb-pythia-m280.root $DATAPLACE/out-heavyHiggs-pythia-m280.root                                         
	hadd -f $DATAPLACE/out-heavyHiggs-pythia-m300.root $DATAPLACE/out-tbh-pythia-m300.root $DATAPLACE/out-htb-pythia-m300.root 
	cp  $DATAPLACE/out-htb-pythia-m350.root $DATAPLACE/out-heavyHiggs-pythia-m350.root                                         
    elif [ "${3}" = "mc2_8" ]; then
	hadd -f $DATAPLACE/out-heavyHiggs-pythia-m400.root $DATAPLACE/out-tbh-pythia-m400.root $DATAPLACE/out-htb-pythia-m400.root 
	hadd -f $DATAPLACE/out-heavyHiggs-pythia-m500.root $DATAPLACE/out-tbh-pythia-m500.root $DATAPLACE/out-htb-pythia-m500.root 
	hadd -f $DATAPLACE/out-heavyHiggs-pythia-m600.root $DATAPLACE/out-tbh-pythia-m600.root $DATAPLACE/out-htb-pythia-m600.root 
	cp  $DATAPLACE/out-htb-pythia-m700.root $DATAPLACE/out-heavyHiggs-pythia-m700.root                                         
    elif [ "${3}" = "mc2_9" ]; then
	hadd -f $DATAPLACE/out-wmultijets.root $DATAPLACE/out-wjets.root $DATAPLACE/out-qcd.root
    fi
	
elif [ "${2}" = "clean" ]; then
  # Clean processed files
    DATAPLACE="${1}/nomt-2012-V1-mc-MU-20GeV/"
    rm $DATAPLACE/out-*.root
    rm $DATAPLACE/out-*.txt
    DATAPLACE="${1}/nomt-2012-V1-data-MU-20GeV/"
    rm $DATAPLACE/out-*.root
    rm $DATAPLACE/out-*.txt
    #rm -rf $DATAPLACE/out-qcd.root
    #rm -rf $DATAPLACE/out-zjets.root
    #rm -rf $DATAPLACE/out-singletop.root
    #rm -rf $DATAPLACE/out-dibosons.root
    #rm -rf $DATAPLACE/out-mc.root
    #DATAPLACE="/lip-sw/cmssw/users/nalmeida/new/CMSSW_5_2_5/src/lipcms/Physics/TopTauDileptons2012/mt-2011-V1-data-MU-20GeV/"
    #rm -rf $DATAPLACE/out-data.root 

fi