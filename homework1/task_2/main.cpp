#include <iostream>
#include <vector>
#include "select_support.hpp"
#include "compact_vector/compact_vector.hpp"
#include <chrono>
using namespace std::chrono;
using namespace std;

// 10011010000010000000010001000001

void run_from_load(string &load_name) {
    compact::vector<unsigned, 1> b_(15);
    rank_support r(&b_);
    select_support s(&r);
    s.load(load_name);

    uint64_t select_res = s.select1(6);
    cout << "select: " << select_res << endl;
    cout << "overhead: " <<  r.overhead() << endl;
    cout << "b size: " <<  s.b_ptr->size() << endl;
    for (auto elem : *s.b_ptr) cout << elem;
}

void run_single_test(bool save, string &save_name) {
    compact::vector<unsigned, 1> b(32);
    b[0] = 1;
    b[3] = 1;
    b[4] = 1;
    b[6] = 1;
    b[12] = 1;
    b[21] = 1;
    b[25] = 1;
    b[31] = 1;
    for (auto elem : b) cout << elem;
    cout << endl;

    rank_support r(&b);
    select_support s(&r);
    u_int64_t rank_res = r.rank1(31);
    for (int i=1; i<=rank_res; i++){
        cout << i << ":" << s.select1(i) << endl;
    }
    cout << endl;

//    uint64_t select_res = s.select1(8);
//    cout << "select: " << select_res << endl;
    cout << "overhead: " <<  r.overhead() << endl;

    if (save) {
        r.save(save_name);
    }
}

void run_user_input() {
    string b_input;
    cout << "Enter bit vector string: ";
    cin >> b_input;
    compact::vector<unsigned, 1> b(b_input.size());
    for (int i=0; i<b_input.size(); i++){
        b[i] = b_input[i];
    }
    rank_support r(&b);

    uint64_t rank_input;
    cout << "Enter i to find rank: ";
    cin >> rank_input;
    cout << "rank: " << r.rank1(rank_input) << endl;
    cout << "overhead: " << r.overhead() << endl;
}

void run_experiment(int select_calls) {
    cout << "vector_length, microseconds, overhead" << endl;
    for (int i=10; i < 20; i++){
        int size = exp2(i);
        compact::vector<unsigned, 1> b(size);
        for (int j=0; j<60; j++){
            b[j] = 1;
        }
        rank_support r(&b);
        select_support s(&r);
        auto start = high_resolution_clock::now();
        for (int j=0; j<select_calls; j++){
            s.select1(20);
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << size << ", " << duration.count() << ", " << r.overhead() << endl;
    }
}

int main() {
    string file_name = "test_save";
    run_from_load(file_name);
//    run_single_test(true, file_name);
//    run_experiment(10000);
//    run_user_input();

    return 0;
}
