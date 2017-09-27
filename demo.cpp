#include <iostream>

#include "fastaq/fasta.h"
#include "fastaq/reference.h"
#include "fastaq/region.h"

int main (int argc, char** argv) {
	if (argc != 3) {
		std::cerr << "USAGE: " << argv[0] << " <FASTA> <region>" << std::endl;
		return 1;
	}
	Fastaq::CReference ref;
	Fastaq::SRegion region;
	
	if (!region.Parse(argv[2])) {
		std::cerr << "ERROR: The region is invalid." << std::endl;
		return 1;
	}
	if (!Fastaq::FastaLoad(ref, argv[1], true, region.chr.c_str())) {
		std::cerr << "ERROR: Cannot load chromosome " << region.chr << " from " << argv[1] <<std::endl;
		return 1;
	}

	if (region.begin >= ref.GetReferenceLength(region.chr.c_str()))
		std::cerr << "ERROR: begin (" << region.begin << ") is out the length (" 
				<< ref.GetReferenceLength(region.chr.c_str()) << ") of chromosome." << std::endl;
	if ((region.end == 0) || (region.end < region.begin)) // region.end is not set. Use the end pos of the chr.
		region.end = ref.GetReferenceLength(region.chr.c_str()) - 1;
	std::cout << ref.GetSubString(region.chr, region.begin, region.end - region.begin + 1) << std::endl;


	return 0;
}
