#include "SSNSet.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int main()
{
	SSNSet ssns;
	cout << "There are " << ssns.size() <<" SSN(s) in the list."<< endl<<endl;
	ssns.add(111223333);
	ssns.add(444221111);
	ssns.add(222335555);
	cout << "===List===" << endl;
	ssns.print();
	cout << "==========" << endl;
	cout << "There are " << ssns.size() << " SSN(s) in the list." << endl << endl;
}