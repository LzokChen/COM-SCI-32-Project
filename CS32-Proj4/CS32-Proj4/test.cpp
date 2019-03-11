#include "Trie.h"
#include "provided.h"
#include <iostream>
#include <istream>
#include <vector>
#include <fstream>
using namespace std;

void print(vector<int> v)
{
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
	{
		cout << (*it)<<" ";
	}
}

int main()
{
	Trie<int> tr;
	tr.insert("ABCC", 1);
	tr.insert("ABCC", 2);
	tr.insert("ABCC", 3);
	tr.insert("ABCC", 1);
	tr.insert("ACCC", 4);
	tr.insert("ACCC", 5);
	tr.insert("ABBC", 6);
	tr.insert("ABCD", 7);
	tr.insert("CCCC", 13);


	print(tr.find("ABCC", true));
	cout << endl;
	print(tr.find("ABCC", false));
	cout << endl;

	string file1 = "./data/Halorientalis_persicus.txt";
	string file2 = "./data/test.txt";
	string s;

	//////////////////////////////////////////////////////

	ifstream infile(file2);
	vector<Genome> vg;
	//bool Genome::load(istream& genomeSource, vector<Genome>& genomes) 
	bool success = Genome::load(infile, vg);

	if (success)
	{
		cout << "Loaded " << vg.size() << " genomes successfully:" << endl;
		for (int k = 0; k != vg.size(); k++)
			cout << vg[k].name() << endl;
	}
	else
		cout << "Error loading genome data" << endl;

	//string ff = "XXX";
	//bool result = vg[0].extract(0, 80, ff);
	//if (result) cout << ff << endl;

	Genome g("oryx","GCTCGGNACACATCCGCCGCGGACGGGACGGGATTCGGGCTGTCGATTGTCTCACAGATCGTCGACGTACATGACTGGGA");
	string f1, f2, f3 = {"xxx"};
	
	bool result1 = g.extract(0, 5, f1); // result1 = true, f1 = ¡°GCTCG¡±;
	bool result2 = g.extract(74, 6, f2); // result2 = true, f2 = ¡°CTGGGA¡±;
	bool result3 = g.extract(74, 7, f3); // result3 = false, f3 is unchanged

	if (result1) cout << f1 << endl;
	if (result2) cout << f2 << endl;
	if (!result3) cout << f3 << endl;
	/////////////////////////////////////////////////////////////
	GenomeMatcher(3);
	Genome a("Genome 1", "ACTG");
	Genome b("Genome 2", "TCGACT");
	Genome c("Genome 3", "TCTCG");
	GenomeMatcher GM(3);
	GM.addGenome(a);
	GM.addGenome(b);
	GM.addGenome(c);
}
