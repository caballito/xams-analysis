/****************************************
 * Read in waveforms in MATLAB format   *
 * (time "\t" amplitude) from 2 PMTs    *
 * and store in a ROOT tree for further *
 * analysis.                            *
 *                          -- May 2014 *
 ****************************************/

#include "TSystem.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
//#include "TEntryList.h"
#include "TH1.h"
//#include "TH2.h"
#include "TF1.h"
//#include "TMath.h"
#include "TLine.h"
#include "TCanvas.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
using namespace std ;

// Usage:
//   read("<data dir/>","<subdir>","<no channel...>%05d.dat",\
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
	int pmtID ;
	int sample ;
	float tInc ;	// Time increment.
	float time[nMaxSamples] ;
	float amplitude[nMaxSamples] ;
	float camplitude[nMaxSamples] ; // Corrected amplitude (amplitude minus baseline)
	int nSamples ;
	float baseline, baselineSpread ;
	float peakAmplitude ;
	int peakBin ;
	//
	tree->Branch("nSamples",&nSamples,"nSamples/I") ;
	tree->Branch("pmtID",&pmtID,"pmtID/I") ;
	tree->Branch("sample",&sample,"sample/I") ;
	tree->Branch("tInc",&tInc,"tInc/F") ;
	tree->Branch("amplitude",amplitude,"amplitude[sample]/F") ;
	tree->Branch("camplitude",camplitude,"camplitude[sample]/F") ;
	tree->Branch("baseline",&baseline,"baseline/F") ;
	tree->Branch("baselineSpread",&baselineSpread,"baselineSpread/F") ;
	tree->Branch("peakAmplitude",&peakAmplitude,"peakAmplitude/F") ;
	tree->Branch("peakBin",&peakBin,"peakBin/I") ;
	//
	TH1F* base = new TH1F("base","baseline;baseline amplitude (V);frequency",200,-0.01,0.01) ;
	
	// "Loop" over PMT IDs.
	TString channel = "C1" ;
	for (pmtID=0;pmtID<2;pmtID++){
		if (pmtID>0) channel = "C2" ;
		// Loop over the waveform files.
		for (int i=0; i<nWaveforms; i++){
			char waveformFileName[100] ;
			sprintf(waveformFileName, waveformPattern, i) ;
			TString inFilePath = dir + voltage + "/" + channel + waveformFileName ;
			//
			// Get the length of the waveform data file.
			TString command("wc -l ") ;
			command += inFilePath ;
			command += " > fuckyou.txt" ;
			gSystem->Exec(command) ;	// Counts the lines inside the waveform data file.
			ifstream dummy("fuckyou.txt") ;
			// Define nSamples for this waveform.
			dummy >> nSamples ;
			//cout << "++ The file is " << nSamples << " lines long." << endl ;
			gSystem->Exec("rm fuckyou.txt") ;
			//
			// Open the data file.
			ifstream inFile(inFilePath) ;
			if ( !inFile.is_open() ){
				cout << "EE Couldn't find input file!" << endl ;
				return 73 ;
			}
			else if (i%1000 == 0)
				cout << "++ Reading data from " << channel << waveformFileName << "..." << endl ;
			//

			// Initialise parameters.
			float t, v ;	// Placeholder for raw data.
			sample = 0 ;
			// Loop over the waveform.
			while ( (inFile >> t >> v).good() ){
				time[sample] = t ;
				amplitude[sample] = v ;
				sample++ ;
			}
			tInc = time[100] - time[99] ;
			/*cout << endl << "++ Found " << nSamples
				<< " samples in this waveform with a time increment of "
				<< tInc * 1e9 << " ns." << endl ;
			*/

			// Calculate average baseline from first 5000 samples of waveform.
			baseline = 0 ;
			baselineSpread = 0 ;
			base->Clear() ;
			//for (sample=0; sample<nSamples/2; sample++){
			for (sample=0; sample<nBaseBins; sample++){
				base->Fill(amplitude[sample]) ;
			}
			baseline = base->GetMean() ;
			baselineSpread = base->GetRMS() ;	// Returns standard deviation; NOT RMS!
			
			// Get peak height and position
			peakAmplitude = 0 ;
			peakBin = 2 ;
			for (sample=0; sample<nSamples ; sample++){
				if ( peakAmplitude > amplitude[sample] - baseline ){
					peakAmplitude = amplitude[sample] - baseline ;
					peakBin = sample ;	// Update position.
				}
				// Get the corrected amplitudes.
				camplitude[sample]=amplitude[sample]-baseline;
			}
			
			/*cout << "++ This waveform has a baseline of (" << baseline * 1e3 << " +/- "
				<< baselineSpread * 1e3 << ") mV " << endl
				<< "   and a maximum peak height of " << peakAmplitude * 1e3 << " mV at "
				<< peakBin * tInc * 1e9 << " ns." << endl ;
			*/

			tree->Fill() ;

		}	// End of loop over waveforms.
	}	// End of loop over PMT channels.

	delete base ;

	if ( rootOutput.Write() )
		cout << "++ Written tree to file." << endl ;
	else {
		cout << "EE Problems with writing the output file :(" << endl ;
		return 54 ;
	}

	return 42 ;
}
