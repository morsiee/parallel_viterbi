/* 
 * File:   Viterbi.cpp
 * Author: essam
 * 
 * Created on June 18, 2015, 6:19 AM
 */

#include "Viterbi.h"
#include "TransList.h"
#include <unordered_map>
#include <string>
#include<iostream>
#include <stddef.h>
#include <omp.h>
//#define NULL 0 

Viterbi::Viterbi() {
}

Viterbi::Viterbi(const Viterbi& orig) {
}

Viterbi::~Viterbi() {
}

/**
 * Start probabilities are uniform, in this function, 
 * so I'll reduce the <\t std::unordered_map<std::string, double> start_p \t>
 * If we are going to using initialize the  transition states we have to add it.
 */
void
Viterbi::trans_to_list(std::unordered_map<std::string, StateInfoTimeStamp*> &T,
        std::vector<std::string> &states,
        std::unordered_map<std::string, std::unordered_map<std::string, double>> &trans_p) {

    //    std::unordered_map<std::string, StateInfoTimeStamp> T;
    for (std::vector<std::string>::size_type i = 0; i != states.size(); i++) {
        std::string state = states[i];
        /*define transition list for all states reachable from the current*/
        TransList *tranList = new TransList;
        TransList::Node* cr_node = new TransList::Node;


        for (std::vector<std::string>::size_type j = 0; j != states.size(); j++) {
            std::string source_state = states[j];
            double trans_prob = 0;
            std::unordered_map<std::string, std::unordered_map<std::string, double> >::const_iterator tp_search = trans_p.find(source_state);
            if (tp_search != trans_p.end()) {
                //find state probability.
                std::unordered_map<std::string, double>::const_iterator tps_search = tp_search->second.find(state);
                if (tps_search != tp_search->second.end()) {
                    trans_prob = tps_search->second;
                }
            }

            if (trans_prob > 0.0) {
                //                std::cout << "add node" << std::endl;
                cr_node = tranList->initNode(source_state, trans_prob);
                tranList->addNode(cr_node);
            }
        }

        StateInfoTimeStamp *csi = new StateInfoTimeStamp(0.0, state, 0.0, tranList);
        //        std::cout << "display list from function:\t" << std::endl;
        //        tranList->displayList(tranList->head);
        T.insert(std::make_pair(state, csi));
    }
    //    return T;
}

