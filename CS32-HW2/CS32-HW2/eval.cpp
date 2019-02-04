//eval.cpp
#include "Set.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;


bool IfValid(string infix);
//check if infix is syntactically valid

bool precendence(char low, char high);
//compare two operator's precendences
//return true if low's precendence is lower or equal to high's precendence

void InfixToPostfix(string infix, string& postfix);
//convert infix to postfix

void evaluation(const Set& trueValues, const Set& falseValues, string& postfix, bool& result);
//evaluate the posfix;


int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result)
{
	if (!IfValid(infix)) return 1;	//the infix is not syntactically valid

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
	InfixToPostfix(infix, postfix);
	evaluation(trueValues, falseValues, postfix, result);
	return 0;
}

bool IfValid(string infix)
{
	int open_p = 0;		// the number of open parentheses
	int close_p = 0;	// the number of close parentheses
	int size = infix.length();

	for (int k = 0; k < size; k++)
	{
		if (!islower(infix[k]))	//if it it not lower case letter
		{
			switch (infix[k])
			{
			case '(':	//if it is '('
				open_p++;
				break;
			case ')':	//if it is ')'	
				close_p++;
				break;
			case '!': case '&': case '|': case ' ':	//if it is an operator or blank
				break;
			default:
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
void InfixToPostfix(string infix, string& postfix)
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
		case ' ':					//if it is blank
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
	Set True;
	True.insert('a');
	True.insert('c');
	True.insert('l');
	True.insert('u');

	Set False;
	False.insert('n');
	False.insert('s');
	False.insert('x');

	string inf = "a & !s";
	string posf;
	bool answer = false;
	cout<<"return "<<evaluate(inf, True, False, posf, answer)<<endl;
	if (answer)
		cout << "the result of " << inf << " is true" << endl;
	else
		cout << "the result of " << inf << " is false" << endl;
}