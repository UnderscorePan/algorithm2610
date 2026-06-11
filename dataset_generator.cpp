// # *********************************************************
// Program: dataset_generator.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC2L
// Tutorial Class: TT5L
// Trimester: 2610
// Member_1: Hew Wee Bo | hewweebo@gmail.com | 0128803121
// Member_2: ID | NAME | EMAIL | PHONE
// Member_3: ID | NAME | EMAIL | PHONE
// Member_4: ID | NAME | EMAIL | PHONE
// # *********************************************************
// Task Distribution
// Member_1: Hew Wee Bo
// Member_2:
// Member_3:
// Member_4:
// # *********************************************************

#include <iostream>
#include <fstream>
#include <random>
#include <unordered_set>
#include <string>

using namespace std;

// student ID 251UC250HB
mt19937_64 gen(2511325082ULL);

// 1 billion to 10 billion
uniform_int_distribution<long long> int_dist(1000000000LL, 9999999999LL);

//single digits letters a-z
uniform_int_distribution<int> char_dist(0, 25);
 
long long generateRandomInteger() {
    return int_dist(gen);
}

string generateRandomString() {
    string str(5, ' ');
    for (int i = 0; i < 5; i++) {
        str[i] = 'a' + char_dist(gen);
    }
    return str;
}

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
    
    while ((int)used.size() < n) {
        long long num = generateRandomInteger();
        
        if (!used.insert(num).second) continue;
        
        output_file << num << "," << generateRandomString() << endl;
    }
    
    cout << "Generated " << n << " records -> " << output_filename << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: dataset_generator <size>" << endl;
        return 1;
    }
    
    int n = stoi(argv[1]);
    generateDataset(n);
    
    return 0;
}
