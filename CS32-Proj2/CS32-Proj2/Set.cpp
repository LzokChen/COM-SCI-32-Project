// Set.cpp

#include "Set.h"

Set::Set()
{
	//since the set list is empty, 
	//the next and prev pointers of m_dummy point to itself
	m_dummy.next = &m_dummy;
	m_dummy.prev = &m_dummy;
	m_size = 0;
}

int Set::size() const
{
	return m_size;
}

bool Set::empty() const
{
	return size() == 0;
}

bool Set::contains(const ItemType& value) const
{
	Node *p = m_dummy.next;

	//iterate thorugh every node in the list
	while (p != &m_dummy)	//if p is point to dummy, it means it reach the end of the list,						
	{						//or the list is empty
		if (p->val == value)
			return true;	//if we find the value
		p = p->next;
	}
	return false;	//if we do find the value
}

bool Set::insert(const ItemType& value)
{
	if (contains(value))	//if the set already contain the value
		return false;
	
	Node *temp = &m_dummy;	
	
	while (temp->next != &m_dummy && temp->next->val < value)
		temp = temp -> next;
		//the loop stop until temp reach the last node of the list
		//or the next node is the first node that its val larger than the value
		//so we can insert the value in order
		//the largest value is at the end of the list

	Node *p = new Node;		//allocate a new node
	p->val = value;			//save the value to the new node
	p->next = temp->next;	//set the new node's next
	p->prev = temp;			//set the new node's prev
	p->next->prev = p;		//change the next node's prev (point to the new node)
	p->prev->next = p;		//change the previous node's next (point to the new node)

	m_size++;			
	return true;
}

bool Set::erase(const ItemType& value)
{
	Node *killme = nullptr;
	Node *p = m_dummy.next;

	//iterate thorugh every node in the list
	while (p != &m_dummy)	//if p is point to dummy, it means it reach the end of the list,
							//or the list is empty
	{
		if (p->val == value)
		{
			killme = p;		//we find the node that contain the value we want to erase
			break;
		}
		p = p->next;
	}

	if (killme == nullptr)
		return false;		//we cannot the node that contain the value we want to erase
	else
	{
		killme->next->prev = killme->prev;	//let the next node's prev point to the previous node
		killme->prev->next = killme->next;	//let the previous node's next point to the next node
		delete killme;						//delete the node (and the value)
		m_size--;
		return true;						//we erased the value
	}
}

bool Set::get(int i, ItemType& value) const
{
	if (i < 0 || i>= m_size)		
		return false;			//if i is out of bound

	//since we insert the value in order, the value that larger than i values is sotre in the (i+1)th node (not include the dummy).
	Node *p = m_dummy.next;
	for (int k = 0; k < i; k++)
	{
		p = p->next;
	}
	value = p->val;
	return true;
}

void Set::swap(Set& other)
{
	Set temp = other;	//copy constructor
	other = *this;		//assignment operator
	*this = temp;		//assignment operator
}

Set::~Set()	//destructor
{
	Node *p = m_dummy.next;	//start from the node next to the dummy node
	while (p != &m_dummy)	//while p does not reach the end of the list(dummy)
	{
		Node *killme = p;
		p = killme->next;
		delete killme;
	}
}

Set::Set(const Set &other)	//copy consturctor
{
	m_dummy.next = &m_dummy;
	m_dummy.prev = &m_dummy;
	m_size = other.m_size;

	Node *other_temp = other.m_dummy.next;
	Node *temp = &m_dummy;

	for (int k = 0; k < m_size; k++)
	{
		Node *p = new Node;					//allocate a new node
		p->val = other_temp->val;			//copy the val of the node of other set to the new node of the current set
		p->next = temp->next;				//set the new node's next
		p->prev = temp;						//set the new node's prev
		p->next->prev = p;					//change the next node's prev (point to the new node)
		p->prev->next = p;					//change the previous node's next (point to the new node)

		temp = temp->next;
		other_temp = other_temp->next;		
	}
}

Set &Set::operator=(const Set& rhs)
{
	//check if the source variable is the target variable itself
	if (&rhs == this)
	{
		return *this;	//do nothing
	}
	else
	{
		//delete the old nodes first
		Node *d = m_dummy.next;	//start from the node next to the dummy node
		while (d != &m_dummy)	//while p does not reach the end of the list(dummy)
		{
			Node *killme = d;
			d = killme->next;
			delete killme;
		}


		//reset the dummy node
		m_dummy.next = &m_dummy;
		m_dummy.prev = &m_dummy;
		m_size = rhs.m_size;

		//copy all the rhs's nodes to current set
		Node *rhs_temp = rhs.m_dummy.next;
		Node *temp = &m_dummy;

		for (int k = 0; k < m_size; k++)
		{
			Node *p = new Node;					//allocate a new node
			p->val = rhs_temp->val;				//copy the val of the node of other set to the new node of the current set
			p->next = temp->next;				//set the new node's next
			p->prev = temp;						//set the new node's prev
			p->next->prev = p;					//change the next node's prev (point to the new node)
			p->prev->next = p;					//change the previous node's next (point to the new node)

			temp = temp->next;
			rhs_temp = rhs_temp->next;
		}
		
		return *this;
	}
}

void unite(const Set& s1, const Set& s2, Set& result)
{
	result = s1;	//use assignment operatior,  after that result only contain the values of s1

	for (int k = 0; k < s2.size(); k++)
	{
		ItemType temp;
		s2.get(k, temp);		//get value from s2
		result.insert(temp);	//insert the value into result if it does not exist
	}
}

void subtract(const Set& s1, const Set& s2, Set& result)
{
	result = s1;	//use assignment operatior,  after that result only contain the values of s1

	for (int k = 0; k < s2.size(); k++)
	{
		ItemType temp;
		s2.get(k, temp);		//get value from s2
		result.erase(temp);		//erase the value if it exist in result
	}
}