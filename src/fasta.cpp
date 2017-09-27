#include <assert.h>
#include <cstring>
#include "fastaq/fasta.h"
#include "htslib/kseq.h"

KSEQ_INIT(gzFile, gzread)

namespace Fastaq {

struct GZfile {
	gzFile fp_;

	GZfile(gzFile fp = 0L)
	: fp_(fp)
	{}

	GZfile(const char *fname, const char *mode = 0L)
	: fp_(gzopen(fname, mode))
	{}

	~GZfile()
	{
		if (fp_)
			gzclose(fp_);
	}

	operator gzFile () const
	{
		return fp_;
	}

	operator bool () const
	{
		return fp_ != 0L;
	}
};

bool FastaLoad(CReference & reference, const char * filename, const bool & convert_case, const char* pChrname)
{
	assert(filename && *filename);

	GZfile fp(filename, "r"); // open the file handler
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
		reference.AddReference(contig->name.s, contig->seq.s);

		load = true;
		if (pChrname != NULL) break; // The target chromosome has been loaded.
	}

	kseq_destroy(contig);

	return load;
}

} // namespace Fasta
