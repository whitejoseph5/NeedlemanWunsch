#include "DPTable.h"

/*
	Reads input from files and initializes table with number of rows 
	and number of columns equal to length of s1 + 1 and length of 
	s2 + 1 respectively.
*/
DPTable::DPTable(const string & inputFile, const string & paramsFile)
	: s1(""), s1Name(""), s2(""), s2Name(""), s1Out(""), cnxnStr(""), s2Out(""),
	table(0), scoreParams(), globalOpt(0), localOpt(0), numMatches(0),
	numMismatches(0), numOpenGaps(0), numGaps(0)
{
	readFastaFile(inputFile);
	readParamsFile(paramsFile);

	table.resize(s1.length() + 1);
	for (int i = 0; i < table.size(); i++)
	{
		table[i].resize(s2.length() + 1);
	}
}


DPTable::~DPTable()
{
}

/*
	parses inputFile and reads contents into s1, s1Name, s2, and s2Name
	assumptions:
		-inputFile is in format of "defaultInput.fasta"
		-no white space
		-all characters same case (all upper or lower)
*/
void DPTable::readFastaFile(const string & fileName)
{
	ifstream file(fileName);
	string line;

	if (file.is_open())
	{
		s1.assign("");
		s2.assign("");

		if (getline(file, line))
			s1Name.assign(line.substr(1, line.length() - 1)); // strip out first character, '>'

		while (getline(file, line))
		{
			if (line[0] == '>')
				break;
			s1.append(line);
		}

		s2Name.assign(line.substr(1, line.length() - 1));

		while (getline(file, line))
			s2.append(line);

		file.close();
	}
	else
	{
		cout << "Error: unable to open file - string members unchanged." << endl << endl;
		// possibly set to default values
	}
}

/*
	assumptions:
		-params file is in format of "parameters.config" file
		-no white space, except space delimiting param name and value
		-lower case
*/
void DPTable::readParamsFile(const string & fileName)
{
	ifstream file(fileName);
	string line;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			int pos = line.find_first_of(' ');
			if (pos == string::npos)
			{
				cout << "Error: invalid scoring parameter file format! Refer to example file parameters.config." << endl << endl;
				scoreParams = { 1, -1, -2, -1 };
				break;
			}

			// pass in first half of line (param name, e.g. "match") followed by it's score, converted to int
			if (!scoreParams.setParamsFromInput(line.substr(0, pos), std::stoi(line.substr(pos + 1, line.length()))))
			{
				cout << "Error: invalid scoring parameter file format! Refer to example file parameters.config." << endl << endl;
				scoreParams = { 1, -1, -2, -1 };
				break;
			}
		}
	}
	else
	{
		cout << "Error: unable to open params file - scoring parameters set to defaults { 1, -1, -2, -1 }" << endl << endl;
		scoreParams = { 1, -1, -2, -1 };
		// set to default values
	}
}

/* 
	populate scores for each DPCell in table similar to table below
	init with low scores across 0th row and down 0th column as boundary

  j    0   1   2   3   4
i	   s2  A   C   G   T
   s1  ------------------
0	  |0  |<- neg inf ->|
	  |
1	A |^   1   0  -1  -2
	  || 
2	C |n   0   2   1   0
	  |e
3	G |g  -1   1   3   2
	  |i
4	T |n  -2   0   2   4
	  |f
	  ||
	  |v

	  
	Assumption: table.size() == s1.length() + 1
			&&  table[i].size() == s2.length() + 1
*/
void DPTable::populateTable()
{
	table[0][0].setScores(0, 0, 0);

	// set 0th column scores to "negative infinity"
	for (int i = 1; i < table.size(); i++)
	{
		table[i][0].setScores(NEG_INF, NEG_INF, NEG_INF);
	}

	// set 0th row scores to "negative infinity"
	for (int j = 1; j < table[0].size(); j++)
	{
		table[0][j].setScores(NEG_INF, NEG_INF, NEG_INF);
	}

	// populate table
	for (int i = 1; i < table.size(); i++)
	{
		for (int j = 1; j < table[i].size(); j++)
		{
			// get max and if match, add match point, else mismatch penalty
			table[i][j].setSub(table[i - 1][j - 1].getMaxScore()
				+ ( (s1[i-1] == s2[j-1]) ? scoreParams.match : scoreParams.mismatch ));
			table[i][j].setDel(table[i - 1][j].getMaxScore(DEL, scoreParams.gap, scoreParams.openGap));
			table[i][j].setIns(table[i][j - 1].getMaxScore(INS, scoreParams.gap, scoreParams.openGap));
		}
	}
}

