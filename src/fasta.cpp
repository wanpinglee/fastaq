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
bool Load(T & reference, const char * filename, const bool & convert_case, const char* pChrname)
{
	gzFile fp = gzopen(filename, "r");
	if (!fp) return false; // Cannot open the file

	bool load = false;
	if (pChrname != NULL) {
		faidx_t *fai = NULL;
		fai = fai_load(filename);
		if (fai = NULL) {
			std::cerr << "Warning: Cannot find fai for " << filename << ". Will generate it." << std::endl;
			if (fai_build(filename) == -1) {
				std::cerr << "ERROR: Cannot load and index " << filename << "." << std::endl;
				return false;
			}
			fai = fai_load(filename);
		}

		if (faidx_has_seq(fai, pChrname) == 0) {
			std::cerr << "ERROR: Cannot find " << pChrname << " in " << filename << "." << std::endl;
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
	}

	gzclose(fp);
	return load;
}
} // namespace

namespace Fastaq {
bool FastaLoad(std::string & reference, const char * filename, const bool & convert_case, const char* pChrname)
{
	return Load(reference, filename, convert_case, pChrname);
}

bool FastaLoad(CReference & reference, const char * filename, const bool & convert_case, const char* pChrname)
{
	return Load(reference, filename, convert_case, pChrname);
}
} // namespace Fastaq
