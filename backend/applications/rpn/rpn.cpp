#include <string>
#include <istream>
#include <sstream>
#include "rpn.hpp"
#include "token.hpp"

using namespace std;

Rpn::Rpn(string calculation){
    vector<Token> tokens;
    istringstream iss(calculation);
    string s;
    while (getline(iss, s, ' ')){
        tokens.push_back(Token(s));
    }
}

void Rpn::calcWith(int library){

}