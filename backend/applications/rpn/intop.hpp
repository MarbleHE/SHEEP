#pragma once

#include <vector>
#include <stack>
#include "circuit.hpp"
#include "op.hpp"

using namespace std;

class IntOp: public Op
{
public:
    // Constructor
    explicit IntOp(int in);
    // int representation of the integer operation
    int i;
    // This implements the core logic of what to do when a integer operation in a calculation has to processed.
    // An intop simply puts the ints into ptvec, such that we can feed it into a circuit later on.
    void handleOp(vector<int> *ptvec, stack<Circuit> *s) override;
};


