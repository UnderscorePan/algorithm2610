// # *********************************************************
// Program: radix_sort.cpp
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
#include <cmath>
#include <chrono>
#include <iomanip>

using namespace std;

// simple record: a 10-digit integer and its 5-char label
struct Record {
    long long num;
    string str;
};

// return the digit at position digitPos (0 = rightmost)
int getDigit(long long num, int digitPos) {
    return (num / (long long)pow(10, digitPos)) % 10;
}

// stable counting sort on a single digit (used by LSD radix)
void countingSortByDigit(vector<Record>& records, int digitPos) {
    int n = (int)records.size();
    vector<Record> output(n);
    vector<int> count(10, 0);

    for (int i = 0; i < n; i++) {
        int digitValue = getDigit(records[i].num, digitPos);
        count[digitValue]++;
    }

    // cumulative counts
    for (int i = 1; i < 10; i++) count[i] += count[i - 1];

    // place items into output array in stable order
    for (int i = n - 1; i >= 0; i--) {
        int digitValue = getDigit(records[i].num, digitPos);
        output[count[digitValue] - 1] = records[i];
        count[digitValue]--;
    }
    records = output;
}

// perform LSD radix sort (10 passes, right-to-left)
void radixSort(vector<Record>& records) {
    const int totalDigits = 10;
    for (int digitPos = 0; digitPos < totalDigits; digitPos++) countingSortByDigit(records, digitPos);
}

string getOutputFilename(const string& inputFilename) {
    size_t pos = inputFilename.find("dataset_");
    if (pos != string::npos) {
        return "radix_sorted_" + inputFilename.substr(pos);
    }
    return "radix_sorted_" + inputFilename;
}

// read entire CSV into memory (simple parser, no header expected)
bool readDataset(const string& filename, vector<Record>& records) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) return false;

    string line;
    while (getline(inputFile, line)) {
        size_t commaPos = line.find(',');
        if (commaPos != string::npos) {
            long long num = stoll(line.substr(0, commaPos));
            string str = line.substr(commaPos + 1);
            records.push_back({num, str});
        }
    }
    return true;
}

// write sorted records and append timing info
bool writeSortedOutput(const string& outputFilename,
                       const vector<Record>& records,
                       const string& inputFilename,
                       double elapsedSeconds) {
    ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) return false;

    for (const auto& record : records) outputFile << record.num << "/" << record.str << endl;
    outputFile << "Radix sort running time for " << inputFilename << ": "
               << fixed << setprecision(6) << elapsedSeconds << " seconds" << endl;
    return true;
}

// read -> sort (timed) -> write
void processFile(const string& filename) {
    vector<Record> records;
    if (!readDataset(filename, records)) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    auto start = chrono::high_resolution_clock::now();
    radixSort(records);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = end - start;
    string outputFilename = getOutputFilename(filename);

    if (!writeSortedOutput(outputFilename, records, filename, elapsed.count())) {
        cerr << "Error: Could not create output file " << outputFilename << endl;
        return;
    }

    cout << "Radix sort running time for " << filename << ": "
         << fixed << setprecision(6) << elapsed.count() << " seconds" << endl;
}

int main() {
    // choose input files by commenting/uncommenting the options below
    // vector<string> inputs = {"dataset_1000.csv"};
    vector<string> inputs = {"dataset_1000.csv", "dataset_10000.csv"}; // default
    // vector<string> inputs = {"dataset_100000.csv"};

    for (const auto& f : inputs) processFile(f);
    return 0;
}
