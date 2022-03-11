#include <iostream>
#include <vector>
#include "sparse_array.hpp"
#include "compact_vector/compact_vector.hpp"
#include <chrono>
using namespace std::chrono;
using namespace std;


void run_from_load(string &load_name) {

}

void run_single_test(bool save, string &save_name) {
    sparse_array sa;
    sa.create(32);
    sa.append("apple", 3);
    sa.append("banana", 7);
    sa.append("coconut", 31);

    string elem;
    bool res = sa.get_at_rank(1, elem);
    cout << res << endl;
    cout << elem << endl;

    res = sa.get_at_index(21, elem);
    cout << res << endl;
    res = sa.get_at_index(31, elem);
    cout << res << endl;
    cout << elem << endl;

    cout << sa.num_elem_at(31) << endl;

    if (save) {
        sa.save(save_name);
    }
}

void run_experiment(int get_at_index_calls) {
    cout << "vector_length, sparsity, microseconds" << endl;
    vector<double> sparsities{.01,.05,.1};
    for (int n=4; n < 8; n++){
        for (int i=0; i<sparsities.size(); i++){
            int size = pow(10, n);
            sparse_array sa;
            sa.create(size);
            for (int pos=0; pos<(int) sparsities[i] * size; pos++){
                sa.append("sample", pos);
            }
            string elem;
            auto start = high_resolution_clock::now();
            for (int j=0; j<get_at_index_calls; j++){
                sa.get_at_index(20, elem);
            }
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            cout << size << ", " << sparsities[i] << ", " << duration.count() << endl;
        }

    }
}

int main() {
    string file_name = "test_save";
//    run_from_load(file_name);
//    run_single_test(true, file_name);
    run_experiment(10000);
//    run_user_input();

    return 0;
}
