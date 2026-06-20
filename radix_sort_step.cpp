// # *********************************************************
// Program: radix_sort_step.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC2L
// Tutorial Class: TT5L
// Trimester: 2610
// Member_1: Hew Wee Bo | hewweebo@gmail.com | 0128803121
// Member_2: ID | NAME | EMAIL | PHONE
// Member_3: ID | NAME | EMAIL | PHONE
// Member_4: ID | NAME | EMAIL | PHONE
// # *********************************************************
// Task Distribution
// Member_1:Hew Wee Bo
// Member_2:
// Member_3:
// Member_4:
// # *********************************************************

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

struct Record {
    long long num;
    string str;
};

// format records into the required bracketed text for the trace
string formatRecords(const vector<Record>& records) {
    stringstream ss;
    ss << "[";
    for (int i = 0; i < (int)records.size(); i++) {
        if (i > 0) ss << ", ";
        ss << records[i].num << "/" << records[i].str;
    }
    ss << "]";
    return ss.str();
}

// helper: get digit at position (0 = rightmost)
int getDigit(long long num, int digitPos) {
    return (num / (long long)pow(10, digitPos)) % 10;
}

// stable counting sort used for each pass of LSD radix
void countingSortByDigit(vector<Record>& records, int digitPos) {
    int n = (int)records.size();
    vector<Record> output(n);
    vector<int> count(10, 0);

    for (int i = 0; i < n; i++) {
        int digitValue = getDigit(records[i].num, digitPos);
        count[digitValue]++;
    }

    for (int i = 1; i < 10; i++) count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        int digitValue = getDigit(records[i].num, digitPos);
        output[count[digitValue] - 1] = records[i];
        count[digitValue]--;
    }

    records = output;
}

int getDatasetSize(const string& filename) {
    size_t pos = filename.find("dataset_");
    if (pos != string::npos) {
        size_t endPos = filename.find(".csv", pos);
        if (endPos != string::npos) {
            string sizeStr = filename.substr(pos + 8, endPos - (pos + 8));
            return stoi(sizeStr);
        }
    }
    return 0;
}

// read only rows startRow..endRow (1-indexed) into records
bool readRowRange(const string& filename, int startRow, int endRow, vector<Record>& records) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        return false;
    }

    string line;
    int currentRow = 0;

    while (getline(inputFile, line)) {
        currentRow++;

        if (currentRow < startRow) continue;
        if (currentRow > endRow) break;

        size_t commaPos = line.find(',');
        if (commaPos != string::npos) {
            long long num = stoll(line.substr(0, commaPos));
            string str = line.substr(commaPos + 1);
            records.push_back({num, str});
        }
    }

    return true;
}

string getStepOutputFilename(const string& inputFilename, int startRow, int endRow) {
    int datasetSize = getDatasetSize(inputFilename);
    stringstream outputSS;
    outputSS << "dataset_" << datasetSize << "_radix_sorted_step_"
             << startRow << "_" << endRow << ".txt";
    return outputSS.str();
}

int main() {
    // choose one input set by uncommenting the line you want
    // string filename = "dataset_1000.csv"; int startRow = 1;  int endRow = 7;
    string filename = "dataset_1000.csv"; int startRow = 1;  int endRow = 7; // default
    // string filename = "dataset_10000.csv"; int startRow = 10; int endRow = 20;
    // string filename = "dataset_100000.csv"; int startRow = 100; int endRow = 110;

    vector<Record> records;
    if (!readRowRange(filename, startRow, endRow, records)) {
        cerr << "Error: Could not open file " << filename << endl;
        return 1;
    }
    
    if (records.empty()) {
        cerr << "Error: start_row or end_row out of range for file " << filename << endl;
        return 1;
    }

    string outputFilename = getStepOutputFilename(filename, startRow, endRow);
    
    ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        cerr << "Error: Could not create output file " << outputFilename << endl;
        return 1;
    }

    outputFile << formatRecords(records) << " original" << endl;

    const int totalDigits = 10;
    for (int digitPos = 0; digitPos < totalDigits; digitPos++) {
        countingSortByDigit(records, digitPos);
        int d = 10 - digitPos;
        outputFile << formatRecords(records) << " d=" << d << endl;
    }
    
    outputFile.close();
    
    return 0;
}
