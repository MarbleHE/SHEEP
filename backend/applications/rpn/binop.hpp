#pragma once

#include <map>
#include <vector>
#include <stack>
#include "circuit.hpp"
#include "op.hpp"

using namespace std;

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

class BinOp: public Op
{
private:
    /* data */
public:
    BinOp(binoptype binopt);
    binoptype btp;
    void handleOp(vector<int> *ptvec, stack<Circuit> *s);
};
