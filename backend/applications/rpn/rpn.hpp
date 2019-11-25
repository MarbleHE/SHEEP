#ifndef RPN_HPP
#define RPN_HPP

#include "simple-circuits.hpp"

//using Calculation = vector <Token>;
using namespace std;

class Rpn{
    public:
        //Calculation calc;
        Rpn(string calculation);
        void calcWith(int library);
    private:
        //vector<int> stack; //might put this in buildCircuit function.
        Circuit composeCircuit(int library);
};

#endif
