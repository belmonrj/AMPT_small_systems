# AMPT in Small Systems

## How to Get AMPT

We use AMPT version 2.26, with custom mofications to the calculation of the initial collision geometry. In collisions involving a 3He nucleus, its nucleon coordinates are sampled for each event from a realistic description of the nuclear wave function. This is also done for the case of deuteron, using the Hulthen wave function instead. For every other nuclear species, a Woods-Saxon distribution is used.

Our modified version of AMPT can be found on RCF: INSERT PATH(!!!!)

For reference, the latest version of AMPT can be found on the author's website:
http://myweb.ecu.edu/linz/ampt/

## How to Run AMPT
1. Modify the configuration file "input.ampt", in the AMPT source folder. This is where the simulation settings are defined, including collision species, center-of-mass collision energy, parton interaction cross section, and hadron cascade duration. The input.ampt file contains documentation explaining what every parameter corresponds to. Some settings of relevance to us are:
    + Parton cross section: The parton screening mass sets the parton cross section, according to the formula $\mu = \sqrt{\frac{31.2287}{\sigma_{parton}}}$. Setting the parton screening mass to a very large value effectively turns off partonic interactions. For example, a cross section of $\sigma=1.5$ mb corresponds to a parton screening mass of 4.5628
    + Duration of hadron scattering: Setting NTMAX = 3 turns off hadronic scattering

2. The nucleon-nucleon interaction cross section is the only parameter that cannot be adjusted in the "input.ampt" file. It is set by default to $\sigma = 42.3$ mb for collisions at $\sqrt{s_{NN}} = 200$ GeV, and should only be changed when running AMPT at other collision energies, as discussed in https://arxiv.org/abs/1512.06949. To do this, modify line 558 of hijing1.383_ampt.f

3. Once the desired settings have been specified, run AMPT from its source folder with the command "./exec $<$integer$>$", where the integer is used as a seed for the random number generator.

4. The result files can be found in the /ana subdirectory. Of the many output files, we use the following for flow analyses
    + npart-xy.dat $\rightarrow$ Initial nucleon coordinates
    + parton-initial-afterPropagation.dat $\rightarrow$ Parton coordinates after string melting
    + ampt.dat $\rightarrow$ Momenta and coordinates of final state particles

# How to Run AMPT on RCF

The best way to run AMPT on RCF is to automate the procedure described above using a script, which can then be run on Condor to generate the desired number of events. Running AMPT takes time, and no more than a few thousand events per Condor job are recommended.

The following are a sample shell script and Condor submission file to run AMPT:

+ Shell Script:      /direct/phenix+u/pengqi/work/run_ampt/run_backup.sh
+ Condor Submission: /direct/phenix+u/pengqi/work/run_ampt/submit_backup

The shell script writes out the input file "ampt.input", runs AMPT, and runs analysis code on the resulting output. The Condor file controls the number of submitted jobs.

## Running a Flow Analysis on AMPT Output

The following subsections describe the various analysis macros contained in this repository, which run on the output files from AMPT. Each macro implements a particular way of calculating azimuthal anisotropy coefficients, including calculations relative to the event plane, the nucleon participant plane, the parton participant plane, and multi-particle cumulants.

### event_plane.C

This code could be run directly on rcf.

Written in C++, applying Event Plane method with three-sub events resolution. Three-sub events come from BBCS, FVTXS, FCTXN.

Applied 6 ranges centrality cut.

Three-sub events method formula could be found in paper "Methods for analyzing anisotropic flow in relativistic nuclear collisions", equation 16. Or ask anyone in Nagle Lab.

Function "processEvent" is used to compute Q vector and calculate resolution. The first part of numerator (of resolution formula) is filled in first bin of TProfile "res_comp". The second part of numerator is filled in second bin of TProfile "res_comp". The denominator is filled in the third bin.

TProfile "v2s[i]" contains v2_pt histogram without modified by resolution. TProfile "res_comp[i]" contains three parts of resolution formula.

It also contains N_ch distribution of BBCS (and FVTXS if need), eta distribution.

#### To run this code, use the following command: root event_plane.C++





### cumulant.C

This code could be run directly on rcf.

Written in C++, applying Multi-particle Correlation (Cumulant) Method with Ron's help with 4 and 6 particle cumulant equation. This file contains reference flow of 2,4,6 particle cumulant (usually used part in my project). And diferential flow of 2,4 particle cumulant (not often used in my projects). Formula and theory could be found from following paper:

Flow analysis with cumulants: direct calculations, A. Bilandzic, R. Snellings, S. Voloshin, Phys.Rev.C83:044913,2011, (or arXiv:1010.0233v2)

Also applied an eta gap in 2 particle cumulant calculation.

#### Functions

"def_ave_2particle_with_gap" 			to calculate <2'> term in diferential flow, with eta gap.

"def_ave_2particle_correlation"		to calculate <2'> term in diferential flow, without eta gap.

"def_ave_4particle_correlation"		to calculate <4'> term in diferential flow.

"with_gap_calculation"				to calculate <2> term in reference flow, with eta gap.

"ave_2particle_correlation" 			to calculate <2> term in reference flow, without eta gap.

"ave_4particle_correlation" 			to calculate <4> term in reference flow.

"ave_6particle_correlation" 			to calculate <6> term in reference flow.

The variable name "raai" (i.e. raa2) means reference flow: <<i>> (i.e. <<2>>). "daai" means diferential flow <<i>>.

#### Output

"comp_Ncharge"			Multiplicity dependence of 2 particle cumulant method with eta gap.

"raa2_Ncharge"			Multiplicity dependence of <<2>> in reference flow.

"raa4_Ncharge"			Multiplicity dependence of <<4>> in reference flow.

"raa6_Ncharge"			Multiplicity dependence of <<6>> in reference flow.

"daa2_Ncharge"			Multiplicity dependence of <<2'>> in differential flow.

"daa4_Ncharge"			Multiplicity dependence of <<4'>> in differential flow.

"dnch" 					N_ch distribution

#### To run this code, use the following command: root cumulant.C++





### parton_pplane.C

This code could be run directly on rcf.

Written in C++, applying participant plane calculation with partons.

Applied centrality cut.

#### Functions

"parse_afterPropagation_file"
Parse initial state partons "ana/parton-initial-afterPropagation.dat"

"parse_ampt_file"
Parse final state nucleons "ana/ampt.dat"

"processEvent_melting"
Using partons info to calculate participant plane Psi_PP.

"processEvent_ampt"
Calculate v_2.

#### Output

"v2s"
Multiplicity dependence of v2.

"v2s_pt[i]"
p_T dependence of v2 in different centrality range.

"epsilon2_nch"
Multiplicity dependence of epsilon_2.

"dhis_eta"
eta distribution.

"dhis_b"
impact parameter distribution.

#### To run this code, use the following command: root parton_pplane.C++

