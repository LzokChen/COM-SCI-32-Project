//History.cpp
#include "History.h"

///////////////////////////////////////////////////////////////////////////
//  Histroy implementations
///////////////////////////////////////////////////////////////////////////

History::History(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    
    // Fill the history grid with dots
    for (int r = 0; r < m_rows; r++)
        for (int c = 0; c < m_cols; c++)
            m_grid[r][c] = '.';
}

bool History::record(int r, int c)
{
    if (r < 0 ||r > m_rows || c < 0 ||c > m_cols)
    {
        return false;	//if r and c are not within bounds, return false
    }
    else
    {
        //save the dead zombie postion / change the history grid content
		int row = r - 1;
		int	col = c - 1;

        switch (m_grid[row][col])	//row 1 col 1 is in the m_grid[0][0]
        {
            case '.':  m_grid[row][col] = 'A'; break;
            case 'Z':  break;
            default:   m_grid[row][col]++; break;  // 'B' through 'Z'
        }
        return true;
    }
}

void History::display() const
{
    // Draw the grid
    clearScreen();
    for (int r = 0; r < m_rows; r++)
    {
        for (int c = 0; c < m_cols; c++)
            cout << m_grid[r][c];
        cout << endl;
    }
    cout << endl;
}
