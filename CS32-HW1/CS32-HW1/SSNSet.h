//SSNSet.h
#ifndef SSNSet_H
#define SSNSet_H

#include "Set.h"	//SSNset declares a Set member

class SSNSet
{
public:
	SSNSet();          // Create an empty SSN set.

	bool add(unsigned long ssn);
	// Add an SSN to the SSNSet.  Return true if and only if the SSN
	// was actually added.

	int size() const;  // Return the number of SSNs in the SSNSet.

	void print() const;
	// Write every SSN in the SSNSet to cout exactly once, one per
	// line.  Write no other text.

private:
	Set m_ssnset;

};
#endif // !SSNSet_H

