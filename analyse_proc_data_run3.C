{
/***************
 * Script to analyse all processed data from run 3
 * Read in 'Plank' files, and use the 'kots' function.
 * Before executing, load the load_environment.C file
 */

// Step 1. Load all the Planks (processed trees)
	 Plank* p02 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-02.root");
	 Plank* p03 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-03.root");
	 Plank* p04 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-04.root");
	 Plank* p05 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-05.root");
	 Plank* p06 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-06.root");
	 Plank* p07 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-07.root");
	 Plank* p08 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-08.root");
	 Plank* p09 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-09.root");
	 Plank* p10 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-10.root");
	 Plank* p11 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-11.root");
	 Plank* p12 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-12.root");
	 Plank* p13 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-13.root");
	 Plank* p18 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-18.root");
	 Plank* p19 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-19.root");
	 Plank* p20 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-20.root");
	 Plank* p21 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-21.root");
	 Plank* p22 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-22.root");
	 Plank* p23 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-23.root");
	 Plank* p24 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-24.root");
	 Plank* p25 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-25.root");
	 Plank* p26 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-26.root");
	 Plank* p27 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-27.root");
	 Plank* p28 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-28.root");
	 Plank* p29 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-29.root");
	 Plank* p30 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-30.root");
	 Plank* p31 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-31.root");
	 Plank* p32 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-32.root");
	 Plank* p33 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-33.root");
	 Plank* p34 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-34.root");
	 Plank* p35 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-35.root");
	 Plank* p36 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-36.root");
	 Plank* p37 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-37.root");
	 Plank* p38 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-38.root");
	 Plank* p39 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-39.root");
	 Plank* p40 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-40.root");
	 Plank* p41 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-41.root");
	 Plank* p42 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-42.root");
	 Plank* p43 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-43.root");
	 Plank* p47 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-47.root");
	 Plank* p48 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-48.root");
	 Plank* p49 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-49.root");
	 Plank* p50 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-50.root");
	 Plank* p51 = new Plank("../liquid_data/processed_data/processed_data_RUN-3_MEAS-51.root");

// Step 2. Execute the kots function

//	kots(p01,"r3m01");
	kots(p02,"r3m02");
	kots(p03,"r3m03");
	kots(p04,"r3m04");
	kots(p05,"r3m05");
	kots(p06,"r3m06");
	kots(p07,"r3m07");
	kots(p08,"r3m08");
	kots(p09,"r3m09");
	kots(p10,"r3m10");
	kots(p11,"r3m11");
	kots(p12,"r3m12");
	kots(p13,"r3m13");
//	kots(p14,"r3m14");
//	kots(p15,"r3m15");
//	kots(p16,"r3m16");
//	kots(p17,"r3m17");
	kots(p18,"r3m18");
	kots(p19,"r3m19");
	kots(p20,"r3m20");
	kots(p21,"r3m21");
	kots(p22,"r3m22");
	kots(p23,"r3m23");
	kots(p24,"r3m24");
	kots(p25,"r3m25");
	kots(p26,"r3m26");
	kots(p27,"r3m27");
	kots(p28,"r3m28");
	kots(p29,"r3m29");
	kots(p30,"r3m30");
	kots(p31,"r3m31");
	kots(p32,"r3m32");
	kots(p33,"r3m33");
	kots(p34,"r3m34");
	kots(p35,"r3m35");
	kots(p36,"r3m36");
	kots(p37,"r3m37");
	kots(p38,"r3m38");
	kots(p39,"r3m39");
	kots(p40,"r3m40");
	kots(p41,"r3m41");
	kots(p42,"r3m42");
	kots(p43,"r3m43");
//	kots(p44,"r3m44");
//	kots(p45,"r3m45");
//	kots(p46,"r3m46");
	kots(p47,"r3m47");
	kots(p48,"r3m48");
	kots(p49,"r3m49");
	kots(p50,"r3m50");
	kots(p51,"r3m51");

}
