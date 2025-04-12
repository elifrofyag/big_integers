# big_integers
A functional program in C++ to handle most arithmetic operations with big integers without using built-in libraries and data structures for large numbers.

This includes support for basic operations—addition, subtraction, multiplication, and division—as well as correct handling of operator precedence and parentheses. The program is capable of processing integers up to 100 digits and handles edge cases like negative numbers, nested expressions, and division by zero.



## How to compile and run

### Compile
```
g++ operation.cpp precedence.cpp main.cpp -o main
```

### Run

There is a `test.txt` file to store all the test cases, where each line is one test case and it is space-insensitive.

Run the code below and check `out.txt` or terminal for outputs.


```
./main test.txt out.txt
```
