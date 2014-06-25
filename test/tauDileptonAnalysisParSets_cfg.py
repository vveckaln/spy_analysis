import FWCore.ParameterSet.Config as cms

#########
# Parameter file to give as input to runTauDileptonPDFBuilderFitter and produceLandSShapes
#########

LandSShapesProducerParSetTb = cms.PSet(
    #    outFolder        = cms.string("shapesForDatacard/"),

    # tb
    outFolder        = cms.string("shapes_tb/"),
    datacardsBaseName  = cms.string("datacard_mutau_tb"),
    massPointName   = cms.vstring("m180","m200","m220","m240","m250","m260","m280","m300","m400","m500","m600","m700"),
    inputFileName      = cms.vstring( "out-data.root", "out-htb-pythia-",  "out-data_rescaled.root", "out-ttbar-mutau.root", "out-ttbar-mcbkg.root", "out-singletop.root", "out-dibosons.root" ),
    sampleName      = cms.vstring( "data_obs", "HTB",  "tau_fake", "tt_ltau", "tt_ll", "singleTop","di_boson", "Z_tautau",  "Z_eemumu" ),
    fancySampleName      = cms.vstring("data", "#splitline{H^{+}#rightarrow tb}{[250 GeV]}", "#splitline{#tau_{h} misID}{DD}", "t#bar{t} #rightarrow #mu#tau_{h}", "other t#bar{t}", "Single t", "Diboson","Z #rightarrow #tau#tau", "Z #rightarrow ee,#mu#mu"),
    sampleColour    = cms.vint32( 1,  2, 799,  614,    8,   824,  596, 831, 809),
    #sampleColour    = cms.vint32( 1,  616, 799, 614, 8, 824,  596, 831, 809),
    sampleFillStyle = cms.vint32( 1001, 0, 1001, 1001, 1001,  1001, 1001, 1001, 1001),
    baseDir = cms.vstring("/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-data-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          #                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-data-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/"
                          ),
    minitree = cms.vstring("m_tau_Selected",
                           "m_tau_Selected",
                           "m_tau_DataDriven",
                           "m_tau_Selected",
                           "m_tau_Selected",
                           "m_tau_Selected",
                           "m_tau_Selected"
                           ),
    isFromData  = cms.vint32( 1, 0, 1, 0, 0, 0, 0, 0, 0),
    isDDbkg     = cms.vint32( 0, 0, 1, 0, 0, 0, 0, 0, 0),
    isSignal    = cms.vint32( 0, 1, 0, 0, 0, 0, 0, 0, 0),
    # htb
    signalNormFactor = cms.vdouble(
    1./ 0.081662,
    # 190: 1./ 1.      ,  
    1./ 0.455760,  
    1./ 0.625690, 
    1./ 0.692009,
    1./ 0.705360,
    1./ 0.717428, 
    1./ 0.733824,
    1./ 0.744171,
#    1./ 0.750776,
    1./ 0.722967,
    1./ 0.563428,
    1./ 0.495196,
    1./ 0.459958,
    ),


    ###################
    # Common parameters
    ###################
    outputFileName  = cms.string("shapes_tb"),
    doMultiDimensionalShapes = cms.bool(False),
    unsplitUncertainties = cms.bool(False),

    
    
    # jes +/-, met +/-, jer +/-
    systComponents = cms.vstring(
    "_plus"    , "_minus"    ,
    "_uncplus" , "_uncminus" ,
    "_jerplus" , "_jerminus" ,
    "_bplus"   , "_bminus"   ,
    "_unbplus" , "_unbminus",
    "_topptuncplus", "_topptuncminus",
    "_tesplus", "_tesminus"
    ),

    systFancyComponents = cms.vstring(
    "_jesUp"    , "_jesDown"    ,
    "_umetUp" , "_umetDown" ,
    "_jerUp" , "_jerDown" ,
    "_btagUp"   , "_btagDown"   ,
    "_unbtagUp" , "_unbtagDown",
    "_topptuncUp", "_topptuncDown",
    "_tesUp", "_tesDown"
    ),
    
    #osCutEff               = cms.double(0.69*220.92/242.915), #    osCutEff               = cms.double(0.577749097), #223.82/387.4),
    #    osCutEff               = cms.double(2054.99/4232.500009), #FIXME: subtract residual shape 0.699), # // FIXME: fixme 220.92/242.915), #    osCutEff               = cms.double(0.577749097), #223.82/387.4),
    osCutEff = cms.double(1386*1386/202.1/2045.11),


    #2810.89/5736.58),



    ############################
    ####### VARIABLE(s) ########
    ############################

    ###    toNorm      = cms.bool(True),
    ###    vars        = cms.vstring( "rc_t"    ),
    ###    fancyName  = cms.vstring("R = p_{T}^{lead.track}/E^{#tau}"),
    ###    mins        = cms.vdouble(   -0.099  ),
    ###    maxs        = cms.vdouble(    1.001  ),
    ###    bins        = cms.vint32(      11    ),
    ###    binNames    = cms.vstring( ), 
    ###    hmin        = cms.vdouble(      0    ),
    ###    hmax        = cms.vdouble(    200    ),
    ###    unbinned    = cms.vint32(       0    ),
    ###    smoothOrder = cms.vint32(       3    ),   
    ###    doLogy      = cms.vint32(     0      ),

    #    vars        = cms.vstring( "Dphi_tau_met"   ),
    #    mins        = cms.vdouble(      0           ),
    #    maxs        = cms.vdouble(    4.8           ),
    #    bins        = cms.vint32(     5             ),
    #    hmin        = cms.vdouble(      0          ),
    #    hmax        = cms.vdouble(    200          ),
    #    unbinned    = cms.vint32(       0          ),
    #    smoothOrder = cms.vint32(       3          ),   

    toNorm      = cms.bool(False),
    vars        = cms.vstring("btagmultiplicity_j" ),
    fancyName   = cms.vstring("N_{b-tagged jets}"  ),
    mins        = cms.vdouble(     1               ),
    maxs        = cms.vdouble(   5                 ),
    bins        = cms.vint32(     4                ),
    binNames    = cms.vstring("0", "1","2","3","#geq4"),
    hmin        = cms.vdouble(     0               ),
    hmax        = cms.vdouble(   600               ),
    unbinned    = cms.vint32(      0               ),
    smoothOrder = cms.vint32(      0               ),                
    doLogy      = cms.vint32(     1                ),
    # vars        = cms.vstring(    "pt_t"  ),
    # mins        = cms.vdouble(         0  ),
    # maxs        = cms.vdouble(       500  ),
    # bins        = cms.vint32(         10  ),
    # hmin        = cms.vdouble(         0  ),
    # hmax        = cms.vdouble(       200  ),
    # unbinned    = cms.vint32(          0  ),
    # smoothOrder = cms.vint32(          3  ),
    
    

    #    vars        = cms.vstring( "rc_t"  ,"Dphi_tau_met"   ),
    #    mins        = cms.vdouble(   -0.099,     0           ),
    #    maxs        = cms.vdouble(    1.001,   4.8           ),
    #    bins        = cms.vint32(      6  ,    5             ),
    #    hmin        = cms.vdouble(      0  ,     0          ),
    #    hmax        = cms.vdouble(    200  ,   200          ),
    #    unbinned    = cms.vint32(       0  ,     0          ),
    #    smoothOrder = cms.vint32(       3  ,     3          ),   
    
    
    #    vars        = cms.vstring( "rc_t"    , "pt_t"  ),
    #    mins        = cms.vdouble(   -0.099  ,      0   ),
    #    maxs        = cms.vdouble(    1.001  ,    200  ),
    #    bins        = cms.vint32(      6    ,     5  ),
    #    hmin        = cms.vdouble(      0    ,      0  ),
    #    hmax        = cms.vdouble(    200    ,    100  ),
    #    unbinned    = cms.vint32(       0    ,      0  ),
    #    smoothOrder = cms.vint32(       3    ,      5  ),   
    
    
    # for multiD    vars        = cms.vstring("rc_t"  , "Dphi_tau_met"   ),
    # for multiD    mins        = cms.vdouble(  -0.099,      0           ),
    # for multiD    maxs        = cms.vdouble(   1.001,    4.8           ),
    # for multiD    bins        = cms.vint32(     11  ,     10           ),
    # for multiD    hmin        = cms.vdouble(     0  ,      0           ),
    # for multiD    hmax        = cms.vdouble(   200  ,    200           ),
    # for multiD    unbinned    = cms.vint32(      0  ,      0           ),
    # for multiD    smoothOrder = cms.vint32(      3  ,      3           ),   
    
    #    "pt_t", ,      "pt_t" 
    #         0, ,          -20
    #       200, ,         200 
    #        20, ,          11 
    #         0, ,           0 
    #       100, ,         100 
    #         0, ,           0 
    #         5, ,           5 
    
    
    #    vars        = cms.vstring("rc_t",   "pt_l", "pt_met", "multiplicity_j", "btagmultiplicity_j",    ),
    #    mins        = cms.vdouble(     0,        0,      0,        2,                0,                  ),
    #    maxs        = cms.vdouble(   1.152,    200,    300,      7,                4,                    ),
    #    bins        = cms.vint32(     12,       20,     30,       5,                4,                   ),
    #    hmin        = cms.vdouble(     0,        0,      0,        0,                0,                  ),
    #    hmax        = cms.vdouble(   200,      100,    100,      400,              600,                  ),
    #    unbinned    = cms.vint32(      0,        0,      0,        0,                0,                  ),
    #    smoothOrder = cms.vint32(      3,        5,      5,        0,                0,                  )
    
    #    vars        = cms.vstring( "pt_l", "pt_met", "multiplicity_j", "btagmultiplicity_j","rc_t",   "Dphi_tau_met"    ),
    #    mins        = cms.vdouble(      0,      0,        2,                0,                   0,        0            ),
    #    maxs        = cms.vdouble(    200,    300,      7,                4,                   1.152,    4.8            ),
    #    bins        = cms.vint32(      20,     30,       5,                4,                   12,       10            ),
    #    hmin        = cms.vdouble(      0,      0,        0,                0,                   0,        0            ),
    #    hmax        = cms.vdouble(    100,    100,      400,              600,                 200,      200            ),
    #    unbinned    = cms.vint32(       0,      0,        0,                0,                   0,        0            ),
    #    smoothOrder = cms.vint32(       5,      5,        0,                0,                   3,        3            )
    
    )



