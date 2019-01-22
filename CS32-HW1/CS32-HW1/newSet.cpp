//newSet.cpp
#include "newSet.h" //we define Set::Set, etc.

#include <iostream>
using namespace std;

///////////////////////////////////////////////////////////////////////////
//  newSet implementations
///////////////////////////////////////////////////////////////////////////


Set::Set()
{
	m_size = 0;
	m_max = DEFAULT_MAX_ITEMS;		
	m_items = new ItemType[m_max];
}

Set::Set(int max)
{
	if (max < 0)
	{
		cout << "invaild max (the maximum capacity of a Set cannot be negative)." << endl;
		exit(-1);
	}
	else
	{
		m_size = 0;
		m_max = max;
		m_items = new ItemType[m_max];
	}
}

Set::~Set()
{
	delete [] m_items;
}

Set::Set(const Set &old)
{
	m_size = old.m_size;	//copy the number of items
	m_max = old.m_max;		//copy the maximum value of size
	m_items = new ItemType[m_max];
	for (int k = 0; k < m_size; k++)
	{
		m_items[k] = old.m_items[k];	//copy all the value
	}
}

Set &Set::operator=(const Set &source)
{
	//check if the source variable is the target variable itself
	if (&source == this)
	{
		return *this;	//do nothing
	}
	else
	{
		delete[] m_items; //free the memory currenty held by the target variable
	
		m_size = source.m_size;
		m_max = source.m_max;	//Determine how much memory is used by the source varialbe
		m_items = new ItemType[m_max];	//allocate enough storage
		for (int k = 0; k < m_size; k++)
		{
			m_items[k] = source.m_items[k];	//copy all the value
		}

		return *this;
	}
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
	if (m_size < m_max && this->contains(value) == false)
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
	Set temp = other;	//copy constructor
	other = *this;		//assignment operator
	*this = temp;		//assignment operator
}