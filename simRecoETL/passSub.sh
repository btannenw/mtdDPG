python submitSampleToCondor.py --outputDir nuGun200PU_v1_$1 --inputTXTfile fileLists/nuGun200PU_v1.txt --layout barzflat
sleep 10
python submitSampleToCondor.py --outputDir singleMuon_v1_$1 --inputTXTfile fileLists/singleMuon_v1.txt --layout barzflat
sleep 10
python submitSampleToCondor.py --outputDir singlePion_v1_$1 --inputTXTfile fileLists/singlePion_v1.txt --layout barzflat

  

