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

    