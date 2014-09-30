{
cout<< "1!" << endl;


// read events. Need to compile read_waveforms.cc first
//read("/data/xenon/xams/pmt/PMT_intitial_characterisation/140210_PMT2_1M_single-pe/","800V","C2_140210_pmt2_base1M_2V_%05d.dat")



TFile* file = new TFile("/data/xenon/xams/pmt/PMT_intitial_characterisation/140210_PMT2_1M_single-pe/raw_data_800V.root") ;	

// load the file
//TFile* file = new TFile("raw_data_800V.root");

cout<< "2!" << endl;


//
// // Make a pointer to the tree in the file
TTree* boom = (TTree*)file->Get("tree")   ;

cout<< "3!" << endl;

//
// // Check it!
// boom->Print() ;
// boom->Show(5) ;
// 
// // Create integer
int lengte;
// For now, let's make a list of 900 samples to store the amplitude
float amp[900] ;

cout<< "3!" << endl;
//
// // Set the address. First: name used in tree. Then: address of lacal variable
boom->SetBranchAddress("nSamples",&lengte) ;
boom->SetBranchAddress("amplitude",&amp) ;
//
cout<< "3!" << endl;
//
int nentries = boom->GetEntries() ;
//
// // Create histogram pointer
// TH1D *hnsamples =  new TH1D("hnsamples","Samples",1000,0,1000) ;
//
// // Fill with entries
//for(int i=0 ; i< nentries ; i++ ) { 
//	boom->GetEntry(i) ; 
//	hnsamples->Fill(lengte);} ;
//
//hnsamples->Draw() ;
cout<< "3!" << endl;

boom->GetEntry(49) ;
cout<< "3!" << endl;
//TH1F *amphist = new TH1F("amphist","Waveform",900,0,900*0.2) ;
TH1F fuckyou("fuckyou","Waveform",1000,0,200) ;

cout<< "3!" << endl;

//for(int i=0 ; i<900 ; i++){	amphist->SetBinContent(i,amp[i]);} 
//for(int i=0 ; i<900 ; i++){	fuckyou.SetBinContent(i,amp[i]); if(i%100==0){cout << "Number: "<< i << endl;} } 
for(int i=0 ; i<900 ; i++){	fuckyou.SetBinContent(i,amp[i]); if(i%100==0){cout << "Number: "<< i << endl;} } 
cout<< "End reached." << endl ;

//amphist->Draw() ;



}
