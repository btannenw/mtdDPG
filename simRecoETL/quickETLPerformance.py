import ROOT as R
import math as M
import argparse
import subprocess
import os

# usage: e.g. 
#               python drawETLPerformance2.py --inputDir=/eos/user/b/btannenw/MTD/10_4_0_mtd3_runHitsRelValMinBias14TeVnoPU-v1V7_r2/190123_041856/0000/ --pattern='DumpHits_10.*' --output=dpg_01-30-19/minBias14TeV_noPU_v4.root --layout=barzflat

def goodTrack(evt, itrack , chi2cut, skipMCmatching):
    #acceptance cuts
    if (evt.track_pt[itrack]<0.7):
        return False
    if (abs(evt.track_eta[itrack])>3.):
        return False
    #for the moment use matching with mc gen particle
    if (abs(evt.track_mcMatch_DR[itrack])>0.05 and not skipMCmatching):
        return False
    if (abs(evt.track_pt[itrack]/evt.track_mcMatch_genPt[itrack]-1.)>0.1 and not skipMCmatching):
        return False
#    if (evt.track_normalizedChi2[itrack] > chi2cut):
#        return False
#    if (evt.track_eta_atBTL[itrack]<-100 and evt.track_eta_atETL[itrack]<-100):
#        return False
    return True

parser = argparse.ArgumentParser()
parser.add_argument('--input',dest='input')
parser.add_argument('--inputDir',dest='inputDir')
parser.add_argument('--pattern',dest='pattern')
parser.add_argument('--output',dest='output')
parser.add_argument('--layout',dest='layout')
parser.add_argument('--chi2cut',dest='chi2cut')
parser.add_argument('--events',dest='events',default='-1')
parser.add_argument('--firstEvent',dest='firstEvent', default='0')
parser.add_argument('--dumpHits',dest='dumpHits',action='store_true',default=False)
parser.add_argument('--dumpAll',dest='dumpAll',action='store_true',default=False)

args = parser.parse_args()

print args.inputDir 

if (args.inputDir != "" and args.inputDir!=None):
    print args.inputDir
    dh=R.TChain("DumpHits")
    files = []
    if args.inputDir[-1] != '/':
        args.inputDir += '/'
    print('>> Creating list of files from: \n'+args.inputDir)
    command = '/bin/find '+args.inputDir+' -type f | grep root | grep -v failed | grep '+args.pattern
    str_files = subprocess.check_output(command,shell=True).splitlines()
    #print str_files
    files.extend(['file:'+ifile for ifile in str_files])
    for file in files:
        print ">> Adding "+file
        dh.Add(file)
else:
    print('>> Opening: '+args.input)
    f=R.TFile.Open(args.input)
    dh=f.Get("DumpHits")
    print dh.GetEntries()

histos = {}

# time offset introduced in mtd2 due to light collection time (pitch.z()/2 * 7.5ps for bars)
t_offset={}
t_offset['ETL']=-0.0066
t_offset['BTL']=0.
if args.layout == "barzflat":
    t_offset['BTL']=0.226875-0.0115
elif args.layout == "barphi":
    t_offset['BTL']=0.160
elif args.layout == "tile":
    t_offset['BTL']=0.200

histos["track_pt"]=R.TH1F("track_pt","track_pt",100,0.,10.)
histos["track_eta"]=R.TH1F("track_eta","track_eta",100,0.,3,)
histos["track_eta_lowPt"]=R.TH1F("track_eta_lowPt","track_eta_lowPt",100,0.,3,)
histos["track_phi"]=R.TH1F("track_phi","track_phi",1000,-M.pi,M.pi)
#histos["track_eta_1"]=R.TH1F("track_eta_1","track_eta_1",100,0.,3,)

#create hist to draw energy distribution plot
#histos["h_recHit_energy"]=R.TH1F("h_recHit_energy","h_recHit_energy")

