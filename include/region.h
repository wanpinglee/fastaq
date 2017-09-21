#ifndef _FASTAQ_REGION_H_
#define _FASTAQ_REGION_H_

#include <string>

namespace Fastaq {
struct SRegion {
	std::string chr;
	unsigned int begin = 0;
	unsigned int end = 0;

	void ParseRegion(SRegion& region, const std::string & in) {
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
};
} // namespace Fastaq

#endif
