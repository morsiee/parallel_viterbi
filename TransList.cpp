/* 
 * File:   TransNode.cpp
 * Author: essam
 * 
 * Created on June 18, 2015, 1:54 PM
 */

#include <iostream>
#include <string>
#include "TransList.h"

TransList::~TransList() {
    Node *current, *temp;
    current = head;
    temp = head;
    while (current != NULL) {
        current = current->next;
        delete temp;
        temp = current;
    }
}

struct TransList::Node* TransList::initNode(std::string state, double prob) {
    struct Node *ptr = new Node;

    // error? then just return
    if (ptr == NULL)
        return static_cast<struct Node *> (NULL);
        // assign it 
        // then return pointer to ne node
    else {
        ptr->state = state;
        ptr->prob = prob;
        return ptr;
    }
}

// adding to the end of list  

void TransList::addNode(struct Node *newNode) {
    // if there is no node, put it to head
    if (head == NULL) {
        head = newNode;
        tail = newNode;
    }

    // link in the new_node to the tail of the list
    // then mark the next field as the end of the list
    // adjust tail to point to the last node

    tail->next = newNode;
    newNode->next = NULL;
    tail = newNode;
}

void TransList::insertNode(struct Node *newnode) {
    struct Node *temp, *prev;

    if (head == NULL) { // if an empty list,         
        head = newnode; // set 'head' to it         
        tail = newnode;
        head->next = NULL; // set end of list to NULL     
        return;
    }

    temp = head; // start at beginning of list 
    // while currentname < newname 
    while (temp->state < newnode->state) { // to be inserted then 
        temp = temp->next; // goto the next node in list  
        if (temp == NULL) // don't go past end of list    
            break;
    }
    // set previous node before we insert  
    // first check to see if it's inserting         
    if (temp == head) { // before the first node 
        newnode->next = head; // link next field to original list    
        head = newnode; // head adjusted to new node          
    } else { // it's not the first node
        prev = head; // start of the list, 
        while (prev->next != temp) {
            prev = prev->next; // will cycle to node before temp 
        }
        prev->next = newnode; // insert node between prev and next   
        newnode->next = temp;
        if (tail == prev) // if the new node is inserted at the  
            tail = newnode; // end of the list the adjust 'end'    
    }
}

struct TransList::Node* TransList::searchState(struct Node* ptr, std::string name) {
    while (name != ptr->state) {
        ptr = ptr->next;
        if (ptr == NULL)
            break;
    }
    return ptr;
}

void TransList::reverse() {
    // we need at least two nodes for the reverse to have any effect
    if (head == NULL || head->next == NULL) return;

    // Starting 2nd list as 'me' and 'head' is now 'me->next'
    // and 'head->next' is pointing to NULL
    // So, the 3rd list is now 'child' of 'me'
    Node *parent = head;
    Node *me = head->next;
    Node *child = me->next;

    // convert head to tail
    head->next = NULL;

    // reverse pointer direction
    me->next = head;

    while (child != NULL) {
        me->next = parent;
        parent = me;
        me = child;
        child = child->next;
    }
    // when me reached the tail
    // me becomes head
    head = me;
    // the head is now pointing to the 2nd last node
    head->next = parent;
}

void TransList::deleteNode(struct TransList::Node *ptr) {
    struct Node *temp, *prev;
    temp = ptr; // node to be deleted 
    prev = head; // start of the list, will cycle to node before temp    

    if (temp == prev) { // deleting first node?  
        head = head->next; // moves head to next node     
        if (tail == temp) // is it end, only one node?   
            tail = tail->next; // adjust end as well          
        delete temp; // free up space 
    } else { // if not the first node, then 
        while (prev->next != temp) { // move prev to the node before
            prev = prev->next; // the one to be deleted       
        }
        prev->next = temp->next; // link previous node to next  
        if (tail == temp) // if this was the end node,   
            tail = prev; // then reset the end pointer  
        delete temp; // free up space
    }
}

void TransList::deleteList(struct Node *ptr) {
    struct Node *temp;

    if (head == NULL) return; // don't try to delete an empty list      

    if (ptr == head) { // if we are deleting the entire list    
        head = NULL; // then reset head and    
        tail = NULL; // end to empty                                     
    } else {
        temp = head; // if it's not the entire list, readjust end   
        while (temp->next != ptr) // locate previous node to ptr   
            temp = temp->next;
        tail = temp; // set end to node before ptr   
    }

    while (ptr != NULL) { // whilst there are still nodes to delete      
        temp = ptr->next; // record address of next node                 
        delete ptr; // free this node                             
        ptr = temp; // point to next node to be deleted           
    }
}

void TransList::displayNode(struct TransList::Node *ptr) const {
    std::cout << ptr->state << ": " << ptr->prob << std::endl;
}

void TransList::displayList(struct TransList::Node *ptr) const {
//    TransList::Node *new_ptr = ptr;
    if (!ptr) std::cout << "Nothing to display" << std::endl;
    while (ptr) {
        displayNode(ptr);
        ptr = ptr->next;
    }
}
