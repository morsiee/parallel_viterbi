/* 
 * File:   StateInfoTimeStamp.cpp
 * Author: essam
 * 
 * Created on June 18, 2015, 1:50 PM
 */

#include "StateInfoTimeStamp.h"
#include "AViterbi.h"
#include <string>
#include<iostream>
#include<stdlib.h>

StateInfoTimeStamp::StateInfoTimeStamp(double p, std::string v_path, double v_prob, TransList *list) {
    this->latest = 0; // most recently written index
    this->lasttime = -1; // time at last write
    this->StatesList = list;

    //for debug
    //    this->prob = 0;
    //    this->v_path = v_path;
    //    this->v_prob = v_prob;
    //    
    //    
    mi[0].v_path = v_path;
    mi[0].v_prob = v_prob;

    mi[1].v_path = "";
    mi[1].v_prob = 0.0;

}

StateInfoTimeStamp::StateInfoTimeStamp(double p, std::string v_path, double v_prob) {
    this->latest = 0; // most recently written index
    this->lasttime = -1; // time at last write
    //    
    mi[0].v_path = v_path;
    mi[0].v_prob = v_prob;

    mi[1].v_path = "";
    mi[1].v_prob = 0.0;

}

struct StateInfoTimeStamp::ViterbiStateInfo StateInfoTimeStamp::Read(int ts_plus1) {
    // if the timestamp is older than expects then the value is zero
    // ts_plus1 is the timestamp +1
    // if the actual timestamp is less than last time, then read the old value
    //   otherwise read the new value
    //if((ts_plus1)<=lasttime)
    //    return mi[latest^1];
    //else
    //    return mi[latest];

    // what is acceptable is the following:
    // I want to read time stamp n and it exists
    // I want to read time stamp n-1 and it exists
    // 1- lasttime stamp is n, and current readts is n-1
    // 2- lasttime stamp is n, and current readts is n
    //	System.out.println("read from ts = "+lasttime+"at time ="+(ts_plus1-1));
    if (((ts_plus1 - 1) == this->lasttime) || ((ts_plus1 - 1) == (this->lasttime - 1))) {
        //        std::cout << "get viterbi state info";
        if ((ts_plus1) <= this->lasttime) {
            return this->mi[latest ^ 1];
        } else {
            return this->mi[latest];
        }
    } else {
        //	    	    System.out.println("reseting...");
        StateInfoTimeStamp::Write(0, "", 0, ts_plus1 - 1);
        //lasttime = ts_plus1-1;
        //mi[0].Write(0,"",0);
        //mi[1].Write(0,"",0);
        return mi[latest]; //either 0 or 1

    }

}

bool StateInfoTimeStamp::Write(double prob, std::string v_path, double v_prob, int ts) {
    //    bool flag = true;
//#pragma omp single
//    {
        if (ts <= lasttime) {
            std::cout << "State error in StateInfoTimeStamp; write time stamp is " << ts << ", while last written timestamp is\t" << lasttime << std::endl;
            //            flag = false;
            return false;
        }

        //    std::cout << "update values" << std::endl;

        latest = latest ^ 1;
        //write values
        mi[latest].prob = prob;
        mi[latest].v_path = v_path;
        mi[latest].v_prob = v_prob;
        mi[latest].ts = ts;

        if ((lasttime + 1) != ts) {
            mi[latest ^ 1].prob = 0;
            mi[latest ^ 1].v_path = "";
            mi[latest ^ 1].v_prob = 0;
            mi[latest ^ 1].ts = ts;
        }

        lasttime = ts; //is this correct? 
//    }
    return true;
}


bool StateInfoTimeStamp::tWrite(int index, double prob, std::string v_path, double v_prob, int ts) {
    //    bool flag = true;
//#pragma omp single
//    {
        if (ts <= lasttime) {
            std::cout << "State @id "<<index <<" error in StateInfoTimeStamp; write time stamp is " << ts << ", while last written timestamp is\t" << lasttime << std::endl;
            //            flag = false;
            return false;
        }

        //    std::cout << "update values" << std::endl;

        latest = latest ^ 1;
        //write values
        mi[latest].prob = prob;
        mi[latest].v_path = v_path;
        mi[latest].v_prob = v_prob;
        mi[latest].ts = ts;

        if ((lasttime + 1) != ts) {
            mi[latest ^ 1].prob = 0;
            mi[latest ^ 1].v_path = "";
            mi[latest ^ 1].v_prob = 0;
            mi[latest ^ 1].ts = ts;
        }

        lasttime = ts; //is this correct? 
//    }
    return true;
}


struct StateInfoTimeStamp::ViterbiStateInfo StateInfoTimeStamp::tRead(int ts_plus1) {
    // if the timestamp is older than expects then the value is zero
    // ts_plus1 is the timestamp +1
    // if the actual timestamp is less than last time, then read the old value
    //   otherwise read the new value
    //if((ts_plus1)<=lasttime)
    //    return mi[latest^1];
    //else
    //    return mi[latest];

    // what is acceptable is the following:
    // I want to read time stamp n and it exists
    // I want to read time stamp n-1 and it exists
    // 1- lasttime stamp is n, and current readts is n-1
    // 2- lasttime stamp is n, and current readts is n
    //	System.out.println("read from ts = "+lasttime+"at time ="+(ts_plus1-1));
    if (((ts_plus1 - 1) == this->lasttime) || ((ts_plus1 - 1) == (this->lasttime - 1))) {
        //        std::cout << "get viterbi state info";
        if ((ts_plus1) <= this->lasttime) {
            return this->mi[latest ^ 1];
        } else {
            return this->mi[latest];
        }
    } else {
        ViterbiStateInfo tmi;
        tmi.prob = 0;
        tmi.v_path="";
        tmi.v_prob=0;
        //	    	    System.out.println("reseting...");
//        StateInfoTimeStamp::Write(0, "", 0, ts_plus1 - 1);
        //lasttime = ts_plus1-1;
        //mi[0].Write(0,"",0);
        //mi[1].Write(0,"",0);
        return tmi; //either 0 or 1

    }

}
//bool StateInfoTimeStamp::Write(AViterbi::vr tvr, int ts) {
//    //    bool flag = true;
//    //#pragma omp critical
//    //    {
//    if (ts <= lasttime) {
//        std::cout << "State error in StateInfoTimeStamp; write time stamp is " << ts << ", while last written timestamp is\t" << lasttime << std::endl;
//        //            flag = false;
//        return false;
//    }
//
//    //    std::cout << "update values" << std::endl;
//
//    latest = latest ^ 1;
//    //write values
//    mi[latest].prob = tvr.total;
//    mi[latest].v_path = tvr.argmax;
//    mi[latest].v_prob = tvr.valmax;
//    mi[latest].ts = ts;
//
//    if ((lasttime + 1) != ts) {
//        mi[latest ^ 1].prob = 0;
//        mi[latest ^ 1].v_path = "";
//        mi[latest ^ 1].v_prob = 0;
//        mi[latest ^ 1].ts = ts;
//    }
//
//    lasttime = ts; //is this correct? 
//    //    }
//    return true;
//}