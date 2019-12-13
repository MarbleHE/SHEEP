#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "include/rpn.hpp"

using namespace std;

string getCalc();

vector<int> getLibs();

int main() {
    cout
            << "Welcome to the HE reverse polish notation calculator powered with SHEEP. Enter your calculation with whitespaces as separator.\n";
    cout << "Example: ((2 + 3) * -(4 - 5) * 2)^2 written in RPN is  2 3 + 4 5 - -- * 2 * ^2 and should equate to 100."
         << endl;
    cout << "Currently the following operations are supported (use the string next to it):" << endl;
    cout << "Addition +" << endl;
    cout << "Subtraction -" << endl;
    cout << "Multiplication *" << endl;
    cout << "Negation --" << endl;
    cout << "Square ^2" << endl;
    while (true) {
        string calc = getCalc();
        if (calc == "q") {
            cout << "Goodbye" << endl;
            return 0;
        } // terminate calculator.
        if (calc.empty()) {
            cout << "No calculation specified. Using example calculation 2 3 + 4 5 - -- * 2 * ^2." << endl;
            calc = "2 3 + 4 5 - -- * 2 * ^2";
        } // standard demo calculation.

        Rpn rpn;
        try {
            rpn = Rpn(calc);
        } catch (const runtime_error &e) {
            cout
                    << "Error: You entered an unsupported operation, something other than integers as data/library, or your RPN has the wrong format:"
                    << endl
                    << e.what()
                    << endl;
            continue;
        }

        cout << "Select your libraries.\n"
             << "0: Plaintext" << endl << "1: HElib_F2" << endl
             << "2: LP"/* << endl << "3: Palisade" << endl << "4: SealBFV"*/; //TODO add all AVAILABLE libraries.
        cout << endl;
        vector<int> libs = getLibs();


        try {
            for (auto x: libs) rpn.calcWith(x);
        }
        catch (const runtime_error &e) { //TODO: implement and use different exception, which makes sense
            cout
                    << "Error: The number of input integers provided does not match the number of inputs required by the operations provided, or the gate is not implemented."
                    << endl;
        }
    }
}

/// get from stdin the calculation in RPN and return as a string.
/// If it is invalid, the user is prompted to retype the calculation.
string getCalc() {
    string in;
    cout << "\nWrite a new calculation and hit enter: ";
    getline(cin, in);
    return in;
}

/// gets from stdin the libraries and returns them as integer vector.
/// If nothing was specified default is plaintext.
/// All invalid inputs will just be ignored, or an invalid argument error from stoi will be raised.
/// \return vector containing user selected libraries as integers
vector<int> getLibs() {
    string in;
    vector<int> libs;
    getline(cin, in);
    istringstream iss(in);
    string s;
    while (getline(iss, s, ' ')) {
        libs.push_back(stoi(s)); //TODO: Catch stoi exception!
    }
    if (libs.empty()) {
        libs.push_back(0);
        cout << "Warning: No valid libraries selected. Plaintext calculation only.\n";
    }
    return libs;
}