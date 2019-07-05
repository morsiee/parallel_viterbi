/* 
 * File:   main_arr_viterbi.cpp
 * Author: essam
 *
 * Created on September 8, 2015, 1:50 PM
 */

#include <cstdlib>
#include "HMM_IO.h"
#include "Viterbi.h"
#include "AViterbi.h"
#include <unordered_map>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>
#include <ctime>
#include<boost/lexical_cast.hpp>
#include <parredhmmlib/timing.hh>
#include<omp.h>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    int nthreads = boost::lexical_cast<int>(argv[1]);
    int nobservables = boost::lexical_cast<int>(argv[2]);
    string hmmFilename = argv[3];
    string seqFilename = argv[4];
    string statesFilename = argv[5];

    omp_set_num_threads(nthreads);

    vector<string> states;
    unordered_map < string, double> initProbs;
    unordered_map<string, unordered_map < string, double>> transProbs;
    unordered_map<string, unordered_map < string, double>> emProbs;
    vector<string> oseq;

    HMM_IO hmm_io;
    //#pragma omp parallel
    //    {
    //read states
    hmm_io.readStates(states, statesFilename);
    hmm_io.readHMM(states, initProbs, transProbs, emProbs, hmmFilename);
    hmm_io.readSeq(oseq, seqFilename);
    //    }

    int nstates = states.size();
    int seq_len = oseq.size();
#if 0
    printf("nstates: %d,nobservables: %d,seq_len: %d\n", nstates, nobservables, seq_len);
#endif

    double init_p[nstates];
    AViterbi::mnode trans_ap[nstates];
    AViterbi::mnode emis_ap[nobservables];
    int seq[seq_len];

    AViterbi viterbi;
    //#pragma omp parallel
    //    {
    viterbi.conv_trans_arr_1(transProbs, states, trans_ap);
#if 0
    vector<AViterbi::node> nodes = trans_ap[15].nodes;
    for (auto it = nodes.begin(); it != nodes.end(); it++) {
        AViterbi::node n = *it;
        cout << n.index << " : " << n.prob << endl;
    }
#endif

    viterbi.conv_emis_arr(emProbs, states, emis_ap);

    viterbi.conv_init(initProbs, states, init_p);
    viterbi.conv_seq(oseq, seq);
    //    }
#if 0
    std::cout << "the number of states from main: " << (sizeof (init_p) / sizeof (*init_p)) << std::endl;
    std::cout << "the number of states from trans main: " << (sizeof (trans_ap) / sizeof (*trans_ap)) << std::endl;
    std::cout << "the number of observables from main: " << (sizeof (emis_ap) / sizeof (*emis_ap)) << std::endl;


    //check emission ...
    vector<AViterbi::node> nodes = emis_ap[1].nodes;
    for (auto it = nodes.begin(); it != nodes.end(); it++) {
        AViterbi::node n = *it;
        cout << n.index << " : " << n.prob << endl;
    }

    for (size_t i = 0; i < (sizeof (init_p) / sizeof (*init_p)); i++) cout << init_p[i] << endl;
    viterbi.print(init_p);
#endif


    //    cout << "start ...." << endl;
    //    vector<int> path;
    string path;
    double elapsed_secs, likelihood;
    parredhmmlib::Timer timer;

#if 1
    printf("Serial ....\n");
    timer.start();
    likelihood = viterbi.forward_viterbi_2(path, seq_len, seq, init_p, trans_ap, emis_ap);
    timer.end();
    elapsed_secs = timer.timeElapsed();
    cout << "elapse time: " << elapsed_secs << endl;
    printf("likelihood = %.*e \t viterbi path: %s\n", 5, likelihood, path.c_str());
#else

    timer.start();
    likelihood = viterbi.forward_viterbi_par_1(path, seq_len, seq, init_p, trans_ap, emis_ap);
    timer.end();
    elapsed_secs = timer.timeElapsed();
    cout << "elapse time: " << elapsed_secs << endl;

    printf("likelihood = %.*e \t viterbi path: %s\n", 5, likelihood, path.c_str());
#endif
    return 0;
}

