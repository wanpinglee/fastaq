#ifndef _FASTAQ_REGION_H_
#define _FASTAQ_REGION_H_

#include <string>

namespace Fastaq {
struct SRegion {
	std::string chr;
	unsigned int begin;
	unsigned int end;

	bool Parse(const std::string & in) {
		size_t pos = in.find(':');
		if (pos != std::string::npos) {
			chr = in.substr(0, pos);
		} else {
			chr = in;
			return true;
		}

		size_t pos1 = in.find('-', pos);
		if (pos1 != std::string::npos) {
			begin = atoi(in.substr(pos + 1, pos1 - pos - 1).c_str());
			end = atoi(in.substr(pos1 + 1).c_str());
			if (end < begin) return false;
		} else { // only having begin position
			begin = atoi(in.substr(pos + 1).c_str());
		}
		return true;
	}
};
} // namespace Fastaq

#endif
