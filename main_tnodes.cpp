/* 
 * File:   main_tnodes.cpp
 * Author: essam
 *
 * Created on September 9, 2015, 6:47 PM
 */

#include <cstdlib>
#include "AViterbi.h"
#include <iostream>
#include <cstdint>
#include <cstring>
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    AViterbi::tnode ts[4];
    cout << "size of the array:\t" << (sizeof (ts) / sizeof (*ts)) << endl;
    for (auto i = 0; i < (sizeof (ts) / sizeof (*ts)); i++) {
        AViterbi::tnode cr = ts[i];
        if (cr.flag) {
            cout << "prob: " << cr.prob;
            cout << "\t v_prob: " << cr.v_prob;
            cout << "\t v_path: " << cr.v_path[0] << endl;
        }
    }
    AViterbi::tnode tsu[4];

    AViterbi::tnode t2;
    t2.flag = true;
    t2.prob = 1.3;
    t2.v_path.push_back(2);
    tsu[2] = t2;
    memcpy(ts, tsu, sizeof (ts));
    //    *ts=&tsu;
    for (auto i = 0; i < (sizeof (ts) / sizeof (*ts)); i++) {
        AViterbi::tnode cr = ts[i];
        if (cr.flag) {
            cout << "prob: " << cr.prob;
            cout << "\t v_prob: " << cr.v_prob;
            cout << "\t v_path: " << cr.v_path[0] << endl;
        }
    }

    //    cout<< "TSU"<<endl;
    //    for(auto i=0; i< (sizeof(tsu)/sizeof(*tsu));i++){
    //        cout<<" in tsu loop"<<endl;
    //        AViterbi::tnode cr = tsu[i];
    //        if(cr.flag){
    //        cout<<"prob: "<<cr.prob;
    //        cout<<"\t v_prob: "<<cr.v_prob;
    //        cout<<"\t v_path: "<<cr.v_path[0]<<endl;
    //        }
    //    }
    return 0;
}

