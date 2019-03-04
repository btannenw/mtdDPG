rm fileLists/*_v1.txt -v

ls /eos/uscms/store/user/benjtann/RelValNuGun/runHitsRelValNuGunv10_v10/190206_162648/0000/DumpHits_*.root > fileLists/nuGun200PU_v1.txt
ls /eos/uscms/store/user/benjtann/RelValSinglePiFlatPt_0p7to10_pythia8_cfi/runHitsRelValSinglePiFlatPt0p7to10pythia8cfiv10_v10/190206_162656/0000/DumpHits_*.root > fileLists/singlePion_v1.txt
ls /eos/uscms/store/user/benjtann/RelValSingleMuFlatPt_0p7to10_pythia8/runHitsRelValSingleMuFlatPt0p7to10pythia8v10_v10/190206_162703/0000/DumpHits_*.root > fileLists/singleMuon_v1.txt
ls /eos/uscms/store/user/benjtann/MinBias_TuneCP5_14TeV-pythia8/runHitsMinBiasTuneCP514TeV-pythia8-v10r2_v10_r2/190301_162542/0000/DumpHits_*.root > fileLists/minBias0PU_v1.txt

sed -i 's/\/eos\/uscms/root:\/\/cmseos.fnal.gov\//g' fileLists/nuGun200PU_v1.txt
sed -i 's/\/eos\/uscms/root:\/\/cmseos.fnal.gov\//g' fileLists/minBias0PU_v1.txt
sed -i 's/\/eos\/uscms/root:\/\/cmseos.fnal.gov\//g' fileLists/singleMuon_v1.txt
sed -i 's/\/eos\/uscms/root:\/\/cmseos.fnal.gov\//g' fileLists/singlePion_v1.txt
