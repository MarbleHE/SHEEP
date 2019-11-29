#include <string>
#include <istream>
#include <sstream>
#include <stack>
#include <vector>
#include <vector>
#include <memory>
#include <cmath>
#include <context.hpp>
#include <context-clear.hpp>
#include "rpn.hpp"
#include "token.hpp"
#include "op.hpp"

enum libs {Plaintext = 0}; //TODO add all

using namespace std;
using namespace SHEEP;

//Transform to tokens containing ints or ops. Whitespace separator.
Rpn::Rpn(string calculation){
    vector<Token> tokens;
    istringstream iss(calculation);
    string s;
    while (getline(iss, s, ' ')){
        tokens.push_back(Token(s));
    }
    calc = tokens;
}


//composes a circuit with the desired library and calculations and runs it.
void Rpn::calcWith(int library){
    composeCircuit(library);
    inttype mbits = minBits();
    //Context *ctxt;
    cout << c;
    switch (library)
    {
    case Plaintext:
    {
        //TODO: case switch based on mbits.
        //I don't know how to do it better, but the template needs to be initialized with a type.
        switch (mbits)
        {
        case inttype::INT_8:
            /* plaintextINT_8(); */
            break;
        case inttype::INT_16:
            plaintextINT_16();
        default:
            break;
        }
        //set plaintext context
        break;
    }
    default:
        //set plaintext context
        break;
    }
}

//composes a circuit given a library based on the calculation stored in the object.
void Rpn::composeCircuit(int library){
    stack<Circuit> *s = new stack<Circuit>;
    for (auto i = calc.begin(); i != calc.end(); ++i){
        Op *cop = i->op;
        cop->handleOp(&ptvec, s);
        //compose circuit, make all plaintext numbers ciphertexts and put into private array for eval. Or maybe I nee to put them into a mapping int,Wire...?
    }
    // stack should now only contain the final circuit.
    if (s->size() > 1)
    {
        throw runtime_error("More than one Circuit remains in stack. Either the RPN was illegal, or Circuit composition failed.");
    }
    c = s->top();
    s->pop();
    delete s;
}

/*minBits determines the minimal required number of bits based on the user input
It is a very easy implementation, assuming no overflow happens in the circuit.
The only thing checked is whether an encoding of each plaintext is possible.
In the future one could expand this such that it is possible to
estimate size of the result and therefore set the CipherText size in the context.
*/
inttype Rpn::minBits(){
    int cmax = 0;
    for (int i: ptvec){
        cmax = max(cmax,abs(i));
    }
    int n_bits = (int) ceil(log(cmax));
    if (n_bits < 8)
    {
        return inttype::INT_8;
    }
    if (n_bits < 16)
    {
        return inttype::INT_16;
    }
    if (n_bits < 32)
    {
        return inttype::INT_32;
    }
    if (n_bits < 64)
    {
        return inttype::INT_64;
    }
    throw invalid_argument("Integer too large for 64 bits");
}

void Rpn::plaintextINT_16(){
    typedef std::vector<std::vector<ContextClear<int16_t>::Plaintext>> PtVec;

    std::cout << "Constructing context...\n";
    ContextClear<int16_t> ctx;  // paramset, bootstrappable

    PtVec inputs;
    for (int i: ptvec){
        vector<ContextClear<int16_t>::Plaintext> v = {(ContextClear<int16_t>::Plaintext) i};
        inputs.push_back(v);
    }

    std::cout << "Inputs are: ";
    for (auto x : inputs) std::cout << std::to_string(x[0]) << " ";
    std::cout << std::endl;

    PtVec sorted = ctx.eval_with_plaintexts(c, inputs);

    std::cout << "Sorted result is: ";
    for (auto x : sorted) std::cout << std::to_string(x[0]) << " ";
    std::cout << std::endl;
    }
