#include <iostream>
#include "rank_support.hpp"
#include "compact_vector/compact_vector.hpp"
using namespace std;


int main() {
    string save_name = "test_save";

    compact::vector<unsigned, 1> b(32);
    b[0] = 1;
    b[3] = 1;
    b[4] = 1;
    b[6] = 1;
    b[12] = 1;
    b[21] = 1;
    b[25] = 1;
    b[31] = 1;

    for (auto elem : b) {
        cout << elem;
    }
    cout << endl;

    rank_support r(&b);
//    compact::vector<unsigned, 1> b_(0);
//    rank_support r(&b_);
//    r.load(save_name);
    auto rank_result = r.rank1(30);
    cout << "rank: " << rank_result << endl;

    auto overhead_res = r.overhead();
    cout << "overhead: " << overhead_res << endl;

//    r.save(save_name);

    return 0;
}
