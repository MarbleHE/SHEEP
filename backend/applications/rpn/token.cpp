#include <string>
#include <utility>
#include "include/token.hpp"
#include "include/intop.hpp"
#include "include/binop.hpp"
#include "include/unop.hpp"

using namespace std;

Token::Token(string srep) {
    rep = std::move(srep);
    // Look for any matching binary or unary operation with the string.
    // Set the Token operation to that.
    auto binop = binop_map.find(rep);
    auto unop = unop_map.find(rep);
    if (binop != binop_map.end()) {
        op = new BinOp(binop->second);
    }
    if (unop != unop_map.end()) {
        op = new UnOp(unop->second);
    }
    // If we didn't have a match, it might be an integer. Try to make an IntOp.
    if (binop == binop_map.end() && unop == unop_map.end()) {
        try {
            op = new IntOp(stoi(rep));
        }
        catch (std::out_of_range &e) {
            throw out_of_range("Int is out of range, cannot create IntOp.");
        }
        catch (...) {
            throw invalid_argument("Cannot convert string to int.");
        }
    }
    // If we didn't have an int either...
    if (!op) {
        throw runtime_error("Token was not able to derive abstract Op from string.");
    }
}

const string &Token::getRep() const {
    return rep;
}

/*
Token::~Token(){
    delete op;
}*/