#define Plank_cxx
#include "Plank.h"
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <iomanip>
#include <TLine.h>

using namespace std ;

// Scale the area according to the gain
// At 800V: 4e6 for PMT1, 4e6 for PMT2
// At 850V: 12e6 for PMT1, 8e6 for PMT2
// At 900V: 24e6 for PMT1, 19e6 for PMT2
double gain1 = 4e6, gain2 = 4e6;
//double gain1 = 12e6, gain2 = 8e6;
//double gain1 = 24e6, gain2 = 19e6;


void Plank::Loop(){
//   In a ROOT session, you can do:
//      Root > .L Plank.C
//      Root > Plank t
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
   }
}

int Plank::GetEntries(){
	return fChain->GetEntriesFast();
}

int Plank::GetTrueNPeaks(int entry){
	// Returns the number of peaks found in a waveform
	// Applies a cut on too short peaks
	GetEntry(entry);
	int nTruePeaks1=GetTrueNPeaks(entry,1);
	int nTruePeaks2=GetTrueNPeaks(entry,2);
	// Return the maximum of the two counters.
	if (nTruePeaks1 > nTruePeaks2)
		return nTruePeaks1;
	else
		return nTruePeaks2;
}

int Plank::GetTrueNPeaks(int entry, int pmt){
	GetEntry(entry);
	int nTruePeaks = 0;
	if (pmt == 1){
	    for (int i=0; i<nPeaks1; i++){
			if (fwhm1[0][i] > 2.001*timeStep){
			nTruePeaks++;
			}
		}
	}
	if (pmt == 2){
		 for (int i=0; i<nPeaks2; i++){
	         if (fwhm2[0][i] > 2.001*timeStep){
	 	        nTruePeaks++;
		     }
		 }
	}
	return nTruePeaks;
}


// Returns a histogram of the number of peaks
TH1D* Plank::GetTrueNPeaksHisto(){
	TH1D* h1 = new TH1D("truePeaks","nPeaks histogram;nPeaks ;counts",4,0,4);
	int imax = GetEntries();
	for (int i=0; i<imax; i++){
		h1->Fill(GetTrueNPeaks(i));
	}
	return h1;
	delete h1;
}


double Plank::GetDriftTime(int entry){
	// Gets the drift time for the events with two peaks
	// First check if the event is accepted.
	//if ( CutEvent(entry) == -1 )	
	//	return 0 ;
	GetEntry(entry);
	// Get the position of the candidates 
	double s1pos1=0, s2pos1=0, s1pos2=0,s2pos2=0,s1pos=0,s2pos=0,dtime=0;
	
	// Loop over all peaks in PMT1 and check for S1 and S2 candidates
	for (int i=0; i<nPeaks1; i++){
		if (CutS1(entry,i,1)==1){
			s1pos1=max_pos1[0][i];
		} else if (CutS2(entry,i,1) == 1){ // 'else if' because one peak can't be both S1 and S2
			s2pos1=max_pos1[0][i];
		}
	}
	// Same for PMT2
 	for (int j=0; j<nPeaks2; j++){
		if (CutS1(entry,j,2)==1){
			s1pos2=max_pos2[0][j];
		} else if (CutS2(entry,j,2) == 1){
			s2pos2=max_pos2[0][j];
		}
	}
	// If there is no S1 or S2 in one of the signals, return 0
	if(s1pos1==0 || s1pos2==0 || s2pos1==0 || s2pos2==0)
		return 0;
	s1pos = (s1pos1+s1pos2)/2.;
	s2pos = (s2pos1+s2pos2)/2.;
	// Final check: both positions must be nonzero. 
	if (s1pos==0 || s2pos==0)
		cout<<"Error in GetDriftTime function at event "<<entry<<"." << endl<< "Event accepted, but either S1 or S2 haven't been found!"<<endl;

	dtime = s2pos - s1pos;
	return dtime;
}

