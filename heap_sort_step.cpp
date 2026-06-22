// # *********************************************************
// Program: heap_sort_step.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC2L
// Tutorial Class: TT5L
// Trimester: 2610
// Member_1: HEW WEE BO | hew.wee.bo@student.mmu.edu.my | 0128803121
// Member_2: ID | JEVAANRAJ A/L RAJA KUMARAN | jevaanraj.raja.kumaran@student.mmu.edu.my | 0179651973
// Member_3: ID | SHANJIF CAKRAVRTHI A/L KUPPAN @ SIVA KUMAR | shanjif.cakravthi@student.mmu.edu.my | 0195601010
// Member_4: ID | TEH ZHAO JIN | teh.zhao.jin@student.mmu.edu.my | 01111279290
// # *********************************************************
// Task Distribution
// Member_1: Hew Wee Bo
// Member_2: Jevaanraj
// Member_3: Shanjif
// Member_4: Teh Zhao Jin
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

// standard max-heapify operation
void maxHeapify(vector<Record>& records, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && records[left].num > records[largest].num)
        largest = left;

    if (right < n && records[right].num > records[largest].num)
        largest = right;

    if (largest != i) {
        Record temp = records[i];
        records[i] = records[largest];
        records[largest] = temp;

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

// read only rows from startRow to endRow (1-indexed)
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

int main() {
    // choose one input set by uncommenting the line you want
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

    int n = (int)records.size();

    // 1. build max heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        maxHeapify(records, n, i);
    }
    outputFile << formatRecords(records) << " initial" << endl;

    // 2. extract elements one by one from heap
    for (int i = n - 1; i > 0; i--) {
        Record temp = records[0];
        records[0] = records[i];
        records[i] = temp;

        maxHeapify(records, i, 0);
        outputFile << formatRecords(records) << " i=" << i << endl;
    }

    outputFile.close();

    // Console output tracking confirmation
    cout << "Successfully traced heap sort steps for rows " << startRow << " to " << endRow << endl;
    cout << "Output exported to: " << outputFilename << endl;

    return 0;
}
