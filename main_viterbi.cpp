/* 
 * File:   main_viterbi.cpp
 * Author: essam
 *
 * Created on June 27, 2015, 2:11 PM
 * CXX_FLAGS =  -g -O3 -Wall -march=native -msse4 -fopenmp
 * 
 * g++ -std=c++11 -g -O3 -Wall -march=native -msse4 main_viterbi.cpp HMM_IO.cpp Viterbi.cpp TransList.cpp StateInfoTimeStamp.cpp ObsIndex.cpp -o oviterbi.o
 * ./main_vit.out $dir/16.hmm $dir/8.seq $dir/16.states
 */

#include <cstdlib>
#include "HMM_IO.h"
#include "Viterbi.h"
#include <unordered_map>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>
#include <ctime>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    string hmmFilename = argv[1];
    string seqFilename = argv[2];
    string statesFilename = argv[3];

    cout << hmmFilename << " , " << seqFilename << endl;
    vector<string> states;
    //    vector<double> initProbs;
    unordered_map < string, double> initProbs;
    unordered_map<string, unordered_map < string, double>> transProbs;
    unordered_map<string, unordered_map < string, double>> emProbs;

    HMM_IO hmm_io;
    //read states
    hmm_io.readStates(states, statesFilename);
    hmm_io.readHMM(states, initProbs, transProbs, emProbs, hmmFilename);
    vector<string> seq;
    hmm_io.readSeq(seq, seqFilename);
    Viterbi vit;
    std::unordered_map<std::string, StateInfoTimeStamp*> T;
    vit.trans_to_list(T, states, transProbs);

    ObsIndex obsIndex;
    unordered_map<string, vector < string>> oi;
    obsIndex.Initialise(oi, states, emProbs);
    clock_t begin = clock();
    string path = vit.forward_viterbi(seq, states, initProbs, emProbs, T, oi);
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    cout << "Viterbi path\t" << path << endl;
    cout << "Elapsed time \t" << elapsed_secs << endl;
   

    return 0;
}

