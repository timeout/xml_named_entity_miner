#pragma once

#include "gtest/gtest.h"
#include "../lib/logger.hpp"
#include "../lib/bilanz.hpp"

class LoggingEnvironment : public ::testing::Environment {
public:
    auto SetUp( ) -> void {
    }

    auto TearDown( ) -> void {
    }
};
