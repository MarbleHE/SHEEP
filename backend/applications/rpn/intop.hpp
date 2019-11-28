#pragma once

#include "circuit.hpp"
#include "op.hpp"

class IntOp: public virtual Op
{
private:
    /* data */
public:
    IntOp(int in);
    int i;
    virtual void handleOp(Circuit *c);
};