LandSShapesProducerParSetTaunu = cms.PSet(
    #    outFolder        = cms.string("shapesForDatacard/"),


    # taunu
    outFolder        = cms.string("shapes_taunu/"),
    datacardsBaseName  = cms.string("datacard_mutau_taunu"),
    massPointName   = cms.vstring("m180", "m200", "m220", "m250", "m300","m400","m500","m600"),
    inputFileName      = cms.vstring( "out-data.root", "out-tbh-pythia-", "out-data_rescaled.root", "out-ttbar-mutau.root", "out-ttbar-mcbkg.root", "out-singletop.root", "out-dibosons.root" ),
    sampleName      = cms.vstring( "data_obs", "TBH", "tau_fake", "tt_ltau", "tt_ll", "singleTop","di_boson", "Z_tautau",  "Z_eemumu" ),
    fancySampleName      = cms.vstring("data", "#splitline{H^{+}#rightarrow#tau#nu}{[250 GeV]}", "#splitline{#tau_{h} misID}{DD}", "t#bar{t} #rightarrow #mu#tau_{h}", "other t#bar{t}", "Single t", "Diboson","Z #rightarrow #tau#tau", "Z #rightarrow ee,#mu#mu"),
    sampleColour    = cms.vint32( 1,  1, 799,  614,    8,   824,  596, 831, 809),
    #sampleColour    = cms.vint32( 1,  616, 799, 614, 8, 824,  596, 831, 809),
    sampleFillStyle = cms.vint32( 1001, 0, 1001, 1001, 1001,  1001, 1001, 1001, 1001),
    baseDir = cms.vstring("/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-data-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
#                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-data-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/"
                          ),
    minitree = cms.vstring("m_tau_Selected",
                           "m_tau_Selected",
                           "m_tau_DataDriven",
                           "m_tau_Selected",
                           "m_tau_Selected",
                           "m_tau_Selected",
                           "m_tau_Selected"
                           ),
    isFromData  = cms.vint32( 1, 0, 1, 0, 0, 0, 0, 0, 0),
    isDDbkg     = cms.vint32( 0, 0, 1, 0, 0, 0, 0, 0, 0),
    isSignal    = cms.vint32( 0, 1, 0, 0, 0, 0, 0, 0, 0),
    # htb
    signalNormFactor = cms.vdouble(
    1./ 0.901490, 
    1./ 0.534418,   
    1./ 0.367483,  
    1./ 0.257198, 
    1./ 0.192875,
    0.98/0.151062,
    0.97/0.110921,
    0.96/0.095882, 
    ),
    


    ###################
    # Common parameters
    ###################
    outputFileName  = cms.string("shapes_taunu"),
    doMultiDimensionalShapes = cms.bool(False),
    unsplitUncertainties = cms.bool(False),

    
    # jes +/-, met +/-, jer +/-
    systComponents = cms.vstring(
    "_plus"    , "_minus"    ,
    "_uncplus" , "_uncminus" ,
    "_jerplus" , "_jerminus" ,
    "_bplus"   , "_bminus"   ,
    "_unbplus" , "_unbminus",
    "_topptuncplus", "_topptuncminus",
    "_tesplus", "_tesminus"
    ),

    systFancyComponents = cms.vstring(
    "_jesUp"    , "_jesDown"    ,
    "_umetUp" , "_umetDown" ,
    "_jerUp" , "_jerDown" ,
    "_btagUp"   , "_btagDown"   ,
    "_unbtagUp" , "_unbtagDown",
    "_topptuncUp", "_topptuncDown",
    "_tesUp", "_tesDown"
    ),
    
    
    #osCutEff               = cms.double(0.69*220.92/242.915), #    osCutEff               = cms.double(0.577749097), #223.82/387.4),
    #    osCutEff               = cms.double(2054.99/4232.500009), #FIXME: subtract residual shape 0.699), # // FIXME: fixme 220.92/242.915), #    osCutEff               = cms.double(0.577749097), #223.82/387.4),
    osCutEff = cms.double(1386*1386/202.1/2045.11),


    #2810.89/5736.58),



    ############################
    ####### VARIABLE(s) ########
    ############################

    ###    toNorm      = cms.bool(True),
    ###    vars        = cms.vstring( "rc_t"    ),
    ###    fancyName  = cms.vstring("R = p_{T}^{lead.track}/E^{#tau}"),
    ###    mins        = cms.vdouble(   -0.099  ),
    ###    maxs        = cms.vdouble(    1.001  ),
    ###    bins        = cms.vint32(      11    ),
    ###    binNames    = cms.vstring( ), 
    ###    hmin        = cms.vdouble(      0    ),
    ###    hmax        = cms.vdouble(    200    ),
    ###    unbinned    = cms.vint32(       0    ),
    ###    smoothOrder = cms.vint32(       3    ),   
    ###    doLogy      = cms.vint32(     0        ),    

    #    vars        = cms.vstring( "Dphi_tau_met"   ),
    #    mins        = cms.vdouble(      0           ),
    #    maxs        = cms.vdouble(    4.8           ),
    #    bins        = cms.vint32(     5             ),
    #    hmin        = cms.vdouble(      0          ),
    #    hmax        = cms.vdouble(    200          ),
    #    unbinned    = cms.vint32(       0          ),
    #    smoothOrder = cms.vint32(       3          ),   

    toNorm      = cms.bool(False),
    vars        = cms.vstring("btagmultiplicity_j" ),
    fancyName   = cms.vstring("N_{b-tagged jets}"  ),
    mins        = cms.vdouble(     1               ),
    maxs        = cms.vdouble(   5                 ),
    bins        = cms.vint32(     4                ),
    binNames    = cms.vstring("0", "1","2","3","#geq4"),
    hmin        = cms.vdouble(     0               ),
    hmax        = cms.vdouble(   600               ),
    unbinned    = cms.vint32(      0               ),
    smoothOrder = cms.vint32(      0               ),                
    doLogy      = cms.vint32(     1                ),
    
    # vars        = cms.vstring(    "pt_t"  ),
    # mins        = cms.vdouble(         0  ),
    # maxs        = cms.vdouble(       500  ),
    # bins        = cms.vint32(         10  ),
    # hmin        = cms.vdouble(         0  ),
    # hmax        = cms.vdouble(       200  ),
    # unbinned    = cms.vint32(          0  ),
    # smoothOrder = cms.vint32(          3  ),
    
    

    #    vars        = cms.vstring( "rc_t"  ,"Dphi_tau_met"   ),
    #    mins        = cms.vdouble(   -0.099,     0           ),
    #    maxs        = cms.vdouble(    1.001,   4.8           ),
    #    bins        = cms.vint32(      6  ,    5             ),
    #    hmin        = cms.vdouble(      0  ,     0          ),
    #    hmax        = cms.vdouble(    200  ,   200          ),
    #    unbinned    = cms.vint32(       0  ,     0          ),
    #    smoothOrder = cms.vint32(       3  ,     3          ),   
    
    
    #    vars        = cms.vstring( "rc_t"    , "pt_t"  ),
    #    mins        = cms.vdouble(   -0.099  ,      0   ),
    #    maxs        = cms.vdouble(    1.001  ,    200  ),
    #    bins        = cms.vint32(      6    ,     5  ),
    #    hmin        = cms.vdouble(      0    ,      0  ),
    #    hmax        = cms.vdouble(    200    ,    100  ),
    #    unbinned    = cms.vint32(       0    ,      0  ),
    #    smoothOrder = cms.vint32(       3    ,      5  ),   
    
    
    # for multiD    vars        = cms.vstring("rc_t"  , "Dphi_tau_met"   ),
    # for multiD    mins        = cms.vdouble(  -0.099,      0           ),
    # for multiD    maxs        = cms.vdouble(   1.001,    4.8           ),
    # for multiD    bins        = cms.vint32(     11  ,     10           ),
    # for multiD    hmin        = cms.vdouble(     0  ,      0           ),
    # for multiD    hmax        = cms.vdouble(   200  ,    200           ),
    # for multiD    unbinned    = cms.vint32(      0  ,      0           ),
    # for multiD    smoothOrder = cms.vint32(      3  ,      3           ),   
    
    #    "pt_t", ,      "pt_t" 
    #         0, ,          -20
    #       200, ,         200 
    #        20, ,          11 
    #         0, ,           0 
    #       100, ,         100 
    #         0, ,           0 
    #         5, ,           5 
    
    
    #    vars        = cms.vstring("rc_t",   "pt_l", "pt_met", "multiplicity_j", "btagmultiplicity_j",    ),
    #    mins        = cms.vdouble(     0,        0,      0,        2,                0,                  ),
    #    maxs        = cms.vdouble(   1.152,    200,    300,      7,                4,                    ),
    #    bins        = cms.vint32(     12,       20,     30,       5,                4,                   ),
    #    hmin        = cms.vdouble(     0,        0,      0,        0,                0,                  ),
    #    hmax        = cms.vdouble(   200,      100,    100,      400,              600,                  ),
    #    unbinned    = cms.vint32(      0,        0,      0,        0,                0,                  ),
    #    smoothOrder = cms.vint32(      3,        5,      5,        0,                0,                  )
    
    #    vars        = cms.vstring( "pt_l", "pt_met", "multiplicity_j", "btagmultiplicity_j","rc_t",   "Dphi_tau_met"    ),
    #    mins        = cms.vdouble(      0,      0,        2,                0,                   0,        0            ),
    #    maxs        = cms.vdouble(    200,    300,      7,                4,                   1.152,    4.8            ),
    #    bins        = cms.vint32(      20,     30,       5,                4,                   12,       10            ),
    #    hmin        = cms.vdouble(      0,      0,        0,                0,                   0,        0            ),
    #    hmax        = cms.vdouble(    100,    100,      400,              600,                 200,      200            ),
    #    unbinned    = cms.vint32(       0,      0,        0,                0,                   0,        0            ),
    #    smoothOrder = cms.vint32(       5,      5,        0,                0,                   3,        3            )
    
    )


