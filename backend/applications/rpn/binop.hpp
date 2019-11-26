#ifndef BINOP_HPP
#define BINOP_HPP

#include <map>
#include "op.hpp"

enum class binoptype {
  Add,
  Subtract,
  Multiply,
  Divide
};

static const std::map<std::string, binoptype> binop_map = {
    {"-", binoptype::Subtract},
    {"+", binoptype::Add},
    {"*", binoptype::Multiply},
    {"/", binoptype::Divide}};
    
class BinOp: Op
{
private:
    /* data */
public:
};



#endif