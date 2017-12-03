#include <assert.h>
#include <cstring>
#include "fastaq/fasta.h"
#include "htslib/kseq.h"

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
	kseq_t *contig = kseq_init(fp);                // initialize seq
	while (kseq_read(contig) >= 0) {
		assert(contig->name.l > 0);
		// The qual is detected so the file is not a FASTA (it's a FASTQ).
		if (contig->qual.l != 0) {
			assert(false && "The format is not FASTA, qual detected!");
			break;
		}

		// The chrmosome is not the target one.
		// Try to load the next one.
		if ((pChrname != NULL) && (std::strcmp(pChrname, contig->name.s) != 0))
			continue;

		if (convert_case) {
			for (char *ptr = contig->seq.s, *ptrE = ptr + contig->seq.l; ptr != ptrE; ++ptr)
				*ptr = toupper(*ptr);
		}

		// Add the contig in reference.
		AddReference(reference, contig->name.s, contig->seq.s);

		load = true;
		if (pChrname != NULL) break; // The target chromosome has been loaded.
	}

	kseq_destroy(contig);
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
