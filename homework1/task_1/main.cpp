#include <iostream>
#include <vector>
#include "rank_support.hpp"
#include "compact_vector/compact_vector.hpp"
#include <chrono>
using namespace std::chrono;
using namespace std;

// 10011010000010000000010001000001

void run_from_load(string &load_name) {
    compact::vector<unsigned, 1> b_(0);
    rank_support r(&b_);
    r.load(load_name);

    uint64_t rank_res = r.rank1(0);
    cout << "rank: " << rank_res << endl;
    cout << "overhead: " <<  r.overhead() << endl;
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
    cout << "rank: ";
    for (int i=0; i<b.size(); i++){
        cout << i << ":" << r.rank1(i) << endl;
    }
    cout << endl;

    uint64_t rank_res = r.rank1(0);
    cout << "rank: " << rank_res << endl;
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

void run_experiment(int rank_calls) {
    vector<int> vec_sizes {1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    cout << "vector_length, microseconds, overhead" << endl;
    for (int i=0; i < vec_sizes.size(); i++){
        compact::vector<unsigned, 1> b(vec_sizes[i]);
        rank_support r(&b);
        auto start = high_resolution_clock::now();
        for (int j=0; j<rank_calls; j++){
            r.rank1(20);
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << vec_sizes[i] << ", " << duration.count() << ", " << r.overhead() << endl;
    }
}

int main() {
    string file_name = "test_save";

//    run_from_load(file_name);
//    run_single_test(false, file_name);
    run_experiment(10000);
//    run_user_input();



    return 0;
}
