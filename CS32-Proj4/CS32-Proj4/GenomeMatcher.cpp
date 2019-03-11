#include "provided.h"
#include "Trie.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <list>
using namespace std;

class GenomeMatcherImpl
{
public:
    GenomeMatcherImpl(int minSearchLength);
    void addGenome(const Genome& genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
	int minSL;
	struct DNAsequence
	{
		Genome* Gptr;
		int position;

	};
	list<Genome> GenomeList;
	Trie<DNAsequence> GenomeTrie;

};

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
{
	minSL = minSearchLength;
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
	DNAsequence temp;
	GenomeList.push_back(Genome(genome));
	temp.Gptr = &GenomeList.back();
	
	int num_subsequence = temp.Gptr->length() - minSL + 1;

	for (int pos = 0; pos < num_subsequence; pos++)
	{
		string label;
		temp.Gptr->extract(pos, minSL, label);
		temp.position = pos;

		GenomeTrie.insert(label, temp);
	}
}

int GenomeMatcherImpl::minimumSearchLength() const
{
    return minSL;  
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    return false;  // This compiles, but may not be correct
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return false;  // This compiles, but may not be correct
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
    m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
    delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
    m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
    return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}
