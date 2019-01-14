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
    
    // Fill the history grid with dots
    for (r = 0; r < m_rows; r++)
        for (c = 0; c < m_cols; c++)
            grid[r][c] = '.';
}

bool History::record(int r, int c)
{
    if (r < 0 ||r > m_rows || c < 0 ||c > m_cols)
    {
        return false;
    }
    else
    {
        //save the dead zombie postion
        switch (grid[r][c])
        {
            case '.':  grid[r][c] = 'A'; break;
            case 'Z':  break;
            default:   grid[r][c]++; break;  // 'B' through 'Z'
        }
        return true;
    }
}

void History::display() const
{
    
    // Draw the grid
    clearScreen();
    for (r = 0; r < m_rows; r++)
    {
        for (c = 0; c < m_cols; c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;
    
}


	// Draw the grid
	clearScreen();/Users/Lzok/.Trash/History.cpp
	for (r = 0; r < m_rows; r++)
	{
		for (c = 0; c < m_cols; c++)
			cout << grid[r][c];
		cout << endl;
	}
	cout << endl;

}
