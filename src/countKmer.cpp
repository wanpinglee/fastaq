#include <vector>
#include <unordered_map>
#include <iostream>
#include <cstdint>
#include <cstring>

#include "fastaq/fasta.h"
#include "fastaq/reference.h"

namespace Fastaq {

bool CountKmer(const char* pFilename, const int kmer_size, std::vector<std::string>& ref_names, std::list<std::list<uint64_t> >& kmer) {
	CReference reference;
	FastaLoad(reference, pFilename);
	
	reference.GetReferenceNames(&ref_names);

	std::unordered_map<char*, uint64_t> kmer_table;
	char kmer_str [kmer_size];

	// Build kmer table.
	// Scan throught references to collect the counts of kmers.
	for (std::vector<std::string>::const_iterator ite = ref_names.begin();
		ite != ref_names.end(); ++ite) {
		const char* chr_str = reference.GetChrString(*ite);
		const unsigned int chr_str_len = reference.GetReferenceLength(ite->c_str());
		for (unsigned int i = 0; i < chr_str_len - kmer_size + 1; ++i) {
			std::memcpy(kmer_str, chr_str, kmer_size);
			++chr_str;
			std::unordered_map<char*, uint64_t>::iterator kmer_table_ite
				= kmer_table.find(kmer_str);
			if (kmer_table_ite == kmer_table.end())
				kmer_table[kmer_str] = 1;
			else 
				++kmer_table[kmer_str];
		}
	}

	// Scan throught references to generate the result.
	for (std::vector<std::string>::const_iterator ite = ref_names.begin();
		ite != ref_names.end(); ++ite) {
		const char* chr_str = reference.GetChrString(*ite);
		const unsigned int chr_str_len = reference.GetReferenceLength(ite->c_str());
		std::list<uint64_t> chr_table(chr_str_len, 0);
		std::list<uint64_t>::iterator chr_table_ite = chr_table.begin();
		for (unsigned int i = 0; i < chr_str_len - kmer_size + 1; ++i) {
			std::memcpy(kmer_str, chr_str, kmer_size);
			++chr_str;
			*chr_table_ite = kmer_table.find(kmer_str)->second;
			++chr_table_ite;
		}
		kmer.push_back(chr_table);
	}

	return true;
}

} // namespace Fastaq
