// # *********************************************************
// Program: hash_table_search_step.cpp
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

/* Purpose:
    Reads a dataset CSv, inserts all records into a hash table 
    using separate chaining (linked list), then searches for 
    a specified target key and logs every step of the search process 
    to an output file */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

static int choosePrimeTableSize(int minSize) {
    if (minSize < 2) return 2;

    int candidate = (minSize % 2 == 0) ? minSize + 1 : minSize;
    while (true) {
        bool isPrime = true;
        for (int i = 2; (long long)i * i <= candidate; ++i) {
            if (candidate % i == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) return candidate;
        candidate += 2;
    }
}


struct Record {
    long long key;  // 10-digit unique int
    string value;   // 5-letter lowercase string
};

struct Node {
    Record data;
    Node* next;
};

static string makeReportFilename(const string& datasetSizeStr) {
    return "dataset_" + datasetSizeStr + "_hash_table_search_step.txt";
}

class HashTable {
private:
    int tableSize;
    vector<Node*> table;

public:
    HashTable(int size) : tableSize(size), table(size, nullptr) {}
    ~HashTable() {
        for (int i = 0; i < tableSize; ++i) {
            Node* current = table[i];
            while (current) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    int hashFunction(long long key) const{
        return (int)((unsigned long long)key% (unsigned long long)tableSize);   
    }

        void insert(const Record& rec) {
        int   idx     = hashFunction(rec.key);
        Node* newNode = new Node();
        newNode->data = rec;
        newNode->next = table[idx]; 
        table[idx]    = newNode;    
    }

        bool searchWithSteps(long long targetKey, ofstream& out) const {
        int idx = hashFunction(targetKey);
 
        out << "Searching for target: " << targetKey << "\n";
        out << "Hash bucket index   : " << idx << "\n";
        out << "-------------------------------------------\n";
 
        Node* curr        = table[idx];
        int   compareCount = 0;
 
        while (curr != nullptr) {
            compareCount++;
            out << "Comparison " << compareCount
                << ": comparing with " << curr->data.key
                << "/" << curr->data.value;
 
            if (curr->data.key == targetKey) {
                out << "  --> MATCH\n";
                out << "-------------------------------------------\n";
                out << targetKey << " = "
                    << curr->data.key << "/" << curr->data.value << "\n";
                return true;
            } else {
                out << "  (no match)\n";
            }
            curr = curr->next;
        }
        out << "-------------------------------------------\n";
                out << "-1 != " << targetKey << "\n";
        return false;
    }
};

vector<Record> parseCSV(const string& filename, ostream* report = nullptr) {
    vector<Record> records;
    ifstream inFile(filename);

     if (!inFile.is_open()) {
        if (report) {
            *report << "Error opening file: " << filename << "\n";
        }
        return records;     
}

string line;
while (getline(inFile, line)) {
    if (line.empty()) continue; 

    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }

    stringstream ss(line);
    string keyStr, valueStr;

    if (getline(ss, keyStr, ',') && getline(ss, valueStr)) {
        try {
            Record rec;
            rec.key   = stoll(keyStr);
            rec.value = valueStr;
            records.push_back(rec);
        }catch (...){
                if (report) {
                    *report << "Error parsing line: " << line << "\n";
                }
        
        }
        }
    }
    
    inFile.close();
    return records;
}

string extractDatasetSize(const string& filename) {
    size_t underPos = filename.rfind('_');
    size_t dotPos   = filename.rfind('.');
    if (underPos != string::npos && dotPos != string::npos && underPos < dotPos) {
        return filename.substr(underPos + 1, dotPos - underPos - 1);
    }
    return "unknown";
}

void runSearch(const HashTable& ht,
               long long targetKey,
               ofstream& outFile) {
    outFile << "\n--- Searching for target: " << targetKey << " ---\n";
    bool found = ht.searchWithSteps(targetKey, outFile);
    if (found) {
        outFile << "Result: FOUND (" << targetKey << " = " << targetKey << ")\n";
    }
    else {
        outFile << "Result: NOT FOUND (" << targetKey << " != -1)\n";
    }
}

int main() {
    // Comment/uncomment lines to choose which datasets to run.
    // Multiple uncommented lines will run one after another.
   
    string datasetFile = "dataset_1000.csv";   
    //string datasetFile = "dataset_10000.csv";
    //string datasetFile = "dataset_100000.csv";
    string datasetSizeStr = extractDatasetSize(datasetFile);
    string reportFilename = makeReportFilename(datasetSizeStr);
    ofstream outFile(reportFilename);

    if (!outFile.is_open()) {
        return 1;
    }

    outFile << "Reading dataset from: " << datasetFile << " ...\n";
    cout << "Reading dataset from: " << datasetFile << endl;
    vector<Record> records = parseCSV(datasetFile, &outFile);
 
    if (records.empty()) {
        outFile << "ERROR: No records loaded. Check the file path and format.\n";
        return 1;
    }
    outFile << "Loaded " << records.size() << " records.\n";

    int tableSize = choosePrimeTableSize((int)records.size() * 2);
    outFile << "Building hash table with " << tableSize << " buckets ...\n";
    HashTable ht(tableSize);
 
    for (const Record& rec : records) {
        ht.insert(rec);
    }
    outFile << "Hash table built successfully.\n";

    long long TARGET_FOUND = records.front().key;
    long long TARGET_NOT_FOUND = -1;

    runSearch(ht, TARGET_FOUND, outFile);
    runSearch(ht, TARGET_NOT_FOUND, outFile);

    outFile << "\nDone.\n";
    cout << "Results written to file: " << reportFilename << "\n";

    return 0;

}

