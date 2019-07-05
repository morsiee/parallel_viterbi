/* 
 * File:   AViterbi.cpp
 * Author: essam
 * 
 * Created on September 8, 2015, 1:37 PM
 */

#include "AViterbi.h"
#include<boost/lexical_cast.hpp>
#include<cstring>
#include<iostream>
#include <float.h>
#include <omp.h>

//int Digs = DECIMAL_DIG;

AViterbi::AViterbi() {
}

AViterbi::AViterbi(const AViterbi& orig) {
}

AViterbi::~AViterbi() {
}

void AViterbi::conv_init(std::unordered_map<std::string, double>& initProbs, std::vector<std::string>& states, double* init) {
    for (auto it = initProbs.begin(); it != initProbs.end(); it++) {
        init[std::find(states.begin(), states.end(), it->first) - states.begin()] = it->second;
    }
}

void AViterbi::conv_seq(std::vector<std::string>& oseq, int* seq) {
    for (size_t i = 0; i < oseq.size(); i++) {
        seq[i] = boost::lexical_cast<int>(oseq[i]);
    }
}

void AViterbi::conv_trans_arr(
        std::unordered_map<std::string, std::unordered_map<std::string, double> >& trans_p,
        std::vector<std::string>& states, mnode* trans_ap) {

    for (auto it = trans_p.begin(); it != trans_p.end(); it++) {
        int from = std::find(states.begin(), states.end(), it->first) - states.begin();
        std::unordered_map<std::string, double> reachables = it->second;
        for (auto it_r = reachables.begin(); it_r != reachables.end(); it_r++) {
            node n;
            n.index = std::find(states.begin(), states.end(), it_r->first) - states.begin();
            //            std::string state = std::to_string(n.index);
            n.prob = it_r->second;
            //            n.csi = new StateInfoTimeStamp(0.0, state, 0.0);
            trans_ap[from].nodes.push_back(n);
        }
        trans_ap[from].csi = new StateInfoTimeStamp(0.0, boost::lexical_cast<std::string>(from), 0.0);
    }

}

/**
 * convert and transpose -- the correct one.
 * @param trans_p
 * @param states
 * @param trans_ap
 */
void AViterbi::conv_trans_arr_1(
        std::unordered_map<std::string, std::unordered_map<std::string, double> >& trans_p,
        std::vector<std::string>& states, mnode* trans_ap) {

    for (auto it = trans_p.begin(); it != trans_p.end(); it++) {
        int from = std::find(states.begin(), states.end(), it->first) - states.begin();


        std::unordered_map<std::string, double> reachables = it->second;
        for (auto it_r = reachables.begin(); it_r != reachables.end(); it_r++) {

            int to = std::find(states.begin(), states.end(), it_r->first) - states.begin();
            //            std::string state = std::to_string(n.index);
            node n;
            n.index = from;
            n.prob = it_r->second;
            //            n.csi = new StateInfoTimeStamp(0.0, state, 0.0);
            trans_ap[to].nodes.push_back(n);
            trans_ap[to].csi = new StateInfoTimeStamp(0.0, boost::lexical_cast<std::string>(to), 0.0);
        }

    }

}

void AViterbi::conv_emis_arr(
        std::unordered_map<std::string, std::unordered_map<std::string, double> >& emis_p,
        std::vector<std::string>& states, mnode* emis_ap) {

    for (auto it = emis_p.begin(); it != emis_p.end(); it++) {

        int index = std::find(states.begin(), states.end(), it->first) - states.begin();
        std::unordered_map<std::string, double> reachables = it->second;
        for (auto it_r = reachables.begin(); it_r != reachables.end(); it_r++) {
            int zone = boost::lexical_cast<int>(it_r->first);

            node n;
            n.index = index;
            n.prob = it_r->second;
            //            if (n.prob == 0) continue;

            emis_ap[zone].nodes.push_back(n);
        }
    }

}

