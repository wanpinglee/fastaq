#include <iostream>

#include "include/fasta.h"
#include "include/reference.h"
#include "include/region.h"

int main (int argc, char** argv) {
	if (argc != 3) {
		std::cerr << "USAGE: " << argv[0] << " <FASTA> <region>" << std::endl;
		return 1;
	}
	Fastaq::CReference ref;
	Fastaq::SRegion region;
	
	region.ParseRegion(region, argv[2]);
	Fastaq::FastaLoad(ref, argv[1], true, region.chr.c_str());
	if (region.end < region.begin) // region.end is not set. Use the end pos of the chr.
		region.end = ref.GetReferenceLength(region.chr.c_str());
	std::cout << ref.GetSubString(region.chr, region.begin, region.end) << std::endl;


	return 0;
}
