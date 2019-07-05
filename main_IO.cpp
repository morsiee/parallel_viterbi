/* 
 * File:   main_IO.cpp
 * Author: essam
 *
 * Created on June 27, 2015, 6:58 AM
 * g++ -std=c++11 main_IO.cpp HMM_IO.cpp Viterbi.cpp TransList.cpp StateInfoTimeStamp.cpp ObsIndex.cpp -o main_io.out
 * dir=/home/essam/HMM/parredhmm/parredhmmlib-1.0.0/benchmark/models/test
 * ./main_io.out $dir/16.hmm $dir/8.seq $dir/16.states
 */

#include <cstdlib>
#include "HMM_IO.h"
#include "Viterbi.h"
#include <unordered_map>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>

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
    cout << "states " << states[5] << endl;

    hmm_io.readHMM(states, initProbs, transProbs, emProbs, hmmFilename);
    vector<string> seq;
    hmm_io.readSeq(seq, seqFilename);

    cout << "initial prob of 1\t" << initProbs.find("1")->second << endl;
    double tp_11 = -1;
    unordered_map<string, unordered_map < string, double>>::const_iterator tp_search = transProbs.find(to_string(1));
    if (tp_search != transProbs.end()) {
        if (tp_search->second.find("1") != tp_search->second.end())
            tp_11 = transProbs.find(to_string(1))->second.find(to_string(1))->second;
    }

    cout << "transition probability at 1_1\t" << tp_11 << endl;
    unordered_map<string, unordered_map < string, double>>::const_iterator emp_search = emProbs.find("1");
    double emp_11 = -1;
    if (emp_search != emProbs.end()) {
        if (emp_search->second.find("1") != emp_search->second.end())
            emp_11 = emProbs.find(to_string(1))->second.find(to_string(1))->second;
    }
    cout << "Emission probability at 1_1\t" << emp_11 << endl;

    Viterbi vit;
    std::unordered_map<std::string, StateInfoTimeStamp*> T;
    vit.trans_to_list(T, states, transProbs);

    ObsIndex obsIndex;
    unordered_map<string, vector < string>> oi;
    obsIndex.Initialise(oi, states, emProbs);

    stringstream result;
    copy(oi.find("1")->second.begin(), oi.find("1")->second.end(), ostream_iterator<string>(result, " "));

    cout << "emitted states for state 1\t" << result.str() << endl;

    /*
     * Iterate stateInfo Time stamp matrix to find the problem in its conversion:
     */
    cout << "start printing:" << endl;

    for (auto it = T.begin(); it != T.end(); ++it) {
        string state = it->first;
        cout << "state\t" << state << endl;
        StateInfoTimeStamp *si = it->second;
        TransList *list = si->StatesList;
        cout << "Head before:\t" << list->head << endl;
        TransList::Node *head = list->head;
        list->displayList(head);
        cout << "Head before:\t" << list->head << endl;
    }

    cout << "Viterbi path\t" << vit.forward_viterbi(seq, states, initProbs, emProbs, T, oi) << endl;

    return 0;
}

