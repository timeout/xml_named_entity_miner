#pragma once
#include "gtest/gtest.h"


class GlobalEnvironment : public testing::Environment {
public:
    auto SetUp( ) -> void override {
    }

    auto TearDown( ) -> void override {
    }

private:
};
