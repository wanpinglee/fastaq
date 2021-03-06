#include <stdio.h>

#include "fastaq/reference.h"

using namespace std;

namespace Fastaq {
CReference::CReference()
{
	last_id_ = 0;
}

CReference::~CReference()
{
}

bool CReference::AddReference(const char * pName, const char * pSeq)
{
	if (pName == NULL) return false;

	if (refs_.find(pName) != refs_.end())
	{
		fprintf(stderr,"ERROR: %s has been added.\n", pName);
		return false;
	}

	SReference ref;
	if (pSeq != NULL) ref.read.bases = pSeq;
	ref.length   = ref.read.bases.size();
	ref.id       = last_id_;
	++last_id_;

	refs_.insert(pair<string, SReference>(pName, ref));
	ref_names_.push_back(pName);

	return true;
}
}
