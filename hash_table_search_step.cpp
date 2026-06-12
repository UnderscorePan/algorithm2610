// # *********************************************************
// Program: hash_table_search_step.cpp
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


struct Record {
    long long key;  // 10-digit unique int
    string value;   // 5-letter lowercase string
};

struct Node {
    Record data;
    Node* next;
};

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

vector<Record> parseCSV(const string& filename) {
    vector<Record> records;
    ifstream inFile(filename);

     if (!inFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
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
                cerr << "Error parsing line: " << line << endl;
        
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
               const string& datasetSizeStr) {
    string outFilename = "dataset_" + datasetSizeStr
                         + "_hash_table_search_step_"
                       + to_string(target) + ".txt";
    ofstream outFile(outFilename);  
    if (!outFile.is_open()) {
        cerr << "Error opening output file: " << outFilename << endl;
        return;
    }   

    cout << "\n--- Searching for target: " << target << " ---\n";
    bool found = ht.searchWithSteps(targetKey, outFile);
    if (found) {
        cout << "Result: FOUND (" << target << " = " << target << ")\n";
    else {
        cout << "Result: NOT FOUND (" << target << " != -1)\n";
    }
    outFile.close();       
   
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <dataset_filename.csv>\n";
                cerr << "Example: " << argv[0] << " dataset_1000.csv\n";
        return 1;
    }
 
    string datasetFile    = argv[1];
    string datasetSizeStr = extractDatasetSize(datasetFile);

    cout << "Reading dataset from: " << datasetFile << " ...\n";
    vector<Record> records = parseCSV(datasetFile);
 
    if (records.empty()) {
        cerr << "ERROR: No records loaded. Check the file path and format.\n";
        return 1;
    }
    cout << "Loaded " << records.size() << " records.\n";

    cout << "Building hash table with " << tableSize << " buckets ...\n";
    HashTable ht(tableSize);
 
    for (const Record& rec : records) {
        ht.insert(rec);
    }
    cout << "Hash table built successfully.\n";

    runSearch(ht, TARGET_FOUND, datasetSizeStr);
    runSearch(ht, TARGET_NOT_FOUND, datasetSizeStr);

    cout << "\nDone.\n";
    return 0;

}


