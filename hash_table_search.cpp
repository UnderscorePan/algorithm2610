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
            :tableSize(size). numElements(0), table(size, nullptr) {}

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

        long long getBestCaseKey() cosnt {
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
                if (table[i]) == nullptr) continue;

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
        int getTableSize() const {
            return tableSize;
        };

         
   }