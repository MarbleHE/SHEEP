#ifndef RPN_HPP
#define RPN_HPP

#include <vector>
#include "token.hpp"
#include "simple-circuits.hpp"

using namespace std;
using Calculation = vector <Token>;

class Rpn{
    public:
        Calculation calc;
        Rpn(string calculation);
        void calcWith(int library);
    private:
        //vector<int> stack; //might put this in buildCircuit function.
        Circuit composeCircuit(int library);
};

#endif
