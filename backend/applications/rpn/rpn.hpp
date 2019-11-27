#pragma once

#include <vector>
#include <map>
#include "token.hpp"

using namespace std;
using Calculation = vector <Token>;

/*This is a representation of the calculator. It is initialized by the String input which gets transformed
    at construction to to Tokens containing the operations. The circuit composition happens lazily when a
    calculation with a specified library is requested.
*/
class Rpn{
    public:
        Rpn(string calculation);
        void calcWith(int library);
    private:
        //vector<int> stack; //might put this in buildCircuit function.
        Calculation calc;
        void composeCircuit(int library);
        Circuit c;
        vector<int> ptvec;
};
