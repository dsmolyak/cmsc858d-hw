//
// Created by Daniel Smolyak on 3/10/22.
//

#include <cstdint>
#include <bitset>
#include <iostream>
#include <cmath>
#include <filesystem>
#include "compact_vector/compact_vector.hpp"
#include "rank_support.hpp"

using namespace std;

class select_support {

public:
    rank_support *r_ptr;
    compact::vector<unsigned, 1> *b_ptr;
    uint64_t n;

    select_support(rank_support *r) {
        r_ptr = r;
        b_ptr = r->b_ptr;
    }

    uint64_t select1(uint64_t i){
        int left = 0;
        int right = b_ptr->size();
        int mid = floor((left + right) / 2);
        uint64_t rank_mid = r_ptr->rank1(mid);
        while (not (rank_mid == i and (mid == 0 or r_ptr->rank1(mid-1) == i-1))) {
//            cout << "Curr mid: " << mid << endl;
//            cout << rank_mid << endl;
//            cout << r_ptr->rank1(mid-1) << endl;
            if (rank_mid < i) { // search right
                left = mid+1;
            }
            else { // search left
                right = mid-1;
            }
            int new_mid = floor((left + right)/2);
//            if (new_mid == mid) {
//                break;
//            }
            mid = new_mid;
            rank_mid = r_ptr->rank1(mid);
        }
        return mid;
    }

    uint64_t overhead() {
        auto result = r_ptr->overhead();
        return result;
    }

    void save(string& fname){
        r_ptr->save(fname);
    }

    void load(string& fname){
        r_ptr->load(fname);
        b_ptr = r_ptr->b_ptr;
    }

};