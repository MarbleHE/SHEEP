#pragma once

#include <map>
#include "circuit.hpp"
#include "op.hpp"

//this is just so that we don't have magic strings/numbers
enum class binoptype {
  Add,
  Subtract,
  Multiply,
  Divide
};

//This map is used for matching strings with operations
static const std::map<std::string, binoptype> binop_map = {
    {"-", binoptype::Subtract},
    {"+", binoptype::Add},
    {"*", binoptype::Multiply},
    {"/", binoptype::Divide}};

class BinOp: public virtual Op
{
private:
    /* data */
public:
    BinOp(binoptype binopt);
    binoptype btp;
    virtual void handleOp(Circuit *c);
};
