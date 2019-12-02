#include <string>
#include "token.hpp"
#include "intop.hpp"
#include "binop.hpp"
#include "unop.hpp"

using namespace std;
// depending on the input, we construct the corresponding operation.
// We interpret anything not similar to any of the operations as integers.
Token::Token (string srep){
    rep = srep;
    auto binop = binop_map.find(rep);
    auto unop = unop_map.find(rep);
    if (binop != binop_map.end() )
    {
        op = new BinOp(binop->second);
    }
    if (unop != unop_map.end()){
        op = new UnOp(unop->second);
    }
    if (binop == binop_map.end() && unop == unop_map.end())
    {
        op = new IntOp(stoi(rep)); //stoi throws invalid argument on inputs other than ints.
    }
    //ignore all other tokens, the code should never reach this section anyway
    
}
/*
Token::~Token(){
    delete op;
}*/