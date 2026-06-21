// # *********************************************************
// Program: heap_sort.cpp
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
 #include <sstream>
 #include <string>
 #include <chrono>

using namespace std;
using namespace std::chrono;

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
        swap(records[i], records[largest]);
        maxHeapify(records, n, largest);
    }
}

// Full heap sort algorithm execution
void heapSort(vector<Record>& records) {
    int n = (int)records.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        maxHeapify(records, n, i);
    }
    for (int i = n - 1; i > 0; i--) {
        swap(records[0], records[i]);
        maxHeapify(records, i, 0);
    }
}

int getDatasetSize(const string& filename) {
    size_t pos = filename.find("dataset_");
    if (pos != string::npos) {
        size_t endPos = filename.find(".csv", pos);
        if (endPos != string::npos) {
            return stoi(filename.substr(pos + 8, endPos - (pos + 8)));
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <dataset_file.csv>" << endl;
        return 1;
    }
    
    string filename = argv[1];
    vector<Record> records;

    // Open file and read all rows
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error reading dataset file." << endl;
        return 1;
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
    inputFile.close();

    // --- TIMING STARTS HERE (Excludes all I/O processing) --- [cite: 27]
    auto start = high_resolution_clock::now();
    heapSort(records);
    auto stop = high_resolution_clock::now();
    // --- TIMING ENDS HERE ---

    auto duration = duration_cast<microseconds>(stop - start);
    double seconds = duration.count() / 1000000.0;

    // Print running time directly to console for quick command prompt screenshots 
    cout << "--------------------------------------" << endl;
    cout << "Dataset Size: " << records.size() << endl;
    cout << "Heap Sort execution time: " << seconds << " seconds" << endl;
    cout << "--------------------------------------" << endl;

    // Generate heap_sort_dataset_n.csv output data 
    int datasetSize = getDatasetSize(filename);
    string outputFilename = "heap_sort_dataset_" + to_string(datasetSize) + ".csv";
    ofstream outputFile(outputFilename);
    
    if (outputFile.is_open()) {
        // Output format matching criteria 
        for (const auto& rec : records) {
            outputFile << rec.num << "," << rec.str << "\n";
        }
        outputFile.close();
    }

    return 0;
}