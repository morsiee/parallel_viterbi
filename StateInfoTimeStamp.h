/* 
 * File:   StateInfoTimeStamp.h
 * Author: essam
 *
 * Created on June 18, 2015, 1:50 PM
 */

#ifndef STATEINFOTIMESTAMP_H
#define	STATEINFOTIMESTAMP_H
#include<string>
#include"TransList.h"

class StateInfoTimeStamp {
public:

    // can be reduced to only two parameter represent v_path, and v_prob.

    struct ViterbiStateInfo {
        double prob; // to be removed
        std::string v_path;
        double v_prob;
        int ts; // to be removed only used for debugging
    };

    StateInfoTimeStamp(double, std::string, double, TransList*);
    StateInfoTimeStamp(double, std::string, double);
    bool Write(double p, std::string vp, double vph, int ts);
    struct ViterbiStateInfo Read(int ts_plus1);
    //debug
    bool tWrite(int index, double prob, std::string v_path, double v_prob, int ts);
    struct ViterbiStateInfo tRead(int ts_plus1);
    
    TransList *StatesList; //first predecessor state transition node (my_node)                                        
//int lasttime; 

    //debug
    //    double prob;
    //    std::string v_path;
    //    double v_prob;
private:
    ViterbiStateInfo mi[2];
    int latest; // the index of the most recently written ViterbiStateInfo
    int lasttime; // the time at which the last write is done to the ViterbiStateInfo
};

#endif	/* STATEINFOTIMESTAMP_H */

