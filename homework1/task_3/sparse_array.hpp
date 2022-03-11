//
// Created by Daniel Smolyak on 3/10/22.
//

#include <cstdint>
#include <bitset>
#include <iostream>
#include <cmath>
#include <filesystem>
#include <vector>
#include "compact_vector/compact_vector.hpp"
#include "rank_support.hpp"

using namespace std;

class sparse_array {

public:
    rank_support *r_ptr;
    compact::vector<unsigned, 1>* b_ptr;
    vector<string> strings{};
    uint64_t n;
    bool made_supports;
    rank_support* r_s;

    void create(uint64_t size) {
        n = size;
        made_supports = false;
        b_ptr = new compact::vector<unsigned, 1>(size);
    }

    void append(string elem, uint64_t pos) {
        strings.push_back(elem);
        b_ptr->at(pos) = 1;
    }

    bool get_at_rank(uint64_t r, std::string& elem) {
        if (strings.size() >= r) {
            elem = strings[r];
            return true;
        }
        else {
            return false;
        }
    }

    bool get_at_index(uint64_t r, std::string& elem){
        if (not made_supports){
            made_supports = true;
            r_s = new rank_support(b_ptr);
        }
        if (b_ptr->at(r) == 1) {
            uint64_t rank_res = r_s->rank1(r);
            cout << "rank res: " << rank_res << endl;
            elem = strings[rank_res-1];
            return true;
        }
        else {
            return false;
        }
    }

    uint64_t num_elem_at(uint64_t r) {
        if (not made_supports){
            made_supports = true;
            r_s = new rank_support(b_ptr);
        }
        return r_s->rank1(r);
    }

    uint64_t size(){
        return n;
    }

    uint64_t num_elem(){
        return strings.size();
    }

    void save(string& fname){
        r_s->save(fname);
        fstream writeFile;
        writeFile.open("../" + fname + "/strings.txt", ios::out);
        for (string str : strings) {
            writeFile << str << endl;
        }
        writeFile.close();
    }

    void load(string& fname){
        r_s->load(fname);
        b_ptr = r_s->b_ptr;
        fstream readFile;
        readFile.open("../" + fname + "/strings.txt", ios::in);
        // reading from the file
        string text;
        while (getline(readFile, text))
        {
            strings.push_back(text);
        }
        readFile.close();
    }

};