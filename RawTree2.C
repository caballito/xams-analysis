/*******************************************
 * RawTree class                           *
 * Class defining the member variables and *
 * methods for a raw data tree of the      *
 * waveforms of two oscilloscope channels. *
 *                E. Hogenbirk & R. Schoen *
 *                        -- May/June 2014 *
 *******************************************/


#define RawTree_cxx
#include "RawTree2.h"
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>

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
	name += "; Time (#mus); Signal (V)";
	TH1F *h1 = new TH1F("h1",name, nSamples,0,nSamples * tInc * 1e6 );
	TH1F *h2 = new TH1F("h2",name, nSamples,0,nSamples * tInc * 1e6 );
	//TH1F *h3 = new TH1F("h3",name, nSamples,0,nSamples * tInc );
	for(int i=0 ; i<nSamples ; i++){
		h1->SetBinContent(i,camplitude0[i]);
		h2->SetBinContent(i,camplitude1[i]);
		//h3->SetBinContent(i,camplitude0[i] + camplitude1[i]);
	}
	h1->Draw();
	h2->SetLineColor(kRed) ;
	h2->Draw("same") ;
	//h3->SetLineColor(kBlue) ;
	//h3->Draw("same") ;
	return;
}




// Return histogram with plotted baseline-corrected waveform.
void RawTree::PlotEventSum(int entry){
// Plot event with number of entry, and sum signal
	GetEntry(entry);
    TString name = "Event ";
    name += entry ;
    name += "; Time (s); Signal (V)";
    TH1F *h1 = new TH1F("h1",name, nSamples,0,nSamples * tInc );
    TH1F *h2 = new TH1F("h2",name, nSamples,0,nSamples * tInc );
	TH1F *h3 = new TH1F("h3",name, nSamples,0,nSamples * tInc );
    for(int i=0 ; i<nSamples ; i++){
    	h1->SetBinContent(i,camplitude0[i]);
        h2->SetBinContent(i,camplitude1[i]);
		h3->SetBinContent(i,camplitude0[i] + camplitude1[i]);
        }
    h1->Draw();
    h2->SetLineColor(kRed) ;
    h2->Draw("same") ;
	h3->SetLineColor(kBlue) ;
    h3->Draw("same") ;
	return;
    }
                                                      

// Plots event below certain threshold.
void RawTree::SearchEvent(double threshold, int nevent){
	int i=0;
	while(nevent != 0){
		GetEntry(i);
		if(peakAmplitude1 > threshold){
			nevent--;
		}
		i++;
	}
	i--;
	PlotEvent(i);
	return ;
}


//------- Timing functions --------------------------------

// Get time bin of half the peak height (rising edge).
int RawTree::GetHalfPeak(int pmt){
	int busy = 1 ;
	if (pmt) {
		while(camplitude1[peakBin1-busy]>0.5*peakAmplitude1)
			busy += 1 ;
		return peakBin1-busy;
	} else {
		while(camplitude0[peakBin0-busy]>0.5*peakAmplitude0)
			busy += 1 ;
		return peakBin0-busy;
	}
}
									 

// Return histogram of peak time spectrum: either of peak itself or its rising edge (50%).
TH1D* RawTree::GetPeaktimeHisto(int nbins, int minbin, int maxbin, const char* method, int pmt){
	TH1D* times = new TH1D("times","Peak position; time bin ; counts",nbins,minbin,maxbin );
    int imax = GetEntries() ;
    for(int i=0 ; i<imax ; i++){
		if(std::string(method)=="peak"){
			GetEntry(i);
			if (pmt)
				times->Fill(peakBin1) ;
			else
				times->Fill(peakBin0) ;
		}
		if(std::string(method)=="half"){
			GetEntry(i);
			times->Fill(GetHalfPeak(pmt));
		}
        if(i%10000==0){
        	cout << "++ Reading event "<< i << " of " << imax << " ..."<< endl ;
        }
	}
    return times ;
    delete times ;
}


// Looks in an event and finds the bin where the signal first gets higher than the threshold.
// Returns zero if no peak is found.
int RawTree::GetS1Pos(int event, int startbin, int stopbin, double threshold, int pmtid){
	GetEntry(event);
	// Iterator to loop over the waveform.
	int i = startbin;
	// Store current amplitude.
	double amp = 0;
	
	// Loop over the waveform until either an amplitude higher than the threshold is found or the end is reached.
	while (amp<threshold && i<stopbin){
		if(pmtid == 0)
			amp = camplitude0[i];
		if(pmtid == 1)
			amp = camplitude1[i];
		i++;
	}
	
	if(amp > threshold)
		return i;
	else 
		return 0 ;
}


