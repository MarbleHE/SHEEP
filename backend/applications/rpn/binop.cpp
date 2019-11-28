#include <vector>
#include "binop.hpp"
#include "circuit.hpp"

using namespace std;

BinOp::BinOp (binoptype binopt){
    btp = binopt;
}


void BinOp::handleOp(Circuit *c, vector<int> *ptvec){}