#pragma once

#include <vector>
#include "circuit.hpp"

//abstract Base class for an operation. Cannot be instantiated, but the subclass objects can be accessed via pointer.
class Op
{
private:
public:
    virtual void handleOp(Circuit *c, std::vector<int> *ptvec) = 0;
    //virtual ~Op(){};
};

//void handleOp(){};
