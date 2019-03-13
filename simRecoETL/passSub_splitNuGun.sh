#!/bin/csh

setenv SPLIT 7

foreach x (`seq 0 $SPLIT 49`)
	#echo `expr $x + $SPLIT - 1`
    if ($x == 0) then
	# *** A) Neutrino Gun, 200 PU
	#python submitSampleToCondor.py --outputDir nuGun200PU_v1_$1 --inputTXTfile fileLists/nuGun200PU_v1.txt --layout barzflat --events `expr $x + $SPLIT - 1`
	# *** B) MinBias, 0 PU
	python submitSampleToCondor.py --outputDir minBias0PU_v1_$1 --inputTXTfile fileLists/minBias0PU_v1.txt --layout barzflat --events `expr $x + $SPLIT - 1`
    else if ($x > 0) then
	# *** A) Neutrino Gun, 200 PU
	#python submitSampleToCondor.py --outputDir nuGun200PU_v1_$1 --inputTXTfile fileLists/nuGun200PU_v1.txt --layout barzflat --events `expr $SPLIT - 1` --firstEvent $x
	# *** B) MinBias, 0 PU
	python submitSampleToCondor.py --outputDir minBias0PU_v1_$1 --inputTXTfile fileLists/minBias0PU_v1.txt --layout barzflat --events `expr $SPLIT - 1` --firstEvent $x
    endif

    sleep 10
end

python submitSampleToCondor.py --outputDir singleMuon_v1_$1 --inputTXTfile fileLists/singleMuon_v1.txt --layout barzflat
sleep 10
python submitSampleToCondor.py --outputDir singlePion_v1_$1 --inputTXTfile fileLists/singlePion_v1.txt --layout barzflat

  

