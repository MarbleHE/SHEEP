#include <vector>
#include "unop.hpp"
#include "circuit.hpp"

using namespace std;

UnOp::UnOp(unoptype unopt){
    utp = unopt;
}

void UnOp::handleOp(Circuit *c, vector<int> *ptvec){}