# /usr/bin/python

#Author: Ben Tannenwald
#Date: January 30, 2019
#Purpose: Script to submit condor jobs for MTD DPG work

import os,sys, argparse

# *** 0. setup parser for command line
parser = argparse.ArgumentParser()
parser.add_argument("--outputDir", help="output directory for processed histograms and roofiles")
parser.add_argument("--inputTXTfile", help=".txt file containing list of input files for a given sample")
parser.add_argument("--layout", help="Layout of BTL crystals. probably always want barzflat")
parser.add_argument("--events", help="Total number of events per job", default='-1')
parser.add_argument("--firstEvent", help="First event to analyze", default='0')
args = parser.parse_args()

if (len(vars(args)) != 5): # 4 --> four: one for each options
    os.system('python submitSampleToCondor.py -h')
    quit()

# ** A. Test output directory existence and create if DNE
if(args.outputDir is None):
    print "#### Need to specify output directory using --outputDir <desired output directory> ####\nEXITING"
    quit()
else:
    if ( not os.path.exists(args.outputDir) ):
        print "Specified input file ({0}) DNE.\nCREATING NOW".format(args.inputTXTfile)
        os.system("mkdir {0}".format(args.outputDir))

    if ( not os.path.exists( (args.outputDir + '/condor_logs/') ) ):
        os.system("mkdir {0}".format( (args.outputDir + '/condor_logs/')) )
    if ( not os.path.exists( (args.outputDir + '/condor_err/') ) ):
        os.system("mkdir {0}".format( (args.outputDir + '/condor_err/')) )
    if ( not os.path.exists( (args.outputDir + '/condor_out/') ) ):
        os.system("mkdir {0}".format( (args.outputDir + '/condor_out/')) )

    print '-- Setting outputDir = {0}'.format(args.outputDir)

# ** B. Test input .txt file and exit if DNE
if(args.inputTXTfile is None):
    print "#### Need to specify input .txt file using --inputTXTfile <address to .txt file> ####\nEXITING"
    quit()
else:
    if ( not os.path.exists(args.inputTXTfile) ):
        print "#### Specified input file ({0}) DNE ####.\nEXITING".format(args.inputTXTfile)
        quit()
    else:
        print '-- Setting inputTXTfile = {0}'.format(args.inputTXTfile)

# ** C. Test layout flag and exit if not sensible
if(args.layout is None):
    print "#### Need to specify BTL layout using --layout <barzflat/tile/other> ####\nEXITING"
    quit()
else:
    if ( args.layout != "barzflat" ):
        print "#### Specified BTL layout ({0}) DNE ####.\nEXITING".format(args.layout)
        quit()
    else:
        print '-- Setting BTL layout = {0}'.format(args.layout)


# ** D. Test events flag and use standard value if not sensible or missing
if(args.events is None):
    print "#### No number of events to run over specified. Using -1 as default ####\n"
else:
    if ( args.events != "-1" ):
        print "#### Specified number of events to run over: {0} ####.\n".format(args.events)
    else:
        print '-- Setting events = {0}'.format(args.events)


# ** E. Test firstEvent flag and use standard value if not sensible or missing
if(args.firstEvent is None):
    print "#### No value passed for firstEvent to analyze. Using 0 as default ####\n"
else:
    if ( args.firstEvent != "0" ):
        print "#### Specified first event number to start with: {0} ####.\n".format(args.firstEvent)
    else:
        print '-- Setting firstEvent = {0}'.format(args.firstEvent)


# ** F. Exit if no grid proxy
if ( not os.path.exists(os.path.expandvars("$X509_USER_PROXY")) ):
    print "#### No GRID PROXY detected. Please do voms-proxy-init -voms cms before submitting Condor jobs ####.\nEXITING"
    quit()


# *** 1. Create .tar of directory and store in personal EOS
print "##########     Tarring workdir     ##########"
tarball_name = "{0}.tar.gz".format(args.outputDir)
os.system("tar -cvzf {0} ./ --exclude 'dpg*' --exclude '.git' --exclude 'test*' --exclude 'submitOneFile_' --exclude '*.tar.gz' --exclude 'minBias*' --exclude 'single*' --exclude '*-19' --exclude '*2019'  --exclude 'quickF*' --exclude '*.*~' --exclude 'nuGun*' --exclude 'oldFilelists' --exclude 'fileLists*'".format(tarball_name))
if ( not os.path.exists("/eos/uscms/store/user/benjtann/{0}/".format(args.outputDir)) ):
    os.system("mkdir /eos/uscms/store/user/benjtann/{0}/".format(args.outputDir))
os.system("xrdcp {0} root://cmseos.fnal.gov//store/user/benjtann/{1}/".format(tarball_name, args.outputDir))


# *** 2. Create temporary .pdl file for condor submission
print "\n##########     Submitting Condor jobs     ##########\n"
txtfile = open(args.inputTXTfile, 'r')

for line in txtfile:
    number = (line.rsplit('_',1))[1].split('.root')[0] # get number of file
    if args.firstEvent != "0":
        number = number + "-" + args.firstEvent

    infile = line.split('\n')[0]
    inputDir = infile.split('DumpHits')[0]
    #print number, infile, inputDir, pattern
    jdl_filename = "submitOneFile_{0}_{1}.jdl".format(args.outputDir, number)

    os.system("touch {0}".format(jdl_filename))
    os.system("echo universe = vanilla > {0}".format(jdl_filename))
    os.system("echo Executable = runOneFile.csh >> {0}".format(jdl_filename))
    os.system("echo Should_Transfer_Files = YES >> {0}".format(jdl_filename))
    os.system("echo WhenToTransferOutput = ON_EXIT >> {0}".format(jdl_filename))
    os.system("echo Transfer_Input_Files = runOneFile.csh, {0} >> {1}".format(tarball_name, jdl_filename))
    os.system("echo Output = {0}/condor_out/outfile_{1}.out  >> {2}".format(args.outputDir, number, jdl_filename))
    os.system("echo Error = {0}/condor_err/outfile_{1}.err >> {2}".format(args.outputDir, number, jdl_filename))
    os.system("echo Log = {0}/condor_logs/outfile_{1}.log >> {2}".format(args.outputDir, number, jdl_filename))
    os.system("echo x509userproxy = ${{X509_USER_PROXY}} >> {0}".format(jdl_filename))
    #os.system("echo Arguments = {0} {1} {2} {3} {4} >> {5}".format(infile, args.outputDir, number, args.layout, tarball_name, jdl_filename))
    os.system("echo Arguments = {0} {1} {2} {3} {4} {5} {6} >> {7}".format(infile, args.outputDir, number, args.layout, args.events, args.firstEvent, tarball_name, jdl_filename))
    os.system("echo Queue 1 >> {0}".format(jdl_filename))   

    
    os.system("condor_submit {0}".format(jdl_filename))


# *** 3. Cleanup submission directory
print "\n##########     Cleanup submission directory     ##########\n"
os.system("rm *.jdl")
#os.system("mv {0} {1}/".format(tarball_name, args.outputDir) )
