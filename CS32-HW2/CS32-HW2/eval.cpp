//eval.cpp
#include "Set.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

void deleteAllBlank(string &s);
//delete all blanks

bool ifValid(string infix);
//check if infix is syntactically valid

bool precendence(char low, char high);
//compare two operator's precendences
//return true if low's precendence is lower or equal to high's precendence

void infixToPostfix(string infix, string& postfix);
//convert infix to postfix

void evaluation(const Set& trueValues, const Set& falseValues, string& postfix, bool& result);
//evaluate the posfix;


int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result)
{
	deleteAllBlank(infix);
	//cout << infix << endl;

	if (!ifValid(infix)) return 1;	//the infix is not syntactically valid

	int size = infix.length();
	for (int k = 0; k < size; k++)
	{
		if (islower(infix[k]))
		{
			//a letter in the expression is in neither the trueValues nor the falseValues sets
			if (!trueValues.contains(infix[k]) && !falseValues.contains(infix[k]))
				return 2;
			//a letter in the expression is in both the trueValues and the falseValues sets
			if (trueValues.contains(infix[k]) && falseValues.contains(infix[k]))
				return 3;
		}
	}

	// if the function reach here, we knew that the infix syntactically valid, it only contains the chr that is operand, '(', ')' or operator 
	infixToPostfix(infix, postfix);
	evaluation(trueValues, falseValues, postfix, result);
	return 0;
}

void deleteAllBlank(string &s)
{
	while (1)
	{
		unsigned int pos = s.find(' ');
		if (pos == string::npos)
		{
			return;
		}
		s.erase(pos, 1);
	}
}


bool ifValid(string infix)
{
	int open_p = 0;		// the number of open parentheses
	int close_p = 0;	// the number of close parentheses
	int size = infix.length();

	if (size == 0) return false; //the infix is empty

	for (int k = 0; k < size; k++) 
	{
		if (islower(infix[k])) //if it is an operend (lower case letter)
		{
			//if there are two consecutive operend, return false
			if (k - 1 >= 0)
				if (islower(infix[k-1]))
					return false;
			if (k + 1 < size)
				if (islower(infix[k+1]))
					return false;
		}
		else					//if it is not an operend £¨lower case letter£©
		{
			switch (infix[k])
			{
			case '(':	//if it is '('
				if (k - 1 >= 0)	
					if (islower(infix[k - 1]))
						return false;	//if it has a operend on the left of open parenthesis
				if (k + 1 >= size)
					return false;		//if it if it does not exist infix[k+1]
				else if (infix[k + 1] == ')')
					return false;			//if it has a close parenthesis on the right of open parenthesis

				open_p++;
				break;

			case ')':	//if it is ')'	
				if (k + 1 < size)
					if (islower(infix[k + 1]))
						return false;	//if it has a operend on the right of close parenthesis
				if (k - 1 < 0)
					return false;		//if it if it does not exist infix[k-1]
				else if (infix[k - 1] == '(')
					return false;			//if it has a open parenthesis on the left of open parenthesis

				close_p++;
				break;

			case '!':	//if it is an unary boolean operator '!'
				if (k + 1 >= size)
					return false; //if it does not exist infix[k+1]
				break;

			case '&': case '|':	//if it is an binary boolean operator '&' or '|'
				if (k - 1 < 0 || k + 1 > size)
					return false;	//if it does not exist infix[k-1] or infix[k+1]
				else if (!islower(infix[k - 1]) && infix[k - 1] != ')')
					return false;	//if infix[k-1] or infix [k+1] is not a operend
				else if (!islower(infix[k + 1]) && infix[k + 1] != '!' && infix[k + 1] != '(')
					return false;
				break;

			default:	//if it is neither an operend nor a boolean operator
				return false;		
				break;
			}
		}
	}
	if (open_p == close_p) return true;
	else return false;
}

bool precendence(char low, char high)
{
	// ! > & > |

	if (low == high)	//two operator have same precendence
		return true;
	else if (low == '|' && high == '&')		// & > |
		return true;
	else if (low == '|' && high == '!')		// ! > |
		return true;
	else if (low == '&' && high == '!')		// ! > &
		return true;
	else return false;
}

