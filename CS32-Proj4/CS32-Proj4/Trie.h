#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>
#include <list>
using namespace std;

template<typename ValueType>
class Trie
{
public:
    Trie();
    ~Trie();
    void reset();
    void insert(const std::string& key, const ValueType& value);
    std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const;

      // C++11 syntax for preventing copying and assignment
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
private:
	struct Node
	{
		char label;
		list<ValueType> vals;
		vector<Node*> childrenList;
	};
	Node* root;
	//a function for delete the sub trie tree whoes root is pointed by cur Node pointer
	void FreeTree(Node *cur);	

	//a recursive function that is used to search for the value associated with a given key string
	//assume that the root node's label match the first char of the key.
	void findval(const string& key, bool exactMatchOnly, Node* root, int unmatchCounter, vector<ValueType>& result) const;
};



/////////////////////////////////////////////
template<typename ValueType>
Trie<ValueType>::Trie()
{
	root = new Node;		//crate a new root node that does not contain any values, children nodes or label
}

/////////////////////////////////////////////
template<typename ValueType>
Trie<ValueType>::~Trie()
{
	FreeTree(root);
}

/////////////////////////////////////////////
template<typename ValueType>
void Trie<ValueType>::reset()
{
	FreeTree(root);
	root = new Node;
}

/////////////////////////////////////////////
template<typename ValueType>
void Trie<ValueType>::insert(const std::string& key, const ValueType& value)
{
	Node* sub = root;
	for (int k = 0; k < key.size(); k++)
	{
		Node* temp = sub;
		for (int i = 0; i < sub->childrenList.size(); i++)	//traverse the childreList
		{
			if (sub->childrenList[i]->label == key[k])		//if we find the child Node we are looking for
			{
				sub = sub->childrenList[i];
				break;
			}
		}
		if (sub == temp)		//if we didn't find the child Node we are looking for
		{
			Node* NEW = new Node;
			NEW->label = key[k];
			sub->childrenList.push_back(NEW);
			sub = NEW;
		}
	}
	sub->vals.push_back(value);	//store the value in the appropriate node.
}


template<typename ValueType>
vector<ValueType> Trie<ValueType>::find(const std::string& key, bool exactMatchOnly) const
{
	vector<ValueType> result;
	if (key.size() == 0) return result;	//if the key is emply, return
	for (int i = 0; i < root->childrenList.size(); i++)
	{
		if (root->childrenList[i]->label == key[0])	//if the fisrt label match
			findval(key.substr(1), exactMatchOnly, root->childrenList[i], 0, result);	//call the findval function to check the rest of uncheck elements of the key
	}
	
	return result;
}

/////////////////////////////////////////////
template<typename ValueType>
void Trie<ValueType>::FreeTree(Node *cur)
{
	if (cur == nullptr) return;


	typename vector<Node*>::iterator it = cur->childrenList.begin();
	while (it != cur->childrenList.end())
	{
		FreeTree(*it);
		(cur->childrenList).erase(it);	//remove the node from the chilren list
		it = cur->childrenList.begin();
	}
	delete cur;	//delete the Node
}

/////////////////////////////////////////////
template<typename ValueType>
void Trie<ValueType>::findval(const string& key, bool exactMatchOnly, Node* root, int unmatchCounter, vector<ValueType>& result) const
{
	if (!exactMatchOnly && unmatchCounter > 1) return;	//if it is not exact_macth_Only and it already unmatched for more than 1 time, return

	if (key.size() == 0)	//if there is no more element in the key string we need to check, it means we matched all the elements of the key and reach the node that we need.
	{	
		for (typename list<ValueType>::iterator it = root->vals.begin(); it != root->vals.end(); it++)
			result.push_back(*it); //push all values into the vector
		return;
	}

	for (int i = 0; i < root->childrenList.size(); i++)	//traverse the childreList
	{
		string substring = key.substr(1);	//get the substring that contain the elements we need to match in the next trial.
		if (root->childrenList[i]->label == key[0])	//if the label match
			findval(substring, exactMatchOnly, root->childrenList[i], unmatchCounter, result);
		else if (!exactMatchOnly)	//if the label is not exactMatchOnly, increase the unmatch counter
			findval(substring, exactMatchOnly, root->childrenList[i], ++unmatchCounter, result);
	}


}
#endif // TRIE_INCLUDED

