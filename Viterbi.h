/* 
 * File:   Viterbi.h
 * Author: essam
 *
 * Created on June 18, 2015, 6:19 AM
 */


#ifndef VITERBI_H
#define	VITERBI_H

#include <unordered_map>
#include <string>
#include "StateInfoTimeStamp.h"
#include "ObsIndex.h"
#include <vector>

class Viterbi {
public:
    Viterbi(void);
    Viterbi(const Viterbi& orig);
    virtual ~Viterbi();
    
    //    std::string* path;
    void
    trans_to_list(std::unordered_map<std::string, StateInfoTimeStamp*> &T,
            std::vector<std::string> &states,
            std::unordered_map<std::string, std::unordered_map<std::string, double>> &trans_p);

    std::string
    forward_viterbi(std::vector<std::string> &obs,
            std::vector<std::string> &states,
            std::unordered_map < std::string, double> &,
            std::unordered_map<std::string, std::unordered_map<std::string, double>> &emit_p,
            std::unordered_map<std::string, StateInfoTimeStamp*> &T,
            std::unordered_map<std::string, std::vector<std::string>> &oi);
    
    std::string
    parallel_viterbi(std::vector<std::string> &obs,
            std::unordered_map < std::string, double> &initProbs,
            std::unordered_map<std::string, std::unordered_map<std::string, double>> &emit_p,
            std::unordered_map<std::string, StateInfoTimeStamp*> &T,
            std::unordered_map<std::string, std::vector<std::string>> &oi);

//    template<typename T>
//    std::string str(T begin, T end);

private:

};

#endif	/* VITERBI_H */

