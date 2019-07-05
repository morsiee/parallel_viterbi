/* 
 * File:   HMM_IO.cpp
 * Author: essam
 * 
 * Created on June 27, 2015, 6:01 AM
 */

#include "HMM_IO.h"
#include<fstream>
#include<iostream>

HMM_IO::HMM_IO() {
}

HMM_IO::HMM_IO(const HMM_IO& orig) {
}

HMM_IO::~HMM_IO() {
}

void
HMM_IO::readHMM(std::vector<std::string> &states,
        std::unordered_map<std::string, double> &resultInitProbs,
        std::unordered_map<std::string, std::unordered_map<std::string, double>> &resultTransProbs,
        std::unordered_map<std::string, std::unordered_map<std::string, double> > &resultEmProbs,
        const std::string &filename) {
    std::ifstream in(filename.c_str());

    if (!in) {
        std::cerr << "Unable to open \"" << filename << "\"" << std::endl;
        exit(-1);
    }

    readHMM(states,
            resultInitProbs,
            resultTransProbs,
            resultEmProbs,
            in);

    in.close();
}

void
HMM_IO::readHMM(std::vector<std::string> &states,
        std::unordered_map<std::string, double> &resultInitProbs,
        std::unordered_map<std::string, std::unordered_map<std::string, double>> &resultTransProbs,
        std::unordered_map<std::string, std::unordered_map<std::string, double>> &resultEmProbs,
        std::istream &in) {

    // Clear probabilities buffers.
    resultInitProbs.clear();
    resultEmProbs.clear();
    resultTransProbs.clear();
    //read number of exit states.
    readTokenOrExit(in, "nStates");
    unsigned nStates = readOrExit<unsigned>(in, "number of states");

    //Read number of observables/zones
    readTokenOrExit(in, "nObservables");
    unsigned nObservables = readOrExit<unsigned>(in, "number of observables");

    // Read initial probabilities.
    readTokenOrExit(in, "initProbs");
    for (unsigned i = 0; i < nStates; ++i)
        resultInitProbs.insert(std::make_pair(states[i], readOrExit<double>(in, "initial probability")));
    //        resultInitProbs.push_back(readOrExit<double>(in, "initial probability"));

    //Populate transition probability map with non-zero transitions.
    readTokenOrExit(in, "transProbs");
    for (unsigned from = 0; from < nStates; ++from) {
        std::unordered_map<std::string, double> tos;
        for (unsigned to = 0; to < nStates; ++to) {
            double tp = readOrExit<double>(in, "transition probability");
            if (tp > 0)
                tos.insert(std::make_pair(states[to], tp));
        }
        resultTransProbs.insert(std::make_pair(states[from], tos));
    }

    //populate emission probability map with non-zero emission probabilities.
    readTokenOrExit(in, "emProbs");
    for (unsigned i = 0; i < nStates; ++i) {
        std::unordered_map<std::string, double> tos;
        for (unsigned ob = 0; ob < nObservables; ++ob) {
            double ep = readOrExit<double>(in, "emission probability");
            if (ep > 0.0)
                tos.insert(std::make_pair(std::to_string(ob), ep));
        }
        resultEmProbs.insert(std::make_pair(states[i], tos));
    }
}

void
HMM_IO::checkTokenOrExit(const std::string &expected,
        const std::string &found) {
    if (expected != found) {
        std::cerr << "Expected to read token \"" << expected
                << "\" but got \"" << found << "\"" << std::endl;
        exit(-1);
    }
}

bool
HMM_IO::readToken(std::istream &in,
        const std::string &expected) {
    std::string found;
    if (!(in >> found))
        return false;
    checkTokenOrExit(expected, found);
    return true;
}

void
HMM_IO::readTokenOrExit(std::istream &in,
        const std::string &expected) {
    if (!readToken(in, expected)) {
        std::cerr << "Error trying to read token " << expected << std::endl;
        exit(-1);
    }
}

void
HMM_IO::readSeq(std::vector<std::string> &result,
        const std::string &filename) {
    std::ifstream in(filename.c_str());

    if (!in) {
        std::cerr << "Unable to open \"" << filename << "\"" << std::endl;
        exit(-1);
    }

    readSeq(result, in);

    in.close();
}

void
HMM_IO::readSeq(std::vector<std::string> &result,
        std::istream &in) {
    result.clear();

    unsigned tmp = 0;
    while (in >> tmp)
        result.push_back(std::to_string(tmp));
}

void
HMM_IO::readStates(std::vector<std::string> &states,
        const std::string &filename) {
    std::ifstream in(filename.c_str());

    if (!in) {
        std::cerr << "Unable to open \"" << filename << "\"" << std::endl;
        exit(-1);
    }

    readStates(states, in);

    in.close();
}

void
HMM_IO::readStates(std::vector<std::string> &states,
        std::istream &in) {

    readTokenOrExit(in, "nStates");
    unsigned nStates = readOrExit<unsigned>(in, "number of states");

    // Read initial probabilities.
    readTokenOrExit(in, "states");
    for (unsigned i = 0; i < nStates; ++i)
        states.push_back(readOrExit<std::string>(in, "states"));
}

template<typename T>
T HMM_IO::readOrExit(std::istream &in,
        const std::string &expectedDesc) {
    T result;
    if (!(in >> result)) {
        std::cerr << "Error reading " << expectedDesc << "." << std::endl;
        exit(-1);
    }
    return result;
}