/*
	White, Joe
	
	This is my implementation of the Needleman Wunsch sequence alignment algorithm
	with affine gap penalty.

	Command line arguments should be passed in the following format:
	./StringAlign [inputfile.fasta] [0(global) or 1(local)] [parameters.config]

	"defaultInput.fasta" and "parameters.config" must be in working directory if no arguments are supplied.

	Future work:
		-add optimal local alignment functionality
		-add ability to find multiple global alignments
			(on backtrace, mark cells with more than one "inherited" score, trace all paths)
*/

#include "DPTable.h"

int main(int argc, char *argv[])
{
	string inputFileName("defaultInput.fasta"), paramsFileName("parameters.config");

	switch (argc) {
		case 1:
			break;
		case 2:
		case 3:
			inputFileName.assign(argv[1]);
			break;
		case 4:
			inputFileName.assign(argv[1]);
			paramsFileName.assign(argv[3]);
			break;
	}

	DPTable t(inputFileName, paramsFileName);

	cout << "populating table" << endl;
	t.populateTable();
	cout << "backtracing" << endl;
	t.backTrace();
	t.printOutput();

	return 0;
}