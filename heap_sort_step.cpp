// # *********************************************************
// Program: heap_sort_step.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC2L
// Tutorial Class: TT5L
// Trimester: 2610
// Member_1: Hew Wee Bo | hewweebo@gmail.com | 0128803121
// Member_2: ID | NAME | EMAIL | PHONE
// Member_3: ID | JEVAANRAJ A/L RAJA KUMARAN | jevaanraj17@gmail.com | 0179651973
// Member_4: ID | NAME | EMAIL | PHONE
// # *********************************************************
// Task Distribution
// Member_1: Hew Wee Bo
// Member_2:
// Member_3: Jevaanraj
// Member_4:
// # *********************************************************

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct Record {
    long long num;
    string str;
};

// Formats records into [num/str, num/str] style
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

// Standard Max-Heapify logic
void maxHeapify(vector<Record>& records, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && records[left].num > records[largest].num)
        largest = left;

    if (right < n && records[right].num > records[largest].num)
        largest = right;

    if (largest != i) {
        swap(records[i], records[largest]);
        maxHeapify(records, n, largest);
    }
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

bool readRowRange(const string& filename, int startRow, int endRow, vector<Record>& records) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) return false;

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
    outputSS << "dataset_" << datasetSize << "_heap_sorted_step_"
             << startRow << "_" << endRow << ".txt";
    return outputSS.str();
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <dataset_file.csv> <start_row> <end_row>" << endl;
        return 1;
    }
    
    string filename = argv[1];
    int startRow = stoi(argv[2]);
    int endRow = stoi(argv[3]);

    vector<Record> records;
    if (!readRowRange(filename, startRow, endRow, records)) {
        cerr << "Error: Could not open file " << filename << endl;
        return 1;
    }
    
    if (records.empty()) {
        cerr << "Error: Range out of bounds or empty data." << endl;
        return 1;
    }

    string outputFilename = getStepOutputFilename(filename, startRow, endRow);
    ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        cerr << "Error creating output file." << endl;
        return 1;
    }

    int n = (int)records.size();

    // 1. Build initial max heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        maxHeapify(records, n, i);
    }
    outputFile << formatRecords(records) << " initial" << endl;// [cite: 119]

    // 2. Extract elements one by one and track steps
    for (int i = n - 1; i > 0; i--) {
        swap(records[0], records[i]);
        maxHeapify(records, i, 0);
        outputFile << formatRecords(records) << " i=" << i << endl; //[cite: 120]
    }
    
    outputFile.close();
    return 0;
}