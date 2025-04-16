# big_integers
A functional program in C++ to handle most arithmetic operations with big integers up to 100 digits, including basic operations such as "/", "*", "+", "-", precedence of operators and parentheses, and error cases such as division by zero and malformed expression.

## How to compile and run

### Change directory
```
cd src
```

### Compile
```
g++ operation.cpp precedence.cpp main.cpp -o main
```

### Run

There is a `test.txt` file in `test` folder to store all the test cases, where each line is one test case and it is space-insensitive. 

Run the code below and check `out.txt` in that same folder or terminal for outputs.


```
./main ../test/test.txt ../test/out.txt
```
