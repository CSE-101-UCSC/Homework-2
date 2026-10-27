// Implement only this file. See avl.h and the assignment for contracts.
#include "avl.h"

int nodeHeight(const Node* node) {
    (void)node;
    return 0;
} // TODO

void updateHeight(Node* node) {
    (void)node;
} // TODO

int balanceFactor(const Node* node) {
    (void)node;
    return 0;
} // TODO

Node* rotateLeft(Node* root) {
    return root;
} // TODO

Node* rotateRight(Node* root) {
    return root;
} // TODO

Node* rebalance(Node* root) {
    return root;
} // TODO

Node* insertNode(Node* root, Node* newNode) {
    (void)newNode;
    return root; // TODO
}

Node* deleteNode(Node* root, int departure) {
    (void)departure;
    return root; // TODO
}

const Node* findNode(const Node* root, int departure) {
    (void)root;
    (void)departure;
    return nullptr; // TODO
}

const Node* earliestNode(const Node* root, int departure) {
    (void)root;
    (void)departure;
    return nullptr; // TODO
}

void findRange(const Node* root, int start, int end,
               const Node* results[], int& count) {
    (void)root;
    (void)start;
    (void)end;
    (void)results;
    (void)count; // TODO
}