void AViterbi::forward_viterbi(std::vector<int>& path, int nstates, int nseq, int* seq, double* init, mnode* trans_ap, mnode* emis_ap) {
    tnode T[nstates];
    //    std::cout << "in viterbi ..." << std::endl;
    //    std::cout<< "the number of states: "<<(sizeof (trans_ap) / sizeof (*trans_ap))<<std::endl;
    std::vector<node> substates = emis_ap[seq[0]].nodes;
    //    std::cout<<"substates size: "<<substates.size()<<std::endl;
    int count = 0;
    for (auto it = substates.begin(); it != substates.end(); it++) {
        node snode = *it;
        int index = snode.index;
        std::cout << index << std::endl;
        T[index].flag = true;
        T[index].prob = init[index];
        T[index].v_prob = init[index];
        T[index].v_path.push_back(index);
    }

    //    std::cout<<"Finish initialization .."<<std::endl;
    for (size_t i = 0; i < nseq; i++) {
        substates = emis_ap[seq[i]].nodes;
        if (substates.empty()) return;
        tnode U[nstates];

        for (size_t j = 0; j < substates.size(); j++) {
            node nxt_state = substates[j];
            double total = 0;
            std::vector<int> argmax;
            double valmax = 0;

            std::vector<node> r_nodes = trans_ap[nxt_state.index].nodes;
            if (r_nodes.empty()) return;
            for (size_t k = 0; k < r_nodes.size(); k++) {
                node state = r_nodes[k];
                tnode v = T[state.index];
                if (!v.flag) continue;
                double prob = v.prob;
                std::vector<int> v_path = v.v_path;
                double v_prob = v.v_prob;

                double p = nxt_state.prob * state.prob;

                prob *= p;
                v_prob *= p;
                total += prob;
                if (v_prob > valmax) {
                    argmax = v_path;
                    v_path.push_back(nxt_state.index);
                    valmax = v_prob;
                }
            }
            U[nxt_state.index].flag = true;
            U[nxt_state.index].prob = total;
            U[nxt_state.index].v_path = argmax;
            U[nxt_state.index].v_prob = valmax;
        }
        memcpy(T, U, sizeof (T));
    }

    double total = 0;
    double valmax = 0;
    //    std::vector<int> argmax;

    for (auto it = substates.begin(); it != substates.end(); it++) {
        node lnode = *it;
        double prob = T[lnode.index].prob;
        std::vector<int> v_path = T[lnode.index].v_path;
        double v_prob = T[lnode.index].v_prob;
        total += prob;
        if (v_prob > valmax) {
            path = v_path;
            valmax = v_prob;
        }
    }
    std::cout << "Prob: " << valmax << std::endl;

}

double AViterbi::forward_viterbi_1(std::string& path, int nseq, int* seq, double* init, mnode* trans_ap, mnode* emis_ap) {
    int count = 0;
    std::vector<node> substates = emis_ap[seq[0]].nodes;
    // initialize states emitted by the first observation ...
    for (size_t i = 0; i < substates.size(); i++) {
        node snode = substates[i];
        int index = snode.index;
        double v_prob = init[index] * snode.prob;
        bool flag = trans_ap[index].csi->Write(0, boost::lexical_cast<std::string>(index), v_prob, count);
        if (!flag) return 0;
    }
    count++;
    for (size_t i = 1; i < nseq; i++) {
        substates = emis_ap[seq[i]].nodes;
        int size = substates.size();
        if (substates.empty()) return 0;
        //        printf("======================== %d ================================\n", seq[i]);

        for (size_t j = 0; j < size; j++) {
            node nxt_state = substates[j];
            std::string s_nxt_state = boost::lexical_cast<std::string>(nxt_state.index);
            double total = 0;
            std::string argmax = "";
            double valmax = 0;

            //            printf("---------------------------- %d --------------------------------\n", nxt_state.index);
            std::vector<node> r_nodes = trans_ap[nxt_state.index].nodes;
            if (r_nodes.empty()) return 0;
            for (size_t k = 0; k < r_nodes.size(); k++) {
                node state = r_nodes[k];
                StateInfoTimeStamp::ViterbiStateInfo v = trans_ap[state.index].csi->tRead(count);

                double prob = v.prob;
                std::string v_path = v.v_path;
                double v_prob = v.v_prob;
                double p = nxt_state.prob * state.prob;

                prob *= p;
                v_prob *= p;
                total += prob;

                //                printf(" (%d) \t %d -> %d \t Prob = %f * %f = %f \t", seq[i], state.index, nxt_state.index, nxt_state.prob, state.prob, p);
                //                printf("v_prob = %.*e \t valmax=  %.*e\n", 5, v_prob, 5, valmax);

                if (v_prob > valmax) {
                    argmax = v_path + "," + s_nxt_state;
                    valmax = v_prob;
                    //                    total = prob;
                }
            }
            bool flag = trans_ap[nxt_state.index].csi->Write(total, argmax, valmax, count);
            if (!flag) return 0;
        }
        count++;
    }

    double total = 0;
    double valmax = 0;
    std::string argmax;


    substates = emis_ap[seq[nseq - 1]].nodes;
    int size = substates.size();
    for (size_t i = 0; i < size; i++) {
        node lnode = substates[i];
        StateInfoTimeStamp::ViterbiStateInfo v = trans_ap[lnode.index].csi->Read(count);
        double prob = v.prob;
        std::string v_path = v.v_path;
        double v_prob = v.v_prob;
        //        std::cout << v_prob << " : " << valmax << " : " << v_path << std::endl;
        total += prob;
        if (v_prob > valmax) {
            argmax = v_path;
            valmax = v_prob;

        }
    }
    path = argmax;
    //    std::cout << "t_argmax" << t_argmax << std::endl;
    return total;
}

