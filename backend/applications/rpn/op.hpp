#pragma once

#include <vector>
#include <stack>
#include "circuit.hpp"

//abstract Base class for an operation. Cannot be instantiated, but the subclass objects can be accessed via pointer.
class Op
{
private:
public:
    //an IntOp updates the ptvec, BinOp and UnOp update the circuit. All three update stack
    virtual void handleOp(std::vector<int> *ptvec, std::stack<Circuit> *s) = 0;
    //virtual ~Op(){};
};
