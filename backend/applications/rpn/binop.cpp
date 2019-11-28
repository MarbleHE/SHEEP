#include <vector>
#include <stack>
#include "binop.hpp"
#include "circuit.hpp"
#include "simple-circuits.hpp"
#include "circuit-util.hpp"

using namespace std;

BinOp::BinOp (binoptype binopt){
    btp = binopt;
}


void BinOp::handleOp(vector<int> *ptvec, stack<Circuit> *s){
    if (s->size() < 2)
    {
        throw runtime_error("Stack has not enough input for binary gate.");
    }
    
    Circuit bc;
    switch (btp)
    {
    case binoptype::Add:
        bc = single_binary_gate_circuit(Gate::Add);
        break;
    case binoptype::Subtract:
        bc = single_binary_gate_circuit(Gate::Subtract);
        break;
    case binoptype::Multiply:
        bc = single_binary_gate_circuit(Gate::Multiply);
        break;
    /*case binoptype::Divide:
        throw GateNotImplementedError*/
    default:
        break;
    }
    Circuit l;
    Circuit r;
    l = s->top();
    s->pop();
    r = s->top();
    s->pop();
    Circuit combc = seq(bc, par(l, r));
    s->push(combc);
}