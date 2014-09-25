//////////////////////////////////////////////////////////
// This class is a copy of the one that has been automatically 
// generated on Mon Feb 24 11:10:55 2014 by ROOT version 5.28/00b
// from TTree tree/Waveform collection found on file: 
// /data/xenon/xams/pmt/PMT_intitial_characterisation/pmt1rc270krd47ra47c100nlon2V7-histat/raw_data_800V.root
//////////////////////////////////////////////////////////

#ifndef RawTree_h
#define RawTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <iostream>
#include <vector>

using namespace std ;

class RawTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types.
   Int_t           nSamples;
   Int_t           sample;
   Float_t         tInc;
   // Variables for PMT1.
   //Float_t         amplitude0[500000];   //[sample]
   Float_t         camplitude0[500000];   //[sample]
   Float_t         baseline0 ;
   Float_t         baselineSpread0 ;
   Float_t         peakAmplitude0;
   Int_t           peakBin0;
   // Variables for PMT2.
   //Float_t         amplitude1[500000];   //[sample]
   Float_t         camplitude1[500000];   //[sample]
   Float_t         baseline1;
   Float_t         baselineSpread1;
   Float_t         peakAmplitude1;
   Int_t           peakBin1;

   // List of branches.
   TBranch        *b_nSamples;   //!
   TBranch        *b_pmtID;   //!
   TBranch        *b_sample;   //!
   TBranch        *b_tInc;   //!
   // For PMT1.
   //TBranch        *b_amplitude0;   //!
   TBranch        *b_camplitude0;   //!
   TBranch        *b_baseline0;   //!
   TBranch        *b_baselineSpread0;   //!
   TBranch        *b_peakAmplitude0;   //!
   TBranch        *b_peakBin0;   //!
   // For PMT2.
   //TBranch        *b_amplitude1;   //!
   TBranch        *b_camplitude1;   //!
   TBranch        *b_baseline1;   //!
   TBranch        *b_baselineSpread1;   //!
   TBranch        *b_peakAmplitude1;   //!
   TBranch        *b_peakBin1;   //!

	struct Peak{
		Int_t start, start50, start90 ;
		Int_t end, end50, end90 ;
		Int_t bottom_before, bottom_after, width_bottom,
			  width, fwhm, max_bin ;
	 	double max, integral, trise, tfall ;
	} ;

   RawTree(const char* path=0);
   virtual ~RawTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   virtual int		GetEntries();
   virtual double	GetIntegral(int entry, int startbin, int stopbin, int pmt);
   virtual void		PlotEvent(int entry);
   virtual void		PlotEventSum(int entry);
   virtual TH1F*	GetAmpHisto(int nbins, double minamp, double maxamp, int pmt);
   virtual TH1F*	GetIntHisto(int nbins, double minint, double maxint, int startbin, int stopbin, int pmt);
   //virtual TH1F*	GetElecHisto(int nbins, double minint, double maxint, int startbin, int stopbin, int lowerbinbdy, int upperbinbdy, const char* method, int pmt);
   virtual TH1F*	GetElecHisto(int nbins, double minint, double maxint);
   virtual TH1D* 	GetPeaktimeHisto(int nbins, int minbin, int maxbin, const char* method, int pmt);
   virtual int		GetHalfPeak(int pmt);
   virtual int 		GetS1Pos(int event, int startbin, int stopbin, double threshold, int pmtid);
   virtual TH1D* 	GetS1PosHisto(int nbins, int nevents, int startbin, int stopbin, double threshold, int pmtid);
   virtual void 	SearchEvent(double threshold, int nevent);
	 virtual vector<Peak> FindPeaks(int event, float sigmas, float sigmasreset, int pmt) ;
	 virtual double GetDriftTime(int event, float sigmas,float sigmasreset, int pmt) ;
	 virtual TH2F*  GetDriftTimeHisto(float sigmas, float sigmasreset, int pmt) ;
	virtual int 	GetMaxBin(int entry, int startbin, int stopbin, int pmt);
	virtual int 	GetXPercentHeight(int entry, int startbin, int stopbin, double maxheight, double percentage, int pmt);
};

#endif



#ifdef RawTree_cxx
RawTree::RawTree(const char* path) // Give path to root file as argument.
{
	// Create empty tree and get data file from path
	TTree *tree=0 ;
	TFile* dataFile = new TFile(path);
	
	if (dataFile == 0 ) {
		cout << "WW Warning: datafile is empty!."<<endl;
	}
	else {
	    tree = (TTree*)dataFile->Get("tree") ;
	}
	Init(tree);
}

	



/*
#ifdef RawTree_cxx
RawTree::RawTree(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/data/xenon/xams/pmt/PMT_intitial_characterisation/pmt1rc270krd47ra47c100nlon2V7-histat/raw_data_800V.root");
      if (!f) {
         f = new TFile("/data/xenon/xams/pmt/PMT_intitial_characterisation/pmt1rc270krd47ra47c100nlon2V7-histat/raw_data_800V.root");
      }
      tree = (TTree*)gDirectory->Get("tree");

   }
   Init(tree);
}
*/


RawTree::~RawTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t RawTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t RawTree::LoadTree(Long64_t entry)
{
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

void RawTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("nSamples", &nSamples, &b_nSamples);
   fChain->SetBranchAddress("sample", &sample, &b_sample);
   fChain->SetBranchAddress("tInc", &tInc, &b_tInc);
   //fChain->SetBranchAddress("amplitude0", amplitude0, &b_amplitude0);
   fChain->SetBranchAddress("camplitude0", camplitude0, &b_camplitude0);
   fChain->SetBranchAddress("baseline0", &baseline0, &b_baseline0);
   fChain->SetBranchAddress("baselineSpread0", &baselineSpread0, &b_baselineSpread0);
   fChain->SetBranchAddress("peakAmplitude0", &peakAmplitude0, &b_peakAmplitude0);
   fChain->SetBranchAddress("peakBin0", &peakBin0, &b_peakBin0);
   //
   //fChain->SetBranchAddress("amplitude1", amplitude1, &b_amplitude1);
   fChain->SetBranchAddress("camplitude1", camplitude1, &b_camplitude1);
   fChain->SetBranchAddress("baseline1", &baseline1, &b_baseline1);
   fChain->SetBranchAddress("baselineSpread1", &baselineSpread1, &b_baselineSpread1);
   fChain->SetBranchAddress("peakAmplitude1", &peakAmplitude1, &b_peakAmplitude1);
   fChain->SetBranchAddress("peakBin1", &peakBin1, &b_peakBin1);
   Notify();
}

Bool_t RawTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void RawTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t RawTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return entry;
}
#endif // #ifdef RawTree_cxx
