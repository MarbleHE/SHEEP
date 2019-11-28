#include <string>
#include <istream>
#include <sstream>
#include <stack>
#include <cmath>
#include "rpn.hpp"
#include "token.hpp"
#include "op.hpp"

enum libs {Plaintext = 0}; //TODO add all

using namespace std;

//Transform to tokens cointaining ints or ops. Whitespace separator.
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
    inttype  minBits();
    switch (library)
    {
    case Plaintext:
        //set plaintext context
        break;
    default:
        //set plaintext context
        break;
    }
    //ctxt.eval(c, ???) //TODO: does this produce output in std::out, or do I need to do additional stuff?
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
*/
inttype Rpn::minBits(){
    int cmax = 0;
    for (auto i = ptvec.begin(); i != ptvec.end(); ++i){
        cmax = max(cmax,abs(*i));
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