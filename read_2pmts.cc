/****************************************
 * Read in waveforms in MATLAB format   *
 * (time "\t" amplitude) from 2 PMTs    *
 * and store in a ROOT tree for further *
 * analysis.                            *
 *                     -- May/June 2014 *
 ****************************************/

#include "TSystem.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TF1.h"
#include "TLine.h"
#include "TCanvas.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
using namespace std ;

// Usage:
//   read("<data dir/>","<subdir>","<no channel...>%05d.dat",
//         <number of waveforms>,<number of bins used for baseline calculation>)
//
int read(TString dir, TString voltage, TString waveformPattern,int nWaveforms,int nBaseBins){
	// Define and open output file.
	TString outFilePath = dir + "raw_data_" + voltage + ".root" ;
	//
	TFile rootOutput(outFilePath,"recreate") ;
	if (!rootOutput.IsOpen()){
		cout << "EE Couldn't open output file!" << endl ;
		return 71 ;
	}
	else
		cout << "++ ROOT tree of waveforms will be written into " 
			<< outFilePath << "..." << endl ;
	//
	// Define tree, its variables and branches.
	TTree* tree = new TTree("tree","Waveform collection") ;
	int nMaxSamples = 500000 ;	// For full 2.5 GS/s resolution.
	int sample ;
	float tInc ;	// Time increment.
	float time[nMaxSamples] ;
	int nSamples ;
	// For PMT1.
	float amplitude0[nMaxSamples] ;
	float camplitude0[nMaxSamples] ; // Corrected amplitude (amplitude minus baseline)
	float baseline0, baselineSpread0 ;
	float peakAmplitude0 ;
	int peakBin0 ;
	// For PMT2.
	float amplitude1[nMaxSamples] ;
	float camplitude1[nMaxSamples] ; // Corrected amplitude (amplitude minus baseline)
	float baseline1, baselineSpread1 ;
	float peakAmplitude1 ;
	int peakBin1 ;
	//
	tree->Branch("nSamples",&nSamples,"nSamples/I") ;
	tree->Branch("sample",&sample,"sample/I") ;
	tree->Branch("tInc",&tInc,"tInc/F") ;
	// PMT1.
	tree->Branch("amplitude0",     amplitude0,      "amplitude0[sample]/F") ;
	tree->Branch("camplitude0",    camplitude0,     "camplitude0[sample]/F") ;
	tree->Branch("baseline0",      &baseline0,      "baseline0/F") ;
	tree->Branch("baselineSpread0",&baselineSpread0,"baselineSpread0/F") ;
	tree->Branch("peakAmplitude0", &peakAmplitude0, "peakAmplitude0/F") ;
	tree->Branch("peakBin0",       &peakBin0,       "peakBin0/I") ;
	// PMT 2.
	tree->Branch("amplitude1",     amplitude1,      "amplitude1[sample]/F") ;
	tree->Branch("camplitude1",    camplitude1,     "camplitude1[sample]/F") ;
	tree->Branch("baseline1",      &baseline1,      "baseline1/F") ;
	tree->Branch("baselineSpread1",&baselineSpread1,"baselineSpread1/F") ;
	tree->Branch("peakAmplitude1", &peakAmplitude1, "peakAmplitude1/F") ;
	tree->Branch("peakBin1",       &peakBin1,       "peakBin1/I") ;
	//
	TH1F* base = new TH1F("base","baseline;baseline amplitude (V);frequency",600,-0.03,0.03) ;
	
	int pmtID ;
	TString inFilePath ;
	// Loop over the waveform files.
	cout << "++ Reading data of waveform " << endl ;
	for (int i=0; i<nWaveforms; i++){
		char waveformFileName[100] ;
		sprintf(waveformFileName, waveformPattern, i) ;
		//
		// "Loop" over PMT IDs. Use pmtID as flag.
		// PMT1.
		pmtID = 0 ;
		inFilePath = dir + voltage + "/C1" + waveformFileName ;
		//
		// Get the length of the waveform data file.
		TString command("wc -l ") ;
		command += inFilePath ;
		command += " > fuckyou.txt" ;
		gSystem->Exec(command) ;	// Counts the lines inside the waveform data file.
		ifstream dummy("fuckyou.txt") ;
		// Define nSamples for this waveform.
		dummy >> nSamples ;
		gSystem->Exec("rm fuckyou.txt") ;
		//
		// Open the data file.
		ifstream inFile(inFilePath) ;
		if ( !inFile.is_open() ){
			cout << "EE Couldn't find input file!" << endl ;
			return 73 ;
		}
		else if (i%1000 == 0)
			cout << " C1: " << i ;
		// Initialise parameters.
		float t, v ;	// Placeholder for raw data.
		sample = 0 ;
		// Loop over the waveform.
		while ( (inFile >> t >> v).good() ){
			time[sample] = t ;
			amplitude0[sample] = v ;
			sample++ ;
		}
		tInc = time[100] - time[99] ;

		// Calculate average baseline from first nBaseBins samples of waveform.
		base->Reset() ;
		baseline0 = 0 ;
		baselineSpread0 = 0 ;
		for (sample=0; sample<nBaseBins; sample++){
			base->Fill(amplitude0[sample]) ;
		}
		baseline0 = base->GetMean() ;
		baselineSpread0 = base->GetRMS() ;
		
		// Get peak height and position.
		peakAmplitude0 = 0 ;
		peakBin0 = 2 ;
		for (sample=0; sample<nSamples ; sample++){
			if ( peakAmplitude0 > amplitude0[sample] - baseline0 ){
				peakAmplitude0 = amplitude0[sample] - baseline0 ;
				peakBin0 = sample ;	// Update position.
			}
			// Get the corrected amplitudes.
			camplitude0[sample] = amplitude0[sample] - baseline0 ;
		}

		//
		// Same for PMT2.
		//
		pmtID = 1 ;
		inFilePath = dir + voltage + "/C2" + waveformFileName ;
		//
		// Get the length of the waveform data file.
		command = "wc -l " ;
		command += inFilePath ;
		command += " > fuckyou.txt" ;
		gSystem->Exec(command) ;	// Counts the lines inside the waveform data file.
		ifstream dummy2("fuckyou.txt") ;
		// Define nSamples for this waveform.
		dummy2 >> nSamples ;
		gSystem->Exec("rm fuckyou.txt") ;
		//
		// Open the data file.
		ifstream inFile2(inFilePath) ;
		if ( !inFile2.is_open() ){
			cout << "EE Couldn't find input file!" << endl ;
			return 73 ;
		}
		else if (i%1000 == 0)
			cout << ", C2: " << i << " ... " << endl ;

		// Initialise parameters.
		//float t, v ;	// Placeholder for raw data.
		sample = 0 ;
		// Loop over the waveform.
		while ( (inFile2 >> t >> v).good() ){
			//time[sample] = t ;
			amplitude1[sample] = v ;
			sample++ ;
		}
		//tInc = time[100] - time[99] ;

		// Calculate average baseline from first nBaseBins samples of waveform.
		base->Reset() ;
		baseline1 = 0 ;
		baselineSpread1 = 0 ;
		for (sample=0; sample<nBaseBins; sample++){
			base->Fill(amplitude1[sample]) ;
		}
		baseline1 = base->GetMean() ;
		baselineSpread1 = base->GetRMS() ;	// Returns standard deviation; NOT RMS!
			
		// Get peak height and position.
		peakAmplitude1 = 0 ;
		peakBin1 = 2 ;
		for (sample=0; sample<nSamples ; sample++){
			if ( peakAmplitude1 > amplitude1[sample] - baseline1 ){
				peakAmplitude1 = amplitude1[sample] - baseline1 ;
				peakBin1 = sample ;	// Update position.
			}
			// Get the corrected amplitudes.
			camplitude1[sample] = amplitude1[sample] - baseline1 ;
		}
		// End of "loop" over PMTs/oscilloscope channels.

		tree->Fill() ;

	}	// End of loop over waveforms.

	delete base ;

	if ( rootOutput.Write() )
		cout << endl << "++ Written tree to file." << endl ;
	else {
		cout << "EE Problems with writing the output file :(" << endl ;
		return 54 ;
	}

	return 42 ;
}
