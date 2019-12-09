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
    cout << "Welcome to the HE reverse polish notation calculator powered with SHEEP. Enter your calculation with whitespaces as separator.\n";
    cout << "Example: ((2 + 3) * -(4 - 5) * 2)^2 written in RPN is  2 3 + 4 5 - -- * 2 * ^2 and should equate to 100." << endl;
    cout << "Currently the following operations are supported (use the string next to it):" << endl;
    cout << "Addition +" << endl;
    cout << "Subtraction -" << endl;
    cout << "Multiplication *" << endl;
    cout << "Negation --" << endl;
    cout << "Square ^2" << endl;
    while (true) {
        string calc = getCalc(); //TODO: handle out_of_range exception from stoi
        if (calc.empty()) {cout << "Goodbye" << endl; return 0;} //terminate calculator, if no calculation specified.
        try {
            Rpn rpn(calc);
            vector<int> libs = getLibs();
            try {
                for (auto x: libs) rpn.calcWith(x);
            }
            catch (runtime_error){ //TODO: implement and use different exception, which makes sense
                cout << "Error: The number of input integers provided does not match the number of inputs required by the operations provided, or the gate is not implemented." << endl;
            }
            //for (auto x : libs) std::cout << std::to_string(x) << " "; //TODO print selected libraries
        }
        catch (runtime_error){ //TODO: catch meaningful stuff only, output meaningful message.
            cout << "Error: You entered an unsupported operation, something other than integers as data/library, or your RPN has the wrong format." << endl;
        }
    }
}

// get from stdin the calculation in RPN and return as a string.
// If it is invalid, the user is prompted to retype the calculation.
string getCalc(){
    string in;
    cout << "\nWrite a new calculation and hit enter: ";
    getline(cin, in);
    while (! isValid(in)){
        cout << "Error: Input invalid. Try again.\n";
        getline(cin, in);
    }
    return in;
}

// checks whether a string is a valid calculation in RPN.
// TODO: implement
bool isValid(string s){
    return true;
}

/* gets from stdin the libraries and returns them as integer vector.
If nothing was specified default is plaintext.
All invalid inputs will just be ignored, or an invalid argument error from stoi will be raised.
*/
vector <int> getLibs(){
    cout << "Select your libraries.\n"
        << "0: Plaintext" << endl << "1: HElib_F2" << endl << "2: LP"/* << endl << "3: Palisade" << endl << "4: SealBFV"*/; //TODO add all libraries.
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