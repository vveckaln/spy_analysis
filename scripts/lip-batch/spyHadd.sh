#!/bin/bash

# Takes as the first argument the output directory (spyOutputArea from the cfg file)
#   and as the second argument the eChONMuChOFF parameter (true/false)


AREA=${1}
ECHAN=${2}

# Set channel depending on area
if [ "${ECHAN}" = "true" ]; then
    AREA=${AREA}"/eltau/"
else
    AREA=${AREA}"/mutau/"
fi


hadd -f ${AREA}out-dibosons.root ${AREA}out-zz.root ${AREA}out-wz.root ${AREA}out-ww.root

#hadd -f ${AREA}out-singletop.root ${AREA}out-stop_s.root ${AREA}out-stop_t.root ${AREA}out-stop-DR_tW.root ${AREA}out-stopbar_s.root ${AREA}out-stopbar-DR_tW.root ${AREA}out-stopbar_t.root
 
mv ${AREA}out-dy_from50.root ${AREA}out-zjets.root