std::string
Viterbi::forward_viterbi(std::vector<std::string> &obs,
        std::vector<std::string> &states,
        std::unordered_map < std::string, double> &initProbs,
        std::unordered_map<std::string, std::unordered_map<std::string, double>> &emit_p,
        std::unordered_map<std::string, StateInfoTimeStamp*> &T,
        std::unordered_map<std::string, std::vector<std::string>> &oi) {

    //     get emission states
    std::unordered_map<std::string, std::vector < std::string>>::const_iterator oi_emit_0_states = oi.find(obs[0]);
    std::vector<std::string> substates;
    if (oi_emit_0_states != oi.end()) {
        substates = oi_emit_0_states->second;
        if (substates.empty()) {
            // return empty path
            return "";
        }
    }
    int count = 0;
    //initialize states
    for (auto &cr_state : substates) {
        double em = emit_p.find(cr_state)->second.find(obs[0])->second;
        double ip = initProbs.find(cr_state)->second;
        //        std::cout << "1" << std::endl;
        //predefined initialization values..
        double v_prob = ip * em;
        //uniform initialization ...
        //        double v_prob = (1.0 / states.size()) * em;
        StateInfoTimeStamp *cr_si = T.find(cr_state)->second;
        //        std::cout << "2" << std::endl;
        bool flag = cr_si->Write(0, cr_state, v_prob, count);
        if (!flag) {
            // return empty path
            return "";
        }
    }
    count++;
   for (auto it=obs.begin()+1; it!=obs.end(); it++) {
        std::string output= *it;
        substates = oi.find(output)->second;
        //        std::cout << "Obs\t" << output << std::endl;
        if (substates.empty()) {
            // return empty path
            return "";
        }

        //        int actual_count = 0;
        for (auto &cr_state : substates) {
            //            std::cout << "current state\t" << cr_state << "\t Observation\t" << output << std::endl;
            double em = emit_p.find(cr_state)->second.find(output)->second;
            //            std::cout << "4 \t em: \t" << em << std::endl;
            double total = 0;
            std::string argmax = "";
            double valmax = 0;
            double prob = 1;
            std::string v_path = "";
            double v_prob = 1;
            StateInfoTimeStamp *ns = T.find(cr_state)->second;

            //            std::cout << "find state --> ts\t" << T.find(cr_state)->first << "\t" << cr_state << std::endl;

            /*Iterate all states reached from the current state*/
            TransList *list = ns->StatesList;
            TransList::Node *cn = list->head;
            //            list->displayList(list->head);
            while (cn) {
                //                std::cout << "in while with source state\n " << cn->state << std::endl;
                std::string source_state = cn->state;
                StateInfoTimeStamp *src_si = T.find(source_state)->second;
                //                std::cout << "6\t" << source_state << std::endl;
                StateInfoTimeStamp::ViterbiStateInfo v = src_si->Read(count);
                //                std::cout << "read viterbi state info" << std::endl;
                prob = v.prob;
                v_path = v.v_path;
                v_prob = v.v_prob;

                //                                prob = src_si.prob;
                //                                v_path = src_si.v_path;
                //                                v_prob = src_si.v_prob;

                double p;
                double trans_prob = cn->prob;
                p = em*trans_prob;
                v_prob *= p;
                if (v_prob > valmax) {
                    argmax = v_path + ',' + cr_state;
                    valmax = v_prob;
                    total = prob;
                }
                cn = cn->next;
            }
            //            std::cout << "write" << std::endl;
            // track write operation and stored values.
            //            std::cout << "Values\t" << argmax << " , " << valmax << std::endl;
            //            std::cout << "Before\t" << ns->mi[ns->latest].v_path << " , " << ns->mi[ns->latest].v_prob << std::endl;
            //            ns->prob = total + 1;
            //            ns->v_path = argmax;
            //            ns->v_prob = valmax;
            bool flag = ns->Write(total + 1, argmax, valmax, count);
            //            std::cout << "After\t" << ns->mi[ns->latest].v_path << " , " << ns->mi[ns->latest].v_prob << std::endl;
            //             remove and re-add ns -- > time consuming fix 
            //                        T.erase(T.find(cr_state));
            //                        T.insert(std::make_pair(cr_state, ns));

            if (!flag) {
                //return empty path;
                return "";
            }
        }
        count++;
    }

    //    std::cout << "start backtracking " << std::endl;
    double total = 0;
    std::string argmax = "";
    double valmax = 0;

    double prob = 1;
    std::string v_path = "";
    double v_prob = 1;
    substates = oi.find(obs[obs.size() - 1])->second;
    for (auto &state : substates) {
        StateInfoTimeStamp *si = T.find(state)->second;
        StateInfoTimeStamp::ViterbiStateInfo v = si->Read(count);
        prob = v.prob;
        v_path = v.v_path;
        v_prob = v.v_prob;
        total += prob;
        if (v_prob > valmax) {
            argmax = v_path;
            valmax = v_prob;
        }
    }
    //    path = argmax;
    return argmax;
}

std::string Viterbi::parallel_viterbi(
        std::vector<std::string>& obs,
        std::unordered_map<std::string, double>& initProbs,
        std::unordered_map<std::string, std::unordered_map<std::string, double> >& emit_p,
        std::unordered_map<std::string, StateInfoTimeStamp*>& T,
        std::unordered_map<std::string, std::vector<std::string> >& oi) {

    
}