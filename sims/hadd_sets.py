#!/opt/phenix/bin/python
'''
Run aggregation over ampt sets
'''

import os
import sys

###########
## Set some running variables
###########

condorDir = os.environ["_CONDOR_SCRATCH_DIR"]
baseOutDir = condorDir + '/'

if len(sys.argv) != 7:
    sysname = 'pau200'
    setnum = 10
    fname = 'cumulants'
    njobs = 10000
    indir = '..'
    outdir = '/phenix/plhf/belmonrj/AMPT_small_systems/output/cumulants/agg/'
else:
    sysname = sys.argv[1]
    setnum = int(sys.argv[2])
    fname = sys.argv[3]
    njobs = int(sys.argv[4])
    indir = sys.argv[5]
    outdir = sys.argv[6]


print('')
print('sys: {}'.format(sysname))
print('set: {}'.format(setnum))
print('fname: {}'.format(fname))
print('njobs: {}'.format(njobs))
print('indir: {}'.format(indir))
print('outdir: {}'.format(outdir))

nk = njobs / 1000

###########
## Run the hadd
###########
os.chdir(baseOutDir)

for i in range(nk):
    print('')
    os.system('hadd -f {}_ampt_{}_{}_{}.root {}/{}/{}_ampt_{}_{}_{}???.root'.format(fname,sysname,setnum,i,indir,sysname,fname,sysname,setnum,i))

print('')
os.system('hadd -f {}_ampt_{}_{}.root {}_ampt_{}_{}_?.root'.format(fname,sysname,setnum,fname,sysname,setnum,i))

os.system('mv {}_ampt_{}_{}.root {}'.format(fname,sysname,setnum,outdir))
