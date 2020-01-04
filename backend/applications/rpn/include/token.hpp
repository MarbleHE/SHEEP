#pragma once

#include <string>
#include "op.hpp"

/// A Token consists of its string representation (for debugging) and abstract operation: an int or add, negate, etc.
class Token {
public:
    /// Constructor, simply copies the string and initializes the operation with a case switch on the content of the string
    /// \param srep String representation of the token, gets copies into rep.
    explicit Token(std::string srep);

    //~Token(); //destructor deletes allocated operation in op.

    /// Abstract operation representing it as a one of its Subclasses.
    Op *op; //this has to be a pointer, because Op is abstract

    /// Getter for string representation. (Debugging)
    const std::string &getRep() const;
private:
    /// The string that will be interpreted by an operation. ("+", "42", etc...)
    std::string rep;
};

