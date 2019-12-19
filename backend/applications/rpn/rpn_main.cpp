#include <iostream>
#include <string>
#include <vector>

#include "context.hpp"

#include "include/rpn.hpp"
#include "include/avlibs.hpp"

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
                    << "Error: You entered an unsupported operation (gat not implemented) or your RPN has the wrong format:"
                    << endl
                    << e.what()
                    << endl;
            continue;
        }

        cout << "Select your libraries." << endl;
        printlibs();

        vector<int> libs;
        while (libs.empty()) {
            try {
                libs = getLibs();
            }
            catch (const invalid_argument &e) {
                cout << e.what() << endl;
                cout << "Select some valid, available libraries." << endl;
                printlibs();
            }
        }

        try {
            for (auto x: libs){
                tuple <vector<int>, DurationContainer> results;
                try {
                    results = rpn.calcWith(x);
                }
                catch (const GateNotImplemented &e){
                    cout << "Evaluation error:" << endl;
                    cout << e.what() << endl << endl;
                    continue;
                }
                catch (const invalid_argument &e){
                    cout << "Evaluation error:" << endl;
                    cout << e.what();
                    cout << " (Library nr. " << x << ")" << endl;
                    cout << endl;
                    continue;
                }
                vector<int> ptv = get<0>(results);
                DurationContainer dc = get<1>(results);
                cout << "Result is: ";
                for (auto x : ptv) cout << to_string(x) << " ";
                cout << endl;
                // Prints timing results. DurationContainer.first saves info about the Circuit, .second about the Gates.
                cout.setf(ios::fixed, ios::floatfield);
                cout.setf(ios::showpoint);
                cout << "Encryption time: " << dc.first[0].count() / 1000 << " milliseconds" << endl;
                cout << "Evaluation time: " << dc.first[1].count() / 1000 << " milliseconds" << endl;
                cout << "Decryption time: " << dc.first[2].count() / 1000 << " milliseconds" << endl;
                cout << endl;
            }
        }
        catch (const runtime_error &e) {
            cout
                    << "Evaluation error: "
                    << endl
                    << e.what()
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
        if (s == "" || s == " ") continue; //skip excess whitespaces
        try {
            libs.push_back(stoi(s));
        }
        catch (...) {
            throw invalid_argument("Invalid input for library selection.");
        }
    }
    return libs;
}