TH1F* Plank::GetDriftTimeHisto(int nbins, double mintime, double maxtime){
	TH1F* h1 = new TH1F("driftTime","Drift times;drift time (#mus);counts",nbins,mintime,maxtime);
	int imax = GetEntries();
	double dtime=0;
	int cutevent=0;
	for (int i=0; i<imax; i++){
		dtime = GetDriftTime(i);
		cutevent = CutEvent(i); 
		// Only fill if the event is accepted
		if (dtime != 0 && cutevent==1 )
			h1->Fill(dtime*1.e6);
	}
	return h1;
	delete h1;
}


// Looks at a peak and tells you if it is a viable S1 candidate or not
// Returns 1 for a good S1, -1 for a bad S1
int Plank::CutS1(int entry,int peakn,int pmt){
	GetEntry(entry);
	// Parameters for a good S1
	// FWHM: between widthlow and widthhigh
	double widthlow  = 10e-9; 
	double widthhigh = 100e-9; 
	// Rise time: less than trisehigh
	// Commented this out because of rise time problems...
	//double trisehigh = 10e-9;
	if (pmt == 1){
		if (widthlow<fwhm1[0][peakn] && fwhm1[0][peakn]<widthhigh)// && trise1[0][peakn]<trisehigh)
			return 1;
		else
			return -1;
	}
	if (pmt == 2){
		if (widthlow<fwhm2[0][peakn] && fwhm2[0][peakn]<widthhigh)// && trise2[0][peakn]<trisehigh)
			return 1;
		else
			return -1;
		}
	cout << "Error in S1 cut finder. Check syntax?"<< endl;
	return 0;
}


// Looks at a peak and tells you if it is a viable S2 candidate or not
// Returns 1 for a good S2, -1 for a bad S2
int Plank::CutS2(int entry,int peakn,int pmt){
	GetEntry(entry);
	// Parameters for a good S2
	// width: between widthlow and widthhigh
	double widthlow  = 1e-6;
	double widthhigh = 10e-6;
	
	if (pmt == 1){
    	if (widthlow<fwhm1[0][peakn] && fwhm1[0][peakn]<widthhigh)
     		return 1;
        else
            return -1;
        }
    if (pmt == 2){
    	if (widthlow<fwhm2[0][peakn] && fwhm2[0][peakn]<widthhigh)
            return 1;
        else
            return -1;
    }
    cout << "Error in S2 cut finder. Check syntax?"<< endl;
    return 0;
}

// Checks if an event looks nice
// Returns 1 for a good event, -1 for a bad event
// Cut on: ...
int Plank::CutEvent(int entry){
	GetEntry(entry);
	// Initialize counters for number of S1 and S2 candidates
	int ns1candidates1=0, ns2candidates1=0,ns1candidates2=0,ns2candidates2=0;
	// Get the position of the candidates 
	double s1pos1=0, s2pos1=0, s1pos2=0,s2pos2=0;
	
	// Loop over all peaks in PMT1 and check for S1 and S2 candidates
	for (int i=0; i<nPeaks1; i++){
		if (CutS1(entry,i,1)==1){
			ns1candidates1++;
			s1pos1=max_pos1[0][i];
		} else if (CutS2(entry,i,1) == 1){ // 'else if' because one peak can't be both S1 and S2
			ns2candidates1++;
			s2pos1=max_pos1[0][i];
		}
	}
	// Same for PMT2
 	for (int j=0; j<nPeaks2; j++){
		if (CutS1(entry,j,2)==1){
			ns1candidates2++;
			s1pos2=max_pos2[0][j];
		} else if (CutS2(entry,j,2) == 1){
			ns2candidates2++;
			s2pos2=max_pos2[0][j];
		}
	}

	// temp
	if(GetTrueNPeaks(entry)!=2)
		return -1;

	
	// If there is no S1 candidate in one of the PMTs: bad event
	if (ns1candidates1==0 || ns1candidates2==0)
		return -1;
	// If there is no S2 candidate in one of the PMTs: bad event
	if (ns2candidates1==0 || ns2candidates2==0)
		return -1;
	// If any of the counters is larger than 1, multiple S1s/S2s: bad event
	if (ns1candidates1>1 || ns2candidates1>1 || ns1candidates2>1 || ns2candidates2>1)
		return -1;
	// If there are S1s in both channels, they must be close to each other. Otherwise: bad event
	// fabs is a float version of absolute value
	if (ns1candidates1==1 && ns1candidates2==1 && fabs(s1pos1-s1pos2)>100e-9)
		return -1;
	// Same for S2
	if (ns2candidates1==1 && ns2candidates2==1 && fabs(s2pos1-s2pos2)>5000e-9)
		return -1;
	// If the S2 is before the S1: bad event
	if( (max(s2pos1,s2pos2) - max(s1pos1,s1pos2))<0)
		return -1;


	
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// This piece: additional cuts to keep only REALLY nice events instead of just sensible events.
	// All plots should be analyzed before applying these cuts.
	//if(



	
	// If both contain an S2, both counters should be 1
	//if(ns2candidates1!=ns2candidates2)
	//	return -1;

	// Cut on events: only with S1 area between 55 and 60 nVs
	// Select mono-energetic events
	//if( GetS1Integral(entry) < 25e-9 || GetS1Integral(entry) > 30e-9)
	//	return -1;


	// If this point is reached, that means that
	// 	- There is an accepted S1 in both channels
	// 	- There is an accepted S2 in both channels
	// 	- If there is an S1 in both channels, they occur simultaneously
	// 	- If there is an S2 in both channels, they occur simultaneously
	// Accept the event!
	return 1;
}

