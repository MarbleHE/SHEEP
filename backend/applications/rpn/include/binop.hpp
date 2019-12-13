#pragma once

#include <map>
#include <vector>
#include <stack>
#include "circuit.hpp"
#include "op.hpp"


/// The supported binary gates. This makes the switch cases more readable.
enum class binoptype {
  Add,
  Subtract,
  Multiply,
  Divide
};

/// This map is used for matching the Token string with its binary operation type
static const std::map<std::string, binoptype> binop_map = {
    {"-", binoptype::Subtract},
    {"+", binoptype::Add},
    {"*", binoptype::Multiply},
    {"/", binoptype::Divide}};

/// A BinOp consists of its type, which determines the computation (the gate)
class BinOp: public Op
{
public:
    /// Constructor
    /// \param binopt The type of operation (the gate). Gets copied into btp.
    explicit BinOp(binoptype binopt);

    /// the type of the binary operation (the gate)
    binoptype btp;

    /// This implements the core logic of what to do when a binary operation in a calculation has to processed.
    /// It consumes 2 Circuits from the stack and pushes a new one on the stack by feeding
    /// them into a new circuit (containing the binary gate), which implements the computation determined by the operation.
    /// \param ptvec Vector, in which the plaintext inputs of the calculation get stored (in order of appearance).
    /// \param s Stack of circuits, which represent the intermediately computed subcomponents of the final circuit.
    void handleOp(std::vector<int> &ptvec, std::stack<Circuit> &s) override;
};
