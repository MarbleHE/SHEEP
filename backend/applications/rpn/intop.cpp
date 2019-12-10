#include <vector>
#include <stack>
#include "intop.hpp"
#include "circuit.hpp"
#include "simple-circuits.hpp"

using namespace std;

/// Constructor
/// \param in The integer. An Integer operation is just an integer.
IntOp::IntOp (int in){
    i = in;
}

/// This implements the core logic of what to do when a integer operation in a calculation has to be processed.
/// An intop simply puts the ints into ptvec, such that we can feed it into a circuit later on and puts an Alias on the Circuit stack as a placeholder for the input pushed into ptvec.
/// \param ptvec Vector, in which the plaintext inputs of the calculation get stored (in order of appearance).
/// \param s Stack of circuits, which represent the intermediately computed subcomponents of the final circuit.
void IntOp::handleOp(vector<int> &ptvec, stack<Circuit> &s){
    ptvec.push_back(i); //push onto vector to later feed into Circuit
    s.push(single_unary_gate_circuit(Gate::Alias)); //Push alias gate on stack as placeholder input to get consumed by other circuits with gates that compute something
}