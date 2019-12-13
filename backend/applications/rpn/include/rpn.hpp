#pragma once

#include <vector>
#include <map>
#include "token.hpp"

/// A Calculation is simply a vector of Tokens
using Calculation = std::vector<Token>;

/// Libraries which can be selected by the user.
enum library {
    Plaintext = 0, HElib_F2 = 1, LP = 2, Palisade = 3, SealBFV = 4, SealCCKS = 5, TFHE = 6
};

/// Integer types to select integer types required by contexts, depends on minimal required input size and is case switched on
enum class inttype {
    INT_8, INT_16, INT_32, INT_64
};

/// This is a representation of the RPN calculator. It is initialized by the String input which gets transformed
/// at construction to Tokens containing the operations of the calculation. The circuit composition happens eagerly when an Rpn object is constructed.
/// A calculation can be evaluated with calcWith, given a library from the enum class. The inputs are stored in ptvec, which happens eagerly at construction of an Rpn object.
class Rpn {
public:
    /// Constructor fills up Circuit c and ptvec by calling composeCircuit
    /// \param calculation RPN calculation as a raw string provided by the user
    explicit Rpn(const std::string &calculation);

    /// Computes the the evaluation of circuit c on a context determined by library on the inputs stored in ptvec.
    /// \param library The library to evaluate the circuit with.
    void calcWith(int library);

private:
    //// Data

    /// A calculation is a vector of Tokens. Tokens consist of their string representation and their abstract operation IntOp, UnOp or BinOp.
    Calculation calc;

    /// The final Circuit composed from the calculation provided by the user.
    Circuit c;

    /// All integer inputs from the calculation. They will all be encrypted and fed into the circuit.
    std::vector<int> ptvec;

    //// Helpers

    /// This function composes the circuit by processing the calculation from left to right and manipulating a Circuit stack.
    /// When the end of the calculation is reached, the stack should contain the final circuit, which gets put into c.
    void composeCircuit();

    /// This function calculates the minimal amount of bits required to represent the biggest input integer to instantiate
    /// the Plaintext type of the context.
    /// In the future, this may also estimate how big the Ciphertext type needs to be by looking at circuit depth or similar techniques.
    /// \return inttype which is at least required to represent the biggest integer in the calculation (in ptvec)
    inttype minBits();

    //// Evaluate functions

    /// calls evaluate with ContextPlain and case switches intType_t template type on number of bits required.
    /// \param minBits Smallest Integer type which is at least required to represent the biggest integer.
    void evalPlain(inttype minBits);

#ifdef HAVE_HElib
    /// calls evaluate with ContextHElib_F2 and case switches intType_t template type on number of bits required
    /// \param minBits Smallest Integer type which is at least required to represent the biggest integer.
    void evalHElib_F2(inttype minBits);
#endif

#ifdef HAVE_LP
    /// calls evaluate with ContextLP and case switches intType_t template type on number of bits required
    /// \param minBits Smallest Integer type which is at least required to represent the biggest integer.
    void evalLP(inttype minBits);
#endif

#ifdef HAVE_PALISADE
    /// calls evaluate with ContextPalisade and case switches intType_t template type on number of bits required
    /// \param minBits Smallest Integer type which is at least required to represent the biggest integer.
    void evalPalisade(inttype minBits);

#endif

#ifdef HAVE_SEAL_BFV
    /// calls evaluate with ContextSealBFV and case switches intType_t template type on number of bits required
    /// \param minBits Smallest Integer type which is at least required to represent the biggest integer.
    void evalSealBFV(inttype minBits);
#endif

    /// This template evaluates Circuit c on some context with some int type (int8_t, etc.) given the inputs from ptvec
    template<typename GenericContext, typename intType_t>
    void eval();
};

