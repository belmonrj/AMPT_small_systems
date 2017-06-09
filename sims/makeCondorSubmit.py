#!/opt/phenix/bin/python
'''
Makes condor submit file for hadd'ing ampt jobs
'''

import os
import sys
from argparse import ArgumentParser

#First get any command line arguments
parser = ArgumentParser(description = """Make condor submit script
                                         for hadd'ing ampt jobs""")

parser.add_argument("-f", "--file", dest="filename",
                  help="write output to FILE", metavar="FILE")
parser.add_argument("-t", "--tag", dest="tag", help="tag")
parser.add_argument("-s", "--set", dest="set", help="set")
parser.add_argument("-n", "--nfiles", dest="nfiles", help="number of files")
parser.add_argument("-i", "--input", dest="indir",
                    help="Get input from DIR", metavar="DIR")
parser.add_argument("-o", "--output", dest="outdir",
                    help="Write output to DIR", metavar="DIR")
parser.add_argument("-v", "--verbose",
                  action="store_true", dest="verbose", default=True,
                  help="print status messages to stdout")

parser.set_defaults(filename="condor.job",
                    tag='dau200',
                    set='0',
                    nfiles='10000',
                    indir="/phenix/plhf/dcm07e/dAuEnergyScan/sims/cumulants",
                    outdir="/phenix/plhf/dcm07e/dAuEnergyScan/sims/cumulants/agg/")

args = parser.parse_args()


#Print out arguments
if args.verbose :
    print(" Writting job file to {}".format(args.filename))
    print(" Tag {}".format(args.tag))
    print(" Set {}".format(args.set))
    print(" Hadd'ing {} files".format(args.nfiles))
    print(" Input from {}".format(args.indir))
    print(" Output of condor jobs will go to {}".format(args.outdir))


#make the file
fo = open(args.filename,'w')

#write the job information
fo.write("Universe        = vanilla\n")
fo.write("Executable      = /phenix/plhf/dcm07e/dAuEnergyScan/sims/cumulants/agg/hadd_sets.py\n")
fo.write("Requirements    = (CPU_Speed >= 1 && CPU_Experiment == \"phenix\")\n")
fo.write("Rank            = CPU_Speed\n")
fo.write("Priority        = +1\n")
fo.write("GetEnv          = True\n")
fo.write("Initialdir      = {}\n".format(os.getcwd()))
fo.write("+Experiment     = \"phenix\"\n")
fo.write("PeriodicHold = (NumJobStarts >= 1 && JobStatus == 1)\n")
fo.write("+Job_Type       = \"cas\"\n")

fo.write("\n")
fo.write("Arguments       = {} {} cumulants {} {} {}\n".format(args.tag,args.set,args.nfiles,args.indir,args.outdir))
fo.write("Output          = {}condor_logs/ran_hadd_cumulants_{}_{}-$(Process).out\n".format(args.outdir,args.tag,args.set))
fo.write("Error           = {}condor_logs/ran_hadd_cumulants_{}_{}-$(Process).err\n".format(args.outdir,args.tag,args.set))
fo.write("Log             = {}condor_logs/ran_hadd_cumulants_{}_{}-$(Process).log\n".format(args.outdir,args.tag,args.set))
fo.write("Queue 1\n")

fo.write("\n")
fo.write("Arguments       = {} {} pplane {} {} {}\n".format(args.tag,args.set,args.nfiles,args.indir,args.outdir))
fo.write("Output          = {}condor_logs/ran_hadd_pplane_{}_{}-$(Process).out\n".format(args.outdir,args.tag,args.set))
fo.write("Error           = {}condor_logs/ran_hadd_pplane_{}_{}-$(Process).err\n".format(args.outdir,args.tag,args.set))
fo.write("Log             = {}condor_logs/ran_hadd_pplane_{}_{}-$(Process).log\n".format(args.outdir,args.tag,args.set))
fo.write("Queue 1\n")

fo.write("\n")
fo.write("Arguments       = {} {} eplane {} {} {}\n".format(args.tag,args.set,args.nfiles,args.indir,args.outdir))
fo.write("Output          = {}condor_logs/ran_hadd_eplane_{}_{}-$(Process).out\n".format(args.outdir,args.tag,args.set))
fo.write("Error           = {}condor_logs/ran_hadd_eplane_{}_{}-$(Process).err\n".format(args.outdir,args.tag,args.set))
fo.write("Log             = {}condor_logs/ran_hadd_eplane_{}_{}-$(Process).log\n".format(args.outdir,args.tag,args.set))
fo.write("Queue 1\n")
