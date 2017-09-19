#ifndef FASTA_H_
#define FASTA_H_

#include <zlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string>

#include "reference.h"

namespace Fasta
{
    bool Load(CReference & reference, const char* pFilename, const bool & convert_case = true);    // Open and Load a FASTA file
}

#endif // FASTA_H_