int Plank::GetAcceptedEntries(){
	int nacc = 0;
	for(int i=0; i<GetEntries(); i++){
		if (CutEvent(i) == 1)
			nacc++;
	}
	return nacc;
}

double Plank::GetS2FWHM(int entry){
	GetEntry(entry);
	double s2width1=0, s2width2=0, s2width=0;
	// Check if it's a real event
	//if (CutEvent(entry) == -1)
	//	return 0;
	// Look for the S2 and get the width
	for (int i=0; i<nPeaks1; i++){
		if ( CutS2(entry,i,1) == 1)
			s2width1=fwhm1[0][i];
	}
	for (int i=0; i<nPeaks2; i++){
		if ( CutS2(entry,i,2) == 1)
			s2width2=fwhm2[0][i];
	}
	// If the width is not defined for one of the PMTs: bad event and return 0
	// Else give the averae
	// Else take the only nonzero value
	if (s2width1==0 || s2width2==0)
		return 0;
	s2width = (s2width1+s2width2)/2.;
	return s2width;
}


double Plank::GetS1Integral(int entry){
	GetEntry(entry); 
	double s1int1=0, s1int2=0, s1int=0;
	// Check if it's a real event
	//if (CutEvent(entry) == -1)
	//	return 0;
	// Look for the S1 and get the integral
	for (int i=0; i<nPeaks1; i++){
		if ( CutS1(entry,i,1) == 1)
			s1int1=integral1[0][i];
	}
	for (int i=0; i<nPeaks2; i++){
		if ( CutS1(entry,i,2) == 1)
			s1int2=integral2[0][i];
	}
	// If one of the areas returns 0: no peak in one of the signals and bad event
	if (s1int1==0 || s1int2==0){
		return 0;
	}
	
	s1int1 = s1int1/(1.6022e-19*50*gain1) ;
	s1int2 = s1int2/(1.6022e-19*50*gain2) ;
	s1int  = s1int1 + s1int2;
	// Return the summed area in p.e.
	return s1int;
}


double Plank::GetS2Integral(int entry){
	GetEntry(entry);
	double s2int1=0, s2int2=0, s2int=0;
	// Check if it's a real event
	//if (CutEvent(entry) == -1)
	//	return 0;
	// Look for the S2 and get the integral
	for (int i=0; i<nPeaks1; i++){
		if ( CutS2(entry,i,1) == 1)
			s2int1=integral1[0][i];
	}
	for (int i=0; i<nPeaks2; i++){
		if ( CutS2(entry,i,2) == 1)
			s2int2=integral2[0][i];
	}
	// If one of the areas returns 0: no peak in one of the signals and bad event
	if (s2int1==0 || s2int2==0){
		return 0;
	}
	
	s2int1 = s2int1/(1.6022e-19*50*gain1) ;
	s2int2 = s2int2/(1.6022e-19*50*gain2) ;
	s2int  = s2int1 + s2int2;
	// Return the summed area in p.e.
	return s2int;
}


