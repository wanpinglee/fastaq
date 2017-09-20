#ifndef FASTA_H_
#define FASTA_H_

#include <zlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string>

#include "reference.h"

namespace Fasta
{
	// Open and Load a FASTA file
	// If chr_name is set, the function will load that specific chromosome only.
	bool Load(CReference & reference, const char* pFilename, const bool & convert_case = true, const char* pChrname = NULL);
}

#endif // FASTA_H_