/**
 * Prefect ....
 * @param path
 * @param nseq
 * @param seq
 * @param init
 * @param trans_ap
 * @param emis_ap
 * @return 
 */
double AViterbi::forward_viterbi_2(std::string& path, int nseq, int* seq, double* init, mnode* trans_ap, mnode* emis_ap) {

//    int count = 0;
    std::vector<node> substates = emis_ap[seq[0]].nodes;
    // initialize states emitted by the first observation ...
    for (size_t i = 0; i < substates.size(); i++) {
        node snode = substates[i];
        int index = snode.index;
        double v_prob = init[index] * snode.prob;
        bool flag = trans_ap[index].csi->Write(0, boost::lexical_cast<std::string>(index), v_prob, 0);
        if (!flag) return 0;
    }
//    count++;
    for (size_t i = 1; i < nseq; i++) {
        substates = emis_ap[seq[i]].nodes;
        int size = substates.size();
        if (substates.empty()) return 0;
        //        printf("======================== %d ================================\n", seq[i]);

        for (size_t j = 0; j < size; j++) {
            node nxt_state = substates[j];
            std::string s_nxt_state = boost::lexical_cast<std::string>(nxt_state.index);
            double total = 0;
            std::string argmax = "";
            double valmax = 0;

            //            printf("---------------------------- %d --------------------------------\n", nxt_state.index);
            std::vector<node> r_nodes = trans_ap[nxt_state.index].nodes;
            if (r_nodes.empty()) return 0;
            for (size_t k = 0; k < r_nodes.size(); k++) {
                node state = r_nodes[k];
                StateInfoTimeStamp::ViterbiStateInfo v = trans_ap[state.index].csi->Read(i);

                double prob = v.prob;
                std::string v_path = v.v_path;
                double v_prob = v.v_prob;
                double p = nxt_state.prob * state.prob;

                prob *= p;
                v_prob *= p;
                total += prob;

                //                printf(" (%d) \t %d -> %d \t Prob = %f * %f = %f \t", seq[i], state.index, nxt_state.index, nxt_state.prob, state.prob, p);
                //                printf("v_prob = %.*e \t valmax=  %.*e\n", 5, v_prob, 5, valmax);

                if (v_prob > valmax) {
                    argmax = v_path + "," + s_nxt_state;
                    valmax = v_prob;
                    //                    total = prob;
                }
            }
            bool flag = trans_ap[nxt_state.index].csi->Write(total, argmax, valmax, i);
            if (!flag) return 0;
        }
//        count++;
    }

    double total = 0;
    double valmax = 0;
    std::string argmax;


    substates = emis_ap[seq[nseq - 1]].nodes;
    int size = substates.size();
    for (size_t i = 0; i < size; i++) {
        node lnode = substates[i];
        StateInfoTimeStamp::ViterbiStateInfo v = trans_ap[lnode.index].csi->Read(nseq);
        double prob = v.prob;
        std::string v_path = v.v_path;
        double v_prob = v.v_prob;
        //        std::cout << v_prob << " : " << valmax << " : " << v_path << std::endl;
        total += prob;
        if (v_prob > valmax) {
            argmax = v_path;
            valmax = v_prob;

        }
    }
    path = argmax;
    //    std::cout << "t_argmax" << t_argmax << std::endl;
    return total;
}

