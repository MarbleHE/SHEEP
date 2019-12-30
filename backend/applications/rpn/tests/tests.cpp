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



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}