#include "pathname.hpp"
#include "dir.hpp"

#include <stdexcept>
#include <algorithm>

#include "gtest/gtest.h"
#include <limits.h>

const Pathname buildDir_ = Dir::getInstance()->pwd();

class DirTests : public ::testing::Test {
protected:
    virtual auto SetUp( ) -> void {
        Dir::getInstance( )->chdir( buildDir_ );
    }
};

TEST_F( DirTests, Exists ) {
    Dir *dir = Dir::getInstance( );
    // relative path
    EXPECT_TRUE( dir->exists( Pathname{"tests"} ) );
    // absolute path
    EXPECT_TRUE( dir->exists( Pathname{"/usr"} ) );
    EXPECT_FALSE( dir->exists( Pathname{"tests/wrong/path"} ) );
    EXPECT_TRUE( dir->exists( Pathname{"."}.dirname( ).dirname( ) ) );
    // file
    EXPECT_FALSE( dir->exists( Pathname{"./dir_test.cpp"} ) );
    Dir::killInstance( );
}

/*
 * Fixture: directory structure:
 * dummy_director/
 *    sub_directory_1/
 *    sub_directory_2/
 *    file_1
 *    file_2
 *    file_3
 */
TEST_F( DirTests, ChangeDir ) {
    const Pathname dd{"tests/dummy_directory"};
    EXPECT_TRUE( Dir::getInstance( )->exists( dd ) );

    const Pathname startPath{Dir::getInstance( )->pwd( )};
    Dir::getInstance( )->chdir( dd );
    Pathname dest = Dir::getInstance( )->pwd( );
    EXPECT_STREQ( ( startPath + dd ).toString( ).c_str( ), dest.toString( ).c_str( ) );

    EXPECT_STREQ(
        "/", Dir::getInstance( )->chdir( Pathname{"/"} ).pwd( ).toString( ).c_str( ) );

    EXPECT_THROW( Dir::getInstance( )->chdir( Pathname{"road/to/nowhere"} ),
                  std::runtime_error );
    EXPECT_TRUE( Dir::getInstance( )->exists( startPath ) );

    Dir::killInstance( );
}

TEST_F( DirTests, Listings ) {
    // read current directory and get entries
    auto ls = Dir::getInstance( )
                  ->chdir( Pathname{"."} + Pathname{"tests/dummy_directory"} )
                  .read( )
                  .entries( );

    //     std::sort( ls.begin( ), ls.end( ) );
    //     for(auto pathname : ls ) {
    //         std::cerr << pathname << std::endl;
    //     }

    // construct pathname of element
    const Pathname startPath{Dir::getInstance( )->pwd( )};
    const Pathname file_1{startPath + Pathname{"file_1"}};
    const Pathname file_2{startPath + Pathname{"file_2"}};

    EXPECT_TRUE( std::find( std::begin( ls ), std::end( ls ), file_1 ) !=
                 std::end( ls ) );
    EXPECT_TRUE( std::find( std::begin( ls ), std::end( ls ), file_2 ) !=
                 std::end( ls ) );
    EXPECT_FALSE( std::find( std::begin( ls ), std::end( ls ), Pathname{"/usr/"} ) !=
                  std::end( ls ) );
    Dir::killInstance( );
}

