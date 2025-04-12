#include "operation.h"

#include <iostream>
#include <string>
#include <algorithm>


using namespace std;

// remove leading zeros from a number string
string removeLeadingZeros(string num) {
    if (num == "0") return num;
    int start = 0;
    while (start < num.length() && num[start] == '0') start++;
    return start == num.length() ? "0" : num.substr(start);
}

// compare absolute values of two number strings (|a| < |b|)
bool absLess(string a, string b) {
    a = removeLeadingZeros(a);
    b = removeLeadingZeros(b);
    if (a.length() != b.length()) return a.length() < b.length();
    return a < b; //lexicographical comparison
}

//-----------------------------------------FOUR ARITHMETIC OPERATIONS AND LOGIC----------------------------------------------------//

// ADDITION (assumes non-negative)
string addStrings(string a, string b) {
    a = removeLeadingZeros(a);
    b = removeLeadingZeros(b);
    //corner cases
    if (a == "0") return b;
    if (b == "0") return a;

    string result;
    int carry = 0;
    int i = a.length() - 1, j = b.length() - 1;

    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += a[i--] - '0';
        if (j >= 0) sum += b[j--] - '0';
        carry = sum / 10;
        result += to_string(sum % 10);
    }

    reverse(result.begin(), result.end());
    return removeLeadingZeros(result);
}

// SUBTRACTION (assumes |a| >= |b|)
string subtractStrings(string a, string b) {
    a = removeLeadingZeros(a);
    b = removeLeadingZeros(b);
    //corner cases
    if (b == "0") return a;
    if (a == b) return "0";

    string result;
    int borrow = 0;
    int i = a.length() - 1, j = b.length() - 1;

    while (i >= 0) {
        int digitA = a[i--] - '0';
        int digitB = j >= 0 ? b[j--] - '0' : 0;
        digitA -= borrow;
        if (digitA < digitB) {
            digitA += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result += to_string(digitA - digitB);
    }

    reverse(result.begin(), result.end());
    return removeLeadingZeros(result);
}

// MULTIPLICATION using Karatsuba algorithm
string multiplyStrings(string a, string b) {
    a = removeLeadingZeros(a);
    b = removeLeadingZeros(b);
    //corner cases
    if (a == "0" || b == "0") return "0";
    if (a == "1") return b;
    if (b == "1") return a;

    // ensure both numbers have the same length by padding with zeros
    int maxLen = max(a.length(), b.length());
    while (a.length() < maxLen) a = "0" + a;
    while (b.length() < maxLen) b = "0" + b;

    // base case: single-digit multiplication
    if (maxLen == 1) {
        int product = (a[0] - '0') * (b[0] - '0');
        return to_string(product);
    }

    // Split numbers into halves
    int mid = maxLen / 2;
    string high1 = a.substr(0, maxLen - mid);
    string low1 = a.substr(maxLen - mid);
    string high2 = b.substr(0, maxLen - mid);
    string low2 = b.substr(maxLen - mid);

    // Recursion
    string z0 = multiplyStrings(low1, low2); // low parts
    string z2 = multiplyStrings(high1, high2); // high parts

    // z3 = (low1 + high1)*(low2 + high2)
    string sumA = addStrings(low1, high1);
    string sumB = addStrings(low2, high2);
    string z3 = multiplyStrings(sumA, sumB); // (low1 + high1)*(low2 + high2)

    // z1 = z3 - z2 - z0
    string temp = subtractStrings(z3, z2);
    string z1 = subtractStrings(temp, z0);

    // results: z2 * 10^(2*mid) + z1 * 10^mid + z0
    string high = z2 + string(2 * mid, '0');
    string middle = z1 + string(mid, '0');
    string result = addStrings(high, middle);
    result = addStrings(result, z0);

    return removeLeadingZeros(result);
}

// INTEGER DIVISION
string divideStrings(string a, string b, bool &divisionByZero) {
    a = removeLeadingZeros(a);
    b = removeLeadingZeros(b);

    //corner cases and error handling
    if (b == "0") {
        divisionByZero = true;
        return "0";
    }
    if (a == "0") return "0";
    if (absLess(a, b)) return "0"; //not result in integer
    if (b == "1") return a; //division by 1

    string quotient;
    string hold = "";
    int idx = 0;

    while (idx < a.length()) {
        hold += a[idx++];
        hold = removeLeadingZeros(hold);
        int q = 0;

        while (!absLess(hold, b)) { //while |hold| >= |b|
            hold = subtractStrings(hold, b);
            q++;
        }

        quotient += to_string(q);
    }

    return removeLeadingZeros(quotient);
}

// LOGIC to PERFORM arithmetic operation 
BigNum performOperation(BigNum a, BigNum b, char op, bool &error) {
    string result;
    bool resultNegative = false;
    error = false;

    if (op == '+') {
        if (!a.isNegative && !b.isNegative) {
            result = addStrings(a.digits, b.digits);
        } else if (a.isNegative && b.isNegative) {
            result = addStrings(a.digits, b.digits);
            resultNegative = true;
        } else if (!a.isNegative && b.isNegative) {
            if (!absLess(a.digits, b.digits)) {
                result = subtractStrings(a.digits, b.digits);
            } else {
                result = subtractStrings(b.digits, a.digits);
                resultNegative = true;
            }
        } else {
            if (!absLess(b.digits, a.digits)) {
                result = subtractStrings(b.digits, a.digits);
            } else {
                result = subtractStrings(a.digits, b.digits);
                resultNegative = true;
            }
        }
    } else if (op == '-') {
        if (!a.isNegative && !b.isNegative) {
            if (!absLess(a.digits, b.digits)) {
                result = subtractStrings(a.digits, b.digits);
            } else {
                result = subtractStrings(b.digits, a.digits);
                resultNegative = true;
            }
        } else if (a.isNegative && b.isNegative) {
            if (!absLess(b.digits, a.digits)) {
                result = subtractStrings(b.digits, a.digits);
            } else {
                result = subtractStrings(a.digits, b.digits);
                resultNegative = true;
            }
        } else if (!a.isNegative && b.isNegative) {
            result = addStrings(a.digits, b.digits);
        } else {
            result = addStrings(a.digits, b.digits);
            resultNegative = true;
        }
    } else if (op == '*') {
        result = multiplyStrings(a.digits, b.digits);
        resultNegative = a.isNegative != b.isNegative;
    } else if (op == '/') {
        result = divideStrings(a.digits, b.digits, error);
        resultNegative = a.isNegative != b.isNegative;
    }

    return BigNum(result, resultNegative);
}
