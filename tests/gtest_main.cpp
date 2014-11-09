#include "gtest/gtest.h"
#include <glog/logging.h>

int main( int argc, char *argv[] ) {
    testing::InitGoogleTest( &argc, argv );
    google::InitGoogleLogging(argv[0]);
    // testing::AddGlobalTestEnvironment( new LoggingEnvironment );
#define LOGGING_LEVEL_1
    LOG(INFO) << "in main";
    return RUN_ALL_TESTS( );
}
