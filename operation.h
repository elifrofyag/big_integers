    
#define OPERATION_H


#include <string>


// define structure to hold large numbers as strings and their sign
struct BigNum {
    std::string digits;
    bool isNegative;
    
    //constructor with default values  
    BigNum(std::string d = "0", bool neg = false) : digits(d), isNegative(neg) {}
};

std::string removeLeadingZeros(std::string num);
bool absLess(std::string a, std::string b);

std::string addStrings(std::string a, std::string b);
std::string subtractStrings(std::string a, std::string b);
std::string multiplyStrings(std::string a, std::string b);
std::string divideStrings(std::string a, std::string b, bool &divisionByZero);

BigNum performOperation(BigNum a, BigNum b, char op, bool &error);