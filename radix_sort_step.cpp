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

// format records for output
string formatRecords(const vector<Record>& records) {
    stringstream ss;
    ss << "[";
    for (int i = 0; i < records.size(); i++) {
        if (i > 0) ss << ", ";
        ss << records[i].num << "/" << records[i].str;
    }
    ss << "]";
    return ss.str();
}

// stable counting sort for digit position p (0 = rightmost)
void countingSort(vector<Record>& records, int p) {
    int n = records.size();
    vector<Record> output(n);
    vector<int> count(10, 0);
    
    for (int i = 0; i < n; i++) {
        int digitValue = (records[i].num / (long long)pow(10, p)) % 10;
        count[digitValue]++;
    }
    
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }
    
    for (int i = n - 1; i >= 0; i--) {
        int digitValue = (records[i].num / (long long)pow(10, p)) % 10;
        output[count[digitValue] - 1] = records[i];
        count[digitValue]--;
    }
    
    records = output;
}

// extract dataset size from filename (dataset_1000.csv -> 1000)
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

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <dataset_file.csv> <start_row> <end_row>" << endl;
        return 1;
    }
    
    string filename = argv[1];
    int startRow = stoi(argv[2]);
    int endRow = stoi(argv[3]);
    
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return 1;
    }
    
    vector<Record> records;
    string line;
    int currentRow = 0;
    
    // read only rows in range [startRow, endRow]
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
    inputFile.close();
    
    if (records.empty()) {
        cerr << "Error: start_row or end_row out of range for file " << filename << endl;
        return 1;
    }
    
    // generate output filename
    int datasetSize = getDatasetSize(filename);
    stringstream outputSS;
    outputSS << "dataset_" << datasetSize << "_radix_sorted_step_" << startRow << "_" << endRow << ".txt";
    string outputFilename = outputSS.str();
    
    ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        cerr << "Error: Could not create output file " << outputFilename << endl;
        return 1;
    }
    
    // write original array
    outputFile << formatRecords(records) << " original" << endl;
    
    // 10 digit passes: d=10 (rightmost) to d=1 (leftmost)
    for (int digit = 0; digit < 10; digit++) {
        countingSort(records, digit);
        int d = 10 - digit;
        outputFile << formatRecords(records) << " d=" << d << endl;
    }
    
    outputFile.close();
    
    return 0;
}
