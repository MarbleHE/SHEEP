#pragma once

#include <map>
#include <vector>
#include <stack>
#include "circuit.hpp"
#include "op.hpp"

using namespace std;

//The supported binary gates. This makes the switch cases more readable.
enum class binoptype {
  Add,
  Subtract,
  Multiply,
  Divide
};

//This map is used for matching the Token string with its binary operation type
static const std::map<std::string, binoptype> binop_map = {
    {"-", binoptype::Subtract},
    {"+", binoptype::Add},
    {"*", binoptype::Multiply},
    {"/", binoptype::Divide}};

// A BinOp consists of its type, which determines the computation.
class BinOp: public Op
{
public:
    // Constructor
    explicit BinOp(binoptype binopt);
    // the type of the binary operation
    binoptype btp;
    // This implements the core logic of what to do when a binary operation in a calculation has to processed.
    // It consumes 2 Circuits from the stack and pushes a new one on the stack by feeding
    // them into a new circuit, which implements the computation determined by the operation.
    void handleOp(vector<int> *ptvec, stack<Circuit> *s) override;
};
