#!/bin/csh

@ nproc = $1
@ nexodus = $2
@ nevents = 500
# @ nevents = 10

#set the seed
@ SEED = $nproc + 100 * $nexodus

# setup environment
set baseOutDir = $_CONDOR_SCRATCH_DIR"/"

source /opt/phenix/bin/phenix_setup.csh 

# Make sure local libraries are available
setenv HOME /phenix/u/dcm07e
source /etc/csh.login
foreach i (/etc/profile.d/*.csh)
  source $i
end

source $HOME/.login


################
# Run AMPT dAu #
################
echo ----RUNNING AMPT----

#directory we want the oscar file in
set exodusDir = $baseOutDir

#copy the ampt directory
cp -r "/phenix/hhj3/dcm07e/dAuEnergyScan/ampt_v2.26t5_lhc" $baseOutDir
cd $baseOutDir"ampt_v2.26t5_lhc/"
rm "ana/*"
pwd

#make the input.ampt file
rm input.ampt

echo "5020"				>> input.ampt		# EFRM (sqrt(S_NN) in GeV if FRAME is CMS)
echo "CMS"				>> input.ampt		# FRAME
echo "A"				>> input.ampt		# PROJ
echo "A"				>> input.ampt		# TARG
echo "208"				>> input.ampt		# IAP (projectile A number) 
echo "82"				>> input.ampt		# IZP (projectile Z number) 
echo "208"				>> input.ampt		# IAT (target A number)     Au: 197 Pb: 208
echo "82"				>> input.ampt		# IZT (target Z number)     Au: 79  Pb: 82
echo $nevents			>> input.ampt		# NEVNT (total number of events)
echo "0.0"				>> input.ampt		# BMIN (mininum impact parameter in fm) 
echo "20.0"				>> input.ampt		# BMAX (maximum impact parameter in fm, also see below)
echo "4"				>> input.ampt		# ISOFT (D=1): select Default AMPT or String Melting(see below)
echo "150"				>> input.ampt		# NTMAX: number of timesteps (D=150), (D=3 off cascade)        -------
echo "0.2"				>> input.ampt		# DT: timestep in fm (hadron cascade time= DT*NTMAX) (D=0.2)
echo "0.5"				>> input.ampt		# PARJ(41): parameter a in Lund symmetric splitting function   d+Au: 2.2  p+Pb: 0.5
echo "0.9"				>> input.ampt		# PARJ(42): parameter b in Lund symmetric splitting function   d+Au: 0.5  p+Pb: 0.9
echo "1"				>> input.ampt		# (D=1,yes;0,no) flag for popcorn mechanism(netbaryon stopping)
echo "1.0"				>> input.ampt		# PARJ(5) to control BMBbar vs BBbar in popcorn (D=1.0)
echo "1"				>> input.ampt		# shadowing flag (Default=1,yes; 0,no)
echo "0"				>> input.ampt		# quenching flag (D=0,no; 1,yes)
echo "1.0"				>> input.ampt		# quenching parameter -dE/dx (GeV/fm) in case quenching flag=1
echo "2.0"				>> input.ampt		# p0 cutoff in HIJING for minijet productions (D=2.0)
echo "4.5628d0"			>> input.ampt		# parton screening mass fm^(-1) (D=2.265d0) 0mb:5588.2820d0  0.75mb:6.4528d0  1.5mb:4.5628d0  3mb:3.2264d0   p+Pb: 3.2d0   -------
echo "0"				>> input.ampt		# IZPC: (D=0 forward-angle parton scatterings; 100,isotropic)
echo "0.33d0"	    	>> input.ampt		# alpha in parton cascade (D=0.33d0), see parton screening mass  d+Au: 0.47d0  p+Pb: 0.33d0
echo "1d6"				>> input.ampt		# dpcoal in GeV
echo "1d6"				>> input.ampt		# drcoal in fm
echo "11"				>> input.ampt		# ihjsed: take HIJING seed from below (D=0)or at runtime(11)
echo $SEED				>> input.ampt		# random seed for HIJING
echo $SEED				>> input.ampt		# random seed for parton cascade
echo "0"				>> input.ampt		# flag for K0s weak decays (D=0,no; 1,yes)
echo "1"				>> input.ampt		# flag for phi decays at end of hadron cascade (D=1,yes; 0,no)
echo "0"				>> input.ampt		# flag for pi0 decays at end of hadron cascade (D=0,no; 1,yes)
echo "3"				>> input.ampt		# optional OSCAR output (D=0,no; 1,yes; 2&3,more parton info)
echo "0"				>> input.ampt		# flag for perturbative deuteron calculation (D=0,no; 1or2,yes)
echo "1"				>> input.ampt		# integer factor for perturbative deuterons(>=1 & <=10000)
echo "1"				>> input.ampt		# choice of cross section assumptions for deuteron reactions
echo "-7."				>> input.ampt		# Pt in GeV: generate events with >=1 minijet above this value
echo "1000"				>> input.ampt		# maxmiss (D=1000): maximum # of tries to repeat a HIJING event
echo "3"				>> input.ampt		# flag to turn off initial and final state radiation (D=3)
echo "1"				>> input.ampt		# flag to turn off Kt kick (D=1)
echo "0"				>> input.ampt		# flag to turn on quark pair embedding (D=0,no; 1,yes)
echo "7., 0."			>> input.ampt		# Initial Px and Py values (GeV) of the embedded quark (u or d)
echo "0., 0."			>> input.ampt		# Initial x & y values (fm) of the embedded back-to-back q/qbar
echo "1, 5., 0."		>> input.ampt		# nsembd(D=0), psembd (in GeV),tmaxembd (in radian).
echo "0"				>> input.ampt		# Flag to enable users to modify shadowing (D=0,no; 1,yes)
echo "1.d0"				>> input.ampt		# Factor used to modify nuclear shadowing
echo "1"				>> input.ampt		# Flag for random orientation of reaction plane (D=0,no; 1,yes)

./exec $SEED

################
# Run ANALYSIS #
################
echo ----RUNNING ANALYSIS----

ln -sf /phenix/hhj3/dcm07e/dAuEnergyScan/cumulants/cumulant.C .
ln -sf /phenix/hhj3/dcm07e/dAuEnergyScan/cumulants/parton_pplane.C .
ln -sf /phenix/hhj3/dcm07e/dAuEnergyScan/cumulants/range.h .
ln -sf /phenix/hhj3/dcm07e/dAuEnergyScan/cumulants/fvtx_acc.root .
ln -sf /phenix/hhj3/dcm07e/dAuEnergyScan/cumulants/fvtx_acc_n.root .
root -b -q cumulant.C++
root -b -q parton_pplane.C++


########################
# Now do some cleanup  #
########################

# now move all the desired output files to there final directories
set finalOutDir = "/phenix/plhf/dcm07e/dAuEnergyScan/sims/cumulants/PbPb5020"

mv six.root $finalOutDir"/cumulants_ampt_pbpb5020_"$nexodus"_"$nproc".root"
mv ppplane.root $finalOutDir"/pplane_ampt_pbpb5020_"$nexodus"_"$nproc".root"


echo "---- DONE EXECUTING JOB "$nexodus" "$nproc" ----"
































