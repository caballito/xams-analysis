{
	// Load Waveform class
	gSystem->CompileMacro("/data/xenon/xams/pmt/src/Waveform.cpp",
			"k","Waveform");
	// Load OscilloscopeWaveformDecoder
	gSystem->CompileMacro("/data/xenon/xams/pmt/src/LecroyWFDecoder.cpp",
			"k","LecroyWFDecoder");
	// Load raw data read-in
	gSystem->CompileMacro("/data/xenon/xams/pmt/src/read_binary.cc",
			"k","read_binary");
	// Load RawTree class
	gSystem->CompileMacro("/data/xenon/xams/pmt/src/RawTree2.C","k","RawTree2");
	// Load peak finder
	gSystem->CompileMacro("/data/xenon/xams/pmt/src/process_raw_data.cc",
			"k","process_raw_data");
	// Load Plank class (processed data tree)
	gSystem->CompileMacro("/data/xenon/xams/pmt/src/Plank.C","k","Plank");
	// Load data analysis (of processed data)
	gSystem->CompileMacro("/data/xenon/xams/pmt/src/analyse_proc_data.cc",
			"k","analyse_proc_data");
	std::cout << "---- Ready to do stuff! ----" << std::endl ;

}
