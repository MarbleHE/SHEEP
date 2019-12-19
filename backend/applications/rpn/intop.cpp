#include <vector>
#include <stack>
#include "include/intop.hpp"
#include "circuit.hpp"
#include "simple-circuits.hpp"

using namespace std;

IntOp::IntOp(int in) {
    i = in;
}

void IntOp::handleOp(vector<int> &ptvec, stack<Circuit> &s) {
    ptvec.push_back(i); //push onto vector to later feed into Circuit
    s.push(single_unary_gate_circuit(
            Gate::Alias)); //Push alias gate on stack as placeholder input to get consumed by other circuits with gates that compute something
}