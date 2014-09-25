{

// Load Waveform class
gSystem->CompileMacro("/data/xenon/xams/pmt/src/Waveform.cpp","k","Waveform");
// Load OscilloscopeWaveformDecoder
gSystem->CompileMacro("/data/xenon/xams/pmt/src/LecroyWFDecoder.cpp","k","LecroyWFDecoder");
// Load raw data read-in
gSystem->CompileMacro("/data/xenon/xams/pmt/src/read_binary.cc","k","read_binary");
// Load RawTree class
//gSystem->CompileMacro("/data/xenon/xams/pmt/src/RawTree2.C","k","RawTree2");
// Load peak finder
//gSystem->CompileMacro("/data/xenon/xams/pmt/src/analyserawdata.cc","k","analyserawdata");

read("../PMT_liquid/","RUN-3_MEAS-25_900V_0V_3000V_widthC2_200ns_10us_Cs_8cm","_900V_%05d.trc",50581,2000);
reed("../PMT_liquid/","RUN-3_MEAS-26_900V_0V_0V_widthC2_200ns_10us_Cs_8cm","_900V_%05d.trc",20122,2000);
read("../PMT_liquid/","RUN-3_MEAS-27_900V_500V_3000V_widthC2_200ns_10us_Cs_8cm","_900V_%05d.trc",51689,2000);
read("../PMT_liquid/","RUN-3_MEAS-28_900V_1000V_3000V_widthC2_200ns_10us_Cs_8cm","_900V_%05d.trc",55000,2000);
read("../PMT_liquid/","RUN-3_MEAS-29_900V_2000V_3000V_widthC2_200ns_10us_Cs_8cm","_900V_%05d.trc",45393,2000);
read("../PMT_liquid/","RUN-3_MEAS-30_900V_6000V_3000V_widthC2_200ns_10us_Cs_8cm","_900V_%05d.trc",38042,2000);
read("../PMT_liquid/","RUN-3_MEAS-31_900V_6000V_3700V_widthC2_200ns_10us_Cs_5cm","_900V_%05d.trc",52342,2000);
read("../PMT_liquid/","RUN-3_MEAS-32_900V_6000V_3350V_widthC2_200ns_10us_Cs_5cm","_900V_%05d.trc",53630,2000);
read("../PMT_liquid/","RUN-3_MEAS-33_900V_6000V_3000V_widthC2_200ns_10us_Cs_5cm","_900V_%05d.trc",50170,2000);
read("../PMT_liquid/","RUN-3_MEAS-34_900V_6000V_2650V_widthC2_200ns_10us_Cs_5cm","_900V_%05d.trc",51729,2000);
read("../PMT_liquid/","RUN-3_MEAS-36_900V_6000V_3000V_cosmiccoin_10us_Colim-Cs_5cm","_900V_%05d.trc",2,2000);
read("../PMT_liquid/","RUN-3_MEAS-37_900V_6000V_3000V_widthC2_200ns_10us_collCs_5cm","_900V_%05d.trc",32952,2000);

}