### ### LandSShapesProducerParSetMhmax = cms.PSet(
### ###     #    outFolder        = cms.string("shapesForDatacard/"),
### ### 
### ###     # mixed
### ###     outFolder        = cms.string("shapes_mhmax/"),
### ###     datacardsBaseName  = cms.string("datacard_mutau_mhmax"),
### ###     massPointName   = cms.vstring("m180","m200","m220","m250","m300"),
### ###     inputFileName      = cms.vstring( "out-data.root", "out-htb-pythia-", "out-tbh-pythia-", "out-data_rescaled.root", "out-ttbar-mutau.root", "out-ttbar-mcbkg.root", "out-singletop.root", "out-dibosons.root" ),
### ###     sampleName      = cms.vstring( "data_obs", "HTB", "TBH", "tau_fake", "tt_ltau", "tt_ll", "singleTop","di_boson", "Z_tautau",  "Z_eemumu" ),
### ###     fancySampleName      = cms.vstring("data", "#splitline{H^{+}#rightarrow tb}{[250 GeV]}","#splitline{H^{+}#rightarrow#tau#nu}{[250 GeV]}", "#splitline{#tau_{h} misID}{DD}", "t#bar{t} #rightarrow #mu#tau_{h}", "other t#bar{t}", "Single t", "Diboson","Z #rightarrow #tau#tau", "Z #rightarrow ee,#mu#mu"),
### ###     sampleColour    = cms.vint32( 1,  2, 1, 799,  614,    8,   824,  596, 831, 809),
### ###     #sampleColour    = cms.vint32( 1,  616, 799, 614, 8, 824,  596, 831, 809),
### ###     sampleFillStyle = cms.vint32( 1001, 0, 0, 1001, 1001, 1001,  1001, 1001, 1001, 1001),
### ###     baseDir = cms.vstring("/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-data-MU-20GeV/",
### ###                           "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
### ###                           "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
### ###                           "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-data-MU-20GeV/",
### ###                           "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
### ###                           "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
### ###                           "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
### ###                           "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/"
### ###                           ),
### ###     minitree = cms.vstring("m_tau_Selected",
### ###                            "m_tau_Selected",
### ###                            "m_tau_Selected",
### ###                            "m_tau_DataDriven",
### ###                            "m_tau_Selected",
### ###                            "m_tau_Selected",
### ###                            "m_tau_Selected",
### ###                            "m_tau_Selected"
### ###                            ),
### ###     isFromData  = cms.vint32( 1, 0, 0, 1, 0, 0, 0, 0, 0, 0),
### ###     isDDbkg     = cms.vint32( 0, 0, 0, 1, 0, 0, 0, 0, 0, 0),
### ###     isSignal    = cms.vint32( 0, 1, 1, 0, 0, 0, 0, 0, 0, 0),
### ### 
### ###         signalNormFactor = cms.vdouble(
### ###     1./100.,
### ###     # 190: 1./100.,
### ###     1./100.,
### ###     1./100.,
### ###     1./100.,
### ###     1./100.,
### ###     1./100.,
### ###     1./100.,
### ###     1./100.,
### ###     1./100., 
### ###     1./100., 
### ###     1./100., 
### ###     1./100., 
### ###     1./100.
### ###     ),
### ### 
### ### 
### ### 
### ### 
### ### 
### ###     ###################
### ###     # Common parameters
### ###     ###################
### ###     outputFileName  = cms.string("shapes"),
### ###     doMultiDimensionalShapes = cms.bool(False),
### ###     unsplitUncertainties = cms.bool(False),
### ### 
### ###     
### ###     
### ###     # jes +/-, met +/-, jer +/-
### ###     systComponents = cms.vstring(
### ###     "_plus"    , "_minus"    ,
### ###     "_uncplus" , "_uncminus" ,
### ###     "_jerplus" , "_jerminus" ,
### ###     "_bplus"   , "_bminus"   ,
### ###     "_unbplus" , "_unbminus",
### ###     "_topptuncplus", "_topptuncminus",
### ###     "_tesplus", "_tesminus"
### ###     ),
### ### 
### ###     systFancyComponents = cms.vstring(
### ###     "_jesUp"    , "_jesDown"    ,
### ###     "_umetUp" , "_umetDown" ,
### ###     "_jerUp" , "_jerDown" ,
### ###     "_btagUp"   , "_btagDown"   ,
### ###     "_unbtagUp" , "_unbtagDown",
### ###     "_topptuncUp", "_topptuncDown",
### ###     "_tesUp", "_tesDown"
### ###     ),
### ###     
### ###     
### ###     #osCutEff               = cms.double(0.69*220.92/242.915), #    osCutEff               = cms.double(0.577749097), #223.82/387.4),
### ###     #    osCutEff               = cms.double(2054.99/4232.500009), #FIXME: subtract residual shape 0.699), # // FIXME: fixme 220.92/242.915), #    osCutEff               = cms.double(0.577749097), #223.82/387.4),
### ###     osCutEff = cms.double(1767.14*1767.14/202.1/2045.11),
### ### 
### ### 
### ###     #2810.89/5736.58),
### ### 
### ### 
### ### 
### ###     ############################
### ###     ####### VARIABLE(s) ########
### ###     ############################
### ### 
### ### 
### ###     ###    vars        = cms.vstring( "rc_t"    ),
### ###     ###    fancyName  = cms.vstring("R = p_{T}^{lead.track}/E^{#tau}"),
### ###     ###    mins        = cms.vdouble(   -0.099  ),
### ###     ###    maxs        = cms.vdouble(    1.001  ),
### ###     ###    bins        = cms.vint32(      11    ),
### ###     ###    binNames    = cms.vstring( ), 
### ###     ###    hmin        = cms.vdouble(      0    ),
### ###     ###    hmax        = cms.vdouble(    200    ),
### ###     ###    unbinned    = cms.vint32(       0    ),
### ###     ###    smoothOrder = cms.vint32(       3    ),   
### ###     
### ### 
### ###     #    vars        = cms.vstring( "Dphi_tau_met"   ),
### ###     #    mins        = cms.vdouble(      0           ),
### ###     #    maxs        = cms.vdouble(    4.8           ),
### ###     #    bins        = cms.vint32(     5             ),
### ###     #    hmin        = cms.vdouble(      0          ),
### ###     #    hmax        = cms.vdouble(    200          ),
### ###     #    unbinned    = cms.vint32(       0          ),
### ###     #    smoothOrder = cms.vint32(       3          ),   
### ###     
### ###     vars        = cms.vstring("btagmultiplicity_j" ),
### ###     fancyName   = cms.vstring("N_{b-tagged jets}"  ),
### ###     mins        = cms.vdouble(     1               ),
### ###     maxs        = cms.vdouble(   5                 ),
### ###     bins        = cms.vint32(     4                ),
### ###     binNames    = cms.vstring("0", "1","2","3","#geq4"),
### ###     hmin        = cms.vdouble(     0               ),
### ###     hmax        = cms.vdouble(   600               ),
### ###     unbinned    = cms.vint32(      0               ),
### ###     smoothOrder = cms.vint32(      0               ),                
### ###     
### ###     # vars        = cms.vstring(    "pt_t"  ),
### ###     # mins        = cms.vdouble(         0  ),
### ###     # maxs        = cms.vdouble(       500  ),
### ###     # bins        = cms.vint32(         10  ),
### ###     # hmin        = cms.vdouble(         0  ),
### ###     # hmax        = cms.vdouble(       200  ),
### ###     # unbinned    = cms.vint32(          0  ),
### ###     # smoothOrder = cms.vint32(          3  ),
### ###     
### ###     
### ### 
### ###     #    vars        = cms.vstring( "rc_t"  ,"Dphi_tau_met"   ),
### ###     #    mins        = cms.vdouble(   -0.099,     0           ),
### ###     #    maxs        = cms.vdouble(    1.001,   4.8           ),
### ###     #    bins        = cms.vint32(      6  ,    5             ),
### ###     #    hmin        = cms.vdouble(      0  ,     0          ),
### ###     #    hmax        = cms.vdouble(    200  ,   200          ),
### ###     #    unbinned    = cms.vint32(       0  ,     0          ),
### ###     #    smoothOrder = cms.vint32(       3  ,     3          ),   
### ###     
### ###     
### ###     #    vars        = cms.vstring( "rc_t"    , "pt_t"  ),
### ###     #    mins        = cms.vdouble(   -0.099  ,      0   ),
### ###     #    maxs        = cms.vdouble(    1.001  ,    200  ),
### ###     #    bins        = cms.vint32(      6    ,     5  ),
### ###     #    hmin        = cms.vdouble(      0    ,      0  ),
### ###     #    hmax        = cms.vdouble(    200    ,    100  ),
### ###     #    unbinned    = cms.vint32(       0    ,      0  ),
### ###     #    smoothOrder = cms.vint32(       3    ,      5  ),   
### ###     
### ###     
### ###     # for multiD    vars        = cms.vstring("rc_t"  , "Dphi_tau_met"   ),
### ###     # for multiD    mins        = cms.vdouble(  -0.099,      0           ),
### ###     # for multiD    maxs        = cms.vdouble(   1.001,    4.8           ),
### ###     # for multiD    bins        = cms.vint32(     11  ,     10           ),
### ###     # for multiD    hmin        = cms.vdouble(     0  ,      0           ),
### ###     # for multiD    hmax        = cms.vdouble(   200  ,    200           ),
### ###     # for multiD    unbinned    = cms.vint32(      0  ,      0           ),
### ###     # for multiD    smoothOrder = cms.vint32(      3  ,      3           ),   
### ###     
### ###     #    "pt_t", ,      "pt_t" 
### ###     #         0, ,          -20
### ###     #       200, ,         200 
### ###     #        20, ,          11 
### ###     #         0, ,           0 
### ###     #       100, ,         100 
### ###     #         0, ,           0 
### ###     #         5, ,           5 
### ###     
### ###     
### ###     #    vars        = cms.vstring("rc_t",   "pt_l", "pt_met", "multiplicity_j", "btagmultiplicity_j",    ),
### ###     #    mins        = cms.vdouble(     0,        0,      0,        2,                0,                  ),
### ###     #    maxs        = cms.vdouble(   1.152,    200,    300,      7,                4,                    ),
### ###     #    bins        = cms.vint32(     12,       20,     30,       5,                4,                   ),
### ###     #    hmin        = cms.vdouble(     0,        0,      0,        0,                0,                  ),
### ###     #    hmax        = cms.vdouble(   200,      100,    100,      400,              600,                  ),
### ###     #    unbinned    = cms.vint32(      0,        0,      0,        0,                0,                  ),
### ###     #    smoothOrder = cms.vint32(      3,        5,      5,        0,                0,                  )
### ###     
### ###     #    vars        = cms.vstring( "pt_l", "pt_met", "multiplicity_j", "btagmultiplicity_j","rc_t",   "Dphi_tau_met"    ),
### ###     #    mins        = cms.vdouble(      0,      0,        2,                0,                   0,        0            ),
### ###     #    maxs        = cms.vdouble(    200,    300,      7,                4,                   1.152,    4.8            ),
### ###     #    bins        = cms.vint32(      20,     30,       5,                4,                   12,       10            ),
### ###     #    hmin        = cms.vdouble(      0,      0,        0,                0,                   0,        0            ),
### ###     #    hmax        = cms.vdouble(    100,    100,      400,              600,                 200,      200            ),
### ###     #    unbinned    = cms.vint32(       0,      0,        0,                0,                   0,        0            ),
### ###     #    smoothOrder = cms.vint32(       5,      5,        0,                0,                   3,        3            )
### ###     
### ###     )



