#include "provided.h"
#include "Trie.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <algorithm>

using namespace std;

bool compareGenomeMatch(const GenomeMatch & a, const GenomeMatch & b);

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
	struct MatchGenome
	{
		Genome* ptr;
		DNAMatch match;
	};
	vector<MatchGenome> matchGenomeVtr;	//this vector store all possible Match DNA's match info and its pointer

	if (fragment.size() < minimumLength || minimumLength < minSL) return false;	

	vector<DNAsequence> vecT = GenomeTrie.find(fragment.substr(0, minSL), true);
	vector<DNAsequence> vec = GenomeTrie.find(fragment.substr(0, minSL), exactMatchOnly);		//get the potential match genomes

	string subfragment = fragment.substr(minSL, fragment.size() - minSL);						//get the uncheck sub fragment

	if (exactMatchOnly && vec.size() == 0) return false;	//no potintial match genome
	for (vector<DNAsequence>::iterator it = vec.begin(); it != vec.end(); it++) //for each potential match genomes
	{
		int numUnmatched = 0;
		if (!exactMatchOnly)
		{
			numUnmatched = 1;
			for (vector<DNAsequence>::iterator itr = vecT.begin(); itr != vecT.end(); itr++)
			{
				if ((*itr).Gptr == (*it).Gptr && (*itr).position == (*it).position)
				{
					numUnmatched = 0;		//the potential match genomes is in te exactMatchOnly list, it means it has no Unmatched before
					break;
				}
			}
		}
		
		int matchlength = minSL;
		string subBases;
		int pos = ((*it).position + minSL);	//get the start prosition of substring of DNA sequence.
		if ((*it).Gptr->extract(pos, ((int)fragment.size() - minSL), subBases))	//get the substring of DNA sequence that pending to check
		{
			for (int i = 0; i < subBases.size(); i++)
			{
				if (subBases[i] == subfragment[i])
					matchlength += 1;
				else if (!exactMatchOnly && numUnmatched < 1)
				{
					matchlength += 1;
					numUnmatched += 1;
				}
				else break;
			}
		}

		if (matchlength >= minimumLength)
		{
			bool Notduplicate = true;
			for (vector<MatchGenome> ::iterator it2 = matchGenomeVtr.begin(); it2 != matchGenomeVtr.end(); it2++)
			{
				if ((*it2).ptr == (*it).Gptr)	//if we find a new duplicate matched DNA 
				{
					if ((*it2).match.length < matchlength)				//with larger length
					{
						(*it2).match.length = matchlength;				//replace the matchlength 
						(*it2).match.position = (*it).position;			//replace the position
					}
					Notduplicate = false;
				}
			}
			if (Notduplicate)
			{
				MatchGenome tempMatchGenome;
				tempMatchGenome.ptr = (*it).Gptr;
				tempMatchGenome.match.genomeName = (*it).Gptr->name();
				tempMatchGenome.match.length = matchlength;
				tempMatchGenome.match.position = (*it).position;
				matchGenomeVtr.push_back(tempMatchGenome);
			}
		}
	}

	for (vector<MatchGenome> ::iterator it3 = matchGenomeVtr.begin(); it3 != matchGenomeVtr.end(); it3++)
		matches.push_back((*it3).match);
	return true;

}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
	if (fragmentMatchLength < minSL) return false;				//invalid fragmentmatchlength

	int numSequence = query.length() / fragmentMatchLength;		//num of sequecne of query string
	map<string, int> countersMap;								//counter map

	for (int i = 0; i < numSequence; i ++)
	{
		vector<DNAMatch> Matchs;
		string subSequence;
		query.extract(i * fragmentMatchLength, fragmentMatchLength, subSequence);
		findGenomesWithThisDNA(subSequence, fragmentMatchLength, exactMatchOnly, Matchs);
		for (vector<DNAMatch>::iterator it = Matchs.begin(); it != Matchs.end(); it++)
			countersMap[(*it).genomeName] += 1;
	}

	for (map<string, int>::iterator it = countersMap.begin(); it != countersMap.end(); it++)
	{
		double p = (double)(*it).second / numSequence * 100;
		if (p > matchPercentThreshold)
		{
			GenomeMatch temp;
			temp.genomeName = (*it).first;
			temp.percentMatch = p;
			results.push_back(temp);
		}
	}

	sort(results.begin(), results.end(), compareGenomeMatch);
	if (countersMap.size() > 0) return true;
    
    return false;
}

//sort rule for GenomeMatch
bool compareGenomeMatch(const GenomeMatch & a, const GenomeMatch & b)
{
	if (a.percentMatch != b.percentMatch)
		return (a.percentMatch > b.percentMatch);	// ordered in descending order by the match proportion p
	else
		return (a.genomeName < b.genomeName);		//breaking ties by the genome name in ascending alphabetical order
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
