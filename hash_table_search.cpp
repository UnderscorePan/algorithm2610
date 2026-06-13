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



        
   }