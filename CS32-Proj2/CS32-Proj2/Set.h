// Set.h

#ifndef SET_INCLUDED
#define SET_INCLUDED

#include <string>

using ItemType = std::string; 

class Set
{
public:
	Set();               // Create an empty set (i.e., one with no items).
	bool empty() const;  // Return true if the set is empty, otherwise false.
	int size() const;    // Return the number of items in the set.

	bool insert(const ItemType& value);
	// Insert value into the set if it is not already present.  Return
	// true if the value was actually inserted.  Leave the set unchanged
	// and return false if the value was not inserted 

	bool erase(const ItemType& value);
	// Remove the value from the set if present.  Return true if the
	// value was removed; otherwise, leave the set unchanged and
	// return false.

	bool contains(const ItemType& value) const;
	// Return true if the value is in the set, otherwise false.

	bool get(int i, ItemType& value) const;
	// If 0 <= i < size(), copy into value the item in the set that is
	// strictly greater than exactly i items in the set and return true.
	// Otherwise, leave value unchanged and return false.

	void swap(Set& other);
	// Exchange the contents of this set with the other one.

	
	~Set();
	//destructor

	Set(const Set& other);
	//copy constructor

	Set& operator=(const Set& rhs);
	//assignment operator

private:
	struct Node	//Node for circular doubly-linked list
	{
		ItemType val;		//store the value
		Node *prev;			//point to previous node
		Node *next;			//point to next node

	};
	int		m_size;						//number of items in the set
	Node	m_dummy;					//dummy node

};

void unite(const Set& s1, const Set& s2, Set& result);
void subtract(const Set& s1, const Set& s2, Set& result);


#endif // SET_INCLUDED
