/*******************************************
 * RawTree class                           *
 * Class defining the member variables and *
 * methods for a raw data tree of          *
 * oscilloscope waveforms.                 *
 *                E. Hogenbirk & R. Schoen *
 *                       -- Jan - Mar 2014 *
 *******************************************/


#define RawTree_cxx
#include "RawTree.h"
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

using namespace std ;

void RawTree::Loop(){
/*
//   In a ROOT session, you can do:
//      Root > .L RawTree.C
//      Root > RawTree t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }*/
}


//------- Basic functions ---------------------------------

// Return number of entries in the tree.
int RawTree::GetEntries(){
	if (fChain == 0) return 666 ;
	int nentries = fChain->GetEntriesFast() ;
	return nentries ;
}


// Return histogram with plotted baseline-corrected waveform.
void RawTree::PlotEvent(int entry){
	// Plot event with number of entry
	GetEntry(entry);
	TString name = "Event ";
	name += entry ;
	name += "; Time (ns); Signal (V)";
	TH1F *h1 = new TH1F("h1",name, nSamples,0,nSamples * 0.2 );
	for(int i=0 ; i<nSamples ; i++){
		h1->SetBinContent(i,camplitude[i]);
	}
	h1->Draw();
	return;
}


// Plots event below certain threshold.
void RawTree::SearchEvent(double threshold, int nevent){
	int i=0;
	while(nevent != 0){
		GetEntry(i);
		if(peakAmplitude < threshold){
			nevent--;
		}
	i++;
	}
	i--;
	PlotEvent(i);
	return ;
}


// Return histogram with average over all waveforms.
TH1F* RawTree::GetAverageEvent(){
	int imax = GetEntries() ;
	int nsamplemax = 490;
	
	double averageamp[490] = {0} ;
	for(int i=0; i<imax; i++){
		GetEntry(i) ;
		for(int j=0; j< nsamplemax; j++){
			averageamp[j] += camplitude[j] / imax ;
		}
		if(i%10000==0){
		    cout << "++ Reading event "<< i << " of " << imax << " ..."<< endl ;
		}
	}
	TH1F *h1 = new TH1F("h1","Average;Time (ns);Average amplitude (V;Time (ns);Average amplitude (V))",490, 0, 490 * 0.2 );
	for(int i=0 ; i<490 ; i++){
	    h1->SetBinContent(i,averageamp[i]) ;
	}
	//h1->Draw();
	return h1 ;
}


//------- Timing functions --------------------------------

// Get time bin of half the peak height (rising edge).
int RawTree::GetHalfPeak(){
	int busy = 1 ;
    while(camplitude[peakBin-busy]<0.5*peakAmplitude){
		busy += 1 ;
	}
	return peakBin-busy;
}
									 

// Return histogram of peak time spectrum: either of peak itself or its rising edge (50%).
TH1D* RawTree::GetPeaktimeHisto(int nbins, int minbin, int maxbin, const char* method){
	TH1D* times = new TH1D("times","Peak position; time bin ; counts",nbins,minbin,maxbin );
    int imax = GetEntries();
    for(int i=0 ; i<imax ; i++){
		if(std::string(method)=="peak"){
			GetEntry(i);
			times->Fill(peakBin);
		}
		if(std::string(method)=="half"){
			GetEntry(i);
			times->Fill(GetHalfPeak());
		}
        if(i%10000==0){
        	cout << "++ Reading event "<< i << " of " << imax << " ..."<< endl ;
        }
	}
    return times ;
    delete times ;
}

// Same as previous fn, but now in time instead of bins.
TH1D* RawTree::GetPeaktimeHistoNoStupidBins(int nbins, int mintime, int maxtime, const char* method){
	TH1D* times = new TH1D("","Peak position; time (ns) ; counts",nbins,mintime,maxtime );
    int imax = GetEntries();
    for(int i=0 ; i<imax ; i++){
		if(std::string(method)=="peak"){
			GetEntry(i);
			times->Fill(peakBin*0.4);
		}
		if(std::string(method)=="half"){
			GetEntry(i);
			times->Fill(GetHalfPeak());
		}
        if(i%10000==0){
        	cout << "++ Reading event "<< i << " of " << imax << " ..."<< endl ;
        }
	}
    return times ;
    delete times ;
}