LandSShapesProducerParSetMhmax = cms.PSet(
    #    outFolder        = cms.string("shapesForDatacard/"),

    # mixed
    outFolder        = cms.string("shapes_mhmax/"),
    datacardsBaseName  = cms.string("datacard_mutau_mhmax"),
    massPointName   = cms.vstring("m180","m200","m220","m250","m300"),
    inputFileName      = cms.vstring( "out-data.root", "out-heavyHiggs-pythia-", "out-data_rescaled.root", "out-ttbar-mutau.root", "out-ttbar-mcbkg.root", "out-singletop.root", "out-dibosons.root" ),
    sampleName      = cms.vstring( "data_obs", "HTB", "tau_fake", "tt_ltau", "tt_ll", "singleTop","di_boson", "Z_tautau",  "Z_eemumu" ),
    fancySampleName      = cms.vstring("data", "#splitline{H^{+}#rightarrow tb,#tau#nu}{[250 GeV]}", "#splitline{#tau_{h} misID}{DD}", "t#bar{t} #rightarrow #mu#tau_{h}", "other t#bar{t}", "Single t", "Diboson","Z #rightarrow #tau#tau", "Z #rightarrow ee,#mu#mu"),
    sampleColour    = cms.vint32( 1,  2, 799,  614,    8,   824,  596, 831, 809),
    #sampleColour    = cms.vint32( 1,  616, 799, 614, 8, 824,  596, 831, 809),
    sampleFillStyle = cms.vint32( 1001, 0, 1001, 1001, 1001,  1001, 1001, 1001, 1001),
    baseDir = cms.vstring("/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-data-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
#                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-data-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/"
                          ),
    minitree = cms.vstring("m_tau_Selected",
                           "m_tau_Selected",
                           "m_tau_DataDriven",
                           "m_tau_Selected",
                           "m_tau_Selected",
                           "m_tau_Selected",
                           "m_tau_Selected"
                           ),
    isFromData  = cms.vint32( 1, 0, 1, 0, 0, 0, 0, 0, 0),
    isDDbkg     = cms.vint32( 0, 0, 1, 0, 0, 0, 0, 0, 0),
    isSignal    = cms.vint32( 0, 1, 0, 0, 0, 0, 0, 0, 0),

        signalNormFactor = cms.vdouble(
    1./100.,
    # 190: 1./100.,
    1./100.,
    1./100.,
    1./100.,
    1./100.,
    1./100.,
    1./100.,
    1./100.,
    1./100., 
    1./100., 
    1./100., 
    1./100., 
    1./100.
    ),





    ###################
    # Common parameters
    ###################
    outputFileName  = cms.string("shapes"),
    doMultiDimensionalShapes = cms.bool(False),
    unsplitUncertainties = cms.bool(False),

    
    
    # jes +/-, met +/-, jer +/-
    systComponents = cms.vstring(
    "_plus"    , "_minus"    ,
    "_uncplus" , "_uncminus" ,
    "_jerplus" , "_jerminus" ,
    "_bplus"   , "_bminus"   ,
    "_unbplus" , "_unbminus",
    "_topptuncplus", "_topptuncminus",
    "_tesplus", "_tesminus"
    ),

    systFancyComponents = cms.vstring(
    "_jesUp"    , "_jesDown"    ,
    "_umetUp" , "_umetDown" ,
    "_jerUp" , "_jerDown" ,
    "_btagUp"   , "_btagDown"   ,
    "_unbtagUp" , "_unbtagDown",
    "_topptuncUp", "_topptuncDown",
    "_tesUp", "_tesDown"
    ),
    
    
    #osCutEff               = cms.double(0.69*220.92/242.915), #    osCutEff               = cms.double(0.577749097), #223.82/387.4),
    #    osCutEff               = cms.double(2054.99/4232.500009), #FIXME: subtract residual shape 0.699), # // FIXME: fixme 220.92/242.915), #    osCutEff               = cms.double(0.577749097), #223.82/387.4),
    osCutEff = cms.double(1386*1386/202.1/2045.11),


    #2810.89/5736.58),



    ############################
    ####### VARIABLE(s) ########
    ############################

    ###    toNorm      = cms.bool(True),
    ###    vars        = cms.vstring( "rc_t"    ),
    ###    fancyName  = cms.vstring("R = p_{T}^{lead.track}/E^{#tau}"),
    ###    mins        = cms.vdouble(   -0.099  ),
    ###    maxs        = cms.vdouble(    1.001  ),
    ###    bins        = cms.vint32(      11    ),
    ###    binNames    = cms.vstring( ), 
    ###    hmin        = cms.vdouble(      0    ),
    ###    hmax        = cms.vdouble(    200    ),
    ###    unbinned    = cms.vint32(       0    ),
    ###    smoothOrder = cms.vint32(       3    ),   
    ###    doLogy      = cms.vint32(     0        ),    

    #    vars        = cms.vstring( "Dphi_tau_met"   ),
    #    mins        = cms.vdouble(      0           ),
    #    maxs        = cms.vdouble(    4.8           ),
    #    bins        = cms.vint32(     5             ),
    #    hmin        = cms.vdouble(      0          ),
    #    hmax        = cms.vdouble(    200          ),
    #    unbinned    = cms.vint32(       0          ),
    #    smoothOrder = cms.vint32(       3          ),   

    toNorm      = cms.bool(False),
    vars        = cms.vstring("btagmultiplicity_j" ),
    fancyName   = cms.vstring("N_{b-tagged jets}"  ),
    mins        = cms.vdouble(     1               ),
    maxs        = cms.vdouble(   5                 ),
    bins        = cms.vint32(     4                ),
    binNames    = cms.vstring("0", "1","2","3","#geq4"),
    hmin        = cms.vdouble(     0               ),
    hmax        = cms.vdouble(   600               ),
    unbinned    = cms.vint32(      0               ),
    smoothOrder = cms.vint32(      0               ),                
    doLogy      = cms.vint32(     1                ),
    
    # vars        = cms.vstring(    "pt_t"  ),
    # mins        = cms.vdouble(         0  ),
    # maxs        = cms.vdouble(       500  ),
    # bins        = cms.vint32(         10  ),
    # hmin        = cms.vdouble(         0  ),
    # hmax        = cms.vdouble(       200  ),
    # unbinned    = cms.vint32(          0  ),
    # smoothOrder = cms.vint32(          3  ),
    
    

    #    vars        = cms.vstring( "rc_t"  ,"Dphi_tau_met"   ),
    #    mins        = cms.vdouble(   -0.099,     0           ),
    #    maxs        = cms.vdouble(    1.001,   4.8           ),
    #    bins        = cms.vint32(      6  ,    5             ),
    #    hmin        = cms.vdouble(      0  ,     0          ),
    #    hmax        = cms.vdouble(    200  ,   200          ),
    #    unbinned    = cms.vint32(       0  ,     0          ),
    #    smoothOrder = cms.vint32(       3  ,     3          ),   
    
    
    #    vars        = cms.vstring( "rc_t"    , "pt_t"  ),
    #    mins        = cms.vdouble(   -0.099  ,      0   ),
    #    maxs        = cms.vdouble(    1.001  ,    200  ),
    #    bins        = cms.vint32(      6    ,     5  ),
    #    hmin        = cms.vdouble(      0    ,      0  ),
    #    hmax        = cms.vdouble(    200    ,    100  ),
    #    unbinned    = cms.vint32(       0    ,      0  ),
    #    smoothOrder = cms.vint32(       3    ,      5  ),   
    
    
    # for multiD    vars        = cms.vstring("rc_t"  , "Dphi_tau_met"   ),
    # for multiD    mins        = cms.vdouble(  -0.099,      0           ),
    # for multiD    maxs        = cms.vdouble(   1.001,    4.8           ),
    # for multiD    bins        = cms.vint32(     11  ,     10           ),
    # for multiD    hmin        = cms.vdouble(     0  ,      0           ),
    # for multiD    hmax        = cms.vdouble(   200  ,    200           ),
    # for multiD    unbinned    = cms.vint32(      0  ,      0           ),
    # for multiD    smoothOrder = cms.vint32(      3  ,      3           ),   
    
    #    "pt_t", ,      "pt_t" 
    #         0, ,          -20
    #       200, ,         200 
    #        20, ,          11 
    #         0, ,           0 
    #       100, ,         100 
    #         0, ,           0 
    #         5, ,           5 
    
    
    #    vars        = cms.vstring("rc_t",   "pt_l", "pt_met", "multiplicity_j", "btagmultiplicity_j",    ),
    #    mins        = cms.vdouble(     0,        0,      0,        2,                0,                  ),
    #    maxs        = cms.vdouble(   1.152,    200,    300,      7,                4,                    ),
    #    bins        = cms.vint32(     12,       20,     30,       5,                4,                   ),
    #    hmin        = cms.vdouble(     0,        0,      0,        0,                0,                  ),
    #    hmax        = cms.vdouble(   200,      100,    100,      400,              600,                  ),
    #    unbinned    = cms.vint32(      0,        0,      0,        0,                0,                  ),
    #    smoothOrder = cms.vint32(      3,        5,      5,        0,                0,                  )
    
    #    vars        = cms.vstring( "pt_l", "pt_met", "multiplicity_j", "btagmultiplicity_j","rc_t",   "Dphi_tau_met"    ),
    #    mins        = cms.vdouble(      0,      0,        2,                0,                   0,        0            ),
    #    maxs        = cms.vdouble(    200,    300,      7,                4,                   1.152,    4.8            ),
    #    bins        = cms.vint32(      20,     30,       5,                4,                   12,       10            ),
    #    hmin        = cms.vdouble(      0,      0,        0,                0,                   0,        0            ),
    #    hmax        = cms.vdouble(    100,    100,      400,              600,                 200,      200            ),
    #    unbinned    = cms.vint32(       0,      0,        0,                0,                   0,        0            ),
    #    smoothOrder = cms.vint32(       5,      5,        0,                0,                   3,        3            )
    
    )