double AViterbi::forward_viterbi_par(std::string& path, int nseq, int* seq, double* init, mnode* trans_ap, mnode* emis_ap) {

    int count = 0;
    std::vector<node> substates = emis_ap[seq[0]].nodes;
    // initialize states emitted by the first observation ...
    bool flag = true;
#pragma omp parallel for
    for (size_t i = 0; i < substates.size(); i++) {
        node snode = substates[i];
        int index = snode.index;
        double v_prob = init[index] * snode.prob;
        trans_ap[index].csi->Write(0, boost::lexical_cast<std::string>(index), v_prob, count);
        //        if (!flag) return 0;
    }
    //    if (!flag) {
    //        printf("Error with writing order");
    //        path = "Error";
    //        return 0;
    //    }
    count++;
    //    bool flag = true;
    //#pragma omp parallel
    //    {
    for (size_t i = 1; i < nseq; i++) {
        substates = emis_ap[seq[i]].nodes;
        int size = substates.size();
        if (substates.empty()) return 0;
        printf("Observation %d\n", i);


#pragma omp parallel for shared(trans_ap) /*if(flag)*/ 
        for (size_t j = 0; j < size; j++) {
            node nxt_state = substates[j];
            std::string s_nxt_state = boost::lexical_cast<std::string>(nxt_state.index);
            double total = 0;
            std::string argmax = "";
            double valmax = 0;

            std::vector<node> r_nodes = trans_ap[nxt_state.index].nodes;
            //            printf("R_nodes %d\n", r_nodes.size());
            //            if (r_nodes.empty()) return 0;
            int r_nodes_size = r_nodes.size();
#pragma omp parallel for shared(nxt_state,s_nxt_state,total,argmax,valmax)
            for (size_t k = 0; k < r_nodes_size; k++) {
                node state = r_nodes[k];
                StateInfoTimeStamp::ViterbiStateInfo v = trans_ap[state.index].csi->Read(count);
                //                printf("Read count: %d\n", count);
                double prob = v.prob;
                std::string v_path = v.v_path;
                double v_prob = v.v_prob;
                double p = nxt_state.prob * state.prob;

                prob *= p;
                v_prob *= p;
                total += prob;
#pragma omp critical
                {
                    if (v_prob > valmax) {
                        argmax = v_path + "," + s_nxt_state;
                        valmax = v_prob;
                    }
                }
            }

            //            if (count <= trans_ap[nxt_state.index].csi->lasttime)
            flag = trans_ap[nxt_state.index].csi->Write(total, argmax, valmax, count);
            //            if (!flag) printf("Error writing state %d at count %d\n", nxt_state.index, count);
        }

        //        if (!flag) {
        //            printf("Error with writing order");
        //            path = "Error";
        //            return 0;
        //        }
#pragma omp atomic
        count++;
        //        printf("Increment count to %d\n", count);
    }

    double total = 0;
    double valmax = 0;
    std::string argmax;


    substates = emis_ap[seq[nseq - 1]].nodes;
    int size = substates.size();
#pragma omp parallel for 
    for (size_t i = 0; i < size; i++) {
        node lnode = substates[i];
        StateInfoTimeStamp::ViterbiStateInfo v = trans_ap[lnode.index].csi->Read(count);
        double prob = v.prob;
        std::string v_path = v.v_path;
        double v_prob = v.v_prob;
        total += prob;
#pragma omp critical
        {
            if (v_prob > valmax) {
                argmax = v_path;
                valmax = v_prob;

            }
        }
    }
    path = argmax;
    return total;
}

