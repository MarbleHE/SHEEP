#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include "op.hpp"

// a Token consists of its operation: an int or add, negate, etc...
class Token {
    public:
        Token(std::string srep);
        Op op;
    private:
        std::string rep;
};


#endif