/*
	push aligned chars to front of output strings
*/
void DPTable::writeAlignedStrings(char s1ch, char s2ch, char cnxnch)
{
	s1Out.insert(0, 1, s1ch);
	cnxnStr.insert(0, 1, cnxnch);
	s2Out.insert(0, 1, s2ch);
}

/* 
	trace optimal alignment path backwards from right most, bottom most cell,
	following path of maximum scores
*/
void DPTable::backTrace()
{
	int i = 0, j = 0, max = 0;
	dir_type dir = NULL_DIR;
	char tempCnxn;

	if (table.empty() || table[0].empty()) // assuming short circuit eval, else table[0] out of range
	{
		cout << "Error: empty DPTable!" << endl;
		return;
	}
	
	i = table.size() - 1;
	j = table[0].size() - 1;

	while (i > 0 && j > 0) // [0][0] == top left edge of table
	{
		dir = table[i][j].getDirection();

		switch (dir) {
			case SUB:
				if (s1[i - 1] == s2[j - 1]) // if match
				{
					tempCnxn = '|';
					numMatches++;
				}
				else
				{
					tempCnxn = ' ';
					numMismatches++;
				}
				writeAlignedStrings(s1[i - 1], s2[j - 1], tempCnxn);
				i--;
				j--;
				break;
			case DEL:
				writeAlignedStrings(s1[i - 1], '-', ' ');
				numGaps++;
				i--;
				break;
			case INS:
				writeAlignedStrings('-', s2[j - 1], ' ');
				numGaps++;
				j--;
				break;
			default:
				cout << "Error: something really stupid happened." << endl << endl;
		}
	}
}

void DPTable::printOutput() const
{
	// index position in output strings, and width of number for formatting
	int pos = 0, posWidth = std::to_string(s1Out.length()).length() + 1;
	// get max name length for output formatting
	int nameWidth = (s1Name.length() > s2Name.length()) ? s1Name.length() : s2Name.length();
	nameWidth += 1;


	ofstream outfile(s1Name + "_" + s2Name + "_output.txt");

	if (outfile.is_open())
	{
		while (pos < s1Out.length())
		{
			outfile << setw(nameWidth) << std::left << s1Name 
				<< setw(posWidth) << pos
				<< s1Out.substr(pos, CHARS_PER_LINE) << endl;
			outfile << setw(nameWidth + posWidth) << " " << cnxnStr.substr(pos, CHARS_PER_LINE) << endl;
			outfile << setw(nameWidth) << std::left << s2Name
				<< setw(posWidth) << pos
				<< s2Out.substr(pos, CHARS_PER_LINE) << endl << endl;
			pos += CHARS_PER_LINE;
		}


		outfile << "Scoring Parameters... match: " << scoreParams.match << ", mismatch: " << scoreParams.mismatch
			<< ", gap: " << scoreParams.gap << ", opening gap: " << scoreParams.openGap << endl << endl;
		outfile << "Global optimal score: " << table[table.size() - 1][table[0].size() - 1].getMaxScore() << endl << endl;
		outfile << "Number of... matches: " << numMatches << ", mismatches: " << numMismatches << ", gaps: " << numGaps
			<< endl << endl;
		outfile << ((double)numMatches / (double)s1Out.length()) * 100 << "% identical ("
			<< numMatches << "/" << s1Out.length() << ")" << endl << endl;

		outfile.close();
	}
	else
		cout << "Error: unable to open output file for writing." << endl;
}

// getter version of operator[]
const vector<DPCell> & DPTable::operator[](int row) const
{
	return table[row];
}