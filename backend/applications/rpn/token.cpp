#include <string>
#include <utility>
#include "token.hpp"
#include "intop.hpp"
#include "binop.hpp"
#include "unop.hpp"

using namespace std;
/// Constructor, simply copies the string and initializes the operation with a case switch on the content of the string
/// Depending on the input, we construct the corresponding operation.
/// We interpret anything not similar to any of the operations as integers.
/// \param srep String representation of the token, gets copies into rep and is the string we case switch on.
Token::Token (string srep){
    rep = std::move(srep);
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