//History.cpp
#include "globals.h"
#include "History.h"

///////////////////////////////////////////////////////////////////////////
//  Histroy implementations
///////////////////////////////////////////////////////////////////////////

History::History(int nRows, int nCols)
{
	if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
	{
		cout << "***** Arena created with invalid size " << nRows << " by "
			<< nCols << "!" << endl;
		exit(1);
	}
	m_rows = nRows;
	m_cols = nCols;
}

bool History::record(int r, int c)
{
	if (r > m_rows || c > m_cols)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void History::display() const
{
	// Position (row,col) of the arena coordinate system is represented in
	// the array element grid[row-1][col-1]
	char grid[MAXROWS][MAXCOLS];
	int r, c;

	// Fill the grid with dots
	for (r = 0; r < m_rows; r++)
		for (c = 0; c < m_cols; c++)
			grid[r][c] = '.';

	// Indicate each dead zombie's position
	for (int k = 0; k <  m_d_nZombies; k++)
	{
		const Zombie* zp = m_d_zombies[k];
		char& gridChar = grid[zp->row() - 1][zp->col() - 1];
		switch (gridChar)
		{
		case '.':  gridChar = 'A'; break;
		case 'Z':  break;
		default:   gridChar++; break;  // 'B' through 'Z'
		}
	}


	// Draw the grid
	clearScreen();
	for (r = 0; r < m_rows; r++)
	{
		for (c = 0; c < m_cols; c++)
			cout << grid[r][c];
		cout << endl;
	}
	cout << endl;

	// Write message
	cout << endl;

}