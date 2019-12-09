#pragma once

#include <string>
#include <map>
#include <vector>
#include <stack>
#include "op.hpp"

//The supported unary gates. This makes the switch cases more readable.
enum class unoptype {
  Negate,
  Square,
  Invert,
  SquareRoot
};

//This map is used for matching the Token string with its unary operation type
static const std::map<std::string, unoptype> unop_map = {
    {"--", unoptype::Negate},
    {"^2", unoptype::Square},
    {"^-1", unoptype::Invert},
    {"^1/2", unoptype::SquareRoot}};

// An UnOp consists of its type, which determines its computation.
class UnOp: public Op
{
public:
    // constructor
    explicit UnOp(unoptype unopt);
    // type of operation
    unoptype utp;
    // This implements the core logic of what to do when a unary operation in a calculation has to processed.
    // A Circuit a from the stack gets consumed, such that when handleOp returns the Circuit on top is that
    // Circuit made sequential with the new operation b: a->b
    void handleOp(std::vector<int> &ptvec, std::stack<Circuit> &s) override;
};

