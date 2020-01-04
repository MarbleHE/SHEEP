//
// Created by mario on 30.12.19.
//
#include <deque>
#include <stack>
#include <vector>

#include "op.hpp"
#include "binop.hpp"
#include "unop.hpp"
#include "intop.hpp"
#include "rpn.hpp"
#include "token.hpp"
#include "simple-circuits.hpp"
#include "circuit-util.hpp"
#include "context.hpp"

#include "gtest/gtest.h"

using namespace std;

/*
 * Basic Op tests
 */

TEST(BasicIntop__Test, One)
{
    auto i = IntOp(1);
    EXPECT_EQ(1, i.i);
}

TEST(BasicUnop__Test, Negate){
    auto t = Token("--");
    auto u = (UnOp*) t.op;
    EXPECT_EQ(unoptype::Negate, u->utp);
}

/*TODO
 * Basic test for ^2 UnOp, with TEST_P
 */

/*
 * Test token generation.
 */
/// Empty string should raise an invalid_argument exception.
TEST(TokenTest, EmptyStringTest){
    try {
        auto t = Token("");
        FAIL() << "No exception. Expected invalid_argument(\"Cannot convert string to int.\")";
    }
    catch (const invalid_argument &e){
        EXPECT_STREQ(e.what(), "Cannot convert string to int.");
    }
    catch(const exception &e){
        FAIL() << "Wrong exception. Expected invalid_argument(\"Cannot convert string to int.\")" << endl << "Got instead: " <<e.what();
    }
}

/// Whitespaces should raise an invalid_argument exception.
TEST(TokenTest, WhitespaceStringTest){
    try {
        auto t = Token(" ");
        FAIL() << "No exception. Expected invalid_argument(\"Cannot convert string to int.\")";
    }
    catch (const invalid_argument &e){
        EXPECT_STREQ(e.what(), "Cannot convert string to int.");
    }
    catch(const exception &e){
        FAIL() << "Wrong exception. Expected invalid_argument(\"Cannot convert string to int.\")" << endl << "Got instead: " <<e.what();
    }
}

/// Special characters should raise an invalid_argument exception.
TEST(TokenTest, SpecialcharStringTest){
    try {
        auto t = Token(")");
        FAIL() << "No exception. Expected invalid_argument(\"Cannot convert string to int.\")";
    }
    catch (const invalid_argument &e){
        EXPECT_STREQ(e.what(), "Cannot convert string to int.");
    }
    catch(const exception &e){
        FAIL() << "Wrong exception. Expected invalid_argument(\"Cannot convert string to int.\")" << endl << "Got instead: " <<e.what();
    }
}

/// Strings which are not an int or any Op should raise an invalid_argument exception.
TEST(TokenTest, SomeGarbageStringTest){
    try {
        auto t = Token("SomeGarbage");
        FAIL() << "No exception. Expected invalid_argument(\"Cannot convert string to int.\")";
    }
    catch (const invalid_argument &e){
        EXPECT_STREQ(e.what(), "Cannot convert string to int.");
    }
    catch(const exception &e){
        FAIL() << "Wrong exception. Expected invalid_argument(\"Cannot convert string to int.\")" << endl << "Got instead: " <<e.what();
    }
}

/// Ints which are too big (or too small) should raise an out_of_range exception.
TEST(TokenTest, OutOfRangeStringTest){
    try {
        auto t = Token("9999999999999999999999999999");
        FAIL() << "No exception. Expected out_of_range(\"Int is out of range, cannot create IntOp.\")";
    }
    catch (const out_of_range &e){
        EXPECT_STREQ(e.what(), "Int is out of range, cannot create IntOp.");
    }
    catch(const exception &e){
        FAIL() << "Wrong exception. Expected out_of_range(\"Int is out of range, cannot create IntOp.\")" << endl << "Got instead: " <<e.what();
    }
}

/// An int processed by a Token should result in a proper IntOp.
TEST(TokenTest, IntTest){
    auto t = Token("42");
    ASSERT_EQ(t.getRep(), "42");
    auto top = (IntOp *) t.op;
    EXPECT_EQ(top->i, IntOp(42).i);
}

/// A "--" should generate a negation UnOp.
TEST(TokenTest, NegUnOpTest){
    auto t = Token("--");
    auto top = (UnOp *) t.op;
    EXPECT_EQ(top->utp, UnOp(unoptype::Negate).utp);
}

/// A "-" should generate a subtract BinOp.
TEST(TokenTest, NegBinOpTest){
    auto t = Token("-");
    auto  top = (BinOp *) t.op;
    EXPECT_EQ(top->btp, BinOp(binoptype::Subtract).btp);
}

/* TODO Tests for Rpn
 */
class RpnTest:public ::testing::Test{
protected:
    Rpn rpn;
    Rpn expected_rpn;
    void SetUp() override{Test::SetUp();};
    void TearDown() override{Test::TearDown();};
};

/*
 * Tests for HandleOp
 */
class HandleOpTest: public ::testing::Test{
protected:
    Op* operation;
    vector<int> ptvec;
    stack<Circuit> s;

    vector<int> expected_ptvec;
    stack<Circuit> expected_s;
    void SetUp() override
    {
    };
    void TearDown() override
    {
        delete operation;
    }
};

/// IntOp cases

/// Test IntOp on initially empty ptvec and stack.
TEST_F(HandleOpTest, IntOnEmptyState){
    operation = (Op *) new IntOp(1);
    expected_s.push(single_unary_gate_circuit(Gate::Alias));
    expected_ptvec.push_back(1);

    operation->handleOp(ptvec,s);

    EXPECT_EQ(ptvec, expected_ptvec);
    EXPECT_EQ(s,expected_s);
}