double Plank::GetS2Width(int entry){
	GetEntry(entry);
	double s2width1=0, s2width2=0, s2width=0;
	// Check if it's a real event
	//if (CutEvent(entry) == -1)
	//	return 0;
	// Look for the S2 and get the width
	for (int i=0; i<nPeaks1; i++){
		if ( CutS2(entry,i,1) == 1)
			s2width1=width1[0][i];
	}
	for (int i=0; i<nPeaks2; i++){
		if ( CutS2(entry,i,2) == 1)
			s2width2=width2[0][i];
	}
	// One of the peaks not found: bad event 
	if (s2width1==0 || s2width2==0)
		return 0;
	s2width = (s2width1+s2width2)/2.;
	return s2width;
}


double Plank::GetAsymmetry(int entry, int signal){
	GetEntry(entry) ;
	double asy = 0, a1 = 0, a2 = 0 ;
	// Check if it's a real event
	//if (CutEvent(entry) == -1)
	//	return 1.5 ;
	if (signal == 1){
		// Look for the S1 and get the integral
		for (int i=0; i<nPeaks1; i++){
			if ( CutS1(entry,i,1) == 1)
				a1=integral1[0][i];
		}
		for (int i=0; i<nPeaks2; i++){
			if ( CutS1(entry,i,2) == 1)
				a2=integral2[0][i];
		}
	} else if (signal == 2){
		// Look for the S2 and get the integral
		for (int i=0; i<nPeaks1; i++){
			if ( CutS2(entry,i,1) == 1)
				a1=integral1[0][i];
		}
		for (int i=0; i<nPeaks2; i++){
			if ( CutS2(entry,i,2) == 1)
				a2=integral2[0][i];
		}
	}
	// If the integral is not defined for a PMT, return 0
	if (a1==0 || a2==0){
		return 0 ;
	}
	// Convert area in pVs to area in p.e. including the gain
	a1 = a1/(1.6022e-19*50*gain1) ;
	a2 = a2/(1.6022e-19*50*gain2) ;
	asy = (a2-a1) / (a2+a1) ;
	return asy ;
}


/******************
 * Get histograms *
 ******************/


TH1F* Plank::GetS1IntegralHisto(int nbins, double minint, double maxint){
	TH1F* h1 = new TH1F("s1Integral","S1 area distribution;S1 area (p.e.);counts",nbins,minint,maxint);
	int imax = GetEntries();
	double s1int = 0;
	for (int i=0; i<imax; i++){
		s1int=GetS1Integral(i);
		if (CutEvent(i) == 1) //Only fill if the event is accepted
			h1->Fill(s1int);
	}
	return h1;
	delete h1;
}

TH1F* Plank::GetS2WidthHisto(int nbins, double minwidth, double maxwidth){
	TH1F* h1 = new TH1F("s2Width","S2 width distribution;S2 width (#mus);counts",nbins,minwidth,maxwidth);
	int imax = GetEntries();
	double s2width = 0;
	for (int i=0; i<imax; i++){
		s2width=GetS2Width(i);
		if (s2width != 0)
			h1->Fill(s2width*1.e6);
	}
	return h1;
	delete h1;
}


TH1F* Plank::GetS2IntegralHisto(int nbins, double minint, double maxint){
	TH1F* h1 = new TH1F("s2Integral","S2 area distribution;S2 area (p.e.);counts",nbins,minint,maxint);
	int imax = GetEntries();
	double s2int = 0;
	for (int i=0; i<imax; i++){
		s2int=GetS2Integral(i);
		if (s2int != 0) //zero is returned if the event is not accepted
			h1->Fill(s2int);
	}
	return h1;
	delete h1;
}

