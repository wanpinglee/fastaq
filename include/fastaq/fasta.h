#ifndef FASTA_H_
#define FASTA_H_

#include <zlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <list>

#include "reference.h"

namespace Fastaq
{
	// Open and Load a FASTA file
	// If chr_name is set, the function will load that specific chromosome only.
	bool FastaLoad(CReference & reference, const char* pFilename, const bool & convert_case = true, const char* pChrname = NULL);
	bool FastaLoad(std::string & reference, const char* pFilename, const bool & convert_case, const char* pChrname);

	bool HeaderLoad(CReference & reference, const char* pFilename);

	// Count Kmer (the implementation is src/countKmer)
	// ref_names for storing chromosome names.
	// The results are kept in list from the first chromosome to the last one.
	// In each list, kmer counts are kept from the first position of the chromosome to the last one.
	bool CountKmer(const char* pFilename, const int kmer_size, std::vector<std::string>& ref_names, std::list<std::list<uint64_t> >& kmer);
}

#endif // FASTA_H_
