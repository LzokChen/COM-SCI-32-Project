//Set.cpp
#include "Set.h" //we define Set::Set, etc.

#include <iostream>
using namespace std;

///////////////////////////////////////////////////////////////////////////
//  Set implementations
///////////////////////////////////////////////////////////////////////////


Set::Set()
{
	m_size = 0;
}

bool Set::empty() const 
{
	return(m_size == 0);
}

int Set::size() const
{
	return m_size;
}

bool Set::insert(const ItemType& value)
{
	//check if the set is full or already contains the value
	if (m_size < DEFAULT_MAX_ITEMS && this->contains(value) == false)	
	{
		m_items[m_size] = value;
		m_size++;
		return true;
	}
	else
	{
		return false;	//the set is full or the value is already in the set
	}
}

bool Set::erase(const ItemType& value)
{ 
	int index = -1; //defalut -1
	//find the location of the value that we want to erase
	for (int k = 0; k < m_size; k++)
	{
		if (m_items[k] == value)
		{
			index = k;
			break;
		}
	}

	if (index != -1)	
	{
		m_size--;

		//move the values(after the index -- the erased value) one position ahead
		for ( ; index < m_size; index++)
		{
			m_items[index] = m_items[index + 1];
		}

		return true;
	}
	else
	{
		return false;	//the value is not in the set
	}
}

bool Set::contains(const ItemType& value) const
{
	
	for (int k = 0; k < m_size; k++)
	{
		if (m_items[k] == value)
		{
			return true;
		}
	}

	return false;
}

bool Set::get(int i, ItemType& value) const
{
	if (0 <= i || i < m_size)
	{
		for (int k = 0; k < m_size; k++)	//for each values in the set
		{
			int counter = 0;
			for (int j = 0; j < m_size; j++)	//compare with each values in the set
			{
				if (m_items[k] > m_items[j])
				{
					counter++;
				}
			}

			if (counter == i)
			{
				value = m_items[k];
				break;
			}
		}
		return true;
	}
	else 
	{
		return false;
	}
}

void Set::swap(Set& other)
{
	int maxlength;
	if (m_size >= other.m_size)
	{
		maxlength = m_size;
	}
	else
	{
		maxlength = other.m_size;
	}

	//swap the elements of m_items
	for (int k = 0; k < maxlength; k++)
	{
		ItemType temp = other.m_items[k];
		other.m_items[k] = m_items[k];
		m_items[k] = temp;
	}

	//swap the m_size
	int tempsize = other.m_size;
	other.m_size = m_size;
	m_size = tempsize;

}