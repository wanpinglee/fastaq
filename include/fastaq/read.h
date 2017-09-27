#ifndef _FASTAQ_READ_H_
#define _FASTAQ_READ_H_

#include <string>

namespace Fastaq {
struct SRead
{
	std::string name;
	std::string bases;
	std::string quals;
};
}
#endif //_FASTAQ_READ_H_
