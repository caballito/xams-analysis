{
	RawTree* r50 = new RawTree("../liquid_data/raw_data/raw_data_RUN-3_MEAS-50_900V_0V_0V_coincS1_200ns_collCs_3_5cm.root") ;
	TH1F* s1spec = new TH1F("s1spec","S1 spectrum;integral (Vs);counts",1000,0,1e-07) ;
	double area = 0 ;
	for (int i=0;i<GetEntries;i++){
		GetEntry(i);
		if(peakAmplitude < 1.8){
			area = GetIntegral(i,100,450,0) ;
			area += GetIntegral(i,100,450,1) ;
			s1spec->Fill(area) ;
		}
	}
	s1spec->Draw() ;
	c1->WaitPrimitive() ;
}