TH1F* Plank::GetS2FWHMHisto(int nbins, double minwidth, double maxwidth){
	TH1F* h1 = new TH1F("s2Fwhm","S2 FWHM distribution;S2 FWHM (#mus);counts",nbins,minwidth,maxwidth);
	int imax = GetEntries();
	double s2width = 0;
	for (int i=0; i<imax; i++){
		s2width=GetS2FWHM(i);
		if (s2width != 0)
			h1->Fill(s2width*1.e6);
	}
	return h1;
	delete h1;
}

TH2F* Plank::GetS2WidthVDriftTime(int nbinstime, double mintime, double maxtime, int nbinswidth, double minwidth,double maxwidth){
	TH2F* h1 = new TH2F("s2WidthVDriftTime","S2 broadening;drift time (#mus);S2 width (#mus)",nbinstime,mintime,maxtime,nbinswidth,minwidth,maxwidth);
	int imax = GetEntries();
	double dtime, s2width;
	for(int i=0; i<imax; i++){
		dtime=GetDriftTime(i);
		s2width=GetS2Width(i);
		if(dtime !=0 && s2width != 0)
			h1->Fill(dtime*1.e6,s2width*1.e6);
	}
	return h1;
	delete h1;
}


TH2F* Plank::GetS2IntegralVDriftTime(int nbinstime, double mintime, double maxtime, int nbinsint, double minint,double maxint){
	TH2F* h1 = new TH2F("s2IntVDriftTime",";drift time (#mus);S2 area (p.e.)",nbinstime,mintime,maxtime,nbinsint,minint,maxint);
	int imax = GetEntries();
	double dtime, s2int;
	for(int i=0; i<imax; i++){
		dtime=GetDriftTime(i);
		s2int=GetS2Integral(i);
		if(CutEvent(i)==1)
			h1->Fill(dtime*1.e6,s2int);
	}
	return h1;
	delete h1;
}


TH2F* Plank::GetS2IntegralVFWHM(int nbinsfwhm, double minfwhm, double maxfwhm, int nbinsint, double minint,double maxint){
	TH2F* h1 = new TH2F("s2IntVs2Fwhm",";S2 FWHM (#mus);S2 area (p.e.)",nbinsfwhm,minfwhm,maxfwhm,nbinsint,minint,maxint);
	int imax = GetEntries();
	double fwhm, s2int;
	for(int i=0; i<imax; i++){
		fwhm=GetS2FWHM(i);
		s2int=GetS2Integral(i);
		if(fwhm !=0 && s2int != 0)
			h1->Fill(fwhm*1.e6,s2int);
	}
	return h1;
	delete h1;
}

TH2F* Plank::GetS2FWHMVDriftTime(int nbinstime, double mintime, double maxtime, int nbinswidth, double minwidth,double maxwidth){
	TH2F* h1 = new TH2F("s2FwhmVDriftTime","S2 broadening;drift time (#mus);S2 FWHM (#mus)",nbinstime,mintime,maxtime,nbinswidth,minwidth,maxwidth);
	int imax = GetEntries();
	double dtime, s2width;
	for(int i=0; i<imax; i++){
		dtime=GetDriftTime(i);
		s2width=GetS2FWHM(i);
		if(dtime !=0 && s2width != 0)
			h1->Fill(dtime*1.e6,s2width*1.e6);
	}
	return h1 ;
	delete h1 ;
}


TH2F* Plank::GetS2IntegralVS1Integral(int nbinss1, double minints1, double maxints1, int nbinss2, double minints2,double maxints2){
	TH2F* s2s1Int = new TH2F("s2IntegralVS1Integral","S1 and S2;S1 area (p.e.);S2 area (p.e.)",nbinss1,minints1,maxints1,nbinss2,minints2,maxints2);
	int imax = GetEntries(); 
	double s1int, s2int;
	for( int i=0; i<imax; i++){
		s1int=GetS1Integral(i);
		s2int=GetS2Integral(i);
		if(s1int!=0 && s2int!=0)
			s2s1Int->Fill(s1int,s2int);
	}
	return s2s1Int ;
	delete s2s1Int ;
}


