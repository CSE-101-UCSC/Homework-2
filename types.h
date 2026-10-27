// PROVIDED FILE: DO NOT EDIT. Shared flight and tree data structures.
#ifndef TYPES_H
#define TYPES_H

#include <string>

struct Flight {
    int departure; // HHMM integer, e.g. 1445; valid clock time; unique AVL key
    std::string id;
    std::string destination;
};

struct Node {
    Flight data; // data.departure is the unique BST key; no duplicate key field.
    Node* left = nullptr;
    Node* right = nullptr;
    int height = 1;
};

struct FlightTree {
    Node* root = nullptr;
    int size = 0;
};

#endif
