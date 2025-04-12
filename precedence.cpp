#include "header/precedence.h"
#include "header/operation.h"

#include <stack>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


// get precedence of operators
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// check if character is an operator
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// tokenize the expression to clear empty spaces and separate numbers and operators
vector<string> tokenize(const string& expr) {
    vector<string> tokens;
    string num = "";
    
    int i = 0;

    //handle unary minus at beginning of epression
    while (isspace(expr[i])) i++;
    if(expr[i] == '-'){
        num += '-';
        i++;
    }
    
    //tokenize the rest
    while(i < expr.length()){
        char ch = expr[i];
        i++;
        if (isspace(ch)) continue;

        if (isdigit(ch)) {
            num += ch;
        } else {
            if (!num.empty()) {
                tokens.push_back(num);
                num.clear();
            }
            //handle unary minus after '(' e.g. 5 - (  -3) + 2
            if(ch == '('){
                int count = i;
                while(isspace(expr[count])) count++;
                if (expr[count] == '-') {
                    tokens.push_back(string(1, ch));
                    num += '-';
                    i = count + 1;
                    continue;
                }
            }
            if (ch == '(' || ch == ')' || isOperator(ch)) {
                tokens.push_back(string(1, ch));
            }
        }
        
    }

    if (!num.empty()) tokens.push_back(num);
    return tokens;
}

// infix to postfix after tokenization
vector<string> infixToPostfix(vector<string> tokens, bool &error) {
    error = false;
    stack<char> operators;
    vector<string> postfix;

    bool expectNumber = true;
    bool unaryMinus = false; //distinguish btw subtraction operator and a unary minus.

    for (const string &token : tokens) { //const string& token to avoid copying instead reference to original
        if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1 && isdigit(token[1]))) { //handle negative numbers like (-42)
            
            if (!expectNumber) {
                error = true;
                return {"Invalid expression"};
            }
            postfix.push_back(token);
            expectNumber = false;

        } else if (token == "(") {
            if (!expectNumber) {
                expectNumber = true;
                return {"Invalid expression"};
            }
            operators.push('(');
            expectNumber = true;

        } else if (token == ")") {
            if (expectNumber) {
                error = true;
                return {"Invalid expression"};
            }
            while(!operators.empty() && operators.top() != '(') {
                postfix.push_back(string(1, operators.top()));
                operators.pop();
            }
            if (!operators.empty() && operators.top() == '(') {
                operators.pop();
            } else {
                error = true;
                return {"Mismatched parentheses"};
            }
            expectNumber = false;

        } else if (isOperator(token[0])) {
            while (!operators.empty() && operators.top() != '(' && 
                    precedence(operators.top()) >= precedence(token[0])) {
                postfix.push_back(string(1, operators.top()));
                operators.pop();
            }
            operators.push(token[0]);
            expectNumber = true;
        }else {
            error = true;
            return {"Invalid character: " + token};
        }
    }

    if (expectNumber) {
        error = true;
        return {"Incomplete expression"};
    }
    
    //push the rest into postfix
    while (!operators.empty()) {
        if (operators.top() == '(') {
            error = true;
            return {"Mismatched parentheses"};
        }
        postfix.push_back(string(1, operators.top()));
        operators.pop();
    }
    
    return postfix;
}

// evaluate postfix expression
string evaluatePostfix(vector<string> postfix, bool &error) {
    error = false;
    stack<BigNum> values;

    for (const string &token : postfix) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            if (values.size() < 2) {
                error = true;
                return "Invalid expression";
            }
            BigNum b = values.top(); values.pop();
            BigNum a = values.top(); values.pop();
            bool opError = false;
            BigNum result = performOperation(a, b, token[0], opError);
            if (opError) {
                error = true;
                return "Division by zero";
            }
            values.push(result);
        } else { 
            //unary minus handling
            if (token[0] == '-' && token.length() > 1 && isdigit(token[1])) {
                string num = token; //since token is const
                num = num.substr(1);
                values.push(BigNum(num, true));
            } else if (isdigit(token[0])) {
                values.push(BigNum(token, false));
            } else {
                error = true;
                return "Invalid token: " + token;
            }
            
        }
    }
    
    if (values.size() != 1) {
        error = true;
        return "Invalid expression";
    }

    BigNum result = values.top();
    string resultStr = result.digits;
    if (result.isNegative && result.digits != "0") {
        resultStr = "-" + resultStr;
    }

    return resultStr;
}

// evaluate original arithmetic expression
string evaluateExpression(string expr, bool &error) {
    vector<string> tokenized = tokenize(expr);
    vector<string> postfix = infixToPostfix(tokenized, error);
    if (error) return postfix[0];
    return evaluatePostfix(postfix, error);
}

