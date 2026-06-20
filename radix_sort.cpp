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

// record: integer (1B-10B) and 5-char string
struct Record {
    long long num;
    string str;
};

int getDigit(long long num, int digitPos) {
    return (num / (long long)pow(10, digitPos)) % 10;
}

void countingSortByDigit(vector<Record>& records, int digitPos) {
    int n = (int)records.size();
    vector<Record> output(n);
    vector<int> count(10, 0);
    
    for (int i = 0; i < n; i++) {
        int digitValue = getDigit(records[i].num, digitPos);
        count[digitValue]++;
    }
    
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }
    
    for (int i = n - 1; i >= 0; i--) {
        int digitValue = getDigit(records[i].num, digitPos);
        output[count[digitValue] - 1] = records[i];
        count[digitValue]--;
    }
    records = output;
}

void radixSort(vector<Record>& records) {
    const int totalDigits = 10;
    for (int digitPos = 0; digitPos < totalDigits; digitPos++) {
        countingSortByDigit(records, digitPos);
    }
}

string getOutputFilename(const string& inputFilename) {
    size_t pos = inputFilename.find("dataset_");
    if (pos != string::npos) {
        return "radix_sorted_" + inputFilename.substr(pos);
    }
    return "radix_sorted_" + inputFilename;
}

bool readDataset(const string& filename, vector<Record>& records) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        return false;
    }

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

bool writeSortedOutput(const string& outputFilename,
                       const vector<Record>& records,
                       const string& inputFilename,
                       double elapsedSeconds) {
    ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        return false;
    }

    for (const auto& record : records) {
        outputFile << record.num << "/" << record.str << endl;
    }

    outputFile << "Radix sort running time for " << inputFilename << ": "
               << fixed << setprecision(6) << elapsedSeconds << " seconds" << endl;
    return true;
}

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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <dataset_file1.csv> [dataset_file2.csv] ..." << endl;
        return 1;
    }
    
    // process each input file
    for (int i = 1; i < argc; i++) {
        processFile(argv[i]);
    }
    
    return 0;
}