// Looks for peaks in an event
// Syntax: event: eventnumber, sigmas: upper boundary to find a peak, sigmasreset: 
vector<RawTree::Peak> RawTree::FindPeaks(int event, float sigmas, float sigmasreset, int pmt){
	/*
	 * The Peak struct contains info about the peaks found in the waveform:
	 *   start          start bin
	 *   end            end bin
	 *   bottom_before  bin when it last was < -2 * baselineDev
	 *   bottom_after   bin when it's again  < -2 * baselineDev
	 *   width          end - start
	 *   width_bottom   width at the bottom
	 *   center         between start and end
	 *   max_bin		bin where the max occurs
	 *   max			maximum value of the peak
	 *   integral		integral from bottom_before to bottom_after
	 */
	
	GetEntry(event) ;
	double baselineDev ;
	if (pmt==0)
		baselineDev = baselineSpread0 ;
	if (pmt==1)
		baselineDev = baselineSpread1 ;
	// Option pmt==2: sum signal. baseline is expected to grow as the root of squares (?)
	if (pmt==2)
		baselineDev = sqrt(pow(baselineSpread0,2)+pow(baselineSpread1,2));
	// Is it > threshold = N * std_deviation of the baseline.
	float threshold = sigmas * baselineDev ;
	float bottom = -sigmasreset * baselineDev ;
	//cout << "++ The threshold (" << sigmas << "*sigma of the baseline) to cross is at " 
	//	<< threshold << " V and the bottom base is at " << bottom << " V." << endl ;
	RawTree::Peak newpeak ;
	vector<RawTree::Peak> peaks ;
	// Loop over waveform.
	newpeak.start = -505 ;
	newpeak.end = -500 ;
	newpeak.bottom_before = 0 ;
	float previous = -999999 ;
	float amp ;
	for (int s=0;s<nSamples;s++){
		if (pmt==0)
			amp = camplitude0[s] ;
		else if (pmt==1)
			amp = camplitude0[s] ;
		else
			amp = camplitude0[s] + camplitude1[s] ;
		//
		if (amp < bottom){
			if (newpeak.end < 0)
				newpeak.bottom_before = s ;
			else {
				// Peak dropped below threshold and has now touched the "bottom base".
				// It's REAL! End and append it.
				newpeak.bottom_after = s ;
				newpeak.width_bottom = newpeak.bottom_after - newpeak.bottom_before ;
				//
				// Get additional peak info.
				newpeak.max_bin = GetMaxBin(event, newpeak.bottom_before,newpeak.bottom_after,pmt);
				if(pmt==0)
					newpeak.max = camplitude0[newpeak.max_bin];
				else if(pmt==1)
					newpeak.max = camplitude1[newpeak.max_bin];
				else
					newpeak.max = camplitude0[newpeak.max_bin] + camplitude1[newpeak.max_bin] ;
				// Recalculate start and end at 10 % of peak height.
				newpeak.start = GetXPercentHeight(event,newpeak.bottom_before,newpeak.max_bin,newpeak.max,0.1,pmt) ;
				newpeak.end = GetXPercentHeight(event,newpeak.bottom_after,newpeak.max_bin,newpeak.max,0.1,pmt) ;
				// Get positions of 50 %.
				newpeak.start50 = GetXPercentHeight(event,newpeak.bottom_before,newpeak.max_bin,newpeak.max,0.5,pmt) ;
				newpeak.end50 = GetXPercentHeight(event,newpeak.bottom_after,newpeak.max_bin,newpeak.max,0.5,pmt) ;
				newpeak.fwhm = newpeak.end50 - newpeak.start50 ;
				// Get positions of 90 %.
				newpeak.start90 = GetXPercentHeight(event,newpeak.bottom_before,newpeak.max_bin,newpeak.max,0.9,pmt) ;
				newpeak.end90 = GetXPercentHeight(event,newpeak.bottom_after,newpeak.max_bin,newpeak.max,0.9,pmt) ;
				newpeak.trise = tInc * (newpeak.start90 - newpeak.start) ;
				newpeak.tfall = tInc * (newpeak.end - newpeak.end90) ;
				// Calculate the integral between start and end (0.1 * peakHeight).
				newpeak.integral = GetIntegral(event, newpeak.start, newpeak.end, pmt);
				newpeak.width = newpeak.end - newpeak.start ;
				//
				// Append peak
				peaks.push_back(newpeak) ;
				//cout << "++ The peak no. " << peaks.size() << " is at " 
				//	<< newpeak.center * tInc << " s." << endl ;
				newpeak.bottom_before = s ;	// To prevent crash in rare case this is start of new peak.
				// Reset also newpeak.end.
				newpeak.start = -101 ;
				newpeak.end = -100 ;
			}
		} else if (amp > threshold && previous < threshold && newpeak.start < 0)
			newpeak.start = s ;
		else if (amp < threshold && previous > threshold){
			newpeak.end = s ;
		}
		previous = amp ;
	}
	return peaks ;

}

