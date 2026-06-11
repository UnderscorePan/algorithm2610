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

// stable counting sort for digit position d (0 = rightmost)
void countingSort(vector<Record>& records, int digit) {
    int n = records.size();
    vector<Record> output(n);
    vector<int> count(10, 0);
    
    for (int i = 0; i < n; i++) {
        int digitValue = (records[i].num / (long long)pow(10, digit)) % 10;
        count[digitValue]++;
    }
    
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }
    
    for (int i = n - 1; i >= 0; i--) {
        int digitValue = (records[i].num / (long long)pow(10, digit)) % 10;
        output[count[digitValue] - 1] = records[i];
        count[digitValue]--;
    }
    records = output;
}

// LSD radix sort: 10 digit passes
void radixSort(vector<Record>& records) {
    for (int digit = 0; digit < 10; digit++) {
        countingSort(records, digit);
    }
}

string getOutputFilename(const string& inputFilename) {
    size_t pos = inputFilename.find("dataset_");
    if (pos != string::npos) {
        return "radix_sorted_" + inputFilename.substr(pos);
    }
    return "radix_sorted_" + inputFilename;
}

void processFile(const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }
    
    vector<Record> records;
    string line;
    
    // parse CSV: integer,string
    while (getline(inputFile, line)) {
        size_t commaPos = line.find(',');
        if (commaPos != string::npos) {
            long long num = stoll(line.substr(0, commaPos));
            string str = line.substr(commaPos + 1);
            records.push_back({num, str});
        }
    }
    inputFile.close();
    
    // time sorting only, exclude I/O
    auto start = chrono::high_resolution_clock::now();
    radixSort(records);
    auto end = chrono::high_resolution_clock::now();
    
    chrono::duration<double> elapsed = end - start;
    
    string outputFilename = getOutputFilename(filename);
    ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        cerr << "Error: Could not create output file " << outputFilename << endl;
        return;
    }
    
    for (const auto& record : records) {
        outputFile << record.num << "/" << record.str << endl;
    }
    
    outputFile << "Radix sort running time for " << filename << ": " 
               << fixed << setprecision(6) << elapsed.count() << " seconds" << endl;
    
    outputFile.close();
    
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
