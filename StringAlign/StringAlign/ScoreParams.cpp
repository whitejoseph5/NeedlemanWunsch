#include "ScoreParams.h"

int ScoreParams::setParamsFromInput(const string & param, int score)
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

void ScoreParams::setToDefault()
{
	match = 1;
	mismatch = -1;
	openGap = -2;
	gap = -1;
}

int ScoreParams::getMatch() const
{
	return match;
}

int ScoreParams::getMismatch() const
{
	return mismatch;
}

int ScoreParams::getOpenGap() const
{
	return openGap;
}

int ScoreParams::getGap() const
{
	return gap;
}