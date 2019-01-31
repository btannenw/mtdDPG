rm fileLists/*_v0.txt -v

ls /eos/uscms/store/user/benjtann/RelValMinBias_14TeV/10_4_0_mtd3_runHitsRelValMinBias14TeVnoPU-v1V7_r2/190123_041856/0000/DumpHits_*.root > fileLists/minBias14TeV_v0.txt
ls /eos/uscms/store/user/benjtann/RelValSinglePiFlatPt_0p7to10_pythia8_cfi/10_4_0_mtd3_runHitsRelValSinglePiFlatPt0p7to10MnoPU-v2V7_r2/190123_042207/0000/DumpHits_*.root > fileLists/singlePion_v0.txt
ls /eos/uscms/store/user/benjtann/RelValSingleMuFlatPt_0p7to10_pythia8/10_4_0_mtd3_runHitsRelValSingleMuFlatPt0p7to10noPU-v2V7_r2/190123_035425/0000/DumpHits_*.root > fileLists/singleMuon_v0.txt

sed -i 's/\/eos\/uscms/root:\/\/cmseos.fnal.gov\//g' fileLists/minBias14TeV_v0.txt
sed -i 's/\/eos\/uscms/root:\/\/cmseos.fnal.gov\//g' fileLists/singleMuon_v0.txt
sed -i 's/\/eos\/uscms/root:\/\/cmseos.fnal.gov\//g' fileLists/singlePion_v0.txt
