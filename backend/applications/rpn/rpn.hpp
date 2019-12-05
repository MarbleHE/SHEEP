#pragma once

#include <vector>
#include <map>
#include "token.hpp"

using namespace std;
using Calculation = vector <Token>;

// This is to switch case based on the libraries selected by the user.
enum library {Plaintext = 0, HElib_F2 = 1, LP = 2/*, Palisade = 3, SealBFV = 4*/}; //TODO add all

// This is the return type of minBits to switch case the type of the context template
enum class inttype {INT_8, INT_16, INT_32, INT_64};

/*This is a representation of the calculator. It is initialized by the String input which gets transformed
    at construction to to Tokens containing the operations. The circuit composition happens lazily when a
    calculation with a specified library is requested.
*/
class Rpn{
    public:
        // Constructor
        explicit Rpn(string calculation);
        // Computes the the evaluation of circuit c on a context determined by @param library.
        void calcWith(int library);
    private:
        // A calculation is a vector of Tokens. Tokens consist of their string representation and their abstract operation IntOp, UnOp or BinOp.
        Calculation calc;
        /* This function composes the circuit by processing the calculation from left to right and manipulating a Circuit stack.
         * When the end of the calculation is reached, the stack should contain the final circuit, which gets put into c.*/
        void composeCircuit();
        // The final Circuit translated from the calculation.
        Circuit c;
        // All integer inputs from the calculation. They will all be encrypted and fed into the circuit.
        vector<int> ptvec;
        /* This function calculates the minimal amount of bits required to represent the biggest input integer to instantiate
         * the Plaintext type of the context.
         * In the future, this may also estimate how big the Ciphertext type needs to be by looking at circuit depth or similar techniques.
         */
        inttype minBits();
        // calls evaluate with ContextPlain and case switches intType_t template type on number of bits required
        void evalPlain(inttype minBits);
        // calls evaluate with ContextHElib_F2 and case switches intType_t template type on number of bits required
        void evalHElib_F2(inttype minBits);
        // calls evaluate with ContextLP and case switches intType_t template type on number of bits required
        void evalLP(inttype minBits);
        // calls evaluate with ContextPalisade and case switches intType_t template type on number of bits required
        void evalPalisade(inttype minBits);
        // calls evaluate with ContextSealBFV and case switches intType_t template type on number of bits required
        void evalSealBFV(inttype minBits);
        /* This template generates a plaintext context and evaluates */
        template<typename GenericContext, typename intType_t>void eval();
};

