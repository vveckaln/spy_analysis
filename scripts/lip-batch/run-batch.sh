#!/bin/bash

archbin=x86_64
SCRAM_ARCH=slc5_amd64_gcc462
# 2011: SCRAM_ARCH=slc5_amd64_gcc434
GLOBUS_LOCATION=/opt/globus
EDG_WL_LOCATION=/opt/edg
VO_CMS_SW_DIR=/cvmfs/cms.cern.ch


SCRIPTS=$LIPCMS_BASE/scripts/lip-batch

source $VO_CMS_SW_DIR/cmsset_default.sh

# cmsenv non funzia, sacripante, even if it is already defined as an alias in cmsset_default.sh. Perhaps aliases are not activated in scripts?

cd $LIPCMS_BASE;

eval `scramv1 runtime -sh`;



