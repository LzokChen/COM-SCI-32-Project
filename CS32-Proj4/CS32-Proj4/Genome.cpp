#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
using namespace std;

class GenomeImpl
{
public:
    GenomeImpl(const string& nm, const string& sequence);
    static bool load(istream& genomeSource, vector<Genome>& genomes);
    int length() const;
    string name() const;
    bool extract(int position, int length, string& fragment) const;
private:
	string genomeName;
	string genomeSequence;
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
{
	genomeName = nm;
	genomeSequence = sequence;
}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes) 
{
	string temp;
	string name = "";
	string sequence = "";
	while (getline(genomeSource, temp))
	{	
		if (temp.size() == 0) return false;	//improper format: empty line.	Case 5

		if (temp[0] == '>')	//if it is an name line
		{
			if (temp.size() == 1) return false; //improper format: a line starting with a greater-than character but containing no other characters	Case 6
			if (name != "")	//if the name is not an empty string, it means there already has a temp Genome pending to store.
			{
				if (sequence == "")
					return false; //improper format: no base line after a name line, the pending Genome has a empty string	Case 4
				else
					genomes.push_back(Genome(name, sequence));	//store the pending Genome	
			}
				
			name = temp.substr(1);	//get the name of the new Genome
			sequence = "";			//empty the sequence string
		}
		else				//if it is a base line
		{
			if (name.size() == 0) return false;		//a file does not starting with a name line.	Case 1
			if (temp.size() > 80) return false;		//improper format: a non-name line has more than 80 chars	Case 2
			for (int i = 0; i < temp.size(); i++)
			{
				if (islower(temp[i]))				//convert lowercase char to uppercase
					temp[i] = toupper(temp[i]);
				if (temp[i] != 'A' && temp[i] != 'C' && temp[i] != 'T' && temp[i] != 'G' && temp[i] != 'N')	//improper format: non-name lines containg improper char.	Case 2
					return false;
				else sequence += temp[i];
			}
		}	
	}
	if(sequence !="")
		genomes.push_back(Genome(name, sequence));	//store the last pending Genome
	else return false;								//improper format: no base line after a name line, the pending Genome has a empty string	Case 4
	
	return true; //load some Genomes successfully.
 
}

int GenomeImpl::length() const
{
    return (int)genomeSequence.size();  
}

string GenomeImpl::name() const
{
    return genomeName;  
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
	
	if ((position + length) > GenomeImpl::length())
		return false;
	fragment = genomeSequence.substr(position, length);
	return true;
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
    m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
    delete m_impl;
}

Genome::Genome(const Genome& other)
{
    m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
    GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
    delete m_impl;
    m_impl = newImpl;
    return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes) 
{
    return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
    return m_impl->length();
}

string Genome::name() const
{
    return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
    return m_impl->extract(position, length, fragment);
}
