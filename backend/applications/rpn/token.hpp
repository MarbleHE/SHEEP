#pragma once

#include <string>
#include "op.hpp"

// a Token consists of its operation: an int or add, negate, etc...
class Token {
    public:
        Token(std::string srep);
        ~Token(); //destructor deletes allocated operation in op.
        Op *op; //this has to be a pointer, because Op is abstract
    private:
        std::string rep;
};

