#pragma once

#include <string>
#include <map>
#include "op.hpp"

//this is just so that we don't have magic strings/numbers
enum class unoptype {
  Negate,
  Square,
  Invert,
  SquareRoot
};

//This map is used for matching strings with operations
static const std::map<std::string, unoptype> unop_map = {
    {"--", unoptype::Negate},
    {"^2", unoptype::Square},
    {"^-1", unoptype::Invert},
    {"^1/2", unoptype::SquareRoot}};

class UnOp: public Op
{
private:
    /* data */
public:
    UnOp(unoptype unopt);
    unoptype utp;
    void handleOp(Circuit *c);
};

