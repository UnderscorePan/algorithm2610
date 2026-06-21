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

/* Purpose
   Measure the running time of hash table search for:
   - Best Case
   - Average Case
   - Worst Case */

   #include <iostream>
   #include <fstream>
   #include <sstream>
   #include <string>
   #include <vector>
   #include <chrono>
   #include <climits>

   using namespace std;
   using namespace chrono;

   struct Record {
       long long key;
       string value;
   };

   struct Node {
       Record data;
       Node* next;
   };

   class HashTable {
    private:
        int tableSize;
        int numElements;
        vector<Node*> table;
    public:
        HashTable(int size)
            :tableSize(size), numElements(0), table(size, nullptr) {}

        ~HashTable(){
            for (int i=0; i < tableSize; i++) {
                Node* curr= table[i];
                while (curr != nullptr) {
                    Node* temp = curr;
                    curr= curr->next;
                    delete temp;
                }  
            }
        }

        int hashFunction(long long key) const{
            return (int)((unsigned long long)key % (unsigned long long)tableSize);
        }
        void insert(const Record& rec) {
        int   idx     = hashFunction(rec.key);
        Node* newNode = new Node();
        newNode->data = rec;
        newNode->next = table[idx];
        table[idx]    = newNode;
        numElements++;
        }

        Node* search(long long targetKey) const {
            int idx = hashFunction(targetKey);
            Node* curr = table[idx];
            while (curr != nullptr) {
                if (curr->data.key == targetKey) {
                    return curr; 
                }
                curr = curr->next;
            }
            return nullptr; 
        }

        long long getBestCaseKey() const {
            for (int i = 0; i < tableSize; ++i) {
                if (table[i] != nullptr) {
                    return table[i]->data.key; 
                }
            }
            return LLONG_MIN;      
        }

        long long getLongestChainKey() const{
            int maxLen=0;
            long long worstKey = LLONG_MIN;

            for (int i = 0; i < tableSize; i++){
                if (table[i] == nullptr) continue;

                int len = 0;
                Node* curr = table[i];
                Node* last = nullptr;

                while (curr != nullptr) {
                    len++;
                    last = curr;
                    curr = curr->next;
                }

                if (len > maxLen) {
                    maxLen = len;
                    worstKey = last->data.key; 
                }

            }
            return worstKey;
        }

        vector<long long> getAllKeys() const {
            vector<long long> keys;
            for (int i = 0; i < tableSize; ++i) {
                Node* curr = table[i];
                while (curr != nullptr) {
                    keys.push_back(curr->data.key);
                    curr = curr->next;
                }
            }
            return keys;
        }

        int getNumElements() const {
            return numElements;
        }

        int getTableSize() const {
            return tableSize;
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

   int choosePrimeTableSize (int minSize) {
                if (minSize < 2) return 2;
                int candidate = (minSize % 2 == 0) ? minSize + 1 : minSize;
                while (true) {
                    bool isPrime = true;
                    for (int i=2; (long long)i * i <= candidate; i++) {
                        if (candidate % i == 0) {
                            isPrime = false;
                            break;
                        }
                    }
                    if (isPrime) return candidate;
                    candidate += 2;
                }
                return -1;
   }

   int main() {
    // Comment/uncomment lines to choose which datasets to run.
    // Multiple uncommented lines will run one after another.
   
    vector<string> inputs = { 
        "dataset_1000.csv",
        // "dataset_10000.csv",
        //"dataset_100000.csv",
    };
    

    for (const string& datasetFile : inputs) {
    string datasetSizeStr = extractDatasetSize(datasetFile);
        cout << "Reading dataset from: " << datasetFile << endl;
        vector<Record> records = parseCSV(datasetFile);

        if (records.empty()) {
            cerr << "No valid records found in the dataset." << endl;
            return 1;
        }

        int n = (int)records.size();
       // cout << "Loaded " << n << " records." << endl;

        int tableSize = choosePrimeTableSize(n * 2);
      //  cout << "Building hash table with " << tableSize << " buckets..." << endl;

        HashTable ht(tableSize);
        for (const Record& rec : records) {
            ht.insert(rec);
        }

       // cout << "Hash table built with " << ht.getNumElements() << " elements." << endl;

        long long bestCaseKey = ht.getBestCaseKey();
        long long worstCaseKey = ht.getLongestChainKey();
        vector<long long> allKeys = ht.getAllKeys();

       // cout << "Best case key: " << bestCaseKey << endl;
        //cout << "Worst case key: " << worstCaseKey << endl;
       // cout << "Average case key: " << allKeys[allKeys.size() / 2] << endl;

        // cout << "TIming best case (" << n << " searches)..." << endl;
            auto bcStart = high_resolution_clock::now();
 
    volatile int bestFound = 0; 
    for (int i = 0; i < n; i++) {
        Node* result = ht.search(bestCaseKey);
        if (result != nullptr) bestFound++;
    }
 
    auto bcEnd = high_resolution_clock::now();
    duration<double, std::milli> bestTime = duration_cast<duration<double, std::milli>>(bcEnd - bcStart);


       // cout << "Timing average case (" << n << " searches)..." << endl;
        auto acStart = high_resolution_clock::now();

    volatile int avgFound = 0;
    for (int i = 0; i < n; i++) {
        Node* result = ht.search(allKeys[i]);
        if (result != nullptr) avgFound++;
    }

    auto acEnd = high_resolution_clock::now();
    duration<double, std::milli> avgTime = duration_cast<duration<double, std::milli>>(acEnd - acStart);

       // cout << "Timing worst case (" << n << " searches)..." << endl;
        auto wcStart = high_resolution_clock::now();

    volatile int worstFound = 0;
    for (int i = 0; i < n; i++) {
        Node* result = ht.search(worstCaseKey);
        if (result != nullptr) worstFound++;
    }

    auto wcEnd = high_resolution_clock::now();
    duration<double, std::milli> worstTime = duration_cast<duration<double, std::milli>>(wcEnd - wcStart);

        string outFilename = "hash_table_search_dataset_" + datasetSizeStr + ".txt";
    ofstream outFile(outFilename);
 
    if (!outFile.is_open()) {
        cerr << "ERROR: Cannot create output file: " << outFilename << "\n";
        return 1;
    }

        outFile << "\nResults for dataset size " << datasetSizeStr << ":\n";
        outFile << "Best case: " << bestFound << "/" << n << " found\n Time = " << bestTime.count() << " milliseconds\n";
        outFile << endl;
        outFile << "Average case: " << avgFound << "/" << n << " found\n Time = " << avgTime.count() << " milliseconds\n";
        outFile << endl;
        outFile << "Worst case: " << worstFound << "/" << n << " found\n Time = " << worstTime.count() << " milliseconds\n";
        outFile << endl;
        outFile << "=======================================================\n";
        outFile << endl;
        outFile << "Results for dataset size n = " << n << "\n";
        outFile << "Best case time: "    << bestTime.count()  << " milliseconds\n";
        outFile << "Average case time: " << avgTime.count()   << " milliseconds\n";
        outFile << "Worst case time: "   << worstTime.count() << " milliseconds\n";
        outFile << endl;
        outFile << "Output written to: " << outFilename << "\n";

        outFile.close();

        cout << "Results written to file: " << outFilename << "\n";

       return 0;
   }
}
