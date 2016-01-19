#pragma once

#include <string>
using std::string;

class ScoreParams
{
public:
	// return value 0 denotes failure
	int setParamsFromInput(const string & param, int score)
	{
		int success = 1;
		if (param.compare("match") == 0)
			match = score;
		else if (param.compare("mismatch") == 0)
			mismatch = score;
		else if (param.compare("h") == 0)
			openGap = score;
		else if (param.compare("g") == 0)
			gap = score;
		else
			success = 0;
		return success;
	}

	int match;
	int mismatch;
	int openGap;
	int gap;
};