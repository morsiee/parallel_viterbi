/* 
 * File:   TransNode.h
 * Author: essam
 *
 * Created on June 18, 2015, 1:54 PM
 */

#ifndef TRANSLIST_H
#define	TRANSLIST_H
#include<string>

class TransList {
public:
    struct Node {
        double prob;
        std::string state;
        struct Node *next;
    } *head, *tail, *ptr;

    TransList() : head(NULL), tail(NULL) {
    } // constructor	
    ~TransList(); // destructor

    struct TransList::Node* searchState(struct TransList::Node*, std::string);
    struct TransList::Node* initNode(std::string, double);

    void reverse();
    void addNode(struct TransList::Node*);
    void insertNode(struct TransList::Node*);
    void deleteNode(struct TransList::Node*);
    void deleteList(struct TransList::Node*);
    void displayList(struct TransList::Node*)const;
    void displayNode(struct TransList::Node*) const;
};
#endif	/* TRANSNODE_H */