LandSShapesProducerParSetMhmodp_tanb5 = cms.PSet(
    #    outFolder        = cms.string("shapesForDatacard/"),

    # mixed
    outFolder        = cms.string("shapes_mhmodp_tanb5/"),
    datacardsBaseName  = cms.string("datacard_mutau_mhmodp_tanb5"),
    massPointName   = cms.vstring("m180","m200","m220","m250","m300","m400","m500","m600"),
    inputFileName      = cms.vstring( "out-data.root", "out-heavyHiggs-pythia-", "out-data_rescaled.root", "out-ttbar-mutau.root", "out-ttbar-mcbkg.root", "out-singletop.root", "out-dibosons.root" ),
    sampleName      = cms.vstring( "data_obs", "HTB", "tau_fake", "tt_ltau", "tt_ll", "singleTop","di_boson", "Z_tautau",  "Z_eemumu" ),
    fancySampleName      = cms.vstring("data", "#splitline{H^{+}#rightarrow tb,#tau#nu}{[250 GeV]}", "#splitline{#tau_{h} misID}{DD}", "t#bar{t} #rightarrow #mu#tau_{h}", "other t#bar{t}", "Single t", "Diboson","Z #rightarrow #tau#tau", "Z #rightarrow ee,#mu#mu"),
    sampleColour    = cms.vint32( 1,  2, 799,  614,    8,   824,  596, 831, 809),
    #sampleColour    = cms.vint32( 1,  616, 799, 614, 8, 824,  596, 831, 809),
    sampleFillStyle = cms.vint32( 1001, 0, 1001, 1001, 1001,  1001, 1001, 1001, 1001),
    baseDir = cms.vstring("/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-data-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-data-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/"
                          ),
    minitree = cms.vstring("m_tau_Selected",
                           "m_tau_Selected",
                           "m_tau_DataDriven",
                           "m_tau_Selected",
                           "m_tau_Selected",
                           "m_tau_Selected",
                           "m_tau_Selected"
                           ),
    isFromData  = cms.vint32( 1, 0, 1, 0, 0, 0, 0, 0, 0),
    isDDbkg     = cms.vint32( 0, 0, 1, 0, 0, 0, 0, 0, 0),
    isSignal    = cms.vint32( 0, 1, 0, 0, 0, 0, 0, 0, 0),
    
    signalNormFactor = cms.vdouble(
    1.,
    # 190: 1./100.,
    1.,
    1.,
    1.,
    1.,
    1.,
    1.,
    1.,
    1., 
    1., 
    1., 
    1., 
    1.
    ),





    ###################
    # Common parameters
    ###################
    outputFileName  = cms.string("shapes"),
    doMultiDimensionalShapes = cms.bool(False),
    unsplitUncertainties = cms.bool(False),

    
    
    # jes +/-, met +/-, jer +/-
    systComponents = cms.vstring(
    "_plus"    , "_minus"    ,
    "_uncplus" , "_uncminus" ,
    "_jerplus" , "_jerminus" ,
    "_bplus"   , "_bminus"   ,
    "_unbplus" , "_unbminus",
    "_topptuncplus", "_topptuncminus",
    "_tesplus", "_tesminus"
    ),

    systFancyComponents = cms.vstring(
    "_jesUp"    , "_jesDown"    ,
    "_umetUp" , "_umetDown" ,
    "_jerUp" , "_jerDown" ,
    "_btagUp"   , "_btagDown"   ,
    "_unbtagUp" , "_unbtagDown",
    "_topptuncUp", "_topptuncDown",
    "_tesUp", "_tesDown"
    ),
    
    
    #osCutEff               = cms.double(0.69*220.92/242.915), #    osCutEff               = cms.double(0.577749097), #223.82/387.4),
    #    osCutEff               = cms.double(2054.99/4232.500009), #FIXME: subtract residual shape 0.699), # // FIXME: fixme 220.92/242.915), #    osCutEff               = cms.double(0.577749097), #223.82/387.4),
    osCutEff = cms.double(1386*1386/202.1/2045.11),


    #2810.89/5736.58),



    ############################
    ####### VARIABLE(s) ########
    ############################

    ###    toNorm      = cms.bool(True),
    ###    vars        = cms.vstring( "rc_t"    ),
    ###    fancyName  = cms.vstring("R = p_{T}^{lead.track}/E^{#tau}"),
    ###    mins        = cms.vdouble(   -0.099  ),
    ###    maxs        = cms.vdouble(    1.001  ),
    ###    bins        = cms.vint32(      11    ),
    ###    binNames    = cms.vstring( ), 
    ###    hmin        = cms.vdouble(      0    ),
    ###    hmax        = cms.vdouble(    200    ),
    ###    unbinned    = cms.vint32(       0    ),
    ###    smoothOrder = cms.vint32(       3    ),   
    ###    doLogy      = cms.vint32(     0        ),    

    #    vars        = cms.vstring( "Dphi_tau_met"   ),
    #    mins        = cms.vdouble(      0           ),
    #    maxs        = cms.vdouble(    4.8           ),
    #    bins        = cms.vint32(     5             ),
    #    hmin        = cms.vdouble(      0          ),
    #    hmax        = cms.vdouble(    200          ),
    #    unbinned    = cms.vint32(       0          ),
    #    smoothOrder = cms.vint32(       3          ),   

    toNorm      = cms.bool(False),
    vars        = cms.vstring("btagmultiplicity_j" ),
    fancyName   = cms.vstring("N_{b-tagged jets}"  ),
    mins        = cms.vdouble(     1               ),
    maxs        = cms.vdouble(   5                 ),
    bins        = cms.vint32(     4                ),
    binNames    = cms.vstring("0", "1","2","3","#geq4"),
    hmin        = cms.vdouble(     0               ),
    hmax        = cms.vdouble(   600               ),
    unbinned    = cms.vint32(      0               ),
    smoothOrder = cms.vint32(      0               ),                
    doLogy      = cms.vint32(     1                ),
    
    
    )


