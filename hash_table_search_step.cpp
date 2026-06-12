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