TH2F* Plank::GetLogS2S1VS1(int nbins1, double minint, double maxint, int nbinslog, double minlog, double maxlog){
	TH2F* logplot = new TH2F("logs2s1",";S1 area (p.e.);Log_{10}(S2/S1)",nbins1,minint,maxint,nbinslog,minlog,maxlog) ;
	int imax = GetEntries() ;
	double s1int, s2int, timber ;
	for (int i=0;i<imax;i++){
		s1int = GetS1Integral(i) ;
		s2int = GetS2Integral(i) ;
		if (s1int!=0 && s2int!=0){
			timber = TMath::Log10(s2int/s1int) ;
			logplot->Fill(s1int,timber) ;
		}
	}
	return logplot ;
	delete logplot ;
}


TH2F* Plank::GetAsymmetry12Histo(int nbins){
	TH2F* asym = new TH2F("s1s2Asymmetry",";asymmetry S1;asymmetry S2",nbins,-1.,1.,nbins,-1.,1.) ;
	int imax = GetEntries() ;
	double a1, a2 ;
	for (int i=0; i<imax; i++){
		a1 = GetAsymmetry(i,1) ;
		a2 = GetAsymmetry(i,2) ;
		//if (-1.1 < a1 && a1 < 1.1 && -1.1 < a2 && a2 < 1.1)
		if (a1!=0 && a2!=0)
			asym->Fill(a1,a2) ;
	}
	return asym ;
	delete asym ;
}


