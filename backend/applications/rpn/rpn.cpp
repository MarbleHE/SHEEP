#include <algorithm>
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
   // vector<Token> tokens;
    istringstream iss(calculation);
    string s;
    while (getline(iss, s, ' ')){
        calc.push_back(Token(s));
    }
   // calc = tokens;
}


//composes a circuit with the desired library and calculations and runs it.
void Rpn::calcWith(int library){
    composeCircuit();
    inttype mbits = minBits();
    cout << c;
    switch (library)
    {
    case Plaintext:
    {
        //case switch based on minimal number of bits needed for representation of inputs
        switch (mbits)
        {
        case inttype::INT_8:
            plaintext(); //TODO: use template instead
            break;
        case inttype::INT_16:
            plaintext();
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
void Rpn::composeCircuit(){
    auto *s = new stack<Circuit>;
    for (Token t: calc ){
        t.op->handleOp(&ptvec, s);
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

//TODO: implement as template with intType getting int8_t, int16_t, etc...
void Rpn::plaintext(){
    typedef vector<vector<ContextClear<int16_t>::Plaintext>> PtVec;

    cout << "Constructing context plaintext...\n";
    ContextClear<int16_t > ctx;  // paramset, bootstrappable

    PtVec inputs;
    for (int i: ptvec){
        vector<ContextClear<int16_t >::Plaintext> v = {(ContextClear<int16_t >::Plaintext) i};
        inputs.push_back(v);
    }

    cout << "Inputs are: ";
    for (auto x : inputs) cout << to_string(x[0]) << " ";
    cout << endl;

    reverse(inputs.begin(),inputs.end()); //This hack is somehow necessary, as input somehow get inserted in reversed order into the Circuit....?
    PtVec sortedPtV = ctx.eval_with_plaintexts(c, inputs);

    cout << "Sorted result is: ";
    for (auto x : sortedPtV) cout << to_string(x[0]) << " ";
    cout << endl;
    }
