#include <vector>
#include <stack>
#include "unop.hpp"
#include "circuit.hpp"

using namespace std;

UnOp::UnOp(unoptype unopt){
    utp = unopt;
}

void UnOp::handleOp(vector<int> *ptvec, stack<Circuit> *s){}