//
// Created by mario on 30.12.19.
//
#include "op.hpp"
#include "binop.hpp"
#include "unop.hpp"
#include "intop.hpp"
#include "rpn.hpp"
#include "token.hpp"

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
class TokenTests: ::testing::Test{
protected:

    void SetUp() override{Test::SetUp();};
    void TearDown() override{Test::TearDown();};
};

/* TODO
 * Test handleOp
 * Maybe it is smarter to use instantiate some Rpn objects, instead of comparing vectors and stacks...
 */
class RpnTest: ::testing::Test{
protected:
    Rpn rpn;
    void SetUp() override{Test::SetUp();};
    void TearDown() override{Test::TearDown();};
};
struct handleState {
    Op* operation;
    vector<int> initial_ptvec;
    vector<int> expected_ptvec;
    stack<Circuit> initial_stack;
    stack<Circuit> expected_stack;
};
class HandleOpTest: RpnTest, ::testing::WithParamInterface<handleState>{
protected:
    Op* operation;
    vector<int> ptvec;
    stack<Circuit> s;
    void SetUp() override
    {
        operation = GetParam().operation;
        ptvec = GetParam().initial_ptvec;
        s = GetParam().initial_stack;
    };
};

TEST_P(HandleOpTest, Int){
    operation->handleOp(ptvec, s);
    EXPECT_EQ(ptvec, GetParam().expected_ptvec);
    EXPECT_EQ(s, GetParam().expected_stack);
}

INSTANTIATE_TEST_CASE_P(Default, HandleOpTest,
        testing::Values(
                handleState{}
                )
        );


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}