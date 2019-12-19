#include <string>
#include <istream>
#include <iostream>
#include <stack>
#include <vector>
#include <memory>
#include <cmath>

using namespace std;

#include "circuit.hpp"
#include "context.hpp"
#include "context-clear.hpp"

#ifdef HAVE_HElib

#include "context-helib.hpp"

#endif

#ifdef HAVE_LP

#include "context-lp.hpp"

#endif

#ifdef HAVE_PALISADE
#include "context-palisade.hpp"
#endif

#ifdef HAVE_SEAL_BFV
#include "context-seal-bfv.hpp"
#endif

#ifdef HAVE_SEAL_CKKS
#include "context-seal-ckks.hpp"
#endif

using namespace SHEEP;

#include "include/rpn.hpp"
#include "include/token.hpp"


Rpn::Rpn(const string &calculation) {
    istringstream iss(calculation);
    string s;
    while (getline(iss, s, ' ')) {
        cout << "Processing: \"" << s << "\"" << endl;
        if (s == "" || s == " ") continue; //ignore excess whitespaces
        try {
            calc.push_back(Token(s));
        }
        catch (exception &e) {
            cout << "Warning: some Token could not be processed and was ignored." << endl;
            cout << e.what() << endl;
        }
    }
    composeCircuit();
}

tuple<vector<int>, DurationContainer> Rpn::calcWith(int library) {
    inttype ptBits = minBits();
    // case switch on library
    switch (library) {
        case Plaintext:
            return evalPlain(ptBits);
            break;
//TODO: Is there a better solution for this?
#ifdef HAVE_HElib
        case HElib_F2:
            return evalHElib_F2(ptBits);
            break;
#endif
#ifdef HAVE_LP
        case LP:
            return evalLP(ptBits);
            break;
#endif
#ifdef  HAVE_PALISADE
        case Palisade:
            return evalPalisade(ptBits);
            break;
#endif
#ifdef HAVE_SEAL_BFV
        case SEAL_BFV:
            return evalSealBFV(ptBits);
            break;
#endif
#ifdef HAVE_SEAL_CKKS
        case SEAL_CKKS:
            // This would only be useful if we have floats? Do we detect floats in the input and choose SEAL_CKKS automatically?
            break;
#endif
#ifdef  HAVE_TFHE
        case TFHE:
            // Lots of options... When do we choose which? Do we choose automatically?
            throw invalid_argument("TFHE not implemented.");
            break;
#endif
        default:
            throw invalid_argument("Some invalid or unavailable library was selected for evaluation.");
            break;
    }
}

void Rpn::composeCircuit() {
    // this stack contains circuits of partial results. Later, they will get combined to a single Circuit using sequential/parallel circuit composition.
    stack<Circuit> s;
    for (Token t: calc) {
        // this virtual function fills up ptvec every time an int is found and if an operation is found, it manipulates the stack s to gradually compose a single circuit
        t.op->handleOp(ptvec, s);
    }
    // stack should now only contain the final circuit.
    if (s.size() > 1) {
        throw runtime_error(
                "More than one Circuit remains in stack. Either the RPN was illegal (wrong format), or Circuit composition failed.");
    }
    c = s.top();
    s.pop();
    cout << "Circuit successfully composed:" << endl;
    cout << c << endl;
}

inttype Rpn::minBits() {
    int cmax = 0;
    for (int i: ptvec) {
        cmax = max(cmax, abs(i));
    }
    int n_bits = (int) ceil(log(cmax));
    if (n_bits < 8) {
        return inttype::INT_8;
    }
    if (n_bits < 16) {
        return inttype::INT_16;
    }
    if (n_bits < 32) {
        return inttype::INT_32;
    }
    if (n_bits < 64) {
        return inttype::INT_64;
    }
    throw invalid_argument("Integer too large for 64 bits");
}


tuple<vector<int>, DurationContainer> Rpn::evalPlain(inttype minBits) {
    cout << "Constructing Plaintext Context..." << endl;
    switch (minBits) {
        // case switch based on minimal number of bits needed for representation of inputs
        // Sadly a type determined at runtime has to be case switched or dynamically bound and cannot be handled with templates.
        // set plaintext context and evaluate c on it
        case inttype::INT_8:
            return eval<ContextClear<int8_t>, int8_t>();
            break;
        case inttype::INT_16:
            return eval<ContextClear<int16_t>, int16_t>();
            break;
        case inttype::INT_32:
            return eval<ContextClear<int32_t>, int32_t>();
            break;
        case inttype::INT_64:
            return eval<ContextClear<int64_t>, int64_t>();
        default:
            break;
    }
}

