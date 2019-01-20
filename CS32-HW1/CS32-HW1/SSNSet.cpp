//SSNSet.cpp
#include "SSNSet.h" //we define SSNSet::SSNSet, etc.
#include"Set.h"	//we used Set::insert, etc.

#include <iostream>
using namespace std;

///////////////////////////////////////////////////////////////////////////
//  SSNSet implementations
///////////////////////////////////////////////////////////////////////////


SSNSet::SSNSet()
{

}

bool SSNSet::add(unsigned long ssn)
{
	return m_ssnset.insert(ssn);
}

int SSNSet::size() const
{
	return m_ssnset.size();
}

void SSNSet::print() const
{
	for (int k = 0; k < m_ssnset.size(); k++)
	{
		unsigned long temp;
		m_ssnset.get(k, temp);
		cout << temp << endl;
	}
}

