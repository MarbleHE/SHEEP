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
#include <context-helib.hpp>
#include <context-lp.hpp>
//#include <context-palisade.hpp> These didn't install correctly on the local machine (can't be found)
//#include <context-seal-bfv.hpp>
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
// This has ugly switch cases, because due to user input, we don't know enough at compile time and can't use templates...
void Rpn::calcWith(int library){
    inttype ptBits = minBits();
    // case switch on library
    switch (library)
    {
        case Plaintext:
            evalPlain(ptBits);
            break;
        case HElib_F2:
            evalHElib_F2(ptBits);
            break;
        case LP:
            evalLP(ptBits);
            break;/*  //Import errors on local machine, because local library install doesn't work for SEAL and Palisade...
        case Palisade:
            evalPalisade(ptBits);
            break;
        case SEAL_BFV:
            evalSealBFV(ptBits);
            break;*/
            /*
        case SEAL_CKKS:
            // This would only be useful if we have floats? Do we detect floats in the input and choose SEAL_CKKS automatically?
        case TFHE:
             // Lots of options... When do we choose which? Do we choose automatically?
            */
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

void Rpn::evalPlain(inttype minBits){
    cout << "Constructing Plaintext Context..." << endl;
    switch (minBits) {
        // case switch based on minimal number of bits needed for representation of inputs
        // Sadly a type determined at runtime has to be case switched or dynamically bound and cannot be handled with templates.
        // set plaintext context and evaluate c on it
        case inttype::INT_8:
            eval<ContextClear<int8_t>, int8_t>();
            break;
        case inttype::INT_16:
            eval<ContextClear<int16_t>, int16_t>();
            break;
        case inttype::INT_32:
            eval<ContextClear<int32_t>, int32_t>();
            break;
        case inttype::INT_64:
            eval<ContextClear<int64_t>, int64_t>();
        default:
            break;
    }
}


void Rpn::evalHElib_F2(inttype minBits){
    cout << "Constructing HElib_F2 Context..." << endl;
    switch (minBits) {
        // case switch based on minimal number of bits needed for representation of inputs
        // Sadly a type determined at runtime has to be case switched or dynamically bound and cannot be handled with templates.
        // set plaintext context and evaluate c on it
        case inttype::INT_8:
            eval<ContextHElib_F2<int8_t >, int8_t >();
            break;
        case inttype::INT_16:
            eval<ContextHElib_F2<int16_t>, int16_t>(); //Comment: in example they used uints...
            break;
        case inttype::INT_32:
            eval<ContextHElib_F2<int32_t>, int32_t>();
            break;
        case inttype::INT_64:
            eval<ContextHElib_F2<int32_t>, int32_t>();
        default:
            break;
    }
}

void Rpn::evalLP(inttype minBits){
    cout << "Constructing LP Context..." << endl;
    switch (minBits) {
        // case switch based on minimal number of bits needed for representation of inputs
        // Sadly a type determined at runtime has to be case switched or dynamically bound and cannot be handled with templates.
        // set plaintext context and evaluate c on it
        case inttype::INT_8:
            eval<ContextLP<int8_t >, int8_t >();
            break;
        case inttype::INT_16:
            eval<ContextLP<int16_t>, int16_t>(); //Comment: in example they used uints...
            break;
        case inttype::INT_32:
            eval<ContextLP<int32_t>, int32_t>();
            break;
        case inttype::INT_64:
            eval<ContextLP<int32_t>, int32_t>();
        default:
            break;
    }
}
/*
void Rpn::evalPalisade(inttype minBits){
    cout << "Constructing Palisade Context..." << endl;
    switch (minBits) {
        // case switch based on minimal number of bits needed for representation of inputs
        // Sadly a type determined at runtime has to be case switched or dynamically bound and cannot be handled with templates.
        // set plaintext context and evaluate c on it
        case inttype::INT_8:
            eval<ContextPalisade<int8_t >, int8_t >();
            break;
        case inttype::INT_16:
            eval<ContextPalisade<int16_t>, int16_t>(); //Comment: in example they used uints...
            break;
        case inttype::INT_32:
            eval<ContextPalisade<int32_t>, int32_t>();
            break;
        case inttype::INT_64:
            eval<ContextPalisade<int32_t>, int32_t>();
        default:
            break;
    }
}

void Rpn::evalSealBFV(inttype minBits){
    cout << "Constructing SealBFV Context..." << endl;
    switch (minBits) {
        // case switch based on minimal number of bits needed for representation of inputs
        // Sadly a type determined at runtime has to be case switched or dynamically bound and cannot be handled with templates.
        // set plaintext context and evaluate c on it
        case inttype::INT_8:
            eval<ContextSealBFV<int8_t >, int8_t >();
            break;
        case inttype::INT_16:
            eval<ContextSealBFV<int16_t>, int16_t>(); //Comment: in example they used uints...
            break;
        case inttype::INT_32:
            eval<ContextSealBFV<int32_t>, int32_t>();
            break;
        case inttype::INT_64:
            eval<ContextSealBFV<int32_t>, int32_t>();
        default:
            break;
    }
}
*/

//constructs a plaintext context with the computed inttype and runs it with the Circuit c, providing the ints from ptvec as input.
template <typename genericContext, typename intType_t>
void Rpn::eval(){
    typedef std::pair<std::vector<std::chrono::duration<double, std::micro> >,
            std::map<std::string, std::chrono::duration<double, std::micro> > > DurationContainer;
    typedef vector<vector<intType_t>> PtVec;

    genericContext ctx;  // paramset, bootstrappable
    DurationContainer dc;

    PtVec inputs;
    for (int i: ptvec){
        vector<intType_t> v = {(intType_t) i};
        inputs.push_back(v);
    }

    cout << "Inputs are: ";
    for (auto x : inputs) cout << to_string(x[0]) << " ";
    cout << endl;

    cout << "Evaluating..." << endl;
    PtVec sortedPtV = ctx.eval_with_plaintexts(c, inputs, dc);

    cout << "Sorted result is: ";
    for (auto x : sortedPtV) cout << to_string(x[0]) << " ";
    cout << endl;
    // Prints timing results. DurationContainer.first saves info about the Circuit, .second about the Gates.
    cout.setf(ios::fixed, ios::floatfield);
    cout.setf(ios::showpoint);
    cout << "Encryption time: " << dc.first[0].count()/1000 << " milliseconds" << endl;
    cout << "Evaluation time: " << dc.first[1].count()/1000 << " milliseconds" << endl;
    cout << "Decryption time: " << dc.first[2].count()/1000 << " milliseconds" << endl;
    cout << endl;
}