double AViterbi::forward_viterbi_par_1(std::string& path, int nseq, int* seq, double* init, mnode* trans_ap, mnode* emis_ap) {

    //    int count = 0;
    std::vector<node> substates = emis_ap[seq[0]].nodes;
#pragma omp parallel for
    for (size_t i = 0; i < substates.size(); i++) {
        node snode = substates[i];
        int index = snode.index;
        double v_prob = init[index] * snode.prob;
        trans_ap[index].csi->Write(0, boost::lexical_cast<std::string>(index), v_prob, 0);
    }

//    printf("Finish initialization \n");
    //    count++;
    //#pragma omp parallel
    //    {
    for (size_t i = 1; i < nseq; i++) {
        substates = emis_ap[seq[i]].nodes;
#if 0
        //for debug
        printf("---------------------\n");
        for (auto it = substates.begin(); it != substates.end(); it++) {
            node n = *it;
            std::cout << n.index << std::endl;
        }
#endif
        int size = substates.size();

        //        std::vector<vr> vr_l(size);
        if (substates.empty()) return 0;

#if 0
        get_reachables_s(substates, trans_ap, i);
#else
        //        printf("-----------Nodes @%d -----------------\n", i);
        //        for (size_t j = 0; j < size; j++) {
        //            printf("Node id:\t %d\n", substates[j].index);
        //        }

        vr* vr_l = new vr[size];
        get_reachables(vr_l, substates, trans_ap, i);
        //        serially update the viterbi paths.....
        //        printf("----------- %d -----------------\n", i);
                for (size_t j = 0; j < size; j++) {
                    vr tmp = vr_l[j];
                    //            printf(" index %d \t %s \n", tmp.index, tmp.argmax.c_str());
                    bool flag = trans_ap[tmp.index].csi->Write(tmp.total, tmp.argmax, tmp.valmax, i);
                    if (!flag) return 0;
                }
        //        printf("----------------------------\n");
#endif

    }

    double total = 0;
    double valmax = 0;
    std::string argmax;


    substates = emis_ap[seq[nseq - 1]].nodes;
    int size = substates.size();
#pragma omp parallel for
    for (size_t i = 0; i < size; i++) {
        node lnode = substates[i];
        StateInfoTimeStamp::ViterbiStateInfo v = trans_ap[lnode.index].csi->Read(nseq);
        double prob = v.prob;
        std::string v_path = v.v_path;
        double v_prob = v.v_prob;
        total += prob;
#pragma omp critical
        {
            if (v_prob > valmax) {
                argmax = v_path;
                valmax = v_prob;

            }
        }
    }
    path = argmax;
    return total;
}

void AViterbi::get_reachables(vr* vr_l, std::vector<node>& substates, mnode* trans_ap, int ts) {
    //    printf("The number of threads is: %d\n",omp_get_num_threads());
    int nthreads = omp_get_max_threads() / 3;
    int chunk = substates.size() / nthreads;
    //    printf("nthreads %d\t chunk: %d\n",nthreads, chunk);
#pragma omp parallel for 
    for (size_t j = 0; j < substates.size(); j++) {
        node nxt_state = substates[j];
        double em = nxt_state.prob;
        std::string s_cr_state = boost::lexical_cast<std::string>(nxt_state.index);
        std::vector<node> r_nodes = trans_ap[nxt_state.index].nodes;
        vr res;
        res.index = nxt_state.index;
        //#pragma omp ordered
        //        {
                vr_l[j].index = nxt_state.index;
        get_argmax(vr_l[j], ts, trans_ap, r_nodes, em, s_cr_state);
        //#pragma omp critical
        //        {
        //            printf("State: %d\n",nxt_state.index);
//        trans_ap[nxt_state.index].csi->Write(res.total, res.argmax, res.valmax, ts);
        //        }
    }
}

void AViterbi::get_reachables_s(std::vector<node>& substates, mnode* trans_ap, int ts) {
    for (size_t j = 0; j < substates.size(); j++) {
        node nxt_state = substates[j];
        double em = nxt_state.prob;
        std::string s_cr_state = boost::lexical_cast<std::string>(nxt_state.index);
        std::vector<node> r_nodes = trans_ap[nxt_state.index].nodes;
        vr res;
        res.index = nxt_state.index;
        get_argmax(res, ts, trans_ap, r_nodes, em, s_cr_state);
        trans_ap[res.index].csi->Write(res.total, res.argmax, res.valmax, ts);
    }
}

