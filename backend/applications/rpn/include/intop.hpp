#pragma once

#include <vector>
#include <stack>
#include "circuit.hpp"
#include "op.hpp"

/// An integer operation IntOp is just an integer. This means an alias gate as placeholder and the integer is stored in ptvec to later feed it into the circuit.
class IntOp : public Op {
public:
    /// Constructor
    /// \param in The integer. An Integer operation is just an integer.
    explicit IntOp(int in);

    /// int representation of the integer operation
    int i;

    /// This implements the core logic of what to do when a integer operation in a calculation has to be processed by manipulating s and ptvec.
    /// \param ptvec Vector, in which the plaintext inputs of the calculation get stored (in order of appearance).
    /// \param s Stack of circuits, which represent the intermediately computed subcomponents of the final circuit.
    void handleOp(std::vector<int> &ptvec, std::stack<Circuit> &s) override;
};


