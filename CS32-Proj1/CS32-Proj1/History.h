//History.h

#include "Zombie.h"
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
	Zombie* m_d_zombies[MAXZOMBIES];
	int     m_d_nZombies;
};
#endif // !History_H

