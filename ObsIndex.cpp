/* 
 * File:   ObsIndex.cpp
 * Author: essam
 * 
 * Created on June 18, 2015, 2:52 PM
 */

#include <unordered_map>
#include <unordered_set>
#include <string>
#include "ObsIndex.h"
#include<iostream>

ObsIndex::ObsIndex() {
}

ObsIndex::ObsIndex(const ObsIndex& orig) {
}

ObsIndex::~ObsIndex() {
}

void
ObsIndex::Initialise(std::unordered_map<std::string, std::vector<std::string>> &oi,
        std::vector<std::string> &states,
        std::unordered_map<std::string, std::unordered_map<std::string, double>> &emit_p) {
    //    std::unordered_map<std::string, std::unordered_set < std::string>> oi;
    // Iterate over all states
    for (std::string state : states) {
        std::unordered_map<std::string, std::unordered_map < std::string, double>>::const_iterator emissions_search = emit_p.find(state);
        if (emissions_search == emit_p.end()) {
            std::cout << "No emissions for state:\t" << state;
        } else {
            std::unordered_map < std::string, double> emissions = emissions_search->second;
            for (auto it = emissions.begin(); it != emissions.end(); ++it) {
                std::string cr_emission = it->first;
                // check of the emission prob is not zero, if so add
                if (it->second != 0) {
                    // check if current ObsIndex doesn't include an entry for the current emission
                    std::unordered_map<std::string, std::vector < std::string>>::const_iterator oi_search = oi.find(cr_emission);
                    std::vector<std::string> al;
                    if (oi_search == oi.end()) {
                        al.push_back(state);
                    } else {
                        //update old values.
                        al = oi_search->second;
                        al.push_back(state);
                        /*
                         * Erase/insert is a time consuming --> to be fixed.
                         */
                        oi.erase(oi_search);
                        //                        oi_search->second = al;
                    }
                    oi.insert(std::make_pair(cr_emission, al));
                }
            }
        }
    }
    //    return oi;
}
