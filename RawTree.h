//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Feb 24 11:10:55 2014 by ROOT version 5.28/00b
// from TTree tree/Waveform collection
// found on file: /data/xenon/xams/pmt/PMT_intitial_characterisation/pmt1rc270krd47ra47c100nlon2V7-histat/raw_data_800V.root
//////////////////////////////////////////////////////////

#ifndef RawTree_h
#define RawTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <iostream>

using namespace std ;

class RawTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           nSamples;
   Int_t           sample;
   Float_t         tInc;
   Float_t         amplitude[1000];   //[sample]
   Float_t         camplitude[1000];   //[sample]
   Float_t         baseline;
   Float_t         baselineSpread;
   Float_t         peakAmplitude;
   Int_t           peakBin;

   // List of branches
   TBranch        *b_nSamples;   //!
   TBranch        *b_sample;   //!
   TBranch        *b_tInc;   //!
   TBranch        *b_amplitude;   //!
   TBranch        *b_camplitude;   //!
   TBranch        *b_baseline;   //!
   TBranch        *b_baselineSpread;   //!
   TBranch        *b_peakAmplitude;   //!
   TBranch        *b_peakBin;   //!

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
   virtual double	GetIntegral(int entry, int startbin, int stopbin);
   virtual void		PlotEvent(int entry);
   virtual TH1F*	GetAmpHisto(int nbins, double minamp, double maxamp);
   virtual TH1F*	GetIntHisto(int nbins, double minint, double maxint, int startbin, int stopbin);
   virtual TH1F*	GetIntHistoCut(int nbins, double minint, double maxint, int startbin, int stopbin, int lowerbinbdy, int upperbinbdy, const char* method);
   virtual TH1F*	GetElecHisto(int nbins, double minint, double maxint, int startbin, int stopbin, int lowerbinbdy, int upperbinbdy, const char* method);
   virtual TH1D* 	GetPeaktimeHisto(int nbins, int minbin, int maxbin, const char* method);
   virtual TH1D* 	GetPeaktimeHistoNoStupidBins(int nbins, int mintime, int maxtime, const char* method);
   virtual int		GetHalfPeak();
   virtual void 	SearchEvent(double treshold, int nevent);
   virtual TH1F*	GetAverageEvent();

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
   fChain->SetBranchAddress("amplitude", amplitude, &b_amplitude);
   fChain->SetBranchAddress("camplitude", camplitude, &b_camplitude);
   fChain->SetBranchAddress("baseline", &baseline, &b_baseline);
   fChain->SetBranchAddress("baselineSpread", &baselineSpread, &b_baselineSpread);
   fChain->SetBranchAddress("peakAmplitude", &peakAmplitude, &b_peakAmplitude);
   fChain->SetBranchAddress("peakBin", &peakBin, &b_peakBin);
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