histos["numberOfEvents"] = R.TH1F("numberOfEvents", "numberOfEvents", 2, 0, 2)
for det in ["BTL","ETL"]:
    # BBT, 01-25-19
    # Fig 1.6
    #histos["recHit_readoutEnergy_"+det]=R.TH1F("track__readoutEnergy_"+det,"track_readoutEnergy_"+det,40,0,10)
    #histos["recHit_readoutEnergy_"+det]=R.TH1F("track__readoutEnergy_"+det,"track_readoutEnergy_"+det,40,0,10)

    # Fig 1.7+1.8
    minEta = 0 if det == 'BTL' else 1.6
    maxEta = 1.4 if det == 'BTL' else 2.9
    histos["track_eta_at"+det]=R.TH1F("track_eta_at"+det,"track_eta_at"+det,30,minEta,maxEta)
    histos["track_eta_at"+det+"_overThreshE"]=R.TH1F("track_eta_at"+det+"_overThreshE","track_eta_at"+det+"_overThreshE",30,minEta,maxEta)
    histos["track_phi_at"+det]=R.TH1F("track_phi_at"+det,"track_phi_at"+det,30,0,M.pi)
    histos["track_phi_at"+det+"_overThreshE"]=R.TH1F("track_phi_at"+det+"_overThreshE","track_phi_at"+det+"_overThreshE",30,0,M.pi)
    histos["track_pt_at"+det]=R.TH1F("track_pt_at"+det,"track_pt_at"+det,30,0.,10)
    histos["track_pt_at"+det+"_overThreshE"]=R.TH1F("track_pt_at"+det+"_overThreshE","track_pt_at"+det+"_overThreshE",30,0.,10)

    histos["track_eta_at"+det+"_reweight"]=R.TH1F("track_eta_at"+det+"_reweight","track_eta_at"+det+"_reweight",30,minEta,maxEta)
    histos["track_eta_at"+det+"_reweight_overThreshE"]=R.TH1F("track_eta_at"+det+"_reweight_overThreshE","track_eta_at"+det+"_reweight_overThreshE",30,minEta,maxEta)
    histos["track_phi_at"+det+"_reweight"]=R.TH1F("track_phi_at"+det+"_reweight","track_phi_at"+det+"_reweight",30,0,M.pi)
    histos["track_phi_at"+det+"_reweight_overThreshE"]=R.TH1F("track_phi_at"+det+"_reweight_overThreshE","track_phi_at"+det+"_reweight_overThreshE",30,0,M.pi)
    histos["track_pt_at"+det+"_reweight"]=R.TH1F("track_pt_at"+det+"_reweight","track_pt_at"+det+"_reweight",30,0.,10)
    histos["track_pt_at"+det+"_reweight_overThreshE"]=R.TH1F("track_pt_at"+det+"_reweight_overThreshE","track_pt_at"+det+"_reweight_overThreshE",30,0.,10)

    
    # Fig 1.7
    maxE = 50 if det == 'BTL' else 1.5
    histos["recHit_energy_dR05_withTrack"+det]=R.TH1F("recHit_energy_dR05_withTrack"+det,"recHit_energy_dR05_withTrack"+det,50,0.,maxE)
    histos["recHit_energy_dR05_withTrack"+det+"_fixedRange"]=R.TH1F("recHit_energy_dR05_withTrack"+det+"_fixedRange","recHit_energy_dR05_withTrack"+det+"_fixedRange",50,0.,5)
    maxHitE = 19 if det == 'BTL' else 2.5
    histos["recHit_maxEnergy_withTrack"+det]=R.TH1F("recHit_maxEnergy_withTrack"+det,"recHit_maxEnergy_withTrack"+det,125,0.,maxHitE)
    histos["recHit_maxEnergy_dRpass_withTrack"+det]=R.TH1F("recHit_maxEnergy_dRpass_withTrack"+det,"recHit_maxEnergy_dRpass_withTrack"+det,125,0.,maxHitE)
    
    # Fig. Occupancy
    maxE = 10 if det == 'BTL' else 3.0

    # occupancy for all rings
    for mtdSide in ["","_plusSide","_minusSide"]:
        histos["recHit_energy_allHitsAllRings_"+det+mtdSide]=R.TH1F("recHit_energy_allHitsAllRings_"+det+mtdSide,"recHit_energy_allHitsAllRings_"+det+mtdSide, int(maxE*100), 0., maxE)
        histos["recHit_eta_allHitsAllRings_"+det+mtdSide]=R.TH1F("recHit_eta_allHitsAllRings_"+det+mtdSide,"recHit_eta_allHitsAllRings_"+det+mtdSide, 100, minEta-0.1, maxEta+0.1)
        # occupancy split by ring
        for iRing in range(1,12):
            histos["recHit_energy_allHitsRing"+str(iRing)+"_"+det+mtdSide]=R.TH1F("recHit_energy_allHitsRing"+str(iRing)+"_"+det+mtdSide,"recHit_energy_allHitsRing"+str(iRing)+"_"+det+mtdSide, int(100*maxE), 0, maxE)
            histos["recHit_eta_allHitsRing"+str(iRing)+"_"+det+mtdSide]=R.TH1F("recHit_eta_allHitsRing"+str(iRing)+"_"+det+mtdSide,"recHit_eta_allHitsRing"+str(iRing)+"_"+det+mtdSide, 100, minEta-0.1, maxEta+0.1)
                
        
    # Paolo + Ang
    histos[det+"track_pt"]= R.TH1F (det+"track_pt" ,det+"track_pt",100,0.,10.)
    histos[det+"track_eta"]=R.TH1F(det+"track_eta",det+"track_eta",100,0.,3,)
    histos[det+"track_eta_lowPt"]=R.TH1F(det+"track_eta_lowPt",det+"track_eta_lowPt",100,0.,3,)
    histos[det+"track_phi"]=R.TH1F(det+"track_phi",det+"track_phi",5000,-M.pi,M.pi)
    
    
    histos[det+"cluster_energy"]=R.TH1F(det+"cluster_energy",det+"cluster_energy",100,0.,20.)
    histos[det+"cluster_time"]=R.TH1F(det+"cluster_time",det+"cluster_time",100,0.,25.)
    histos[det+"cluster_size"]=R.TH1F(det+"cluster_size",det+"cluster_size",20,0.,20.)
    histos[det+"cluster_sizeX"]=R.TH1F(det+"cluster_sizeX",det+"cluster_sizeX",20,0.,20.)
    histos[det+"cluster_sizeY"]=R.TH1F(det+"cluster_sizeY",det+"cluster_sizeY",20,0.,20.)
    histos[det+"cluster_seedEnergyRatio"]=R.TH1F(det+"cluster_seedEnergyRatio",det+"cluster_seedEnergyRatio",110,0.,1.1)
    
    histos[det+"recHit_energy"]=R.TH1F(det+"recHit_energy",det+"recHit_energy",100,0.,20.)
    histos[det+"recHit_time"]=R.TH1F(det+"recHit_time",det+"recHit_time",100,0.,25.)
    histos[det+"recHit_energy_h"]=R.TH1F(det+"recHit_energy_h",det+"recHit_energy_h",200,0.,100.)