// Return the drift time for events with only 2 peaks,
// so the difference between the first and the second peak.
double RawTree::GetDriftTime(int event, float sigmas, float sigmasreset, int pmt){
	GetEntry(event) ;
	double deltat = 0. ;
	double widthS2 = 28 ;
	vector<Peak> peaks = FindPeaks(event,sigmas,sigmasreset,pmt) ;
	int nPeaks = peaks.size() ;

	if (nPeaks == 0)
		//cout << "-- There's no peak here..." << endl ;
		return 0 ;
	else if (nPeaks == 1)
		//cout << "-- Only one peak here (width = " 
		//	<< peaks[1].width * tInc << " s), probably only S2..." << endl ;
		return 0 ;
	else if (nPeaks > 2)
		//cout << "-- More than just 2 peaks... Discard." << endl ;
		return 0 ;
	else	// So 2 peaks.
		if (peaks[0].width > peaks[1].width)
			//cout << "-- The wider peak is first. Does not compute!" << endl ;
			return 0 ;
		else {
			deltat = peaks[1].max_bin - peaks[0].max_bin ;
			// Changed this line from line below, because center is not defined.
			//deltat = peaks[1].center - peaks[0].center ;
			widthS2 = peaks[1].width ;
		}

	return deltat * tInc ;	// Returns in s.
}

// Return the drift time histogram.
TH2F* RawTree::GetDriftTimeHisto(float sigmas, float sigmasreset,  int pmt){
	TH2F* h1 = new TH2F("h1",";drift time (#mus);S2 width (#mus)",100,0,100,100,0,15) ;
	int entries = GetEntries() ;
	double deltat, widthS2 ;
	for (int i=0;i<entries;i++){
		if (i%10000==0) cout << "      " << i << "..." << endl ;
		vector<Peak> peaks = FindPeaks(i,sigmas,sigmasreset,pmt) ;
		if (peaks.size() == 2){
			deltat = peaks[1].max_bin - peaks[0].max_bin ;
			widthS2 = peaks[1].width ;
			if (deltat*tInc > 2e-6 && widthS2*tInc > 1e-6)
				cout << "   event " << i ;
			h1->Fill(deltat*tInc*1e6,widthS2*tInc*1e6) ;
		}
	}
	cout << endl ;

	return h1 ;
	delete h1 ;
}




//------- Pulse height, integral and electron spectra -----

// Return histogram of S1 peak times
TH1D* RawTree::GetS1PosHisto(int nbins, int nevents, int startbin, int stopbin, double threshold, int pmtid){
	TH1D* h1 = new TH1D("h1","S1 pos ;bin number; counts",nbins,startbin,stopbin) ;
	for(int i=0; i<nevents ; i++){
		h1->Fill(GetS1Pos(i,startbin,stopbin,threshold,pmtid));
		if(i%1000 == 0)
			cout << "Reading event " << i << " of " << nevents << " ..." << endl ;
	}
	return h1 ;
}

// Return histogram of pulse height spectrum.
TH1F* RawTree::GetAmpHisto(int nbins, double minamp, double maxamp, int pmt){
    TH1F *h1 = new TH1F("h1","Peak Amplitude; Peak voltage (V); counts",nbins,minamp,maxamp );
	int imax = GetEntries() ;
	// int imax = 1000 ; 
	for(int i=0 ; i<imax ; i++){
		GetEntry(i);
		if (pmt)
			h1->Fill(peakAmplitude1) ;
		else
			h1->Fill(peakAmplitude0) ;
		if(i%10000==0){
			cout << "++ Reading event "<< i << " of " << imax << " ..."<< endl ;
		}
	}
	//h1->Draw();
	return h1 ;
}

