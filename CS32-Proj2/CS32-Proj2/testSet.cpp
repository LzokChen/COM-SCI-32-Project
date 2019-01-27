//testSet.cpp
#include "Set.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
	string temp;

	Set ss1;
	ss1.insert("aaa");
	ss1.insert("ccc");
	ss1.insert("bbb");

	Set ss2;
	ss2.insert("fff");
	ss2.insert("ggg");

	//test copy constructor
	Set ss3 = ss1;
	assert(ss3.size() == 3);			      
	assert(ss3.contains("bbb"));		
	assert(ss3.get(1, temp) && temp =="bbb");

	//test assignment operator
	ss1 = ss2;
	assert(ss1.size() == 2);
	assert(!ss1.contains("bbb"));
	assert(ss1.get(1, temp) && temp == "ggg");

	//test swap
	ss1.swap(ss3);
	assert(ss3.size() == 2 && ss1.size()==3);
	assert(ss3.contains("ggg")&& ss1.contains("aaa"));

}

int main()
{
	test();
	cout << "Passed all tests" << endl;
}