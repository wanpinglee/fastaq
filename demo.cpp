#include <iostream>
#include "include/fasta.h"
#include "include/reference.h"

struct SRegion {
	std::string chr;
	unsigned int begin = 0;
	unsigned int end = 0;
};

inline void ParseRegion(SRegion& region, const std::string & in) {
	size_t pos = in.find(':');
	if (pos != std::string::npos) {
		region.chr = in.substr(0, pos);
	} else {
		region.chr = in;
		return;
	}

	size_t pos1 = in.find('-', pos);
	if (pos1 != std::string::npos) {
		region.begin = atoi(in.substr(pos + 1, pos1 - pos - 1).c_str());
		region.end = atoi(in.substr(pos1 + 1).c_str());
	} else { // only having begin position
		region.begin = atoi(in.substr(pos + 1).c_str());
	}
}

int main (int argc, char** argv) {
	if (argc != 3) {
		std::cerr << "USAGE: " << argv[0] << " <FASTA> <region>" << std::endl;
		return 1;
	}
	Fastaq::CReference ref;
	SRegion region;
	
	ParseRegion(region, argv[2]);
	Fastaq::FastaLoad(ref, argv[1], true, region.chr.c_str());
	if (region.end < region.begin) // region.end is not set. Use the end pos of the chr.
		region.end = ref.GetReferenceLength(region.chr.c_str());
	std::cout << ref.GetSubString(region.chr, region.begin, region.end) << std::endl;


	return 0;
}
