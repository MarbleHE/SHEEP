#include <string>
#include <istream>
#include <sstream>
#include <stack>
#include <vector>
#include <list>
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
    //NOTE: This seems to be an example with an outdated interface. Compile does not exist.
    ContextClear<int16_t> ctx;
    //ContextClear<int16_t>::CircuitEvaluator run_circuit; run_circuit = ctx.compile(c);*/
    //loads inputs from ptvec and casts them to the new type
    list<ContextClear<int16_t>::Plaintext> plaintext_inputs;
    for (int i: ptvec){
        plaintext_inputs.push_back((ContextClear<int16_t>::Plaintext) i);
    }
    list<ContextClear<int16_t>::Ciphertext> ciphertext_inputs;

    for (ContextClear<int16_t>::Plaintext pt: plaintext_inputs)
      ciphertext_inputs.push_back(ctx.encrypt(pt)); //TODO why does this expect a whole Plaintext vector? Outdated interface?

    list<ContextClear<int16_t>::Ciphertext> ciphertext_outputs;
    using microsecond = std::chrono::duration<double, std::micro>;
    microsecond time = ctx.eval(c, ciphertext_inputs, ciphertext_outputs);

    list<ContextClear<int16_t>::Plaintext> plaintext_outputs;
    for (ContextClear<int16_t>::Ciphertext ct: ciphertext_outputs) {
      ContextClear<int16_t>::Plaintext pt = ctx.decrypt(ct);
      plaintext_outputs.push_back(pt);
      std::cout << "output: "<<std::to_string(pt) << std::endl;
    }
    std::cout << "time was " << time.count() << " microseconds\n";
    }
