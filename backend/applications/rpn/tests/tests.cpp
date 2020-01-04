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

TEST_F(HandleOpTest, IntOnEmptyState){
    operation = (Op *) new IntOp(1);
    expected_s.push(single_unary_gate_circuit(Gate::Alias));
    expected_ptvec.push_back(1);

    operation->handleOp(ptvec,s);

    EXPECT_EQ(ptvec, expected_ptvec);
    EXPECT_EQ(s,expected_s);
}

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

//TODO also test exception raised on empty stack.

//TODO test on one alias gate
TEST_F(HandleOpTest, UnOpOnSimpleState){
    operation = (Op *) new UnOp(unoptype::Negate);

}

//TODO test on some complicated setting

//TODO test all binop cases

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}