{
	cout << endl << "----- Running the kots() function on all processed data -----"
		<< endl ;
	Plank* p = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-30.root") ;
	kots(p,"r3m30") ;
	p = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-31.root") ;
	kots(p,"r3m31") ;
	p = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-32.root") ;
	kots(p,"r3m32") ;
	p = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-33.root") ;
	kots(p,"r3m33") ;
	p = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-34.root") ;
	kots(p,"r3m34") ;
	p = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-35.root") ;
	kots(p,"r3m35") ;
	//
	p = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-59.root") ;
	kots(p,"r3m59") ;
	p = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-60.root") ;
	kots(p,"r3m60") ;
	p = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-61.root") ;
	kots(p,"r3m61") ;
	p = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-62.root") ;
	kots(p,"r3m62") ;
	p = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-63.root") ;
	kots(p,"r3m63") ;
	p = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-64.root") ;
	kots(p,"r3m64") ;
	p = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-65.root") ;
	kots(p,"r3m65") ;
	p = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-66.root") ;
	kots(p,"r3m66") ;
	//
	p = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-71.root") ;
	kots(p,"r3m71") ;
	p = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-72.root") ;
	kots(p,"r3m72") ;
	//
	p = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-74.root") ;
	kots(p,"r3m74") ;
	p = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-76.root") ;
	kots(p,"r3m76") ;
	//
	cout << endl << "----- Done! -----" << endl ;

}

