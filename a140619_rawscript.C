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

//read("../PMT_liquid/","RUN-3_MEAS-38_900V_6000V_3000V_widthC2_200ns_10us_collCs_2cm","_900V_%05d.trc",30028,2000);
//read("../PMT_liquid/","RUN-3_MEAS-39_900V_6000V_3000V_widthC2_200ns_10us_collCs_8cm","_900V_%05d.trc",33414,2000);
read("../PMT_liquid/","RUN-3_MEAS-40_900V_6000V_3000V_widthC2_200ns_10us_collCs_3_5cm","_900V_%05d.trc",42878,2000);

}
