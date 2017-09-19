#include <iostream>
#include "include/fasta.h"
#include "include/reference.h"

int main (int argc, char** argv) {
	if (argc == 1) {
		std::cerr << "USAGE: " << argv[0] << " <FASTAs>" << std::endl;
		return 1;
	}
	CReference ref;
	Fasta::Load(ref, argv[1]);

	std::cout << ref.GetReferenceCount() << std::endl;

	return 0;
}
