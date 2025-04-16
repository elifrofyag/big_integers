#include <iostream>
#include <fstream>
#include "./header/operation.h"
#include "./header/precedence.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: ./executable_file input_file output_file" << endl;
        return 1;
    }

    ifstream inFile(argv[1]);
    if (!inFile) {
        cerr << "Cannot open input file: " << argv[1] << endl;
        return 1;
    }

    ofstream outFile(argv[2]);
    if (!outFile) {
        cerr << "Cannot open output file: " << argv[2] << endl;
        return 1;
    }

    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        bool error = false;
        string result = evaluateExpression(line, error);
        cout << result << endl;
        outFile << result << endl;
    }

    inFile.close();
    outFile.close();
    return 0;
}


