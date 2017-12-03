#include <vector>
#include <list>

#include "fastaq/fasta.h"
#include "fastaq/reference.h"

int main (int argc, char** argv) {
	if (argc != 3) {
		std::cerr << "USAGE: " << argv[0] << " <FASTA> <kmer_size>" << std::endl;
		return 1;
	}

	std::vector<std::string> ref_names;
	std::list<std::list<uint64_t> > kmer;
	Fastaq::CountKmer(argv[1], atoi(argv[2]), ref_names, kmer);

	std::list<std::list<uint64_t> >::const_iterator table_ite = kmer.begin();

	for (std::vector<std::string>::const_iterator ite = ref_names.begin(); ite != ref_names.end(); ++ite) {
		std::cout << *ite << std::endl;
		for (std::list<uint64_t>::const_iterator kmer_ite = table_ite->begin();
			kmer_ite != table_ite->end(); ++kmer_ite) {
			std::cout << *kmer_ite << " ";
		}
		std::cout << std::endl;
		++table_ite;
	}

	return 0;
}
