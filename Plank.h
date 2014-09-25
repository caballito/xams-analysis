//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jun 23 11:02:33 2014 by ROOT version 5.28/00b
// from TTree datatree/Processed data
// found on file: processed_data_RUN-3_MEAS-35.root
//////////////////////////////////////////////////////////

#ifndef Plank_h
#define Plank_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TFile.h>
#include <TMath.h>
#include <TH1.h>
#include <TH2.h>
#include <iostream>
#include "RawTree2.h"

class Plank {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Double_t        timeStep;
   Int_t           nPeaks1;
   Int_t           nPeaks2;
   vector<double>  *start1;
   vector<double>  *start501;
   vector<double>  *start901;
   vector<double>  *end1;
   vector<double>  *end501;
   vector<double>  *end901;
   vector<double>  *width1;
   vector<double>  *width_bottom1;
   vector<double>  *fwhm1;
   vector<double>  *max_pos1;
   vector<double>  *max1;
   vector<double>  *integral1;
   vector<double>  *trise1;
   vector<double>  *tfall1;
   vector<double>  *start2;
   vector<double>  *start502;
   vector<double>  *start902;
   vector<double>  *end2;
   vector<double>  *end502;
   vector<double>  *end902;
   vector<double>  *width2;
   vector<double>  *width_bottom2;
   vector<double>  *fwhm2;
   vector<double>  *max_pos2;
   vector<double>  *max2;
   vector<double>  *integral2;
   vector<double>  *trise2;
   vector<double>  *tfall2;

   // List of branches
   TBranch        *b_timeStep;   //!
   TBranch        *b_nPeaks1;   //!
   TBranch        *b_nPeaks2;   //!
   TBranch        *b_start1;   //!
   TBranch        *b_start501;   //!
   TBranch        *b_start901;   //!
   TBranch        *b_end1;   //!
   TBranch        *b_end501;   //!
   TBranch        *b_end901;   //!
   TBranch        *b_width1;   //!
   TBranch        *b_width_bottom1;   //!
   TBranch        *b_fwhm1;   //!
   TBranch        *b_max_pos1;   //!
   TBranch        *b_max1;   //!
   TBranch        *b_integral1;   //!
   TBranch        *b_trise1;   //!
   TBranch        *b_tfall1;   //!
   TBranch        *b_start2;   //!
   TBranch        *b_start502;   //!
   TBranch        *b_start902;   //!
   TBranch        *b_end2;   //!
   TBranch        *b_end502;   //!
   TBranch        *b_end902;   //!
   TBranch        *b_width2;   //!
   TBranch        *b_width_bottom2;   //!
   TBranch        *b_fwhm2;   //!
   TBranch        *b_max_pos2;   //!
   TBranch        *b_max2;   //!
   TBranch        *b_integral2;   //!
   TBranch        *b_trise2;   //!
   TBranch        *b_tfall2;   //!

	// Member functions.
	Plank(const char* path);
	virtual ~Plank();
	virtual Int_t    Cut(Long64_t entry);
	virtual Int_t    GetEntry(Long64_t entry);
	virtual Long64_t LoadTree(Long64_t entry);
	virtual void     Init(TTree *tree);
	virtual void     Loop();
	virtual Bool_t   Notify();
	virtual void     Show(Long64_t entry = -1);
	//
	virtual int		GetEntries();
	virtual int		GetAcceptedEntries();
	virtual int 	GetTrueNPeaks(int entry) ;
	virtual int 	GetTrueNPeaks(int entry, int pmt) ;
	virtual TH1D*	GetTrueNPeaksHisto();
	virtual int		CutS1(int entry, int peakn, int pmt);
	virtual int		CutS2(int entry, int peakn, int pmt);
	virtual int		CutEvent(int entry);
	//
	virtual double	GetDriftTime(int entry) ;
	virtual double	GetS2Width(int entry);
	virtual double	GetS1Integral(int entry);
	virtual double	GetS2Integral(int entry);
	virtual double	GetS2FWHM(int entry);
	virtual double	GetAsymmetry(int entry, int signal) ;
	//
	virtual void	PeakTrainer(RawTree* rt, int entry);
	//
	virtual TH1F* 	GetDriftTimeHisto(int nbins, double mintime, double maxtime);
	virtual TH1F* 	GetS2WidthHisto(int nbins, double minwidth, double maxwidth);
	//virtual TH1F* 	GetS1IntegralHisto(int nbins, double minint, double maxint);
	virtual TH1F* 	GetS1IntegralHisto(int nbins, double minint, double maxint);
	virtual TH1F* 	GetS2IntegralHisto(int nbins, double minint, double maxint);
	virtual TH1F* 	GetS2FWHMHisto(int nbins, double minwidth, double maxwidth);
	//
	virtual TH2F* 	GetS2WidthVDriftTime(int nbinstime, double mintime, double maxtime, int nbinswidth, double minwidth, double maxwidth);
	virtual TH2F* 	GetS2FWHMVDriftTime(int nbinstime, double mintime, double maxtime, int nbinswidth, double minwidth, double maxwidth);
	//
	virtual TH2F* 	GetS2IntegralVDriftTime(int nbinstime, double mintime, double maxtime, int nbinsint, double minint, double maxint);
	virtual TH2F* 	GetS2IntegralVFWHM(int nbinsfwhm, double minfwhm, double maxfwhm, int nbinsint, double minint, double maxint);
	virtual TH2F* 	GetS2IntegralVS1Integral(int nbinss1, double minints1, double maxints1, int nbinsints2, double minints2, double maxints2);
	virtual TH2F*	GetLogS2S1VS1(int nbins1, double minint, double maxint, int nbinslog, double minlog, double maxlog) ;
	//
	virtual TH2F*	GetAsymmetry12Histo(int nbins) ;
	virtual TH2F*	GetS1AsymmetryVDriftTime(int nbinstime, double mintime, double maxtime, int nbins) ;
	virtual TH2F*	GetS2Saturation(int nbins, double minint, double maxint);
};

