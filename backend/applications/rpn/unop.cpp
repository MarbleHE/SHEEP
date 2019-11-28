#include <vector>
#include <stack>
#include "unop.hpp"
#include "circuit.hpp"
#include "simple-circuits.hpp"
#include "circuit-util.hpp"

using namespace std;

UnOp::UnOp(unoptype unopt){
    utp = unopt;
}

void UnOp::handleOp(vector<int> *ptvec, stack<Circuit> *s){
    if (s->size() < 1)
    {
        throw runtime_error("Stack has not enough input for unary gate.");
    }
    
    Circuit uc;
    switch (utp)
    {
    case unoptype::Negate:
    {
        uc = single_unary_gate_circuit(Gate::Negate);
        Circuit l;
        l = s->top();
        s->pop();
        Circuit sc = seq(uc, l);
        s->push(sc);
        break;
    }
    case unoptype::Square:
    {
        uc = single_binary_gate_circuit(Gate::Multiply);
        Circuit l;
        l = s->top();
        s->pop();
        Circuit sc = seq(uc, par(l, l));
        s->push(sc);
        break;
    }
    /*case unoptype::SquareRoot:
        throw GateNotImplementedError
        break;
    case unoptype::Invert:
        throw GateNotImplementedError
        */
    default:
        break;
    }
}