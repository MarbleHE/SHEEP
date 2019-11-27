#pragma once

#include "circuit.hpp"
#include "op.hpp"

class IntOp: public Op
{
private:
    /* data */
public:
    IntOp(int in);
    int i;
    void handleOp(Circuit *c);
};