/// Test handling of IntOp on some stack and ptvec.
TEST_F(HandleOpTest, IntOnSomeState){
    operation = (Op *) new IntOp(42);
    s.push(single_binary_gate_circuit(Gate::Multiply)); //some circuits previously residing on stack...
    s.push(single_unary_gate_circuit(Gate::Negate));
    ptvec.push_back(1);
    ptvec.push_back(3);
    expected_ptvec = ptvec;
    expected_ptvec.push_back(42);
    expected_s = s;
    expected_s.push(single_unary_gate_circuit(Gate::Alias));

    operation->handleOp(ptvec, s);

    EXPECT_EQ(ptvec, expected_ptvec);
    EXPECT_EQ(s,expected_s);
}

/// UnOp cases

/// Test exception raised on empty stack.
TEST_F(HandleOpTest, UnOpOnEmptyState){
    operation = (Op *) new UnOp(unoptype::Negate);
    try {
        operation->handleOp(ptvec,s);
        FAIL() << "No exception raised. Expected runtime_error(\"Stack has not enough input for unary gate.\")";
    }
    catch (const runtime_error &e){
        EXPECT_STREQ(e.what(), "Stack has not enough input for unary gate.");
    }
    catch (const exception &e){
        FAIL() << "Wrong exception raised. Expected runtime_error(\"Stack has not enough input for unary gate.\")" << endl << "Got instead: " <<e.what();
    }

}

/// Test unimplemented exception with operation Invert
TEST_F(HandleOpTest, UnOpUnimplemented){
    operation = (Op *) new UnOp(unoptype::Invert);
    s.push(single_unary_gate_circuit(Gate::Alias)); //some gate to prevent exception

    try {
        operation->handleOp(ptvec,s);
        FAIL() << "No exception raised. Expected GateNotImplemented(\"Gate not implemented.\")";
    }
    catch (const GateNotImplemented &e){
        EXPECT_STREQ(e.what(), "Gate not implemented.");
    }
    catch (const exception &e){
        FAIL() << "Wrong exception raised. Expected GateNotImplemented(\"Gate not implemented.\")" << endl << "Got instead: " <<e.what();
    }
}

/// Test on one alias gate in stack with Negate UnOp
TEST_F(HandleOpTest, UnOpOnSimpleState){
    operation = (Op *) new UnOp(unoptype::Negate);
    s.push(single_unary_gate_circuit(Gate::Alias));
    expected_s.push(seq(single_unary_gate_circuit(Gate::Alias),single_unary_gate_circuit(Gate::Negate)));

    operation->handleOp(ptvec,s);

    EXPECT_EQ(s, expected_s);
}

/// Test UnOp on some more complicated stack
TEST_F(HandleOpTest, UnOponSomeState){
    operation = (Op *) new UnOp(unoptype::Negate);
    s.push(single_unary_gate_circuit(Gate::Alias));
    s.push(single_unary_gate_circuit(Gate::Alias));
    s.push(single_binary_gate_circuit(Gate::Add));
    expected_s = s;
    auto circ = expected_s.top();
    expected_s.pop();
    expected_s.push(seq(circ,single_unary_gate_circuit(Gate::Negate)));

    operation->handleOp(ptvec,s);

    EXPECT_EQ(s,expected_s);
}

/// Binop cases

/// Test BinOp to raise exception on empty stack.
TEST_F(HandleOpTest, BinOpOnEmptyState){
    operation = (Op *) new BinOp(binoptype::Add);

    try {
        operation->handleOp(ptvec,s);
        FAIL() << "No exception raised. Expected runtime_error(\"Stack has not enough input for binary gate.\")";
    }
    catch (const runtime_error &e){
        EXPECT_STREQ(e.what(), "Stack has not enough input for binary gate.");
    }
    catch (const exception &e){
        FAIL() << "Wrong exception raised. Expected runtime_error(\"Stack has not enough input for binary gate.\")" << endl << "Got instead: " <<e.what();
    }
}

/// Test BinOp to raise exception on stack with only one element.
TEST_F(HandleOpTest, BinOpOnSingleElementStack){
    operation = (Op *) new BinOp(binoptype::Add);
    s.push(single_unary_gate_circuit(Gate::Alias));

    try {
        operation->handleOp(ptvec,s);
        FAIL() << "No exception raised. Expected runtime_error(\"Stack has not enough input for binary gate.\")";
    }
    catch (const runtime_error &e){
        EXPECT_STREQ(e.what(), "Stack has not enough input for binary gate.");
    }
    catch (const exception &e){
        FAIL() << "Wrong exception raised. Expected runtime_error(\"Stack has not enough input for binary gate.\")" << endl << "Got instead: " <<e.what();
    }
}

/// Test BinOp on some stack with more than one element.
TEST_F(HandleOpTest, BinOpOnSomeState){
    operation = (Op *) new BinOp(binoptype::Add);
    s.push(single_unary_gate_circuit(Gate::Alias));
    s.push((single_unary_gate_circuit(Gate::Alias)));
    s.push((single_unary_gate_circuit(Gate::Alias)));
    expected_s = s;
    auto c_r = expected_s.top();
    expected_s.pop();
    auto c_l = expected_s.top();
    expected_s.pop();
    expected_s.push(seq(par(c_l,c_r),single_binary_gate_circuit(Gate::Add)));

    operation->handleOp(ptvec,s);

    EXPECT_EQ(s,expected_s);
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}