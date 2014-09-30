/******************************************
 * Read in a processed data tree and make *
 * an output ROOT file with lots of       *
 * histograms in it.                      *
 *                 -- R.Schoen, July 2014 *
 ******************************************/

#include "TString.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include <iostream>
#include <fstream>

#include "Plank.h"

using namespace std ;

int kots(Plank* plank, TString outFileName){
	int nEvents = plank->GetEntries() ;
	cout << "++ " << plank->GetAcceptedEntries() << " events of " << nEvents << " in total are accepted." << endl ;
	// Create output file.
	TString outFilePath = "/data/xenon/xams/pmt/liquid_data/results/" + outFileName + ".root" ;
	TFile outFile(outFilePath,"recreate") ;
	if (!outFile.IsOpen()){
		cout << "EE Couldn't open output file!" << endl ;
		return 72 ;
	} else
		cout << "++ The plots will be written into " << outFilePath << endl ;

	// Create all histograms.
	TH1D* truePeaks = plank->GetTrueNPeaksHisto() ;
	TH1F* driftTime = plank->GetDriftTimeHisto(160,0,80) ;	// Expecting drift times around 50-60 us.
	TH1F* s1Integral = plank->GetS1IntegralHisto(200,0,1.5e3) ;
	TH1F* s2Integral = plank->GetS2IntegralHisto(200,0,30e3) ;
	TH1F* s2Width = plank->GetS2WidthHisto(200,0,20) ;
	TH1F* s2Fwhm = plank->GetS2FWHMHisto(200,0,20) ;
	//
	TH2F* s2WidthVDriftTime = plank->GetS2WidthVDriftTime(160,0,80,200,0,20) ;
	TH2F* s2FwhmVDriftTime = plank->GetS2FWHMVDriftTime(160,0,80,200,0,20) ;
	TH2F* s2IntVDriftTime = plank->GetS2IntegralVDriftTime(160,0,80,200,0,30e3) ;
	TH2F* s2IntVs2Fwhm = plank->GetS2IntegralVFWHM(200,0,20,200,0,30e3) ;
	//
	TH2F* s1s2Correlation = plank->GetS2IntegralVS1Integral(200,0,1.5e3,200,0,30e3) ;
	TH2F* logS2S1 = plank->GetLogS2S1VS1(200,0,1.5e3,100,0.1,3.6) ;
	TH2F* s1s2Asymmetry = plank->GetAsymmetry12Histo(400) ;
	TH2F* s1AsymmetryVDriftTime = plank->GetS1AsymmetryVDriftTime(160,0,80,400) ;
	//
	TH2F* s2Saturation = plank->GetS2Saturation(200,0,30e3) ;

	// Write all histograms.
	truePeaks->Write() ;
	driftTime->Write() ;
	s1Integral->Write() ;
	s2Integral->Write() ;
	s2Width->Write() ;
	s2Fwhm->Write() ;
	//
	s2WidthVDriftTime->Write() ;
	s2FwhmVDriftTime->Write() ;
	s2IntVDriftTime->Write() ;
	s2IntVs2Fwhm->Write() ;
	s1s2Correlation->Write() ;
	logS2S1->Write() ;
	s1s2Asymmetry->Write() ;
	s1AsymmetryVDriftTime->Write() ;
	s2Saturation->Write() ;

	outFile.Close() ;
	cout << "++ All plots written to file." << endl ;

	return 42 ;
}
