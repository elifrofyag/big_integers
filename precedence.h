#define PRECEDENCE_H
#include <vector>
#include <string>

#include "operation.h"

using namespace std;

// get precedence of operators
int precedence(char op);

// check if character is an operator
bool isOperator(char c);

// tokenize the expression to clear empty spaces and separate numbers and operators
vector<string> tokenize(const string& expr);

// infix to postfix after tokenization
vector<string> infixToPostfix(vector<string> tokens, bool &error);

// evaluate postfix expression
string evaluatePostfix(vector<string> postfix, bool &error);

// evaluate original arithmetic expression
string evaluateExpression(string expr, bool &error);