{
	Plank* p = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-35.root") ;
	TFile f("../liquid_data/results/140715_slices.root","recreate") ;
	f.cd() ;

	TH2F* s2IntVDriftTime = p->GetS2IntegralVDriftTime(80,0,80,100,0,20e3) ;	// Binning a bit more coarse.
	double dtime, s2int, s1int ;
	double s1lo=300 ;
	double s1hi=350 ;
	TH2F* s2IntVDriftTimeCut = new TH2F("s2IntVDriftTimeCut",";drift time (#mus);S2 area (p.e.)",80,0,80,100,0,20e3) ;
	for (int i=0;i<p->GetEntries();i++){ s1int = p->GetS1Integral(i); if (s1lo<s1int && s1int<s1hi){  dtime=p->GetDriftTime(i); s2int=p->GetS2Integral(i); if (dtime!=0 && s2int!=0) s2IntVDriftTimeCut->Fill(dtime*1.e6,s2int);  } }

	TF1* lan = new TF1("lan","landau",0,20e3) ;
	lan->SetParLimits(1,10,15e3) ;
	lan->SetParameter(1,4000) ;

	TF1* vis = new TF1("vis","TMath::Poisson(x,[0])",0,20e3) ;
	vis->SetParLimits(0,10,15e3) ;
	vis->SetParameter(0,4000) ;

	TH1D* py1 = s2IntVDriftTimeCut->ProjectionY("py1",5,10) ;
	TH1D* py2 = s2IntVDriftTimeCut->ProjectionY("py2",10,15) ;
	TH1D* py3 = s2IntVDriftTimeCut->ProjectionY("py3",15,20) ;
	TH1D* py4 = s2IntVDriftTimeCut->ProjectionY("py4",20,25) ;
	TH1D* py5 = s2IntVDriftTimeCut->ProjectionY("py5",25,30) ;
	TH1D* py6 = s2IntVDriftTimeCut->ProjectionY("py6",30,35) ;
	TH1D* py7 = s2IntVDriftTimeCut->ProjectionY("py7",35,40) ;
	TH1D* py8 = s2IntVDriftTimeCut->ProjectionY("py8",40,45) ;
	TH1D* py9 = s2IntVDriftTimeCut->ProjectionY("py9",45,50) ;
	TH1D* py10 = s2IntVDriftTimeCut->ProjectionY("py10",50,55) ;

	py1->Fit(lan) ;
	py2->Fit(lan) ;
	py3->Fit(lan) ;
	py4->Fit(lan) ;
	py5->Fit(lan) ;
	py6->Fit(lan) ;
	py7->Fit(lan) ;
	py8->Fit(lan) ;
	py9->Fit(lan) ;
	py10->Fit(lan) ;

	s2IntVDriftTimeCut->FitSlicesY(lan,5,60,0,"G2") ;

	s2IntVDriftTime->Write() ;
	s2IntVDriftTimeCut->Write() ;
	s2IntVDriftTimeCut_0->Write() ;
	s2IntVDriftTimeCut_1->Write() ;
	s2IntVDriftTimeCut_2->Write() ;
	s2IntVDriftTimeCut_chi2->Write() ;

	py1->Write() ;
	py2->Write() ;
	py3->Write() ;
	py4->Write() ;
	py5->Write() ;
	py6->Write() ;
	py7->Write() ;
	py8->Write() ;
	py9->Write() ;
	py10->Write() ;

	f.Close() ;
	cout << "Allet prima." << endl ;

}
