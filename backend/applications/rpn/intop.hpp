#pragma once

#include <vector>
#include <stack>
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
    void handleOp(vector<int> *ptvec, stack<Circuit> *s);
};


