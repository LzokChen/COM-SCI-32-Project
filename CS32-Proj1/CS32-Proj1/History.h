//History.h
#include "globals.h"
#ifndef History_H
#define History_H

class History
{
public:
	// Constructor
	History(int nRows, int nCols);

	// Mutators
	bool record(int r, int c);
	void display() const;

private:
	int     m_rows;
	int     m_cols;
    char    m_grid[MAXROWS][MAXCOLS];
};
#endif // !History_H

