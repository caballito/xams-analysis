// Functions for analysis of the class RawTree


#include "RawTree.h"
using namespace std;

double integrate(RawTree* rawtree,int eventn, int binstart, int binend){
	rawtree->GetEntry(eventn);
	double integral = 0 ;
	integral = rawtree->amplitude[binstart];
	return integral ;
}