// Returns the bin of a local maximum (in a certain range).
int RawTree::GetMaxBin(int entry, int startbin, int stopbin, int pmt){
	GetEntry(entry);
	double maxval=-10;
	int maxbin=0;
	for(int i=startbin; i<stopbin; i++){
		// Bottom PMT
		if(pmt==0){		
			if(camplitude0[i]>maxval){
				maxval=camplitude0[i];
				maxbin=i;
			}
		}
		// Top PMT
		if(pmt==1){
			if(camplitude1[i]>maxval){
				maxval=camplitude1[i];
				maxbin=i;
			}
		}
		// Sum signal
		if(pmt==2){
			if( (camplitude0[i]+camplitude1[i])>maxval){
				maxval = camplitude0[i]+camplitude1[i];
				maxbin=i;
			}
		}
	}
	return maxbin;
}

// Returns position of 10 % the peak height (in a range).
int RawTree::GetXPercentHeight(int entry, int startbin, int stopbin, double max, double percentage, int pmt){
	GetEntry(entry) ;
	int pos = 0 ;
	int i ;
	if (startbin<stopbin){	// Rising edge of the peak.
		for (i=startbin;i<stopbin;i++){
			if (pmt==0)
				if(camplitude0[i]>percentage*max){
					pos = i ;
					return pos ;
				}
			if (pmt==1)
				if(camplitude1[i]>percentage*max){
					pos = i ;
					return pos ;
				}
		}
	} else {	// Falling edge of the peak.
		for (i=startbin;i>stopbin;i--){
			if(pmt==0)
				if(camplitude0[i]>percentage*max){
					pos = i ;
					return pos ;
				}
			if(pmt==1)
				if(camplitude1[i]>percentage*max){
					pos = i ;
					return pos ;
				}
		}
	}

	return 45 ;
}


// Calculate the integral (in Vs) of a baseline-corrected waveform
// between startbin and stopbin.
double RawTree::GetIntegral(int entry, int startbin, int stopbin,int pmt){
	double integral = 0 ;
	GetEntry(entry);
	for(int i=startbin ; i<stopbin ; i++){
		if (pmt)
			integral += camplitude1[i] ;
		else
			integral += camplitude0[i] ;
	}
	// Return integral in unit Vs.
	return integral * tInc ;	
}


// Return histogram of integral spectrum (corrected for baseline, but not pulse timing).
TH1F* RawTree::GetIntHisto(int nbins, double minint, double maxint, int startbin, int stopbin, int pmt){
	TH1F* h1= new TH1F("h1","Integral; Area (Vs) ; counts",nbins,minint,maxint ) ;
	int imax = GetEntries() ;
	for(int i=0 ; i<imax ; i++){
		GetEntry(i) ;
		h1->Fill(GetIntegral(i,startbin,stopbin,pmt));
		if(i%10000==0){
			cout << "++ Reading event "<< i << " of " << imax << " ..."<< endl ;
		}
	}
	return h1 ;
	delete h1;		 
}


// Return histogram of SPE spectrum, calibrated in number of electrons.
// Commented this out since we don't use all parameters. Don.t want to see 6 warnings at each compiling
//TH1F* RawTree::GetElecHisto(int nbins, double minint, double maxint, int startbin, int stopbin,
//		int lowerbinbdy, int upperbinbdy,const char* method, int pmt){
TH1F* RawTree::GetElecHisto(int nbins, double minint, double maxint){
	TH1F* elec = new TH1F("elec","SPE spectrum; Gain (#times10^{6}) ; counts",nbins,minint,maxint );
	int imax = GetEntries();
	double area = 0 ;
	double nofe = 0 ;	// Number of electrons.
	for(int i=0 ; i<imax ; i++){
	    GetEntry(i);
		// Cut on the minimum
		/*
		if(std::string(method)=="peak")
			if(peakBin < upperbinbdy && peakBin > lowerbinbdy)
				area = GetIntegral(i,startbin,stopbin) ;
		// Or: cut on half of the minimum
		if (std::string(method)=="half")
			if(GetHalfPeak(pmt)<upperbinbdy && GetHalfPeak(pmt) > lowerbinbdy)
				area = GetIntegral(i,startbin,stopbin) ;
		// */
		nofe = area / (50 * 1.602e-7) ;	// Area in Vs.
	    elec->Fill( nofe/1e6 );
	    if(i%10000==0)
			cout << "++ Reading event "<< i << " of " << imax << " ..."<< endl ;
	}
	return elec ;
	delete elec ;		 
}


