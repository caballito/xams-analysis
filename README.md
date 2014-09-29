# This is the README file for the XAMS analysis code.
# (The code collection will be on a GitHub repository
#  at https://github.com/caballito/xams-analysis.git)
#
# created on 25/09/2014 by R. Schoen


#------- CODE -------#
The code includes the C++ classes (.C,.h) for 
 - the raw data of one or two PMTs:
   RawTree, RawTree2,
 - the processed raw data:
   Plank.

It also contains the functions (.cc) to
 - read in the raw data in binary format (i.e. to make the RawTree)
   read_binary,
 - process the raw data tree
   process_raw_data,
 - and to analyse the processed data
   analyse_proc_data

There is also a macro to compile all the classes etc:
   load_environment.C
Do not forget to adjust it to the correct path on your
local system.

A typical workflow for TPC data looks like this:
a) You have the raw data (in binary format "WORD") from the 
   oscilloscope: one file per waveform per channel:
   C<1,2>_<some identifier>_%05d.trc (5 digits with trailing 0)
b) Run read_binary code on raw data directory.
c) You have the raw data in a TTree (RawTree). This class contains
   the time resolution, the baseline (averaged over the first 2000
   samples), the baseline spread and the corrected (i.e. baseline-
   subtracted) waveform for each event for both PMTs.
   The RawTree class has member functions to create simple
   histograms (amplitude, integral, peak time) and the FindPeaks()
   function.   
d) Run process_raw_data code on RawTree. This invokes the peak
   finder.
e) You have the peak information per event per PMT in a TTree (Plank).
   This class contains the time resolution, the number and parameters
   of every identified peak per event.
f) Run analyse_proc_data code on Plank to make a collection of
   histograms for a quick quality control of the data.


#------- DATA -------#
The data is divided by the number of PMTs:
 - /data/xenon/xams/pmt/PMT_initial_characterisation/ 
   for a single PMT in the dark box during the characterization.
 - /data/xenon/xams/pmt/liquid_data/ for both PMT signals
   inside the XAMS TPC.
