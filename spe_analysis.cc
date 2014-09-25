/*****************************************
 * Analysis of the raw data tree of high *
 * statistics data during initial PMT    *
 * characterization measurements.        *
 * Uses RawTree class.                   *
 *              E. Hogenbirk & R. Schoen *
 *                           -- Mar 2014 *
 *****************************************/

#include "RawTree.h"
#include <TH2.h>
#include <TH1.h>
#include <TF1.h>
#include <TLine.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

using namespace std ;

void getTimeLimits(TH1D* timeHist, double cut, int& lowerLimit, int& upperLimit) ;


int mainanalysis(const char* inputtree,const char* outputFile){
	cout << "++ Checking the data in " << inputtree << endl ;
	RawTree roar = RawTree(inputtree) ;
	cout << "++ Found " << roar.GetEntries() << " waveforms." << endl ;

	TFile* output = new TFile(outputFile,"recreate") ;
	cout << "++ Will write results into " << outputFile << endl ;

	// Relevant parameters.
	double cutvalue = 0. ; // Cut at 20% of peak height in peak time spectrum.
    int nTimeBins = 500 ;
	int lowercut, uppercut ;

	// Get the distribution of times where the peak occurs
    // (based on half the height of the peak).
	cout << "++ Checking the timing of the waveform peaks..." << endl ;
    TH1D* times = roar.GetPeaktimeHisto(nTimeBins,1,500,"half") ;
	// Get the limits for when the waveform peak has the right timing.
	getTimeLimits(times, cutvalue, lowercut, uppercut) ;

    //-------- Integral spectrum ------
	//
	// Apply these limits on the integration of the waveforms
	// for the single photoelectron spectrum.
	TH1F* speInt = roar.GetIntHistoCut(500, -900., 100., 250, 440, lowercut, uppercut, "half") ;
    // Find valley approximately.
    int valley = 66 ;
    int bin = speInt->GetMaximumBin() ;
    while ( speInt->GetBinContent(bin-1) < speInt->GetBinContent(bin) ){
        valley = bin-1 ;
        bin-- ;
    }
	cout << "++ Valley of SPE spectrum around bin " << valley << endl ;
	// Find spe peak approximately.
	int peak = 33 ;
	int max = speInt->GetBinContent(valley) ;
	for (bin=valley-1; bin>0; bin--){
		if ( speInt->GetBinContent(bin) > max ){
			peak = bin ;
			max = speInt->GetBinContent(bin) ;
		}
	}
	cout << endl << "++ Peak is probably around bin " << peak << endl ;
    // Define fit functions.
    // Gaussian for noise.
    TF1* noise = new TF1("noise","gaus") ;
    noise->SetParameter(1,0.) ; // Set mean to zero.
	noise->SetLineColor(kRed) ;
	//
	TF1* segaus = new TF1("segaus","gaus") ;
	segaus->SetParameter(1,peak) ;
	segaus->SetParameter(2,50) ;
	segaus->SetLineColor(kViolet) ;
	//
	cout << endl << "++ Fitting a Gaussian to spe spectrum noise:" << endl ;
	speInt->Fit(noise,"Q","",valley,50) ;
	cout << endl << "++ Fitting a Gaussian to spe spectrum:" << endl ;
	speInt->Fit(segaus,"Q","",-800,valley) ;
	//
    /*TF1* sevis = new TF1("sevis","pow([0],x)*exp(-[0])/TMath::Factorial(x)") ;
	sevis->SetParameter(0,segaus->GetParameter(1)) ;
	sevis->SetLineColor(kBlue) ;
	cout << "++ Fitting a Poisson to spe spectrum:" << endl ;
	speInt->Fit(sevis,"","",-800,valley) ;
*/
	speInt->Draw() ;
	noise->Draw("same") ;
	segaus->Draw("same") ;
	//
	speInt->Write() ;
	// Calculate gain in # of electrons from peak of spe spectrum.
	double gain = segaus->GetParameter(1) / (-50. * 1.602e-07) ;
	cout << "++ The gain is " << gain << " electrons." << endl ;

    //-------- Electron spectrum ------
	//
	// Apply the same limits on the integration of the waveforms
	// for the real single photoelectron spectrum (in no. of electrons).
	TH1F* speElec = roar.GetElecHisto(500, -5., 95., 250, 440, lowercut, uppercut, "half") ;
    // Find valley approximately.
    valley = 66 ;
    bin = speElec->GetMaximumBin() ;
    while ( speElec->GetBinContent(bin) > speElec->GetBinContent(bin+1) ){
        valley = bin+1 ;
        bin++ ;
    }
	cout << "++ Valley of SPE spectrum around bin " << valley << endl ;
	// Find spe peak approximately.
	peak = 33 ;
	max = speElec->GetBinContent(valley) ;
	for (bin=valley+1; bin<speElec->GetNbinsX(); bin++){
		if ( speElec->GetBinContent(bin) > max ){
			peak = bin ;
			max = speElec->GetBinContent(bin) ;
		}
	}
	cout << endl << "++ Peak is probably around bin " << peak << endl ;
	//
	//
	//
    // Define fit functions.
    // Gaussian for noise.
    //TF1* noise = new TF1("noise","gaus") ;
    noise->SetParameter(1,0.) ; // Set mean to zero.
	//noise->SetLineColor(kRed) ;
	//
	//TF1* segaus = new TF1("segaus","gaus") ;
	segaus->SetParameter(1,peak) ;
	segaus->SetParameter(2,5) ;
	//segaus->SetLineColor(kViolet) ;
	//
	cout << endl << "++ Fitting a Gaussian to spe spectrum noise:" << endl ;
	speElec->Fit(noise,"Q","",-1,valley) ;
	cout << endl << "++ Fitting a Gaussian to spe spectrum:" << endl ;
	speElec->Fit(segaus,"Q","",valley) ;
	//
    /*TF1* sevis = new TF1("sevis","pow([0],x)*exp(-[0])/TMath::Factorial(x)") ;
	sevis->SetParameter(0,segaus->GetParameter(1)) ;
	sevis->SetLineColor(kBlue) ;
	cout << "++ Fitting a Poisson to spe spectrum:" << endl ;
	speInt->Fit(sevis,"","",-800,valley) ;
*/
	speElec->Draw() ;
	noise->Draw("same") ;
	segaus->Draw("same") ;
	//
	speElec->Write() ;
	// Calculate gain in # of electrons from peak of spe spectrum.
	gain = segaus->GetParameter(1) * 1e6 ;
	cout << "++ The gain is " << gain << " electrons." << endl ;
	output->Close() ;
	
	return 42 ;
}



