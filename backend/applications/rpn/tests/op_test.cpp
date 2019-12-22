//
// Created by mario on 22.12.19.
//
#include "gtest/gtest.h"

#include "op.hpp"
#include "intop.hpp"

TEST(Basic, OneIsOne){
    auto intop = IntOp(1);
    ASSERT_EQ(intop.i, 1);
}