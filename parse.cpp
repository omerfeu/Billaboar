#include <fstream>
#include <iostream>
using namespace std;
#include "parse.hpp"
/*
Read file from 'filename' to a vector of pairs of names (string) and payments (float).
*/
void read_file(const string filename, vector<pair<string, float>>& result){
    ifstream file;
    file.open(filename);
    if (file.fail()) {
        cerr << "Error loading file";
    }
    string line;
    getline(file, line);
    while (getline(file, line)){
        int d = line.find(",");        
        result.push_back(pair<string, float>(line.substr(0, d), stof(line.substr(d+1))));
    }
    file.close();
}

/*
Group payments by name
*/
void group_bills(const vector<pair<string, float>> &data, map<string, float>& dict){
    for (auto elem: data){
        dict[elem.first] += elem.second;
    }
}

/*
Gets a dictionary of 'name:total payment' and computes the total debt of each one to the group.
*/
void compute_debts(const map<string,float>& dict, map<string, float>& result){
    float sum = 0;
    for (auto elem: dict){
        sum += elem.second;
    }
    for (auto elem: dict){
        result[elem.first] = (sum/dict.size()) - elem.second;
    }
}

void debts_to_vectors(const map<string,float>& dict, map<int,string>& d_dict, map<int,string>& c_dict, vector<float>& debtors, vector<float>& creditors){
    int d_index = 0, c_index = 0;
    for (auto elem: dict) {
        if (elem.second > 0) {
            
            creditors.push_back(elem.second);
            c_dict[c_index++] = elem.first;
        }
        else {
            debtors.push_back(elem.second);
            d_dict[d_index++] = elem.first;
        }
    }
}