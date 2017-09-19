#include <assert.h>
#include "fasta.h"
#include "../lib/htslib/htslib/kseq.h"

KSEQ_INIT(gzFile, gzread)

namespace Fasta {

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

bool Load(CReference & reference, const char * filename, const bool & convert_case)
{
    assert(filename && *filename);

    GZfile fp(filename, "r"); // open the file handler
    if (!fp)
    	return false;

    kseq_t *contig = kseq_init(fp);                // initialize seq
    while (kseq_read(contig) >= 0) {
		assert(contig->name.l > 0);
		if (contig->qual.l != 0) {
			assert(false && "The format is not FASTA, qual detected!");
			kseq_destroy(contig);
			return false;
		}

		if (convert_case) {
			for (char *ptr = contig->seq.s, *ptrE = ptr + contig->seq.l; ptr != ptrE; ++ptr)
				*ptr = toupper(*ptr);
		}

		if (!reference.AddReference(contig->name.s, contig->seq.s)) {
			kseq_destroy(contig);
			return false;
		}
    }

    kseq_destroy(contig);
    return true;
}

} // namespace Fasta
