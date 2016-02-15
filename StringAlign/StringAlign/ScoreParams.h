#pragma once

#include <string>
using std::string;

class ScoreParams
{
public:
	/*
	sets score parameter based on param, regardless of order in file
	return value 0 denotes failure
	*/
	int setParamsFromInput(const string & param, int score);

	void setToDefault();

	// accessors
	int getMatch() const;
	int getMismatch() const;
	int getOpenGap() const;
	int getGap() const;

private:
	int match;
	int mismatch;
	int openGap;
	int gap;
};