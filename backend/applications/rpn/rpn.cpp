#include <string>
#include <istream>
#include <sstream>
#include <stack>
#include "rpn.hpp"
#include "token.hpp"
#include "simple-circuits.hpp"


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
    Circuit c = composeCircuit(library);
    //then run.
}

//composes a circuit given a library based on the calculation stored in the object.
Circuit Rpn::composeCircuit(int library){
    stack<Token> s;
    for (auto i = calc.begin(); i != calc.end(); ++i){
        Op cop = i->op;

    }
    
    //return ;
}