#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include "op.hpp"

class Token {
    public:
        Token(std::string srep);
        Op op;
    private:
        std::string rep;
};


#endif