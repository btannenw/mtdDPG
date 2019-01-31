#ETL reconstruction and simulation

### Running plot-making python script
To run plot-making code over a specified input file (can be hosted over EOS)
```
python drawETLPerformance.py --input=/eos/uscms/store/user/benjtann/RelValSingleMuFlatPt_0p7to10_pythia8/10_4_0_mtd3_runHitsRelValSingleMuFlatPt0p7to10noPU-v2V7_r2/190123_035425/0000/DumpHits_1.root --output=dpg_01-30-19/minBias14TeV_noPU_v4.root --layout=barzflat
```


### Submitting to condor pool on LPC
To submit to condor "by hand"
```
python submitSampleToCondor.py --outputDir test_singleMuon_01-31-19_v7 --inputTXTfile fileLists/singleMuon_v0_oneFile.txt --layout barzflat
```

To submit all samples to condor via script
```
source passSub.sh rN_MM-DD-YY 
```
where N is the revision number, MM is the month, DD is the day, and YY is the year. 