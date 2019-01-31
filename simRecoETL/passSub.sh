python submitSampleToCondor.py --outputDir minBias14TeV_v0_$1 --inputTXTfile fileLists/minBias14TeV_v0.txt --layout barzflat
sleep 10
python submitSampleToCondor.py --outputDir singleMuon_v0_$1 --inputTXTfile fileLists/singleMuon_v0.txt --layout barzflat
sleep 10
python submitSampleToCondor.py --outputDir singlePion_v0_$1 --inputTXTfile fileLists/singlePion_v0.txt --layout barzflat

  

