//
// Created by Daniel Smolyak on 3/7/22.
//

#include <cstdint>
#include <bitset>
#include <iostream>
#include <cmath>
#include <filesystem>
#include "compact_vector/compact_vector.hpp"

using namespace std;

class rank_support {

public:
    compact::vector<unsigned, 1> *b_ptr;
    compact::vector<uint64_t> *r_s;
    compact::vector<uint64_t> *r_b;
    int s_length;
    int b_length;
    int s_width;
    int b_width;
    uint64_t n;
    uint64_t lo_set[65];

    rank_support(compact::vector<unsigned, 1> *bit_vector) {
        b_ptr = bit_vector;
        auto b = *bit_vector;
        n = b.size();
        int lg_n = ceil(log2(n));
        s_length = lg_n * lg_n;
        b_length = lg_n;
        s_width = lg_n;
        b_width = ceil(log2(s_length));

//        while (b_ptr->size() % b_length != 0){
//            b_ptr->push_back(0);
//        }

        r_s = new compact::vector<uint64_t>(s_width);
        r_b = new compact::vector<uint64_t>(b_width);
        r_s->push_back(0);
        r_b->push_back(0);

        int r_s_counter = b[0];
        int r_b_counter = b[0];
        for (int i = 1; i < n; i += 1) {
            if (i % s_length == 0) {
                r_s->push_back(r_s_counter);
                r_b_counter = 0;
            }
            if (i % b_length == 0) {
                r_b->push_back(r_b_counter);
            }
            r_s_counter += b[i];
            r_b_counter += b[i];
        }

        make_lo_set();
    }

    void make_lo_set(){
        for (int i = 0; i < 65; i++) {
            lo_set[i] = exp2(i) - 1;
        }
    }

    uint64_t rank1(uint64_t i) {
        uint64_t res = 0;

        int r_s_i = floor(i / s_length);
        auto r_s_v = *r_s;
        res += r_s_v[r_s_i];

        int r_b_i = floor(i / b_length);
        auto r_b_v = *r_b;
        res += r_b_v[r_b_i];

        uint64_t from = r_b_i * b_length;
        uint64_t len = i - from + 1;
        if (len > 0){
            auto vec_ptr = b_ptr->get();
            uint64_t arr_int = get_int(from, len, vec_ptr);
            res += popcount(arr_int);
        }

        return res;
    }

    // Used logic from sdsl-lite read_int function
    // https://github.com/simongog/sdsl-lite/blob/c32874cb2d8524119f25f3b501526fe692df29f4/include/sdsl/bits.hpp#L501
    uint64_t get_int(uint64_t from, uint64_t len, unsigned long long * vec_ptr) {
        uint64_t res = (*vec_ptr) >> from;
//        cout << len << ", " << bitset<5>(res) << endl;
        res = res & lo_set[len];
//        cout << "block int " << res << endl;
        return res;
    }

    uint64_t overhead() {
        uint64_t total_size = n; // storing bit vector
        total_size += s_width * (n / s_length); // storing r_s
        total_size += b_width * (n / b_length); // storing r_b
        return total_size;
    }

    void save(string &fname) {
        std::filesystem::create_directory("../" + fname);
        std::ofstream seqOut_b_v("../" + fname + "/bit_vector");
        b_ptr->serialize(seqOut_b_v);
        std::ofstream seqOut_r_s("../" + fname + "/r_s");
        r_s->serialize(seqOut_r_s);
        std::ofstream seqOut_r_b("../" + fname + "/r_b");
        r_b->serialize(seqOut_r_b);
    }

    void load(string &fname) {
        compact::vector<unsigned, 1> b(0);
        compact::vector<uint64_t> bitvec_{1};
        auto bits_per_element = compact::get_bits_per_element("../" + fname + "/bit_vector");
        bitvec_.set_m_bits(bits_per_element);
        bitvec_.deserialize("../" + fname + "/bit_vector");
        for (auto elem: bitvec_) {
            b.push_back(elem);
        }
        b_ptr = &b;
        n = b_ptr->size();

        auto n = b.size();
        int lg_n = ceil(log2(n));
        s_length = lg_n * lg_n;
        b_length = lg_n;
        s_width = lg_n;
        b_width = ceil(log2(s_length));

        compact::vector<uint64_t> r_s_v{1};
        bits_per_element = compact::get_bits_per_element("../" + fname + "/r_s");
        r_s_v.set_m_bits(bits_per_element);
        r_s_v.deserialize("../" + fname + "/r_s");

        r_s = new compact::vector<uint64_t>(s_width);
        for (auto elem: r_s_v) {
            r_s->push_back(elem);
        }

        compact::vector<uint64_t> r_b_v{1};
        bits_per_element = compact::get_bits_per_element("../" + fname + "/r_b");
        r_b_v.set_m_bits(bits_per_element);
        r_b_v.deserialize("../" + fname + "/r_b");
        r_b = &r_b_v;

        r_b = new compact::vector<uint64_t>(b_width);
        for (auto elem: r_b_v) {
            r_b->push_back(elem);
        }
    }

};