TH2F* Plank::GetS1AsymmetryVDriftTime(int nbinstime, double mintime, double maxtime, int nbins){
	TH2F* asytime = new TH2F("s1AsymmetryVDriftTime",";drift time (#mus);S1 asymmetry",nbinstime,mintime,maxtime,nbins,-1.,1.) ;
	int imax = GetEntries() ;
	double time, a ;
	for (int i=0; i<imax;i++){
		time = GetDriftTime(i) ;
		a = GetAsymmetry(i,1) ;	// Get S1 asymmetry.
		if (time != 0 && a!=0 )
			asytime->Fill(time*1.e6,a) ;
	}
	return asytime ;
	delete asytime ;
}
	
	
void Plank::PeakTrainer(RawTree* rt, int entry){
	rt->PlotEvent(entry);
	GetEntry(entry);
	// Get lines to show the thresholds
	rt->GetEntry(entry); 
	double base1 = rt->baselineSpread0;
	double base2 = rt->baselineSpread1;
	TLine* upper1 = new TLine(0,base1*10,100,base1*10);
	TLine* upper2 = new TLine(0,base2*10,100,base2*10);
	upper1->SetLineStyle(kDashed);
	upper2->SetLineStyle(kDashed);
	upper1->SetLineColor(kBlack);
	upper2->SetLineColor(kRed);
	upper1->Draw("same");
	upper2->Draw("same");
	TLine* lower1 = new TLine(0,base1*(-2),100,base1*(-2));
	TLine* lower2 = new TLine(0,base2*(-2),100,base2*(-2));
	lower1->SetLineStyle(kDotted);
	lower2->SetLineStyle(kDotted);
	lower1->SetLineColor(kBlack);
	lower2->SetLineColor(kRed);
	lower1->Draw("same");
	lower2->Draw("same");

	// Start the output text
	cout<<setw(20)<<left<<"Event number:"<< entry<<endl;
	cout<<"==General=============="<<endl;
	cout<<setw(20)<<left<<"Accepted:" ;
	if (CutEvent(entry) ==1)
		cout<< "Yes" <<endl;
	else
		cout<< "No" <<endl;
	cout<<setw(20)<<left<<"Drift time:"<<GetDriftTime(entry)*1e6<<" us"<<endl; 
	cout<<setw(20)<<left<<"Baseln. spread PMT1:"<<base1*1e3 << " mV" <<endl;
	cout<<setw(20)<<left<<"Baseln. spread PMT2:"<<base2*1e3 << " mV" <<endl;
	cout<<"==Peak Properties======"<<endl;
	cout<<setw(20)<<left<<"nPeaks:"<<setw(10)<<left<<nPeaks1<<setw(10)<<left<<nPeaks2<<endl;
	for(int i=0; i<nPeaks1;i++){
		cout<<setw(20)<<left<<"Peak number:"<<i<<endl;
		cout<<setw(20)<<left<<"Peak ID:";
		if (CutS1(entry,i,1) ==1)
			cout<<"S1"<<endl;
		else if (CutS2(entry,i,1)==1)
			cout<<"S2"<<endl;
		else
			cout<<"Undefined"<<endl;
		cout<<setw(20)<<left<<"Max position:"<<setw(10)<<left<<max_pos1[0][i]*1e6 << " us"<<endl;
		cout<<setw(20)<<left<<"Central position:"<<setw(10)<<left<<(start1[0][i]+end1[0][i])*0.5*1e6 << " us"<<endl;
		cout<<setw(20)<<left<<"Width:"<<setw(10)<<left<<width1[0][i]*1e6 << " us"<<endl;
		cout<<setw(20)<<left<<"FWHM:"<<setw(10)<<left<<fwhm1[0][i]*1e6 << " us"<<endl;
		cout<<setw(20)<<left<<"Integral:"<<setw(10)<<left<<integral1[0][i]/200e-12 << " p.e."<<endl;
		cout<<"-------------"<<endl ;
	}
	cout<<"------------------------------------------"<<endl ;
	for(int i=0; i<nPeaks2;i++){
		cout<<setw(20)<<left<<"Peak number:"<<i<<endl;
		cout<<setw(20)<<left<<"Peak ID:";
		if (CutS1(entry,i,2) ==1)
			cout<<"S1"<<endl;
		else if (CutS2(entry,i,2)==1)
			cout<<"S2"<<endl;
		else
			cout<<"Undefined"<<endl;
		cout<<setw(20)<<left<<"Max position:"<<setw(10)<<left<<max_pos2[0][i]*1e6 << " us"<<endl;
		cout<<setw(20)<<left<<"Central position:"<<setw(10)<<left<<(start2[0][i]+end2[0][i])*0.5*1e6 << " us"<<endl;
		cout<<setw(20)<<left<<"Width:"<<setw(10)<<left<<width2[0][i]*1e6 << " us"<<endl;
		cout<<setw(20)<<left<<"FWHM:"<<setw(10)<<left<<fwhm2[0][i]*1e6 << " us"<<endl;
		cout<<setw(20)<<left<<"Integral:"<<setw(10)<<left<<integral2[0][i]/200e-12 << " p.e."<<endl;
		cout<<"-------------"<<endl ;
	}
	return ;
}


TH2F* Plank::GetS2Saturation(int nbins, double minint, double maxint){
	int imax=GetEntries();
	TH2F* s2Saturation = new TH2F("s2Saturation","S2 saturation;S2 PMT1 (p.e.);S2 PMT2 (p.e.)",nbins,minint,maxint,nbins,minint,maxint);
		
	double s2int1=0, s2int2=0;
	for (int j=0;j<imax;j++){
		if (CutEvent(j) ==1){
			// Look for the S2 and get the integral
			for (int i=0; i<nPeaks1; i++){
				if ( CutS2(j,i,1) == 1)
					s2int1=integral1[0][i];
			}
			for (int i=0; i<nPeaks2; i++){
				if ( CutS2(j,i,2) == 1)
					s2int2=integral2[0][i];
			}

			// Since we require the event to be real, both integrals should be nonzero
			if (s2int1==0 || s2int2==0){
				cout << "EE Only one S2 found in an accepted event!?" << endl ;
			}
	
			s2int1 = s2int1/(1.6022e-19*50*gain1) ;
			s2int2 = s2int2/(1.6022e-19*50*gain2) ;
			// Return the summed area in p.e.
			s2Saturation->Fill(s2int1,s2int2);
		}
	}
	return s2Saturation ;
}


