#!/opt/phenix/bin/python
'''
Run AMPT
'''

import os
import sys
import stat
import shutil

##############################################
# Get the command line arguments
##############################################
nproc = int(sys.argv[1])
nexodus = int(sys.argv[2])

##############################################
# Set some parameters for running
##############################################
nevents = 5000

condorDir = os.environ["_CONDOR_SCRATCH_DIR"]
baseOutDir = condorDir + '/'
finalOutDir = "/phenix/plhf/belmonrj/AMPT_small_systems/output/dau39"

SEED = nproc + 10000 * nexodus


################
# Run AMPT dAu #
################
print('----RUNNING AMPT----')
print('nexodus: {}'.format(nexodus))
print('nproc: {}'.format(nproc))
print('nevents: {}'.format(nevents))
print('finalOutDir: {}'.format(finalOutDir))
print('\n')


#directory we want the oscar file in
exodusDir = baseOutDir

#copy the ampt directory
os.system("cp -r /phenix/hhj3/dcm07e/dAuEnergyScan/ampt_v2.26t5_dau39 {}".format(baseOutDir))
os.chdir(baseOutDir + "ampt_v2.26t5_dau39/")
os.getcwd()

for file in os.listdir('ana/'):
    os.remove('ana/' + file)
# shutil.rmtree("ana")

#make the input.ampt file
os.remove('input.ampt')

target = open('input.ampt', 'w')

target.write("39\n")					# EFRM (sqrt(S_NN) in GeV if FRAME is CMS)
target.write("CMS\n")					# FRAME
target.write("A\n")						# PROJ
target.write("A\n")						# TARG
target.write("2\n")						# IAP (projectile A number) 
target.write("1\n")						# IZP (projectile Z number) 
target.write("197\n")					# IAT (target A number)     Au: 197 Pb: 208
target.write("79\n")					# IZT (target Z number)     Au: 79  Pb: 82
target.write("{}\n".format(nevents))	# NEVNT (total number of events)
target.write("0.0\n")					# BMIN (mininum impact parameter in fm) 
target.write("20.0\n")					# BMAX (maximum impact parameter in fm, also see below)
target.write("4\n")						# ISOFT (D=1): select Default AMPT or String Melting(see below)
target.write("150\n")					# NTMAX: number of timesteps (D=150), (D=3 off cascade)        -------
target.write("0.2\n")					# DT: timestep in fm (hadron cascade time= DT*NTMAX) (D=0.2)
target.write("2.2\n")					# PARJ(41): parameter a in Lund symmetric splitting function   d+Au: 2.2  p+Pb: 0.5
target.write("0.5\n")					# PARJ(42): parameter b in Lund symmetric splitting function   d+Au: 0.5  p+Pb: 0.9
target.write("1\n")						# (D=1,yes;0,no) flag for popcorn mechanism(netbaryon stopping)
target.write("1.0\n")					# PARJ(5) to control BMBbar vs BBbar in popcorn (D=1.0)
target.write("1\n")						# shadowing flag (Default=1,yes; 0,no)
target.write("0\n")						# quenching flag (D=0,no; 1,yes)
target.write("1.0\n")					# quenching parameter -dE/dx (GeV/fm) in case quenching flag=1
target.write("2.0\n")					# p0 cutoff in HIJING for minijet productions (D=2.0)
target.write("6.4528d0\n")				# parton screening mass fm^(-1) (D=2.265d0) 0mb:5588.2820d0  0.75mb:6.4528d0  1.5mb:4.5628d0  3mb:3.2264d0   p+Pb: 3.2d0   -------
target.write("0\n")						# IZPC: (D=0 forward-angle parton scatterings; 100,isotropic)
target.write("0.47140452d0\n")			# alpha in parton cascade (D=0.33d0), see parton screening mass  d+Au: 0.47d0  p+Pb: 0.33d0
target.write("1d6\n")					# dpcoal in GeV
target.write("1d6\n")					# drcoal in fm
target.write("11\n")					# ihjsed: take HIJING seed from below (D=0)or at runtime(11)
target.write("{}\n".format(SEED))		# random seed for HIJING
target.write("{}\n".format(SEED))		# random seed for parton cascade
target.write("0\n")						# flag for K0s weak decays (D=0,no; 1,yes)
target.write("1\n")						# flag for phi decays at end of hadron cascade (D=1,yes; 0,no)
target.write("0\n")						# flag for pi0 decays at end of hadron cascade (D=0,no; 1,yes)
target.write("3\n")						# optional OSCAR output (D=0,no; 1,yes; 2&3,more parton info)
target.write("0\n")						# flag for perturbative deuteron calculation (D=0,no; 1or2,yes)
target.write("1\n")						# integer factor for perturbative deuterons(>=1 & <=10000)
target.write("1\n")						# choice of cross section assumptions for deuteron reactions
target.write("-7.\n")					# Pt in GeV: generate events with >=1 minijet above this value
target.write("1000\n")					# maxmiss (D=1000): maximum # of tries to repeat a HIJING event
target.write("3\n")						# flag to turn off initial and final state radiation (D=3)
target.write("1\n")						# flag to turn off Kt kick (D=1)
target.write("0\n")						# flag to turn on quark pair embedding (D=0,no; 1,yes)
target.write("7., 0.\n")				# Initial Px and Py values (GeV) of the embedded quark (u or d)
target.write("0., 0.\n")				# Initial x & y values (fm) of the embedded back-to-back q/qbar
target.write("1, 5., 0.\n")				# nsembd(D=0), psembd (in GeV),tmaxembd (in radian).
target.write("0\n")						# Flag to enable users to modify shadowing (D=0,no; 1,yes)
target.write("1.d0\n")					# Factor used to modify nuclear shadowing
target.write("1\n")						# Flag for random orientation of reaction plane (D=0,no; 1,yes)

target.close()

print("\n-- input.ampt --\n")
with open('input.ampt', 'r') as fin:
    print fin.read()
print("\n")

os.system('./exec {}'.format(SEED))

################
# Run ANALYSIS #
################
print("----RUNNING ANALYSIS----")

os.system("ln -sf /phenix/plhf/belmonrj/AMPT_small_systems/cumulant.C .")
os.system("ln -sf /phenix/plhf/belmonrj/AMPT_small_systems/parton_pplane.C .")
os.system("ln -sf /phenix/plhf/belmonrj/AMPT_small_systems/event_plane.C .")
os.system("ln -sf /phenix/plhf/belmonrj/AMPT_small_systems/range.h .")
os.system("ln -sf /phenix/plhf/belmonrj/AMPT_small_systems/consts.h .")
os.system("ln -sf /phenix/plhf/belmonrj/AMPT_small_systems/fvtx_eff_pteta.root .")
os.system("root -b -q cumulant.C++")
os.system("root -b -q parton_pplane.C++\(39\)")
os.system("root -b -q event_plane.C++\(39\)")


########################
# Now do some cleanup  #
########################

# now move all the desired output files to there final directories
shutil.copy2("six.root", "{}/cumulants_ampt_dau39_{}_{:04d}.root".format(finalOutDir, nexodus, nproc))
shutil.copy2("ppplane.root", "{}/pplane_ampt_dau39_{}_{:04d}.root".format(finalOutDir, nexodus, nproc))
shutil.copy2("out_EP.root", "{}/eplane_ampt_dau39_{}_{:04d}.root".format(finalOutDir, nexodus, nproc))


print("---- DONE EXECUTING JOB {} {} ----".format(nexodus, nproc))
































