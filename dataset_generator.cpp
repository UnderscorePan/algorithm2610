// # *********************************************************
// Program: dataset_generator.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC2L
// Tutorial Class: TT5L
// Trimester: 2610
// Member_1: HEW WEE BO | hew.wee.bo@student.mmu.edu.my | 0128803121
// Member_2: ID | JEVAANRAJ A/L RAJA KUMARAN | jevaanraj.raja.kumaran@student.mmu.edu.my | 0179651973
// Member_3: ID | SHANJIF CAKRAVRTHI A/L KUPPAN @ SIVA KUMAR | shanjif.cakravthi@student.mmu.edu.my | 0195601010
// Member_4: ID | TEH ZHAO JIN | teh.zhao.jin@student.mmu.edu.my | 01111279290
// # *********************************************************
// Task Distribution
// Member_1: Hew Wee Bo
// Member_2: Jevaanraj
// Member_3: Shanjif
// Member_4: Teh Zhao Jin
// # *********************************************************

#include <iostream>
#include <fstream>
#include <random>
#include <unordered_set>
#include <string>

using namespace std;

// student ID 251UC250HB
// random engine seeded with student-specific value
mt19937_64 gen(2511325082ULL);

// 1 billion to 10 billion (inclusive) for generated ids
uniform_int_distribution<long long> int_dist(1000000000LL, 9999999999LL);

// single-letter generator (a-z) used to build 5-char strings
uniform_int_distribution<int> char_dist(0, 25);
 
// return one random 10-digit integer
long long generateRandomInteger() {
    return int_dist(gen);
}

// return a random 5-character lowercase string
string generateRandomString() {
    string str(5, ' ');
    for (int i = 0; i < 5; i++) {
        str[i] = 'a' + char_dist(gen);
    }
    return str;
}

// write n unique records to dataset_n.csv
void generateDataset(int n) {
    unordered_set<long long> used;
    used.reserve(n);
    
    // create file in format dataset_n.csv
    string output_filename = "dataset_" + to_string(n) + ".csv";
    ofstream output_file(output_filename);
    
    if (!output_file.is_open()) {
        cerr << "Error: Could not open file " << output_filename << " for writing" << endl;
        return;
    }
    
    // keep generating until we have n unique integers
    while ((int)used.size() < n) {
        long long num = generateRandomInteger();
        if (!used.insert(num).second) continue; // skip duplicates
        output_file << num << "," << generateRandomString() << endl;
    }
    
    cout << "Generated " << n << " records -> " << output_filename << endl;
}

int main() {
    // pick a size, edit/comment lines to try different sizes
    // int n = 100;     // small quick test
    int n = 1000;    // default
    // int n = 10000;  // larger
    // int n = 100000; // heavy

    generateDataset(n);
    return 0;
}
