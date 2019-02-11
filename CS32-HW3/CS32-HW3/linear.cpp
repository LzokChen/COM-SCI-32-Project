//linear.cpp


#include <iostream>
#include <string>
using namespace std;

bool anyTrue(const double a[], int n);
int countTrue(const double a[], int n);
int firstTrue(const double a[], int n);
int positionOfMin(const double a[], int n);
bool includes(const double a1[], int n1, const double a2[], int n2);

bool somePredicate(double x)
{
	return x > 0;
}

void main()
{
	int n = 5;
	double arr1[5] = {  .1, .2, .3, .4, .5 };
	double arr2[5] = { -.1,-.2,-.3,-.4,-.5 };
	double arr3[5] = { -1, 2, -3, -4, -5 };
	double arr4[5] = { 5, 4, -3, 2, 1 };
	double arr5[7] = { 10,50,40,20,50,40,30 };
	double arr6[3] = { 50,20,50 };
	double arr7[3] = { 50,30,20 };


	if (anyTrue(arr3, 5))
		cout << "True" << endl;
	else
		cout << "False" << endl;
		
	cout << "countTrue: " << countTrue(arr3, 5) << endl;
	cout << "firstTrue: " << firstTrue(arr3, 5) << endl;
	cout << "positionOfMin: " << positionOfMin(arr4, 5) << endl;

	if (includes(arr5, 7, arr6, 3))
		cout << "True" << endl;
	else
		cout << "False" << endl;
}

///////////////////////////////////////////////////

// Return true if the somePredicate function returns true for at
// least one of the array elements, false otherwise.
bool anyTrue(const double a[], int n)
{
	if (n <= 0) return false;
	if (n == 1) return somePredicate(a[0]);

	bool sub = anyTrue(a, n - 1);
	bool result = sub || somePredicate(a[n - 1]);
	return result;
}

// Return the number of elements in the array for which the
// somePredicate function returns true.
int countTrue(const double a[], int n)
{
	if (n <= 0) return 0;
	if (n == 1)
	{
		if (somePredicate(a[0])) return 1;
		else return 0;
	}
	
	int sub = countTrue(a, n - 1);
	int result = sub + somePredicate(a[n - 1]);
	return result;
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns true.  If there is no such
// element, return -1.
int firstTrue(const double a[], int n)
{
	if (n <= 0) return -1;
	if (n == 1)
	{
		if (somePredicate(a[0])) return 0;
		else return -1;
	}

	int sub = firstTrue(a, n - 1);
	int result;

	if (sub == -1)
	{
		if (somePredicate(a[n-1])) result = n-1;	//check the last double of the subset
		else result = sub; //-1
	}
	else result = sub;

	return result;
}

// Return the subscript of the smallest element in the array (i.e.,
// return the smallest subscript m such that a[m] <= a[k] for all
// k from 0 to n-1).  If the function is told to examine no 
// elements, return -1.
int positionOfMin(const double a[], int n)
{
	if (n <= 0) return -1;
	if (n == 1) return 0;

	int sub = positionOfMin(a, n - 1);
	int result;

	if (a[n - 1] < a[sub]) result = n - 1;
	else result = sub; 

	return result;
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
	if (n2 <= 0) return true;		//we searched all double in the a2
	if (n1 <= 0) return false;		//a1 is empty

	bool result;

	if (a1[0] == a2[0])
		result = includes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
	else
		result = includes(a1 + 1, n1 - 1, a2, n2);

	return result;
}