#endif

#ifdef Plank_cxx
Plank::Plank(const char* path){	// Give path to ROOT file.
	// Create empty tree and get datatree from path.
	TTree* tree = 0 ;
	TFile* file = new TFile(path) ;

	if (file == 0) {
		std::cout << "WW Warning: datafile is empty!" << std::endl ;
	} else {
		tree = (TTree*)file->Get("datatree") ;
	}
	Init(tree);
}




Plank::~Plank(){
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}



Int_t Plank::GetEntry(Long64_t entry){
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t Plank::LoadTree(Long64_t entry){
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void Plank::Init(TTree *tree){
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   start1 = 0;
   start501 = 0;
   start901 = 0;
   end1 = 0;
   end501 = 0;
   end901 = 0;
   width1 = 0;
   width_bottom1 = 0;
   fwhm1 = 0;
   max_pos1 = 0;
   max1 = 0;
   integral1 = 0;
   trise1 = 0;
   tfall1 = 0;
   start2 = 0;
   start502 = 0;
   start902 = 0;
   end2 = 0;
   end502 = 0;
   end902 = 0;
   width2 = 0;
   width_bottom2 = 0;
   fwhm2 = 0;
   max_pos2 = 0;
   max2 = 0;
   integral2 = 0;
   trise2 = 0;
   tfall2 = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("timeStep", &timeStep, &b_timeStep);
   fChain->SetBranchAddress("nPeaks1", &nPeaks1, &b_nPeaks1);
   fChain->SetBranchAddress("nPeaks2", &nPeaks2, &b_nPeaks2);
   fChain->SetBranchAddress("start1", &start1, &b_start1);
   fChain->SetBranchAddress("start501", &start501, &b_start501);
   fChain->SetBranchAddress("start901", &start901, &b_start901);
   fChain->SetBranchAddress("end1", &end1, &b_end1);
   fChain->SetBranchAddress("end501", &end501, &b_end501);
   fChain->SetBranchAddress("end901", &end901, &b_end901);
   fChain->SetBranchAddress("width1", &width1, &b_width1);
   fChain->SetBranchAddress("width_bottom1", &width_bottom1, &b_width_bottom1);
   fChain->SetBranchAddress("fwhm1", &fwhm1, &b_fwhm1);
   fChain->SetBranchAddress("max_pos1", &max_pos1, &b_max_pos1);
   fChain->SetBranchAddress("max1", &max1, &b_max1);
   fChain->SetBranchAddress("integral1", &integral1, &b_integral1);
   fChain->SetBranchAddress("trise1", &trise1, &b_trise1);
   fChain->SetBranchAddress("tfall1", &tfall1, &b_tfall1);
   fChain->SetBranchAddress("start2", &start2, &b_start2);
   fChain->SetBranchAddress("start502", &start502, &b_start502);
   fChain->SetBranchAddress("start902", &start902, &b_start902);
   fChain->SetBranchAddress("end2", &end2, &b_end2);
   fChain->SetBranchAddress("end502", &end502, &b_end502);
   fChain->SetBranchAddress("end902", &end902, &b_end902);
   fChain->SetBranchAddress("width2", &width2, &b_width2);
   fChain->SetBranchAddress("width_bottom2", &width_bottom2, &b_width_bottom2);
   fChain->SetBranchAddress("fwhm2", &fwhm2, &b_fwhm2);
   fChain->SetBranchAddress("max_pos2", &max_pos2, &b_max_pos2);
   fChain->SetBranchAddress("max2", &max2, &b_max2);
   fChain->SetBranchAddress("integral2", &integral2, &b_integral2);
   fChain->SetBranchAddress("trise2", &trise2, &b_trise2);
   fChain->SetBranchAddress("tfall2", &tfall2, &b_tfall2);
   Notify();
}

Bool_t Plank::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Plank::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Plank::Cut(Long64_t entry)
{
	if (GetTrueNPeaks(entry) == 2)
		return 1 ;
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef Plank_cxx