void AViterbi::get_argmax(vr& res, int ts, mnode* trans_ap, std::vector<node>& r_nodes, double em, std::string nxt_state) {
    double total = 0;
    std::string argmax = "";
    double valmax = 0;

    int r_nodes_size = r_nodes.size();
    //    vr* tmp_l = new vr[r_nodes_size];
    //    int chunk = r_nodes_size / 3;
    //    printf("argmax chunk %d\n",chunk);
#pragma omp parallel for shared(total, argmax, valmax)
    for (size_t k = 0; k < r_nodes_size; k++) {
        node state = r_nodes[k];
        StateInfoTimeStamp::ViterbiStateInfo v = trans_ap[state.index].csi->tRead(ts);
        double prob = v.prob;
        std::string v_path = v.v_path;
        double v_prob = v.v_prob;
        double p = em * state.prob;

        prob *= p;
        v_prob *= p;
#pragma omp critical
        {
            total += prob;
            if (v_prob > valmax) {
                argmax = v_path + "," + nxt_state;
                valmax = v_prob;
            }
        }
        //        prob *= p;
        //        v_prob *= p;
        //        
        //        tmp_l[k].total = prob;
        //        tmp_l[k].argmax = v_path + "," + cr_state;
        //        tmp_l[k].valmax = v_prob;

    }

    res.total = total;
    res.argmax = argmax;
    res.valmax = valmax;

    //    int max = 0;
    //    for (size_t i = 0; i < r_nodes_size; i++) {
    //        if (tmp_l[max].valmax < tmp_l[i].valmax) {
    //            max = i;
    //        }
    //    }
    //    copy_vr(tmp_l[max], res);
}

void AViterbi::copy_vr(vr& src, vr& dst) {

    dst.total = src.total;
    dst.argmax = src.argmax;
    dst.valmax = src.valmax;
}

double AViterbi::forward_viterbi_par_2(std::string& path, int nseq, int* seq, double* init, mnode* trans_ap, mnode* emis_ap) {

    //    int count = 0;
    std::vector<node> substates = emis_ap[seq[0]].nodes;
    // initialize states emitted by the first observation ...
#pragma omp parallel for
    for (size_t i = 0; i < substates.size(); i++) {
        node snode = substates[i];
        int index = snode.index;
        double v_prob = init[index] * snode.prob;
        trans_ap[index].csi->Write(0, boost::lexical_cast<std::string>(index), v_prob, 0);
        //        if (!flag) return 0;
    }
    //    count++;
    for (size_t i = 1; i < nseq; i++) {
        substates = emis_ap[seq[i]].nodes;
        int size = substates.size();
        if (substates.empty()) return 0;
        //        printf("======================== %d ================================\n", seq[i]);

#pragma omp parallel for 
        for (size_t j = 0; j < size; j++) {
            node nxt_state = substates[j];
            std::string s_nxt_state = boost::lexical_cast<std::string>(nxt_state.index);

            double total = 0;
            std::string argmax = "";
            double valmax = 0;

            //            printf("---------------------------- %d --------------------------------\n", nxt_state.index);
            std::vector<node> r_nodes = trans_ap[nxt_state.index].nodes;
            //            if (r_nodes.empty()) return 0;
            //#pragma omp parallel for shared(total, valmax, argmax) 
            for (size_t k = 0; k < r_nodes.size(); k++) {
                node state = r_nodes[k];
                StateInfoTimeStamp::ViterbiStateInfo v = trans_ap[state.index].csi->Read(i);

                double prob = v.prob;
                std::string v_path = v.v_path;
                double v_prob = v.v_prob;
                double p = nxt_state.prob * state.prob;

                prob *= p;
                v_prob *= p;
                //#pragma omp critical
                //                {
                total += prob;
                if (v_prob > valmax) {
                    argmax = v_path + "," + s_nxt_state;
                    valmax = v_prob;
                    //                    total = prob;
                    //                    }
                }
            }
            //#pragma omp ordered
            trans_ap[nxt_state.index].csi->Write(total, argmax, valmax, i);

        }
        //        count++;
    }

    double total = 0;
    double valmax = 0;
    std::string argmax;


    substates = emis_ap[seq[nseq - 1]].nodes;
    int size = substates.size();
    //#pragma omp parallel for shared(total, valmax, argmax) 
    for (size_t i = 0; i < size; i++) {
        node lnode = substates[i];
        StateInfoTimeStamp::ViterbiStateInfo v = trans_ap[lnode.index].csi->Read(nseq);
        double prob = v.prob;
        std::string v_path = v.v_path;
        double v_prob = v.v_prob;
        //        std::cout << v_prob << " : " << valmax << " : " << v_path << std::endl;
        //#pragma omp critical
        //        {
        total += prob;
        if (v_prob > valmax) {
            argmax = v_path;
            valmax = v_prob;

        }
        //        }
    }
    path = argmax;
    return total;
}

void AViterbi::print(double* init) {
    std::cout << (sizeof (init) / sizeof (*init)) << std::endl;
}
