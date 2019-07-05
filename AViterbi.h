/* 
 * File:   AViterbi.h
 * Author: essam
 *
 * Created on September 8, 2015, 1:37 PM
 */

#ifndef AVITERBI_H
#define	AVITERBI_H
#include "StateInfoTimeStamp.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

class AViterbi {
public:
    AViterbi();
    AViterbi(const AViterbi& orig);
    virtual ~AViterbi();

    typedef struct {
        int index;
        double prob;

    } node;

    typedef struct tnode {
        double prob;
        double v_prob;
        std::vector<int> v_path;
        //        std::string tv_path;
        bool flag;
        int count;

        tnode() : flag(false), prob(0.0), v_prob(0.0), count(-1) {
        }
    };

    //    template <size_t N>

    typedef struct {
        std::vector<node> nodes;
        StateInfoTimeStamp *csi;
        //        int index;
        //        node* nodes;
        //        int size;
    } mnode;

    // for debug, viterbi record.

    typedef struct vr {
        int index;
        double total;
        std::string argmax;
        double valmax;

        vr() : index(-1), total(0), argmax(""), valmax(0) {
        }
    };

    template <typename T, unsigned S>
    inline unsigned arraysize(const T(&v)[S]) {
        return S;
    }

    void conv_seq(std::vector<std::string>& oseq, int* seq);

    void conv_init(std::unordered_map < std::string, double>& initProbs, std::vector<std::string>& states, double* init);
    void conv_trans_arr(
            std::unordered_map<std::string, std::unordered_map < std::string, double>>&trans_p,
            std::vector<std::string>& states, mnode* trans_ap);
    void conv_trans_arr_1(
            std::unordered_map<std::string, std::unordered_map < std::string, double>>&trans_p,
            std::vector<std::string>& states, mnode* trans_ap);
    void conv_emis_arr(
            std::unordered_map<std::string, std::unordered_map < std::string, double>>&emis_p,
            std::vector<std::string>& states, mnode* emis_ap);

    void print(double* init);
    void forward_viterbi(std::vector<int>& path, int nstates, int seq_leng, int* seq, double* init, mnode* trans_ap, mnode* emis_ap);

    double forward_viterbi_1(std::string& path, int seq_leng, int* seq, double* init, mnode* trans_ap, mnode* emis_ap);
    double forward_viterbi_2(std::string& path, int seq_leng, int* seq, double* init, mnode* trans_ap, mnode* emis_ap);
    double forward_viterbi_par(std::string& path, int nseq, int* seq, double* init, mnode* trans_ap, mnode* emis_ap);
    double forward_viterbi_par_1(std::string& path, int nseq, int* seq, double* init, mnode* trans_ap, mnode* emis_ap);
    double forward_viterbi_par_2(std::string& path, int nseq, int* seq, double* init, mnode* trans_ap, mnode* emis_ap);
    void get_argmax(vr& tvr, int count, mnode* trans_ap, std::vector<node>& r_nodes, double em, std::string cr_state);
    void get_reachables(vr* vr_l, std::vector<node>& substates, mnode* trans_ap, int ts);
    void get_reachables_s(std::vector<node>& substates, mnode* trans_ap, int ts);
    void copy_vr(vr& src, vr& dst);
private:


};

#endif	/* AVITERBI_H */

