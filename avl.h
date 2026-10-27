// PROVIDED FILE: DO NOT EDIT. Implement these eleven functions in avl.cpp.
#ifndef AVL_H
#define AVL_H

#include "types.h"

// AVL helpers: use these when implementing insertion and deletion.

// Return the cached height, or 0 for nullptr; do not recompute recursively.
int nodeHeight(const Node* node);

// Set height to 1 + max(child heights). Node is non-null; child heights are correct.
void updateHeight(Node* node);

// Return left height minus right height, or 0 for nullptr.
int balanceFactor(const Node* node);

// Rotate left, update both heights, and return the new root. Right child must exist.
Node* rotateLeft(Node* root);

// Rotate right, update both heights, and return the new root. Left child must exist.
Node* rotateRight(Node* root);

// Update height, restore AVL balance if needed, and return the updated subtree root.
Node* rebalance(Node* root);

// Updates and queries: called by the provided flights.cpp.

// Insert newNode, restore AVL balance, and return the updated subtree root.
Node* insertNode(Node* root, Node* newNode);

// Delete the node at departure, restore AVL balance, and return the updated subtree root.
Node* deleteNode(Node* root, int departure);

// Return the node at exactly this departure, or nullptr if absent.
const Node* findNode(const Node* root, int departure);

// Return the earliest node at or after this departure, or nullptr if none exists.
const Node* earliestNode(const Node* root, int departure);

// Collect node pointers with departure in [start, end], in time order, and update count.
// If start > end, add nothing; if start == end, collect only an exact match.
void findRange(const Node* root, int start, int end,
               const Node* results[], int& count);

#endif
