import ROOT as R
import subprocess
import os

fileList = open('fileLists/nuGun200PU_v1.txt','r')
files50 = 0

for filename in fileList:
    f=R.TFile.Open(filename.strip('\n'))
    dh=f.Get("DumpHits")
    if dh.GetEntries()!=50:
        print dh.GetEntries(), filename.strip('\n')
    else:
        files50 += 1

print "Files with == 50 events: ", files50