//we assume that the infix syntactically valid, it only contains the chr that is operand, '(', ')' or operator 
void infixToPostfix(string infix, string& postfix)
{
	postfix = "";
	stack<char> operator_stack;

	int size = infix.length();
	
	for (int k = 0; k < size; k++)
	{
		switch (infix[k])
		{

		default:					//if it is a operand (by defualt)
			postfix += infix[k];
			break;

		case '(':					//if it is an open open parenthesis
			operator_stack.push(infix[k]);
			break;

		case ')':					//if it is an open close parenthesis
			while (operator_stack.top() != '(') // pop stack until matching '('
			{
				postfix += operator_stack.top();
				operator_stack.pop();
			}
			operator_stack.pop();			// remove the '('
			break;

		case '!': case'&': case'|':	//if it is an operator
			while (!operator_stack.empty() && operator_stack.top() != '(' && precendence(infix[k], operator_stack.top()))
			{
				postfix += operator_stack.top();
				operator_stack.pop();
			}
			operator_stack.push(infix[k]);
			break;
		}
	}

	while (!operator_stack.empty())
	{
		postfix += operator_stack.top();
		operator_stack.pop();
	}
}

void evaluation(const Set& trueValues, const Set& falseValues, string& postfix, bool& result)
{
	stack<bool> operand_stack;
	int size = postfix.length();

	for (int k = 0; k < size; k++)
	{
		if (islower(postfix[k]))	//if it is an operand (low case char)
		{
			if (trueValues.contains(postfix[k]))
				operand_stack.push(true);
			else operand_stack.push(false);
		}
		else if (postfix[k]=='!')				//if it is an unary boolean operator
		{
			bool operand1 = operand_stack.top();
			operand_stack.pop();
			operand_stack.push(!operand1);
		}
		else									//if it is a binary operator
		{
			bool operand2 = operand_stack.top();
			operand_stack.pop();
			bool operand1 = operand_stack.top();
			operand_stack.pop();

			if(postfix[k] == '&')
				operand_stack.push(operand1 && operand2);
			if(postfix[k] == '|')
				operand_stack.push(operand1 || operand2);
		}
	}

	result = operand_stack.top();
}


int main()
{
	string trueChars = "tywz";
	string falseChars = "fnx";
	Set trues;
	Set falses;
	for (int k = 0; k < trueChars.size(); k++)
		trues.insert(trueChars[k]);
	for (int k = 0; k < falseChars.size(); k++)
		falses.insert(falseChars[k]);

	string pf;
	bool answer;
	assert(evaluate("w| f", trues, falses, pf, answer) == 0 && pf == "wf|" &&  answer);
	assert(evaluate("y|", trues, falses, pf, answer) == 1);
	assert(evaluate("n t", trues, falses, pf, answer) == 1);
	assert(evaluate("nt", trues, falses, pf, answer) == 1);
	assert(evaluate("()", trues, falses, pf, answer) == 1);
	assert(evaluate("y(n|y)", trues, falses, pf, answer) == 1);
	assert(evaluate("t(&n)", trues, falses, pf, answer) == 1);
	assert(evaluate("(n&(t|7)", trues, falses, pf, answer) == 1);
	assert(evaluate("", trues, falses, pf, answer) == 1);
	assert(evaluate("f  |  !f & (t&n) ", trues, falses, pf, answer) == 0
		&& pf == "ff!tn&&|" && !answer);
	assert(evaluate(" x  ", trues, falses, pf, answer) == 0 && pf == "x" && !answer);
	trues.insert('x');
	assert(evaluate("((x))", trues, falses, pf, answer) == 3);
	falses.erase('x');
	assert(evaluate("((x))", trues, falses, pf, answer) == 0 && pf == "x"  &&  answer);
	trues.erase('w');
	assert(evaluate("w| f", trues, falses, pf, answer) == 2);
	falses.insert('w');
	assert(evaluate("w| f", trues, falses, pf, answer) == 0 && pf == "wf|" && !answer);
	cout << "Passed all tests" << endl;
}