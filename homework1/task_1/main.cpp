#include <iostream>
#include "rank_support.hpp"
#include "compact_vector/compact_vector.hpp"
using namespace std;

// 10011010000010000000010001000001

int main() {
    string save_name = "test_save";

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

//    compact::vector<unsigned, 1> b(32);
//    b[0] = 1;
//    b[3] = 1;
//    b[4] = 1;
//    b[6] = 1;
//    b[12] = 1;
//    b[21] = 1;
//    b[25] = 1;
//    b[31] = 1;
//    for (auto elem : b) cout << elem;
//    cout << endl;


//    compact::vector<unsigned, 1> b_(0);
//    rank_support r(&b_);
//    r.load(save_name);

//    rank_support r(&b);
//    cout << "rank: ";
//    for (int i=0; i<b.size(); i++){
//        cout << i << ":" << r.rank1(i) << endl;
//    }
//    cout << endl;

//    uint64_t rank_res = r.rank1(0);
//    cout << "rank: " << rank_res << endl;
//    cout << "overhead: " <<  r.overhead() << endl;

//    r.save(save_name);

    return 0;
}
