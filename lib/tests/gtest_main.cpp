#include "gtest/gtest.h"
// #include "global_env.hpp"

int main( int argc, char *argv[] ) {
    testing::InitGoogleTest( &argc, argv );
    // google::InitGoogleLogging(argv[0]);
    // testing::AddGlobalTestEnvironment( new GlobalEnvironment );
    // LOG(INFO) << "in main";
    return RUN_ALL_TESTS( );
}