LandSShapesProducerParSetMhmodp_tanb30 = cms.PSet(
    #    outFolder        = cms.string("shapesForDatacard/"),

    # mixed
    outFolder        = cms.string("shapes_mhmodp_tanb30/"),
    datacardsBaseName  = cms.string("datacard_mutau_mhmodp_tanb30"),
    massPointName   = cms.vstring("m180","m200","m220","m250","m300","m400","m500","m600"),
    inputFileName      = cms.vstring( "out-data.root", "out-heavyHiggs-pythia-", "out-data_rescaled.root", "out-ttbar-mutau.root", "out-ttbar-mcbkg.root", "out-singletop.root", "out-dibosons.root" ),
    sampleName      = cms.vstring( "data_obs", "HTB", "tau_fake", "tt_ltau", "tt_ll", "singleTop","di_boson", "Z_tautau",  "Z_eemumu" ),
    fancySampleName      = cms.vstring("data", "#splitline{H^{+}#rightarrow tb,#tau#nu}{[250 GeV]}", "#splitline{#tau_{h} misID}{DD}", "t#bar{t} #rightarrow #mu#tau_{h}", "other t#bar{t}", "Single t", "Diboson","Z #rightarrow #tau#tau", "Z #rightarrow ee,#mu#mu"),
    sampleColour    = cms.vint32( 1,  2, 799,  614,    8,   824,  596, 831, 809),
    #sampleColour    = cms.vint32( 1,  616, 799, 614, 8, 824,  596, 831, 809),
    sampleFillStyle = cms.vint32( 1001, 0, 1001, 1001, 1001,  1001, 1001, 1001, 1001),
    baseDir = cms.vstring("/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-data-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-data-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/"
                          ),
    minitree = cms.vstring("m_tau_Selected",
                           "m_tau_Selected",
                           "m_tau_DataDriven",
                           "m_tau_Selected",
                           "m_tau_Selected",
                           "m_tau_Selected",
                           "m_tau_Selected"
                           ),
    isFromData  = cms.vint32( 1, 0, 1, 0, 0, 0, 0, 0, 0),
    isDDbkg     = cms.vint32( 0, 0, 1, 0, 0, 0, 0, 0, 0),
    isSignal    = cms.vint32( 0, 1, 0, 0, 0, 0, 0, 0, 0),

        signalNormFactor = cms.vdouble(
    1.,
    # 190: 1./100.,
    1.,
    1.,
    1.,
    1.,
    1,
    1,
    1,
    1.,
    1.,
    1.,
    1., 
    1., 
    1., 
    1., 
    1.
    ),

    ###################
    # Common parameters
    ###################
    outputFileName  = cms.string("shapes"),
    doMultiDimensionalShapes = cms.bool(False),
    unsplitUncertainties = cms.bool(False),

    
    
    # jes +/-, met +/-, jer +/-
    systComponents = cms.vstring(
    "_plus"    , "_minus"    ,
    "_uncplus" , "_uncminus" ,
    "_jerplus" , "_jerminus" ,
    "_bplus"   , "_bminus"   ,
    "_unbplus" , "_unbminus",
    "_topptuncplus", "_topptuncminus",
    "_tesplus", "_tesminus"
    ),

    systFancyComponents = cms.vstring(
    "_jesUp"    , "_jesDown"    ,
    "_umetUp" , "_umetDown" ,
    "_jerUp" , "_jerDown" ,
    "_btagUp"   , "_btagDown"   ,
    "_unbtagUp" , "_unbtagDown",
    "_topptuncUp", "_topptuncDown",
    "_tesUp", "_tesDown"
    ),
    
    
    #osCutEff               = cms.double(0.69*220.92/242.915), #    osCutEff               = cms.double(0.577749097), #223.82/387.4),
    #    osCutEff               = cms.double(2054.99/4232.500009), #FIXME: subtract residual shape 0.699), # // FIXME: fixme 220.92/242.915), #    osCutEff               = cms.double(0.577749097), #223.82/387.4),
    osCutEff = cms.double(1386*1386/202.1/2045.11),


    #2810.89/5736.58),



    ############################
    ####### VARIABLE(s) ########
    ############################

    ###    toNorm      = cms.bool(True),
    ###    vars        = cms.vstring( "rc_t"    ),
    ###    fancyName  = cms.vstring("R = p_{T}^{lead.track}/E^{#tau}"),
    ###    mins        = cms.vdouble(   -0.099  ),
    ###    maxs        = cms.vdouble(    1.001  ),
    ###    bins        = cms.vint32(      11    ),
    ###    binNames    = cms.vstring( ), 
    ###    hmin        = cms.vdouble(      0    ),
    ###    hmax        = cms.vdouble(    200    ),
    ###    unbinned    = cms.vint32(       0    ),
    ###    smoothOrder = cms.vint32(       3    ),   
    ###    doLogy      = cms.vint32(     0        ),    

    #    vars        = cms.vstring( "Dphi_tau_met"   ),
    #    mins        = cms.vdouble(      0           ),
    #    maxs        = cms.vdouble(    4.8           ),
    #    bins        = cms.vint32(     5             ),
    #    hmin        = cms.vdouble(      0          ),
    #    hmax        = cms.vdouble(    200          ),
    #    unbinned    = cms.vint32(       0          ),
    #    smoothOrder = cms.vint32(       3          ),   

    toNorm      = cms.bool(False),
    vars        = cms.vstring("btagmultiplicity_j" ),
    fancyName   = cms.vstring("N_{b-tagged jets}"  ),
    mins        = cms.vdouble(     1               ),
    maxs        = cms.vdouble(   5                 ),
    bins        = cms.vint32(     4                ),
    binNames    = cms.vstring("0", "1","2","3","#geq4"),
    hmin        = cms.vdouble(     0               ),
    hmax        = cms.vdouble(   600               ),
    unbinned    = cms.vint32(      0               ),
    smoothOrder = cms.vint32(      0               ),                
    doLogy      = cms.vint32(     1                ),
    
    
    )