det_id = { 'BTL':1  , 'ETL':2 }
etaCut = { 'BTL':[0,1.5]  , 'ETL':[1.5,3] }
modulesPerETLRing = [64., 64., 80., 96., 112., 112., 128., 144., 160., 160., 176.]
thresholdEnergy = { 'BTL': 3  , 'ETL': 0.015 } # MeV

singleMu_trackPtAtETL_weights = { 1:0 , 2:0 , 3:13.3733 , 4:6.10089 , 5:3.31314 , 6:1.60481 , 7:1.00255 , 8:0.59895 , 9:0.372216 , 10:0.21744 , 11:0.137574 , 12:0.0875561 , 13:0.0612986 , 14:0.0484957 , 15:0.0336223 , 16:0.0226924 , 17:0.0178273 , 18:0.0135571 , 19:0.00864238 , 20:0.00776581 , 21:0.00642448 , 22:0.00476772 , 23:0.00304024 , 24:0.00403177 , 25:0.00327606 , 26:0.00196983 , 27:0.00225542 , 28:0.00145646 , 29:0.00182964 , 30:0.0224877 } 
singlePi_trackPtAtETL_weights = { 1:0 , 2:0 , 3:16.8581 , 4:6.49493 , 5:3.4822 , 6:1.64186 , 7:0.950607 , 8:0.631338 , 9:0.344325 , 10:0.243136 , 11:0.16214 , 12:0.0969625 , 13:0.0645502 , 14:0.0460892 , 15:0.0290301 , 16:0.0219781 , 17:0.0173903 , 18:0.0135819 , 19:0.00929119 , 20:0.00654508 , 21:0.00687348 , 22:0.00438949 , 23:0.00330055 , 24:0.00391885 , 25:0.00317265 , 26:0.0020943 , 27:0.00195157 , 28:0.00122565 , 29:0.00166819 , 30:0.0223251 } 

 
if (args.dumpAll):
    print "Dumping also non matched MTD hits"

