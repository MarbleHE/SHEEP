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
 * Basic tests for CMake build.
 */

TEST(BasicIntop__Test, One)
{
    auto i = IntOp(1);
    ASSERT_EQ(1, i.i);
}

TEST(BasicUnop__Test, Negate){
    auto t = Token("--");
    auto u = (UnOp*) t.op;
    ASSERT_EQ(unoptype::Negate, u->utp);
}

/*TODO
 * Basic test for ^2 UnOp, with TEST_P
 */

/* TODO
 * Test all valid token generations.
 */
class TokenTests:public ::testing::Test{
protected:
    void SetUp() override{Test::SetUp();};
    void TearDown() override{Test::TearDown();};
};

/* TODO
 * Test Rpn
 * Maybe it is smarter to use instantiations of some Rpn objects, instead of comparing vectors and stacks...
 */
class RpnTest:public ::testing::Test{
protected:
    Rpn rpn;
    void SetUp() override{Test::SetUp();};
    void TearDown() override{Test::TearDown();};
};

/*
 * TODO
 * Test HandleOp
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
    catch (...){
        FAIL() << "Wrong exception raised. Expected runtime_error(\"Stack has not enough input for unary gate.\")";
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
    catch (...){
        FAIL() << "Wrong exception raised. Expected GateNotImplemented(\"Gate not implemented.\")";
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

//TODO test all binop cases

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}