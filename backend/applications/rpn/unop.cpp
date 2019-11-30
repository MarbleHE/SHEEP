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
    if (s->empty())
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
        Circuit sc = seq(l,uc);
        s->push(sc);
        break;
    }
    case unoptype::Square:
    {
        Circuit spreader;
        Wire in1 = spreader.add_input("in1");
        Wire out1 = spreader.add_assignment("out1", Gate::Alias, in1);
        Wire out2 = spreader.add_assignment("ou2", Gate::Alias, in1);
        spreader.set_output(out1);
        spreader.set_output(out2);
        uc = single_binary_gate_circuit(Gate::Multiply);
        Circuit in;
        in = s->top();
        s->pop();
        Circuit sc = seq(seq(in, spreader),uc);
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