#ifndef UNOP_HPP
#define UNOP_HPP

#include <string>
#include <map>
#include "op.hpp"

enum class unoptype {
  Negate,
  Square,
  Invert,
  SquareRoot
};

static const std::map<std::string, unoptype> unop_map = {
    {"--", unoptype::Negate},
    {"^2", unoptype::Square},
    {"^-1", unoptype::Invert},
    {"^1/2", unoptype::SquareRoot}};

class UnOp: Op
{
private:
    /* data */
public:
};





#endif