if (args.dumpHits):
    print "Dumping RecHits"

for ievent,event in enumerate(dh):
    #if (int(args.events) != -1 and ievent>int(args.events) ):
    #    break
    if (int(args.events) != -1 and (ievent>(int(args.events)+int(args.firstEvent)) or ievent<int(args.firstEvent)) ):
        continue
    if (int(args.events)==-1 and ievent<int(args.firstEvent)):
        continue 

    #print "Analyzing event %d"%ievent    
    histos["numberOfEvents"].Fill(1)
    if (ievent%100==0): #fixme
        print "Analyzing event %d"%ievent
    
    if (args.dumpAll):    
        for det in ["BTL","ETL"]:
            for iclus in range(0,event.clusters_n):
                if ( event.clusters_det[iclus] !=  det_id[det] ):
                    continue 
                histos[det+"cluster_energy"].Fill(event.clusters_energy[iclus])
                histos[det+"cluster_time"].Fill(event.clusters_time[iclus])
                histos[det+"cluster_size"].Fill(event.clusters_size[iclus])
                histos[det+"cluster_sizeX"].Fill(event.clusters_size_x[iclus])
                histos[det+"cluster_sizeY"].Fill(event.clusters_size_y[iclus])
                histos[det+"cluster_seedEnergyRatio"].Fill(event.clusters_seed_energy[iclus]/event.clusters_energy[iclus])

            if (args.dumpHits):    
                for ihit in range(0,event.recHits_n):
                    if ( event.recHits_det[ihit] !=  det_id[det] ):
                        continue 
                    histos[det+"recHit_energy"].Fill(event.recHits_energy[ihit])
                    histos[det+"recHit_time"].Fill(event.recHits_time[ihit])
    
    #occupancy plots
    for iRecHit in range(0, event.recHits_n):   #loop over all recHits
        if (event.recHits_det[iRecHit]==det_id['ETL'] and abs(event.recHits_eta[iRecHit])>1.5 and abs(event.recHits_eta[iRecHit])<3.0):
            histos["recHit_energy_allHitsAllRings_ETL"].Fill(event.recHits_energy[iRecHit])
            histos["recHit_energy_allHitsRing"+str(event.recHits_rr[iRecHit])+"_ETL"].Fill(event.recHits_energy[iRecHit])
            if(event.recHits_side[iRecHit]==0):
                histos["recHit_energy_allHitsAllRings_ETL_minusSide"].Fill(event.recHits_energy[iRecHit])
                histos["recHit_energy_allHitsRing"+str(event.recHits_rr[iRecHit])+"_ETL_minusSide"].Fill(event.recHits_energy[iRecHit])
            if(event.recHits_side[iRecHit]==1):
                histos["recHit_energy_allHitsAllRings_ETL_plusSide"].Fill(event.recHits_energy[iRecHit])
                histos["recHit_energy_allHitsRing"+str(event.recHits_rr[iRecHit])+"_ETL_plusSide"].Fill(event.recHits_energy[iRecHit])
        if (event.recHits_det[iRecHit]==det_id['ETL'] and event.recHits_energy[iRecHit]>thresholdEnergy["ETL"]):
            histos["recHit_eta_allHitsAllRings_ETL"].Fill(abs(event.recHits_eta[iRecHit]))
            histos["recHit_eta_allHitsRing"+str(event.recHits_rr[iRecHit])+"_ETL"].Fill(abs(event.recHits_eta[iRecHit]))
            if(event.recHits_side[iRecHit]==0):
                histos["recHit_eta_allHitsAllRings_ETL_minusSide"].Fill(abs(event.recHits_eta[iRecHit]))
                histos["recHit_eta_allHitsRing"+str(event.recHits_rr[iRecHit])+"_ETL_minusSide"].Fill(abs(event.recHits_eta[iRecHit]))
            if(event.recHits_side[iRecHit]==1):
                histos["recHit_eta_allHitsAllRings_ETL_plusSide"].Fill(event.recHits_eta[iRecHit])
                histos["recHit_eta_allHitsRing"+str(event.recHits_rr[iRecHit])+"_ETL_plusSide"].Fill(abs(event.recHits_eta[iRecHit]))

                
    # loop over all tracks
    #print "Number of tracks: {0}".format(len(event.track_idx))
    for itrack in range(0,len(event.track_idx)):
        recHit_energy_totalval_BTL = 0   #the total value of recHit energy from BTL
        recHit_energy_totalval_ETL = 0   #from ETL
        n_recHits_BTL = 0
        n_recHits_ETL = 0

        isNuGun = True if args.input.find("NuGun")>0 else False
        if (not goodTrack(event,itrack,args.chi2cut, isNuGun)):
            continue
        
        #if (itrack%100==0): #fixme
        #print "Analyzing track %d"%itrack

        ########################   BTL   ####################################################################
        # BBT, 01-25-19
        cut_dR = 0.05
        if (event.track_eta_atBTL[itrack]>-100.):  #if the extrapolated track hits the BTL
            recHit_matched_totalE_BTL = 0
            maxRecHitEnergy = 0
            maxRecHitEnergy_dRpass = 0
            histos["track_eta_atBTL"].Fill(abs(event.track_eta_atBTL[itrack]))
            histos["track_phi_atBTL"].Fill(abs(event.track_phi_atBTL[itrack]))
            histos["track_pt_atBTL"].Fill(event.track_pt[itrack])
            for iRecHit in range(0,event.recHits_n):   #loop over all recHits
                dR= M.sqrt(pow((event.track_eta_atBTL[itrack]-event.recHits_eta[iRecHit]),2)+pow(event.track_phi_atBTL[itrack]-event.recHits_phi[iRecHit],2))
                if (event.recHits_det[iRecHit]!=det_id["BTL"]):
                    continue
                #print "BTL: {0}th track and {1}th hit have dR = {2}".format(itrack, iRecHit, dR)

                histos["recHit_energy_allHitsAllRings_BTL"].Fill(event.recHits_energy[iRecHit])
                if(event.recHits_energy[iRecHit]>thresholdEnergy["BTL"]):
                    histos["recHit_eta_allHitsAllRings_BTL"].Fill(event.recHits_eta[iRecHit])
                
                if (event.recHits_energy[iRecHit] > maxRecHitEnergy):
                    maxRecHitEnergy = event.recHits_energy[iRecHit]
                if(dR < cut_dR):
                    recHit_matched_totalE_BTL += event.recHits_energy[iRecHit]
                    if (event.recHits_energy[iRecHit] > maxRecHitEnergy_dRpass):
                        maxRecHitEnergy_dRpass = event.recHits_energy[iRecHit]
            histos["recHit_energy_dR05_withTrackBTL"].Fill(recHit_matched_totalE_BTL)
            histos["recHit_maxEnergy_withTrackBTL"].Fill(maxRecHitEnergy)
            histos["recHit_maxEnergy_dRpass_withTrackBTL"].Fill(maxRecHitEnergy_dRpass)
            if(recHit_matched_totalE_BTL > thresholdEnergy["BTL"] ):
                histos["track_eta_atBTL_overThreshE"].Fill(abs(event.track_eta_atBTL[itrack]))
                histos["track_phi_atBTL_overThreshE"].Fill(abs(event.track_phi_atBTL[itrack]))
                histos["track_pt_atBTL_overThreshE"].Fill(event.track_pt[itrack])
            
        ########################   ETL   ####################################################################
        if (event.track_eta_atETL[itrack]>-100 and  # extrapolated track hits the ETL
            abs(event.track_eta[itrack])>1.5 and abs(event.track_eta[itrack])<3.0 and event.track_pt[itrack]>0.7   # track quality and acceptance for ETL
            ):
        
            emptyModulesPerETLRing = list(2*nMod for nMod in modulesPerETLRing) # need to double modules since combining +z and -z discs
            allModulesPerETLRing   = list(2*nMod for nMod in modulesPerETLRing) # need to double modules since combining +z and -z discs
            
            recHit_matched_totalE_ETL = 0 
            maxRecHitEnergy = 0
            maxRecHitEnergy_dRpass = 0
            histos["track_eta_atETL"].Fill(abs(event.track_eta_atETL[itrack]))
            histos["track_phi_atETL"].Fill(abs(event.track_phi_atETL[itrack]))
            histos["track_pt_atETL"].Fill(event.track_pt[itrack])
            ptBin = histos["track_pt_atETL"].FindBin(event.track_pt[itrack]) if histos["track_pt_atETL"].FindBin(event.track_pt[itrack]) <= 30 else 30
            #print histos["track_pt_atETL"].FindBin(event.track_pt[itrack]), ptBin, event.track_pt[itrack]
            if args.input.find("SingleMu")>0:
                histos["track_eta_atETL_reweight"].Fill(abs(event.track_eta_atETL[itrack]), singleMu_trackPtAtETL_weights[ ptBin ] )
                histos["track_phi_atETL_reweight"].Fill(abs(event.track_phi_atETL[itrack]), singleMu_trackPtAtETL_weights[ ptBin ] )
                histos["track_pt_atETL_reweight"].Fill(event.track_pt[itrack], singleMu_trackPtAtETL_weights[ ptBin ] )
            if args.input.find("SinglePi")>0:
                histos["track_eta_atETL_reweight"].Fill(abs(event.track_eta_atETL[itrack]), singlePi_trackPtAtETL_weights[ ptBin ] )
                histos["track_phi_atETL_reweight"].Fill(abs(event.track_phi_atETL[itrack]), singlePi_trackPtAtETL_weights[ ptBin ] )
                histos["track_pt_atETL_reweight"].Fill(event.track_pt[itrack], singlePi_trackPtAtETL_weights[ ptBin ] )

            
            for iRecHit in range(0,event.recHits_n):   #loop over all recHits
                dR= M.sqrt(pow((event.track_eta_atETL[itrack]-event.recHits_eta[iRecHit]),2)+pow(event.track_phi_atETL[itrack]-event.recHits_phi[iRecHit],2))
                if (event.recHits_det[iRecHit]!=det_id["ETL"]):
                    continue

                #print "ETL: {0}th track and {1}th hit have dR = {2}".format(itrack, iRecHit, dR)
                emptyModulesPerETLRing[event.recHits_rr[iRecHit] - 1] -= 1

                if (event.recHits_energy[iRecHit] > maxRecHitEnergy):
                    maxRecHitEnergy = event.recHits_energy[iRecHit]
                if (dR < cut_dR):
                    recHit_matched_totalE_ETL += event.recHits_energy[iRecHit]
                    if (event.recHits_energy[iRecHit] > maxRecHitEnergy_dRpass):
                        maxRecHitEnergy_dRpass = event.recHits_energy[iRecHit]

            # fill histograms after loop
            histos["recHit_energy_dR05_withTrackETL"].Fill(recHit_matched_totalE_ETL)
            histos["recHit_energy_dR05_withTrackETL_fixedRange"].Fill(recHit_matched_totalE_ETL)
            histos["recHit_maxEnergy_withTrackETL"].Fill(maxRecHitEnergy)
            histos["recHit_maxEnergy_dRpass_withTrackETL"].Fill(maxRecHitEnergy_dRpass)
            if(recHit_matched_totalE_ETL > thresholdEnergy["ETL"] ):
                histos["track_eta_atETL_overThreshE"].Fill(abs(event.track_eta_atETL[itrack]))
                histos["track_phi_atETL_overThreshE"].Fill(abs(event.track_phi_atETL[itrack]))
                histos["track_pt_atETL_overThreshE"].Fill(event.track_pt[itrack])
                ptBin = histos["track_pt_atETL"].FindBin(event.track_pt[itrack]) if histos["track_pt_atETL"].FindBin(event.track_pt[itrack]) <= 30 else 30
                if args.input.find("SingleMu")>0:
                    histos["track_eta_atETL_reweight_overThreshE"].Fill(abs(event.track_eta_atETL[itrack]), singleMu_trackPtAtETL_weights[ ptBin ] )
                    histos["track_phi_atETL_reweight_overThreshE"].Fill(abs(event.track_phi_atETL[itrack]), singleMu_trackPtAtETL_weights[ ptBin ] )
                    histos["track_pt_atETL_reweight_overThreshE"].Fill(event.track_pt[itrack], singleMu_trackPtAtETL_weights[ ptBin ] )
                if args.input.find("SinglePi")>0:
                    histos["track_eta_atETL_reweight_overThreshE"].Fill(abs(event.track_eta_atETL[itrack]), singlePi_trackPtAtETL_weights[ ptBin ] )
                    histos["track_phi_atETL_reweight_overThreshE"].Fill(abs(event.track_phi_atETL[itrack]), singlePi_trackPtAtETL_weights[ ptBin ] )
                    histos["track_pt_atETL_reweight_overThreshE"].Fill(event.track_pt[itrack], singlePi_trackPtAtETL_weights[ ptBin ] )

        ############################################################################################3

        histos["track_pt"].Fill(event.track_pt[itrack])
        histos["track_eta"].Fill(abs(event.track_eta[itrack]))

        if (event.track_pt[itrack]<1.):
            histos["track_eta_lowPt"].Fill(abs(event.track_eta[itrack]))
        histos["track_phi"].Fill(event.track_phi[itrack])
        

        for det in ["BTL","ETL"]:
            if ( 
                abs(event.track_eta[itrack]) < etaCut[det][0] or
                abs(event.track_eta[itrack]) > etaCut[det][1]
                ):
                continue
            
            histos[det+"track_pt"].Fill(event.track_pt[itrack])
            histos[det+"track_eta"].Fill(abs(event.track_eta[itrack]))

            if (event.track_pt[itrack]<1.):
                histos[det+"track_eta_lowPt"].Fill(abs(event.track_eta[itrack]))
            histos[det+"track_phi"].Fill(event.track_phi[itrack])



    

for det in ["BTL","ETL"]:
    #BBT
    histos["track_pt_at"+det+"_graph"]=R.TGraphAsymmErrors( histos["track_pt_at"+det+"_overThreshE"], histos["track_pt_at"+det])
    histos["track_eta_at"+det+"_graph"]=R.TGraphAsymmErrors( histos["track_eta_at"+det+"_overThreshE"], histos["track_eta_at"+det])
    histos["track_phi_at"+det+"_graph"]=R.TGraphAsymmErrors( histos["track_phi_at"+det+"_overThreshE"], histos["track_phi_at"+det])


#Check to see if output directory exists. if not, create it
outputDir = args.output.split('/')[0]
if (not os.path.exists("./"+outputDir)):
    os.system("mkdir "+outputDir)

fOut=R.TFile(args.output,"RECREATE")
for hn, histo in histos.iteritems():
    if isinstance(histo,R.TH1F):
        histo.SetMinimum(0.)
    if isinstance(histo,R.TGraphAsymmErrors):
        histo.SetMinimum(0.)
        histo.SetMaximum(1.1)
    histo.Write()
fOut.Close()
print "Saved histos in "+args.output
