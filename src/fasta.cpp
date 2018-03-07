#include <assert.h>
#include <cstring>
#include <iostream>
#include "fastaq/fasta.h"
#include "htslib/kseq.h"
#include "htslib/faidx.h"

KSEQ_INIT(gzFile, gzread)

namespace {

void AddReference(Fastaq::CReference & reference, const char* name, const char* seq)
{
	reference.AddReference(name, seq);
}
void AddReference(std::string & reference, const char* name, const char* seq)
{
	reference = seq;
}

template <typename T>
bool Load(T & reference, const char * filename, const bool & convert_case, const char* pChrname, const bool quiet)
{

	bool load = false;
	if (pChrname != NULL) {
		faidx_t *fai = NULL;
		fai = fai_load(filename);
		// If cannot load fai, then try to build it.
		if (fai == NULL) {
			std::cerr << "Indexing " << filename << "......" << std::endl;
			if (fai_build(filename) == -1) {
				if (!quiet) std::cerr << "ERROR: Cannot load and index " << filename << "." << std::endl;
				return false;
			}
			fai = fai_load(filename);
		}
		// Check if the query chr name is there.
		if (faidx_has_seq(fai, pChrname) == 0) {
			if (!quiet) std::cerr << "ERROR: Cannot find " << pChrname << " in " << filename << "." << std::endl;
		} else {
			const int length = faidx_seq_len(fai, pChrname);
			int load_length = 0;
			char * seq = faidx_fetch_seq(fai, pChrname, 0, length - 1, &load_length);

			if (convert_case) {
				char *ptr = seq;
				for (int i = 0; i < load_length; ++i, ++ptr)
					*ptr = toupper(*ptr);
			}
			// Add the contig in reference.
			AddReference(reference, pChrname, seq);
			free(seq);

			load = true;
		}
		fai_destroy(fai);
	} else {
		gzFile fp = gzopen(filename, "r");
		if (!fp) return false; // Cannot open the file

		kseq_t *contig = kseq_init(fp);                // initialize seq
		while (kseq_read(contig) >= 0) {
			assert(contig->name.l > 0);
			// The qual is detected so the file is not a FASTA (it's a FASTQ).
			if (contig->qual.l != 0) {
				assert(false && "The format is not FASTA, qual detected!");
				break;
			}

			if (convert_case) {
				for (char *ptr = contig->seq.s, *ptrE = ptr + contig->seq.l; ptr != ptrE; ++ptr)
					*ptr = toupper(*ptr);
			}

			// Add the contig in reference.
			AddReference(reference, contig->name.s, contig->seq.s);

			load = true;
		}
		kseq_destroy(contig);
		gzclose(fp);
	}

	return load;
}
} // namespace

namespace Fastaq {
bool HeaderLoad(CReference & reference, const char * filename)
{
	faidx_t *fai = NULL;
	fai = fai_load(filename);
	// If cannot load fai, then try to build it.
	if (fai == NULL) {
		std::cerr << "Indexing " << filename << "......" << std::endl;
		if (fai_build(filename) == -1) {
			std::cerr << "ERROR: Cannot load and index " << filename << "." << std::endl;
			return false;
		}
		fai = fai_load(filename);
	}

	for (int i = 0; i < faidx_nseq(fai); ++i)
		reference.AddReference(faidx_iseq(fai, i), NULL);
	fai_destroy(fai);

	return true;
}

bool FastaLoad(std::string & reference, const char * filename, const bool & convert_case, const char* pChrname, const bool quiet)
{
	return Load(reference, filename, convert_case, pChrname, quiet);
}

bool FastaLoad(CReference & reference, const char * filename, const bool & convert_case, const char* pChrname, const bool quiet)
{
	return Load(reference, filename, convert_case, pChrname, quiet);
}
} // namespace Fastaq
