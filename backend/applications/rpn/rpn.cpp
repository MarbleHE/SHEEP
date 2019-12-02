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

using namespace std;
using namespace SHEEP;

// Transform a string to tokens containing ints or ops and fills the vector calc with them. Whitespace separator.
// Then it composes the Circuit.
Rpn::Rpn(string calculation){
    istringstream iss(calculation);
    string s;
    while (getline(iss, s, ' ')){
        calc.push_back(Token(s));
    }
    composeCircuit();
}


//Computes the types for the contexts with minBits, then (with the switch cased templates)
// constructs the selected contexts and runs the previously composed circuit on them.
void Rpn::calcWith(int library){
    inttype mbits = minBits();
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

//composes a circuit given a calculation stored in the object.
void Rpn::composeCircuit(){
    // this stack contains circuits of partial results. Later, they will get combined to a single Circuit using sequential/parallel circuit composition.
    auto *s = new stack<Circuit>;
    for (Token t: calc ){
        // this virtual function fills up ptvec every time an int is found and if an operation is found, it manipulates the stack s to compose a single circuit
        t.op->handleOp(&ptvec, s);
    }
    // stack should now only contain the final circuit.
    if (s->size() > 1)
    {
        throw runtime_error("More than one Circuit remains in stack. Either the RPN was illegal, or Circuit composition failed.");
    }
    c = s->top();
    s->pop();
    delete s;
    cout << "Circuit successfully composed:" << endl;
    cout << c << endl;
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
//constructs a plaintext context with the computed inttype and runs it with the Circuit c, providing the ints from ptvec as input.
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

    reverse(inputs.begin(),inputs.end()); //This hack is somehow necessary, as input somehow gets inserted in reverse order into the Circuit...
    PtVec sortedPtV = ctx.eval_with_plaintexts(c, inputs);

    cout << "Sorted result is: ";
    for (auto x : sortedPtV) cout << to_string(x[0]) << " ";
    cout << endl;
    }
