/* 
 * File:   main_par_arr.cpp
 * Author: essam
 *
 * Created on September 13, 2015, 5:50 PM
 */

#include <cstdlib>
#include <omp.h>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "AViterbi.h"

using namespace std;


void populate(AViterbi::vr*, int);

/*
 * 
 */
int main(int argc, char** argv) {

    int size = 4;
    AViterbi::vr* vr_l = new AViterbi::vr[size];

    populate(vr_l, size);

    for (size_t i = 0; i < size; i++) {
        printf("%d \t %f\n", vr_l[i].index, vr_l[i].total);
    }

    return 0;
}

void populate(AViterbi::vr* vr_l, int size) {
#pragma omp parallel for
for (size_t i = 0; i < size; i++) {
        vr_l[i].index = i;
        vr_l[i].total = pow(i, 2);
    }
}

