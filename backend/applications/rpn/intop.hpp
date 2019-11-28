#pragma once

#include <vector>
#include "circuit.hpp"
#include "op.hpp"

using namespace std;

class IntOp: public Op
{
private:
    /* data */
public:
    IntOp(int in);
    int i;
    void handleOp(Circuit *c, vector<int> *ptvec);
};


