#include "DPCell.h"

// constructor w/ initializer list, default values = 0
DPCell::DPCell(int s, int d, int i)
	: subScore(s),
	delScore(d),
	insScore(i)
{
}


DPCell::~DPCell()
{
}

/* 
	determines max score based on relation:
	where g = gapPenalty and h = openGapPenalty

	S(i, j) == sub score at table position (i, j)

	getMax = max score of { S(i, j), D(i, j), I(i, j) }

	S(i, j) = T(i-1, j-1) + match or mismatch

	D(i, j) = max of { D(i-1, j) + g,
					   S(i-1, j) + h + g,
					   I(i-1, j) + h + g }

	I(i, j) = max of { I(i, j-1) + g,
					   S(i, j-1) + h + g,
					   D(i, j-1) + h + g }
*/
int DPCell::getMaxScore(dir_type gapType, int gapPenalty, int openGapPenalty) const
{
	int currMax = subScore + gapPenalty + openGapPenalty;
	int tempDel = delScore + gapPenalty + ( (gapType == DEL) ? 0 : openGapPenalty );
	int tempIns = insScore + gapPenalty + ( (gapType == INS) ? 0 : openGapPenalty );

	currMax = (currMax > tempDel) ? currMax : tempDel;
	currMax = (currMax > tempIns) ? currMax : tempIns;

	return currMax;
}

/*
	returns direction to adjacent cell from which 
	this cell inherited its optimal score
	
	used to trace back optimal alignment
*/
dir_type DPCell::getDirection() const
{
	dir_type dir = SUB;
	int currMax = subScore;

	if (delScore > currMax)
	{
		currMax = delScore;
		dir = DEL;
	}
	
	if (insScore > currMax)
	{
		//currMax = insScore;
		dir = INS;
	}

	return dir;
}

// setters
void DPCell::setScores(int sub, int del, int ins)
{
	subScore = sub;
	delScore = del;
	insScore = ins;
}

void DPCell::setSub(int s)
{
	subScore = s;
}

void DPCell::setDel(int d)
{
	delScore = d;
}

void DPCell::setIns(int i)
{
	insScore = i;
}