#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "rpn.hpp"

using namespace std;

string getCalc();
bool isValid(string s);
vector <int> getLibs();

int main(){
    string calc = getCalc();
    vector <int> libs = getLibs();
    //for (auto x : libs) std::cout << std::to_string(x) << " "; //TODO print selected libraries
    Rpn rpn(calc);
    for (auto x: libs) rpn.calcWith(x);
}

// get from stdin the calculation in RPN and return as a string.
//If it is invalid, the user is prompted to retype the calculation.
string getCalc(){
    string in;
    cout << "Welcome to the HE reverse polish notation calculator. Enter your calculation with whitespaces as separator.\n";
    getline(cin, in);
    while (! isValid(in)){
        cout << "Error: Input invalid. Try again.\n";
        getline(cin, in);
    }
    return in;
}

// checks whether a string is a valid calculation in RPN.
bool isValid(string s){
    return true;
}

/* gets from stdin the libraries and returns them as integer vector.
If nothing was specified default is plaintext.
All invalid inputs will just be ignored.
*/
vector <int> getLibs(){
    cout << "Select your libraries.\n"
        << "0: plaintext"; //TODO add all libraries.
    cout << endl;
    string in;
    vector<int> libs;
    getline(cin, in);
    istringstream iss(in);
    string s;
    while (getline(iss, s, ' ')){
        libs.push_back(stoi(s));
    }
    if (libs.empty()){
        libs.push_back(0);
        cout << "Warning: No valid libraries selected. Plaintext calculation only.\n";
    }
    return libs;
}