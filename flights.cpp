// PROVIDED FILE: DO NOT EDIT.
#include "flights.h"
#include "avl.h"
#include <iostream>
#include <cstdlib>

// Diagnostic context supplied by the fixed input reader, not student tree state.
static int inputLine = 0;
static std::string inputCommand;
void setInputContext(int lineNumber, const std::string& command) {
    inputLine = lineNumber;
    inputCommand = command;
}
// Independent balance-only check: calculate actual heights, not cached heights.
// Its O(n) validation overhead is excluded from student algorithm costs.
static int inspectBalance(const Node* node) {
    if (!node)
        return 0;

    int left = inspectBalance(node->left);
    int right = inspectBalance(node->right);

    if (left - right < -1 || left - right > 1) {
        std::cout << "AVL balance check failed after input line " << inputLine << ".\n"
                  << inputCommand << '\n';
        std::exit(2);
    }

    return 1 + (left > right ? left : right);
}

static void checkTree(const FlightTree& tree) {
    inspectBalance(tree.root);
}

void addFlight(FlightTree& t, const Flight& f) {
    Node* newNode = new Node{f};
    t.root = insertNode(t.root, newNode);
    ++t.size;

    checkTree(t);
}

void cancelFlight(FlightTree& t, int departure) {
    t.root = deleteNode(t.root, departure);
    --t.size;

    checkTree(t);
}

void delayFlight(FlightTree& t, int oldDeparture, int newDeparture) {
    Flight moved = *findFlight(t, oldDeparture);
    cancelFlight(t, oldDeparture);
    moved.departure = newDeparture;
    addFlight(t, moved);
}

const Flight* findFlight(const FlightTree& tree, int departure) {
    const Node* node = findNode(tree.root, departure);

    return node ? &node->data : nullptr;
}

const Flight* earliestFlight(const FlightTree& tree, int departure) {
    const Node* node = earliestNode(tree.root, departure);

    return node ? &node->data : nullptr;
}

void printFlight(const Flight* flight) {
    if (!flight) {
        std::cout << "NONE\n";
        return;
    }
    int h = flight->departure / 100, m = flight->departure % 100;
    std::cout << flight->id << ' ' << (h < 10 ? "0" : "") << h
              << (m < 10 ? "0" : "") << m << ' ' << flight->destination << '\n';
}

static void printSubtree(const Node* node) {
    if (!node)
        return;
    printSubtree(node->left);
    printFlight(&node->data);
    printSubtree(node->right);
}

void printSchedule(const FlightTree& tree) {
    printSubtree(tree.root);
}

void printRange(const FlightTree& tree, int start, int end) {
    const Node* results[1440];
    int count = 0;
    findRange(tree.root, start, end, results, count);

    for (int i = 0; i < count; ++i) {
        printFlight(&results[i]->data);
    }
}

static void destroy(Node* n) {
    if (!n)
        return;
    destroy(n->left);
    destroy(n->right);
    delete n;
}

void clearTree(FlightTree& t) {
    destroy(t.root);
    t.root = nullptr;
    t.size = 0;
}
