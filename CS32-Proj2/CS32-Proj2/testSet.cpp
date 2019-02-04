//testSet.cpp
#include "Set.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
	Set res;
	res.insert("r");

	Set ss1;
	ss1.insert("a");
	ss1.insert("b");
	
	Set ss2;
	ss2.insert("b");
	ss2.insert("c");

	//test function unite
	unite(ss1, ss2, res);		//res should only contains "a" "b" and "c"
	assert(!res.contains("r"));
	assert(res.contains("a")&&res.contains("b")&&res.contains("c"));

	subtract(ss1, ss2, res);	//res should only contains "a"
	assert(!res.contains("b") && !res.contains("c"));
	assert(res.contains("a"));

}

int main()
{
	test();
	cout << "Passed all tests" << endl;
}