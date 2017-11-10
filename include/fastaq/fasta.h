#ifndef FASTA_H_
#define FASTA_H_

#include <zlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string>

#include "reference.h"

namespace Fastaq
{
	// Open and Load a FASTA file
	// If chr_name is set, the function will load that specific chromosome only.
	bool FastaLoad(CReference & reference, const char* pFilename, const bool & convert_case = true, const char* pChrname = NULL);
	bool FastaLoad(std::string & reference, const char* pFilename, const bool & convert_case, const char* pChrname);
}

#endif // FASTA_H_
