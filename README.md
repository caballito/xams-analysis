# This is the README file for the XAMS analysis code.
# (The code collection will be on a GitHub repository
#  with the name https://github.com/caballito/xams-analysis.git)
#
# created on 25/09/2014 by R. Schoen

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
