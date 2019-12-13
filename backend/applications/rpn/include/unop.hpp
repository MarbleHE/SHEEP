#pragma once

#include <string>
#include <map>
#include <vector>
#include <stack>
#include "op.hpp"

/// The supported unary gates. This makes the switch cases more readable.
enum class unoptype {
    Negate,
    Square,
    Invert,
    SquareRoot
};

/// This map is used for matching the Token string with its unary operation type.
static const std::map<std::string, unoptype> unop_map = {
        {"--",   unoptype::Negate},
        {"^2",   unoptype::Square},
        {"^-1",  unoptype::Invert},
        {"^1/2", unoptype::SquareRoot}};

/// An UnOp consists of its type, which determines its computation (its gate)
class UnOp : public Op {
public:
    /// Constructor
    /// \param unopt type of the operation (the gate)
    explicit UnOp(unoptype unopt);

    /// type of operation (what kind of gate)
    unoptype utp;

    /// This implements the core logic of what to do when a unary operation in a calculation has to processed.
    /// A circuit x from the stack gets consumed, such that when handleOp returns the circuit on top is that
    /// circuit made sequential with the new operation y (the unary gate): ->x->y->
    /// \param ptvec Vector, in which the plaintext inputs of the calculation get stored (in order of appearance).
    /// \param s Stack of circuits, which represent the intermediately computed subcomponents of the final circuit.
    void handleOp(std::vector<int> &ptvec, std::stack<Circuit> &s) override;
};

