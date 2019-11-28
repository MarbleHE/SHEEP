#include <vector>
#include <stack>
#include "binop.hpp"
#include "circuit.hpp"
#include "simple-circuits.hpp"

using namespace std;

BinOp::BinOp (binoptype binopt){
    btp = binopt;
}


void BinOp::handleOp(vector<int> *ptvec, stack<Circuit> *s){
    Circuit bc;
    switch (btp)
    {
    case binoptype::Add:
        bc = single_binary_gate_circuit(Gate::Add);

        break;
    
    default:
        break;
    }
}