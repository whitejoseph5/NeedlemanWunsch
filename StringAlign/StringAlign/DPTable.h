#ifndef DPTABLE_H
#define DPTABLE_H

#include "DPCell.h"
#include "ScoreParams.h"

#include <fstream>
#include <iomanip>

#define CHARS_PER_LINE 60

using std::ifstream;
using std::ofstream;
using std::getline;
using std::setw;

class DPTable
{
public:
	//Worst case time complexity is constant time unless otherwise noted
	//Where m = s1.length + 1 and n = s2.length + 1
	
	// O(m + n)
	DPTable(const string & inputFile = "defaultInput.fasta", const string & paramsFile = "parameters.config");
	~DPTable();

	// O(m + n)
	void readFastaFile(const string & fileName);
	void readParamsFile(const string & fileName);
	// O(mn)
	void populateTable();
	// O(m + n)
	void backTrace();
	// O(m + n)
	void printOutput() const;

	// accessor version of operator[]
	const vector<DPCell> & operator[](int row) const;

private:
	void writeAlignedStrings(char s1ch, char s2ch, char cnxn);

	vector<vector<DPCell>> table;

	// raw string input
	string s1;
	string s1Name;
	string s2;
	string s2Name;

	// scoring penalty parameters
	ScoreParams scoreParams;

	// aligned strings
	string s1Out;
	string cnxnStr;
	string s2Out;

	// report data
	int globalOpt;
	int localOpt;
	int numMatches;
	int numMismatches;
	int numOpenGaps;
	int numGaps;
};

#endif