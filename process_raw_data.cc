/******************************************
 * Call the peak finding function of the  *
 * RawTree class to fill a TTree with the *
 * peak information of all events and of  *
 * both PMTs for further analysis.        *
 *                           -- June 2014 *
 ******************************************/

#include "TSystem.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
//#include "TH1.h"
//#include "TH2.h"
//#include "TF1.h"
//#include "TLine.h"
//#include "TCanvas.h"
#include <iostream>
#include <iomanip>
//#include <sstream>
//#include <fstream>
#include <vector>

#include "RawTree2.h"

using namespace std ;

//
int process(RawTree* rawtree, float sigmas, float sigmasreset, TString dir, TString run, TString measurement){
	int nWaveforms = rawtree->GetEntries() ;
	cout << "++ Will look for peaks in " << nWaveforms << " waveforms." << endl ;
	// Define and open output file.
	TString outFilePath = dir + "processed_data_RUN-" + run + "_MEAS-" + measurement + ".root" ;
	//
	TFile rootOutput(outFilePath,"recreate") ;
	if (!rootOutput.IsOpen()){
		cout << "EE Couldn't open output file!" << endl ;
		return 71 ;
	} else
		cout << "++ ROOT tree of processed data will be written into " 
			<< outFilePath << "..." << endl ;
	// Define tree, its variables and branches.
	TTree* datatree = new TTree("datatree","Processed data") ;
	//
	Double_t timeStep ;
	Int_t nPeaks1 ;
	Int_t nPeaks2 ;
	//
	vector<Double_t> start1 ;
	vector<Double_t> start501 ;
	vector<Double_t> start901 ;
	vector<Double_t> end1 ;
	vector<Double_t> end501 ;
	vector<Double_t> end901 ;
	vector<Double_t> bottom_before1 ;
	vector<Double_t> bottom_after1 ;
	vector<Double_t> width1 ;
	vector<Double_t> width_bottom1 ;
	vector<Double_t> fwhm1 ;
	vector<Double_t> max_pos1 ;
	vector<Double_t> max1 ;
	vector<Double_t> integral1 ;
	vector<Double_t> trise1 ;
	vector<Double_t> tfall1 ;
	//
	vector<Double_t> start2 ;
	vector<Double_t> start502 ;
	vector<Double_t> start902 ;
	vector<Double_t> end2 ;
	vector<Double_t> end502 ;
	vector<Double_t> end902 ;
	vector<Double_t> bottom_before2 ;
	vector<Double_t> bottom_after2 ;
	vector<Double_t> width2 ;
	vector<Double_t> width_bottom2 ;
	vector<Double_t> fwhm2 ;
	vector<Double_t> max_pos2 ;
	vector<Double_t> max2 ;
	vector<Double_t> integral2 ;
	vector<Double_t> trise2 ;
	vector<Double_t> tfall2 ;

	// Syntax: tree->Branch("branchname",address,buffersize,splitlevel=1).
	datatree->Branch("timeStep",&timeStep) ;
	datatree->Branch("nPeaks1",&nPeaks1) ;
	datatree->Branch("nPeaks2",&nPeaks2) ;
	//
	datatree->Branch("start1",&start1) ;
	datatree->Branch("start501",&start501) ;
	datatree->Branch("start901",&start901) ;
	datatree->Branch("end1",&end1) ;
	datatree->Branch("end501",&end501) ;
	datatree->Branch("end901",&end901) ;
	datatree->Branch("width1",&width1) ;
	datatree->Branch("width_bottom1",&width_bottom1) ;
	datatree->Branch("fwhm1",&fwhm1) ;
	datatree->Branch("max_pos1",&max_pos1) ;
	datatree->Branch("max1",&max1) ;
	datatree->Branch("integral1",&integral1) ;
	datatree->Branch("trise1",&trise1) ;
	datatree->Branch("tfall1",&tfall1) ;
	//
	datatree->Branch("start2",&start2) ;
	datatree->Branch("start502",&start502) ;
	datatree->Branch("start902",&start902) ;
	datatree->Branch("end2",&end2) ;
	datatree->Branch("end502",&end502) ;
	datatree->Branch("end902",&end902) ;
	datatree->Branch("width2",&width2) ;
	datatree->Branch("width_bottom2",&width_bottom2) ;
	datatree->Branch("fwhm2",&fwhm2) ;
	datatree->Branch("max_pos2",&max_pos2) ;
	datatree->Branch("max2",&max2) ;
	datatree->Branch("integral2",&integral2) ;
	datatree->Branch("trise2",&max2) ;
	datatree->Branch("tfall2",&max2) ;
	//
	//
	vector<RawTree::Peak> peaks1 ;
	vector<RawTree::Peak> peaks2 ;
	//
	// Loop over the waveforms in the tree.
	for(int i=0; i<nWaveforms;i++){
		if(i%1000 == 0)
			cout << "++ Analysing waveform " << i << " of " << nWaveforms << " ..." << endl;
		// Get peak vectors and their length from raw data tree.
		peaks1 = rawtree->FindPeaks(i,sigmas,sigmasreset,0);
		nPeaks1 = peaks1.size() ;
		peaks2 = rawtree->FindPeaks(i,sigmas,sigmasreset,1);
		nPeaks2 = peaks2.size() ;

		// Reset all vectors.
		start1.clear() ; start501.clear() ; start901.clear() ;
		end1.clear() ; end501.clear() ; end901.clear() ;
		width1.clear() ; width_bottom1.clear() ; fwhm1.clear() ;
		max_pos1.clear() ; max1.clear() ; integral1.clear() ;
		trise1.clear() ; tfall1.clear() ;
		//
		start2.clear() ; start502.clear() ; start902.clear() ;
		end2.clear() ; end502.clear() ; end902.clear() ;
		width2.clear() ; width_bottom2.clear() ; fwhm2.clear() ;
		max_pos2.clear() ; max2.clear() ; integral2.clear() ;
		trise2.clear() ; tfall2.clear() ;

		for (int j=0;j<nPeaks1;j++){
			start1.push_back(rawtree->tInc * peaks1[j].start) ;
			start501.push_back(rawtree->tInc * peaks1[j].start50) ;
			start901.push_back(rawtree->tInc * peaks1[j].start90) ;
			end1.push_back(rawtree->tInc * peaks1[j].end) ;
			end501.push_back(rawtree->tInc * peaks1[j].end50) ;
			end901.push_back(rawtree->tInc * peaks1[j].end90) ;
			width1.push_back(rawtree->tInc * peaks1[j].width) ;
			width_bottom1.push_back(rawtree->tInc * peaks1[j].width_bottom) ;
			fwhm1.push_back(rawtree->tInc * peaks1[j].fwhm) ;
			max_pos1.push_back(rawtree->tInc * peaks1[j].max_bin) ;
			//
			max1.push_back(peaks1[j].max) ;
			integral1.push_back(peaks1[j].integral) ;
			trise1.push_back(peaks1[j].trise) ;
			tfall1.push_back(peaks1[j].tfall) ;
		}
		for (int j=0;j<nPeaks2;j++){
			start2.push_back(rawtree->tInc * peaks2[j].start) ;
			start502.push_back(rawtree->tInc * peaks2[j].start) ;
			start902.push_back(rawtree->tInc * peaks2[j].start) ;
			end2.push_back(rawtree->tInc * peaks2[j].end) ;
			end502.push_back(rawtree->tInc * peaks2[j].end) ;
			end902.push_back(rawtree->tInc * peaks2[j].end) ;
			width2.push_back(rawtree->tInc * peaks2[j].width) ;
			width_bottom2.push_back(rawtree->tInc * peaks2[j].width_bottom) ;
			fwhm2.push_back(rawtree->tInc * peaks2[j].fwhm) ;
			max_pos2.push_back(rawtree->tInc * peaks2[j].max_bin) ;
			//
			max2.push_back(peaks2[j].max) ;
			integral2.push_back(peaks2[j].integral) ;
			trise2.push_back(peaks2[j].trise) ;
			tfall2.push_back(peaks2[j].tfall) ;
		}
		rawtree->GetEntry(i) ;
		timeStep = rawtree->tInc ;
		// That's it. Now fill the information into the NEW tree!
		datatree->Fill() ;
	}
	if ( rootOutput.Write() )
		cout << endl << "++ Written tree to file." << endl ;
	else {
		cout << "EE Problems with writing the output file :(" << endl ;
		return 54 ;
	}

	return 42 ;
}
