#include <string>
#include "token.hpp"
#include "op.hpp"
#include "intop.hpp"
#include "binop.hpp"
#include "unop.hpp"

// depending on the input, we construct the corresponding operation.
// We interpret anything not similar to any of the operations as integers.
Token::Token (std::string srep){
    rep = srep;
    
}