LandSShapesProducerParSetPlot = cms.PSet(
    #    outFolder        = cms.string("shapesForDatacard/"),


    # plot:
    outFolder        = cms.string("draw_shapes/"),
    datacardsBaseName  = cms.string("dummy"),
    massPointName = cms.vstring("m250"),
    inputFileName      = cms.vstring( "out-data.root", "out-htb-pythia-", "out-tbh-pythia-", "out-data_rescaled.root", "out-ttbar-mutau.root", "out-ttbar-mcbkg.root", "out-singletop.root", "out-dibosons.root" ),
    sampleName      = cms.vstring( "data_obs", "HTB", "TBH", "tau_fake", "tt_ltau", "tt_ll", "singleTop","di_boson", "Z_tautau",  "Z_eemumu" ),
#    fancySampleName      = cms.vstring("data", "#splitline{100#times H^{+}#rightarrow tb}{[250 GeV]}","#splitline{100#times H^{+}#rightarrow#tau#nu}{[250 GeV]}", "#splitline{#tau_{h} misID}{DD}", "t#bar{t} #rightarrow #mu#tau_{h}", "other t#bar{t}", "Single t", "Diboson","Z #rightarrow #tau#tau", "Z #rightarrow ee,#mu#mu"),
    fancySampleName      = cms.vstring("data", "#splitline{H^{+}#rightarrow tb [1pb]}{[250 GeV]}","#splitline{H^{+}#rightarrow#tau#nu [1pb]}{[250 GeV]}", "#splitline{#tau_{h} misID}{DD}", "t#bar{t} #rightarrow #mu#tau_{h}", "other t#bar{t}", "Single t", "Diboson","Z #rightarrow #tau#tau", "Z #rightarrow ee,#mu#mu"),
    sampleColour    = cms.vint32( 1,  2, 1, 799,  614,    8,   824,  596, 831, 809),
    #sampleColour    = cms.vint32( 1,  616, 799, 614, 8, 824,  596, 831, 809),
    sampleFillStyle = cms.vint32( 1001, 0, 0, 1001, 1001, 1001,  1001, 1001, 1001, 1001),
    baseDir = cms.vstring("/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-data-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          #"/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-data-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May_n/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/",
                          "/lustre/ncg.ingrid.pt/cmslocal/vischia/store/tau/Jan13ReReco/dataRedo/test_May/nomt-2012-V1-mc-MU-20GeV/"
                          ),

    minitree = cms.vstring("m_tau_Selected",
                           "m_tau_Selected",
                           "m_tau_Selected",
                           "m_tau_DataDriven",
                           "m_tau_Selected",
                           "m_tau_Selected",
                           "m_tau_Selected",
                           "m_tau_Selected"
                           ),
    isFromData  = cms.vint32( 1, 0, 0, 1, 0, 0, 0, 0, 0, 0),
    isDDbkg     = cms.vint32( 0, 0, 0, 1, 0, 0, 0, 0, 0, 0),
    isSignal    = cms.vint32( 0, 1, 1, 0, 0, 0, 0, 0, 0, 0),
    # htb
    signalNormFactor = cms.vdouble(
    1.524899/2.
    ),

###    1./ 0.081662,
###    # 190: 1./ 1.      ,  
###    1./ 0.455760,  
###    1./ 0.625690, 
###    1./ 0.692009,
###    1./ 0.705360,
###    1./ 0.717428, 
###    1./ 0.733824,
###    1./ 0.744171,
###    1./ 0.750776,
###    1./ 0.722967,
###    1./ 0.563428,
###    1./ 0.495196,
###    1./ 0.459958,

    ###################
    # Common parameters
    ###################
    outputFileName  = cms.string("shapes"),
    doMultiDimensionalShapes = cms.bool(False),
    unsplitUncertainties = cms.bool(False),

    
    
    # jes +/-, met +/-, jer +/-
    systComponents = cms.vstring(
    "_plus"    , "_minus"    ,
    "_uncplus" , "_uncminus" ,
    "_jerplus" , "_jerminus" ,
    "_bplus"   , "_bminus"   ,
    "_unbplus" , "_unbminus",
    "_topptuncplus", "_topptuncminus",
    "_tesplus", "_tesminus"
    ),

    systFancyComponents = cms.vstring(
    "_jesUp"    , "_jesDown"    ,
    "_umetUp" , "_umetDown" ,
    "_jerUp" , "_jerDown" ,
    "_btagUp"   , "_btagDown"   ,
    "_unbtagUp" , "_unbtagDown",
    "_topptuncUp", "_topptuncDown",
    "_tesUp", "_tesDown"
    ),
    
    
    #osCutEff               = cms.double(0.69*220.92/242.915), #    osCutEff               = cms.double(0.577749097), #223.82/387.4),
    #    osCutEff               = cms.double(2054.99/4232.500009), #FIXME: subtract residual shape 0.699), # // FIXME: fixme 220.92/242.915), #    osCutEff               = cms.double(0.577749097), #223.82/387.4),

    #osCutEff = cms.double(1386*1386/202.1/2045.11),
    osCutEff = cms.double(1386*1386/202.1/2045.11),


    #2810.89/5736.58),



    ############################
    ####### VARIABLE(s) ########
    ############################

    ###   toNorm      = cms.bool(True),
    ###   vars        = cms.vstring( "rc_t"    ),
    ###   fancyName  = cms.vstring("R = p_{T}^{lead.track}/E^{#tau}"),
    ###   mins        = cms.vdouble(   -0.099  ),
    ###   maxs        = cms.vdouble(    1.001  ),
    ###   bins        = cms.vint32(      11    ),
    ###   binNames    = cms.vstring( ), 
    ###   hmin        = cms.vdouble(      0    ),
    ###   hmax        = cms.vdouble(    200    ),
    ###   unbinned    = cms.vint32(       0    ),
    ###   smoothOrder = cms.vint32(       3    ),   
    ###   doLogy      = cms.vint32(     0                ),    

    #    vars        = cms.vstring( "Dphi_tau_met"   ),
    #    mins        = cms.vdouble(      0           ),
    #    maxs        = cms.vdouble(    4.8           ),
    #    bins        = cms.vint32(     5             ),
    #    hmin        = cms.vdouble(      0          ),
    #    hmax        = cms.vdouble(    200          ),
    #    unbinned    = cms.vint32(       0          ),
    #    smoothOrder = cms.vint32(       3          ),   

     toNorm      = cms.bool(False),
     vars        = cms.vstring("btagmultiplicity_j" ),
     fancyName   = cms.vstring("N_{b-tagged jets}"  ),
     mins        = cms.vdouble(     1               ),
     maxs        = cms.vdouble(   5                 ),
     bins        = cms.vint32(     4                ),
     binNames    = cms.vstring("0", "1","2","3","#geq4"),
     hmin        = cms.vdouble(     0               ),
     hmax        = cms.vdouble(   600               ),
     unbinned    = cms.vint32(      0               ),
     smoothOrder = cms.vint32(      0               ),                
     doLogy      = cms.vint32(     1                ),

    
    # vars        = cms.vstring(    "pt_t"  ),
    # mins        = cms.vdouble(         0  ),
    # maxs        = cms.vdouble(       500  ),
    # bins        = cms.vint32(         10  ),
    # hmin        = cms.vdouble(         0  ),
    # hmax        = cms.vdouble(       200  ),
    # unbinned    = cms.vint32(          0  ),
    # smoothOrder = cms.vint32(          3  ),
    
    

    #    vars        = cms.vstring( "rc_t"  ,"Dphi_tau_met"   ),
    #    mins        = cms.vdouble(   -0.099,     0           ),
    #    maxs        = cms.vdouble(    1.001,   4.8           ),
    #    bins        = cms.vint32(      6  ,    5             ),
    #    hmin        = cms.vdouble(      0  ,     0          ),
    #    hmax        = cms.vdouble(    200  ,   200          ),
    #    unbinned    = cms.vint32(       0  ,     0          ),
    #    smoothOrder = cms.vint32(       3  ,     3          ),   
    
    
    #    vars        = cms.vstring( "rc_t"    , "pt_t"  ),
    #    mins        = cms.vdouble(   -0.099  ,      0   ),
    #    maxs        = cms.vdouble(    1.001  ,    200  ),
    #    bins        = cms.vint32(      6    ,     5  ),
    #    hmin        = cms.vdouble(      0    ,      0  ),
    #    hmax        = cms.vdouble(    200    ,    100  ),
    #    unbinned    = cms.vint32(       0    ,      0  ),
    #    smoothOrder = cms.vint32(       3    ,      5  ),   
    
    
    # for multiD    vars        = cms.vstring("rc_t"  , "Dphi_tau_met"   ),
    # for multiD    mins        = cms.vdouble(  -0.099,      0           ),
    # for multiD    maxs        = cms.vdouble(   1.001,    4.8           ),
    # for multiD    bins        = cms.vint32(     11  ,     10           ),
    # for multiD    hmin        = cms.vdouble(     0  ,      0           ),
    # for multiD    hmax        = cms.vdouble(   200  ,    200           ),
    # for multiD    unbinned    = cms.vint32(      0  ,      0           ),
    # for multiD    smoothOrder = cms.vint32(      3  ,      3           ),   
    
    #    "pt_t", ,      "pt_t" 
    #         0, ,          -20
    #       200, ,         200 
    #        20, ,          11 
    #         0, ,           0 
    #       100, ,         100 
    #         0, ,           0 
    #         5, ,           5 
    
    
    #    vars        = cms.vstring("rc_t",   "pt_l", "pt_met", "multiplicity_j", "btagmultiplicity_j",    ),
    #    mins        = cms.vdouble(     0,        0,      0,        2,                0,                  ),
    #    maxs        = cms.vdouble(   1.152,    200,    300,      7,                4,                    ),
    #    bins        = cms.vint32(     12,       20,     30,       5,                4,                   ),
    #    hmin        = cms.vdouble(     0,        0,      0,        0,                0,                  ),
    #    hmax        = cms.vdouble(   200,      100,    100,      400,              600,                  ),
    #    unbinned    = cms.vint32(      0,        0,      0,        0,                0,                  ),
    #    smoothOrder = cms.vint32(      3,        5,      5,        0,                0,                  )
    
    #    vars        = cms.vstring( "pt_l", "pt_met", "multiplicity_j", "btagmultiplicity_j","rc_t",   "Dphi_tau_met"    ),
    #    mins        = cms.vdouble(      0,      0,        2,                0,                   0,        0            ),
    #    maxs        = cms.vdouble(    200,    300,      7,                4,                   1.152,    4.8            ),
    #    bins        = cms.vint32(      20,     30,       5,                4,                   12,       10            ),
    #    hmin        = cms.vdouble(      0,      0,        0,                0,                   0,        0            ),
    #    hmax        = cms.vdouble(    100,    100,      400,              600,                 200,      200            ),
    #    unbinned    = cms.vint32(       0,      0,        0,                0,                   0,        0            ),
    #    smoothOrder = cms.vint32(       5,      5,        0,                0,                   3,        3            )
    
    )

