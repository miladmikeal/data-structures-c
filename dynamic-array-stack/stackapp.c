/*	stack.c: Stack application. */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "dynArray.h"

/****************************************************************
Using stack to check for unbalanced parentheses.
***************************************************************** */

/* Returns the next character of the string, once reaches end return '0' (zero)
	param: 	s pointer to a string 	
*/
char nextChar(char *s)
{
	static int i = -1;
	char c;
	++i;
	c = *(s + i);
	if (c == '\0')
		return '\0';
	else
		return c;
}

/* Helper function for isBalanced
 * Checks for matching opening & closing symbols
 */
int matches(char open, char close)
{
	char openers[] = {'(', '{', '['};
	char closers[] = {')', '}', ']'};
	int openIdx, closeIdx;
	openIdx = closeIdx = 0;
	for (int i = 0; i < 3; i++)
	{
		if (openers[i] == open)
		{
			openIdx = i;
		}
		if (closers[i] == close)
		{
			closeIdx = i;
		}
	}
	return openIdx == closeIdx;
}

/* Checks whether the (), {}, and [] are balanced or not
	param: 	s pointer to a string 	
	pre: 	
	post:	
*/
int isBalanced(char *s)
{
	DynArr *stack = NULL;
	stack = newDynArr(10);
	int bal = 1;
	char top;
	char current = nextChar(s);
	// Loop until end of string
	while (current != '\0' && bal)
	{
		// Skip to next character if current is not open/close symbol
		if (current != '(' && current != '{' && current != '[' &&
				current != ')' && current != '}' && current != ']')
		{
			current = nextChar(s);
			continue;
		}
		// Push opening symbols onto stack
		if (current == '(' || current == '{' || current == '[')
		{
			pushDynArr(stack, current);
		}
		else
		{
			// If stack is empty at this point, stack is unbalanced
			if (isEmptyDynArr(stack))
			{
				bal = 0;
			}
			else
			{
				top = topDynArr(stack);
				popDynArr(stack);
				// Use matches to check if top of stack matches current
				if (!matches(top, current))
				{
					bal = 0;
				}
			}
		}
		current = nextChar(s);
	}
	if (isEmptyDynArr(stack) && bal)
	{
		deleteDynArr(stack);
		return 1;
	}
	else
	{
		deleteDynArr(stack);
		return 0;
	}
}

int main(int argc, char *argv[])
{
	// Prompt error if command line arg not given
	if (argc != 2)
	{
		printf("ERROR: Please enter a string command line argument.\n");
		return 1;
	}
	char *s = argv[1];
	int res;

	printf("Assignment 2\n");

	res = isBalanced(s);

	if (res)
		printf("The string %s is balanced\n", s);
	else
		printf("The string %s is not balanced\n", s);

	return 0;
}
