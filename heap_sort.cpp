// # *********************************************************
// Program: heap_sort.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC2L
// Tutorial Class: TT5L
// Trimester: 2610
// Member_1: HEW WEE BO | hew.wee.bo@student.mmu.edu.my | 0128803121
// Member_2: ID | JEVAANRAJ A/L RAJA KUMARAN | jevaanraj17@gmail.com | 0179651973
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
#include <chrono>
#include <iomanip>

using namespace std;

struct Record {
    long long num;
    string str;
};

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

// complete heap sort algorithm implementation
void heapSort(vector<Record>& records) {
    int n = (int)records.size();

    // 1. build max heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        maxHeapify(records, n, i);
    }

    // 2. heap sort
    for (int i = n - 1; i > 0; i--) {
        Record temp = records[0];
        records[0] = records[i];
        records[i] = temp;

        maxHeapify(records, i, 0);
    }
}

string getOutputFilename(const string& inputFilename) {
    size_t pos = inputFilename.find("dataset_");
    if (pos != string::npos) {
        return "heap_sort_" + inputFilename.substr(pos);
    }
    return "heap_sort_" + inputFilename;
}

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

bool writeSortedOutput(const string& outputFilename, const vector<Record>& records, const string& inputFilename, double elapsedSeconds) {
    ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) return false;

    for (const auto& record : records) {
        outputFile << record.num << "/" << record.str << endl;
    }

    // Append the performance breakdown to match the style found in your radix_sort file
    outputFile << "Heap sort running time for " << inputFilename << ": "
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
    heapSort(records);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = end - start;
    string outputFilename = getOutputFilename(filename);

    if (!writeSortedOutput(outputFilename, records, filename, elapsed.count())) {
        cerr << "Error: Could not create output file " << outputFilename << endl;
        return;
    }

    cout << "Heap sort running time for " << filename << ": "
         << fixed << setprecision(6) << elapsed.count() << " seconds" << endl;
}

int main() {
    // choose input files by commenting/uncommenting the options below
    vector<string> inputs = {"dataset_1000.csv", "dataset_10000.csv", "dataset_100000.csv"}; // default
    // vector<string> inputs = {"dataset_100000.csv"};

    for (const string& filename : inputs) {
        processFile(filename);
    }

    return 0;
}