#ifdef HAVE_HElib

tuple<vector<int>, DurationContainer> Rpn::evalHElib_F2(inttype minBits) {
    cout << "Constructing HElib_F2 Context..." << endl;
    switch (minBits) {
        // case switch based on minimal number of bits needed for representation of inputs
        // Sadly a type determined at runtime has to be case switched or dynamically bound and cannot be handled with templates.
        // set plaintext context and evaluate c on it
        case inttype::INT_8:
            return eval<ContextHElib_F2<int8_t>, int8_t>();
            break;
        case inttype::INT_16:
            return eval<ContextHElib_F2<int16_t>, int16_t>(); //Comment: in example they used uints...
            break;
        case inttype::INT_32:
            return eval<ContextHElib_F2<int32_t>, int32_t>();
            break;
        case inttype::INT_64:
            return eval<ContextHElib_F2<int32_t>, int32_t>();
        default:
            break;
    }
}

#endif

#ifdef HAVE_LP

tuple<vector<int>, DurationContainer> Rpn::evalLP(inttype minBits) {
    cout << "Constructing LP Context..." << endl;
    switch (minBits) {
        // case switch based on minimal number of bits needed for representation of inputs
        // Sadly a type determined at runtime has to be case switched or dynamically bound and cannot be handled with templates.
        // set plaintext context and evaluate c on it
        case inttype::INT_8:
            return eval<ContextLP<int8_t>, int8_t>();
            break;
        case inttype::INT_16:
            return eval<ContextLP<int16_t>, int16_t>(); //Comment: in example they used uints...
            break;
        case inttype::INT_32:
            return eval<ContextLP<int32_t>, int32_t>();
            break;
        case inttype::INT_64:
            return eval<ContextLP<int32_t>, int32_t>();
        default:
            break;
    }
}

#endif


#ifdef HAVE_PALISADE
tuple<vector<int>, DurationContainer> Rpn::evalPalisade(inttype minBits){
    cout << "Constructing Palisade Context..." << endl;
    switch (minBits) {
        // case switch based on minimal number of bits needed for representation of inputs
        // Sadly a type determined at runtime has to be case switched or dynamically bound and cannot be handled with templates.
        // set plaintext context and evaluate c on it
        case inttype::INT_8:
            return eval<ContextPalisade<int8_t >, int8_t >();
            break;
        case inttype::INT_16:
            return eval<ContextPalisade<int16_t>, int16_t>(); //Comment: in example they used uints...
            break;
        case inttype::INT_32:
            return eval<ContextPalisade<int32_t>, int32_t>();
            break;
        case inttype::INT_64:
            return eval<ContextPalisade<int32_t>, int32_t>();
        default:
            break;
    }
}
#endif

#ifdef HAVE_SEAL_BFV
tuple<vector<int>, DurationContainer> Rpn::evalSealBFV(inttype minBits){
    cout << "Constructing SealBFV Context..." << endl;
    switch (minBits) {
        // case switch based on minimal number of bits needed for representation of inputs
        // Sadly a type determined at runtime has to be case switched or dynamically bound and cannot be handled with templates.
        // set plaintext context and evaluate c on it
        case inttype::INT_8:
            return eval<ContextSealBFV<int8_t >, int8_t >();
            break;
        case inttype::INT_16:
            return eval<ContextSealBFV<int16_t>, int16_t>(); //Comment: in example they used uints...
            break;
        case inttype::INT_32:
            return eval<ContextSealBFV<int32_t>, int32_t>();
            break;
        case inttype::INT_64:
            return <ContextSealBFV<int32_t>, int32_t>();
        default:
            break;
    }
}
#endif

template<typename genericContext, typename intType_t>
tuple<vector<int>, DurationContainer> Rpn::eval() {
    typedef vector<vector<intType_t>> PtVec;

    genericContext ctx;  // paramset, bootstrappable
    DurationContainer dc;

    PtVec inputs;
    for (auto i: ptvec) {
        vector<intType_t> v = {(intType_t) i};
        inputs.push_back(v);
    }

    cout << "Inputs are: ";
    for (auto x : inputs) cout << to_string(x[0]) << " ";
    cout << endl;

    cout << "Evaluating..." << endl;
    PtVec ptv;
    try {
        ptv = ctx.eval_with_plaintexts(c, inputs, dc);
    }
    catch (const GateNotImplemented &e) {
        throw GateNotImplemented();
    }
    vector<int> iptv;
    for (auto x: ptv) {
        iptv.push_back((int) x[0]);
    }
    return make_tuple(iptv, dc);
}