TauDileptonPDFBuilderFitterParSet = cms.PSet(
    outFolder        = cms.string("likelihoodFit/"),
    resultsFileName  = cms.string("likelihoodResults"),
    
    baseMCDir        = cms.string("/lustre/ncg.ingrid.pt/cmslocal/samples/CMSSW_5_3_7_patch4/chiggs/nomt-2012-V1-mc-MU-20GeV/"),
    baseDataDir      = cms.string("/lustre/ncg.ingrid.pt/cmslocal/samples/CMSSW_5_3_7_patch4/chiggs/nomt-2012-V1-data-MU-20GeV/"),

    signalFileNameWH   = cms.string("out-wh-pythia-m120.root"),
    signalFileNameHH   = cms.string("out-hh-pythia-m120.root"),
    
    dataFileName       = cms.string("out-data.root"),
    ddBkgFileName      = cms.string("out-data_rescaled.root"),
    ttbarmcBkgFileName = cms.string("out-ttbar_mutau.root"),
    mcBkgFileName      = cms.string("out-mcbkg.root"),
    
    minitreeSelected   = cms.string("m_tau_Selected"),
    minitreeDataDriven = cms.string("m_tau_DataDriven"),

    #fitType     = cms.vstring( HIGGS2BKG = 2, HIGGS3BKG = 3, SM2BKG = 0, SM3BKG = 1)
    fitType     = cms.vint32( 1),
    #, 2, 3, 0),
    useOS       = cms.bool(False),

    #    osCutEff               = cms.double(0.69 * 220.92/242.915),
    osCutEff               = cms.double(0.69 * 220.92/242.915),
    cHiggsBR               = cms.double(0.01),
    
    vars        = cms.vstring( "pt_l",  "multiplicity_j", "btagmultiplicity_j",  "Dphi_tau_met"    ),
    mins        = cms.vdouble(      0,       2,                0,                     0            ),
    maxs        = cms.vdouble(    200,     7,                4,                     4.8            ),
    bins        = cms.vint32(      20,      5,                4,                     10            ),
    hmin        = cms.vdouble(      0,       0,                0,                     0            ),
    hmax        = cms.vdouble(    100,     400,              600,                   200            ),
    unbinned    = cms.vint32(       0,       0,                0,                     0            ),
    smoothOrder = cms.vint32(       5,       0,                0,                     2            )

#"pt_met",
#     0,  
#   300,  
#    30,  
#     0,  
#   100,  
#     0,  
#     5,  

#"rc_t",
#     0,
#   1.2,
#    12,
#     0,
#   200,
#     0,
#     3,

    )


