#pragma once

#include <string>
#include "op.hpp"

// A Token consists of its string representation (for debugging) and abstract operation: an int or add, negate, etc.
class Token {
    public:
        explicit Token(std::string srep);
        //~Token(); //destructor deletes allocated operation in op.
        Op *op; //this has to be a pointer, because Op is abstract
    private:
        // The string that will be interpreted by an operation. ("+", "42", etc...)
        std::string rep;
};