//------- Pulse height, integral and electron spectra -----

// Return histogram of pulse height spectrum.
TH1F* RawTree::GetAmpHisto(int nbins, double minamp, double maxamp){
    TH1F *h1 = new TH1F("h1","Peak Amplitude; Peak voltage (V); counts",nbins,minamp,maxamp );
	int imax = GetEntries();
	// int imax = 1000 ; 
	for(int i=0 ; i<imax ; i++){
		GetEntry(i);
		h1->Fill(peakAmplitude);
		if(i%10000==0){
			cout << "++ Reading event "<< i << " of " << imax << " ..."<< endl ;
		}
	}
	//h1->Draw();
	return h1 ;
}


// Calculate the integral (in pVs) of a baseline-corrected waveform
// between statbin and stopbin.
double RawTree::GetIntegral(int entry, int startbin, int stopbin){
	double integral = 0 ;
	GetEntry(entry);
	for(int i=startbin ; i<stopbin ; i++){
		integral += camplitude[i];
	}
	// Return integral in unit pVs
	return integral*0.2*1000;	
}


// Return histogram of integral spectrum (corrected for baseline, but not pulse timing).
TH1F* RawTree::GetIntHisto(int nbins, double minint, double maxint, int startbin, int stopbin){
	TH1F* h1= new TH1F("h1","Integral; Area (pVs) ; counts",nbins,minint,maxint ) ;
	int imax = GetEntries() ;
	for(int i=0 ; i<imax ; i++){
		GetEntry(i) ;
		h1->Fill(GetIntegral(i,startbin,stopbin));
		if(i%10000==0){
			cout << "++ Reading event "<< i << " of " << imax << " ..."<< endl ;
		}
	}
	return h1 ;
	delete h1;		 
}


// Return histogram of integral spectrum, corrected for baseline, pre- and afterpulses.
TH1F* RawTree::GetIntHistoCut(int nbins, double minint, double maxint, int startbin, int stopbin,
		int lowerbinbdy, int upperbinbdy,const char* method){
	TH1F* integral = new TH1F("integral","Integral; area (pVs) ; counts",nbins,minint,maxint );
	int imax = GetEntries();
	for(int i=0 ; i<imax ; i++){
		GetEntry(i);
		// Cut on the minimum
		if(std::string(method)=="peak"){
			if(peakBin < upperbinbdy && peakBin >lowerbinbdy){
				integral->Fill(GetIntegral(i,startbin,stopbin));
			}
		}
		// Or: cut on half of the minimum
		if (std::string(method)=="half"){
			if(GetHalfPeak()<upperbinbdy && GetHalfPeak() > lowerbinbdy){
				integral->Fill(GetIntegral(i,startbin,stopbin));
			}
		}
		//
		if(i%10000==0)
			cout << "++ Reading event "<< i << " of " << imax << " ..."<< endl ;
	}
	return integral ;
	delete integral ;
}


// Return histogram of SPE spectrum, calibrated in number of electrons. 
TH1F* RawTree::GetElecHisto(int nbins, double minint, double maxint, int startbin, int stopbin,
		int lowerbinbdy, int upperbinbdy,const char* method){
	TH1F* elec = new TH1F("elec","SPE spectrum; Gain (#times10^{6}) ; counts",nbins,minint,maxint );
	int imax = GetEntries();
	double area = 0 ;
	double nofe = 0 ;	// Number of electrons.
	for(int i=0 ; i<imax ; i++){
	    GetEntry(i);
		// Cut on the minimum
		if(std::string(method)=="peak")
			if(peakBin < upperbinbdy && peakBin > lowerbinbdy)
				area = GetIntegral(i,startbin,stopbin) ;
		// Or: cut on half of the minimum
		if (std::string(method)=="half")
			if(GetHalfPeak()<upperbinbdy && GetHalfPeak() > lowerbinbdy)
				area = GetIntegral(i,startbin,stopbin) ;
		//
		nofe = area / (-50 * 1.602e-7) ;	// Area in pVs.
	    elec->Fill( nofe/1e6 );
	    if(i%10000==0)
			cout << "++ Reading event "<< i << " of " << imax << " ..."<< endl ;
	}
	return elec ;
	delete elec ;		 
}


