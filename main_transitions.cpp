/* 
 * File:   main_transitions.cpp
 * Author: essam
 *
 * Created on June 28, 2015, 3:25 PM
 */

#include <cstdlib>
#include "HMM_IO.h"
#include "Viterbi.h"
#include <unordered_map>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>
#include<stddef.h>
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
    std::unordered_map<std::string, StateInfoTimeStamp> T;
    vit.trans_to_list(T, states, transProbs);

    /*
     * Iterate stateInfo Time stamp matrix to find the problem in its conversion:
     */
    cout << "start printing:" << endl;
    for (auto it = T.begin(); it != T.end(); ++it) {
        string state = it->first;
        cout << "state\t" << state << endl;
        StateInfoTimeStamp si = it->second;
        TransList list = *si.StatesList;
        list.displayList(list.head);
    }
    return 0;
}

