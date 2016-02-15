#pragma once

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

// set this value such that it is always < adjacent cells
#define NEG_INF -999

enum dir_type {
	NULL_DIR,
	SUB,	// diagonal	(i-1, j-1)
	DEL,	// up		(i-1, j)
	INS		// left		(i, j-1)
};

// Simple 3-integer cell of which the DPTable is composed
class DPCell
{
public:
	explicit DPCell(int s = 0, int d = 0, int i = 0);
	~DPCell();

	// getters
	int getMaxScore(dir_type gapType = NULL_DIR, int gapPenalty = 0, int openGapPenalty = 0) const;
	dir_type getDirection() const;

	// setters
	void setScores(int s, int d, int i);
	void setSub(int s);
	void setDel(int d);
	void setIns(int i);

private:
	int subScore;
	int delScore;
	int insScore;
};