void getTimeLimits(TH1D* peakTimes, double cutvalue, int& lowercut, int& uppercut){
	// Help parameters.	
    double sum = 0 ;
    int bin = 0 ;
	int nTimeBins = peakTimes->GetNbinsX() ;
	
	// Get baseline of peakTime histogram.
    for (bin=1; bin < nTimeBins/2+1; bin++){
	    sum += peakTimes->GetBinContent(bin);
    }
    double timesbaseline = 2.*sum/nTimeBins ;   // Average over number of bins.
    cout<< "++ Baseline in peak time histogram: " << timesbaseline << endl ;

	// Correct for baseline.
	double bincontent = 0 ;
	for (bin=1; bin<nTimeBins+1; bin++){
	    bincontent = peakTimes->GetBinContent(bin);
	    peakTimes->SetBinContent(bin,bincontent-timesbaseline);
	}
	
	// Incidentally, estimate the number of actual events!
	double neventstotal=0;
    for (bin=nTimeBins/2.; bin<350; bin++){
        neventstotal += peakTimes->GetBinContent(bin);
    }
	cout<< "++ Estimated number of real events: " << neventstotal << endl;


	// Now find the boundaries for the time cut.
	double timesmax = peakTimes->GetMaximum() ;	// Get maximum.
    int startbin = peakTimes->GetMaximumBin() ;	// Get maximum position.
    double cutoff = timesmax * cutvalue ;	// Cut height depending on peak height.
    // Find lower limit on left side of peak.
    bin = startbin;
    while( peakTimes->GetBinContent(bin)>cutoff ){
        bin-- ;
    }
    lowercut = bin ;
    // Now on right side of peak.
    bin = startbin ;	// Reset start bin.
    while( peakTimes->GetBinContent(bin) > cutoff ){
        bin++ ;
    }
    uppercut = bin ;
    cout << "++ Place cut at " << 100 * cutvalue << "\% of maximum:" << endl ;
    cout << "   That's at bin " << lowercut << " and " << uppercut << endl ;
	//
	TLine lower(lowercut,0,lowercut,timesmax) ;
	lower.SetLineColor(kViolet) ;
	TLine upper(uppercut,0,uppercut,timesmax) ;
	upper.SetLineColor(kViolet) ;
	peakTimes->Draw() ;
	lower.Draw("same") ;
	upper.Draw("same") ;

	peakTimes->Write() ;

	/*char dummy ;
	cout << "?? Should we continue? (y/n)  " ;
	cin >> dummy ;
	cout << endl ;
*/
	return ;
}



