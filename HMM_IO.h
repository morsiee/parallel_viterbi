/* 
 * File:   HMM_IO.h
 * Author: essam
 *
 * Created on June 27, 2015, 6:01 AM
 */

#ifndef HMM_IO_H
#define	HMM_IO_H
#include<unordered_map>
#include<string>
#include<vector>
#include<cstdlib>

class HMM_IO {
public:
    HMM_IO();
    HMM_IO(const HMM_IO& orig);
    virtual ~HMM_IO();
    void
    checkTokenOrExit(const std::string &expected,
            const std::string &found);
    void
    readHMM(std::vector<std::string> &states,
            std::unordered_map<std::string, double> &resultInitProbs,
            std::unordered_map<std::string, std::unordered_map<std::string, double>> &resultTransProbs,
            std::unordered_map<std::string, std::unordered_map<std::string, double> > &resultEmProbs,
            const std::string &filename);

    void
    readHMM(std::vector<std::string> &states,
            std::unordered_map<std::string, double> &resultInitProbs,
            std::unordered_map<std::string, std::unordered_map<std::string, double>> &resultTransProbs,
            std::unordered_map<std::string, std::unordered_map<std::string, double> >&resultEmProbs,
            std::istream &in);

    template<typename T>
    T readOrExit(std::istream &in,
            const std::string &expectedDesc);

    void
    readSeq(std::vector<std::string> &result,
            const std::string &filename);
    void
    readSeq(std::vector<std::string> &result,
            std::istream &in);

    void
    readStates(std::vector<std::string> &states,
            const std::string &filename);
    void
    readStates(std::vector<std::string> &states,
            std::istream &in);

    bool readToken(std::istream &in,
            const std::string &token);

    void readTokenOrExit(std::istream &in,
            const std::string &expectedToken);
private:

};

#endif	/* HMM_IO_H */

