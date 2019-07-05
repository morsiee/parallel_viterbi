/* 
 * File:   main.cpp
 * Author: essam
 *
 * Created on June 28, 2015, 8:55 PM
 */

#include <cstdlib>
#include "TransList.h"
#include<iostream>
#include<string>
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    int id;
    string name;
    TransList myList;
    TransList::Node* ptr;

    // add
    ptr = myList.initNode("s1", 1);
    myList.addNode(ptr);
    ptr = myList.initNode("s2", 2);
    myList.addNode(ptr);
    ptr = myList.initNode("s3", 3);
    myList.addNode(ptr);
    ptr = myList.initNode("s4", 4);
    myList.addNode(ptr);
    ptr = myList.initNode("s5", 5);
    myList.addNode(ptr);
    
    myList.displayList(myList.head);

    // delete
    name = "s2";
    ptr = myList.searchState(myList.head, name);
    if (ptr == NULL) {
        cout << "\nName: " << name << " not found" << endl;
    } else {
        cout << "\nDeleting a node ...  ";
        myList.displayNode(ptr);
        myList.deleteNode(ptr);
    }
    myList.displayList(myList.head);

    // insert
    name = "s2";
    id = 2;
    ptr = myList.initNode(name, id);
    myList.insertNode(ptr);
    cout << "\nInserting a node ...  ";
    myList.displayNode(ptr);
    myList.displayList(myList.head);

    // reverse
    cout << "\nReversing the list ...  \n";
    myList.reverse();
    myList.displayList(myList.head);

    // delete
    cout << "\nIn the end, deleting the list ...  \n";
    myList.deleteList(myList.head);
    myList.displayList(myList.head);
    return 0;
}

