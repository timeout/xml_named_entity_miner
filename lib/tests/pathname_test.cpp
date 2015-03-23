#include "../lib/pathname.hpp"
#include <stdexcept>

#include <gtest/gtest.h>
#include <limits.h>

TEST( PathnameTest, DefaultConstructor ) {
    const Pathname invalid;
    EXPECT_THROW( invalid.isAbsolute( ), std::runtime_error );
    EXPECT_THROW( invalid.basename( ), std::runtime_error );
    EXPECT_THROW( invalid.toString( ), std::runtime_error );
}

TEST( PathnameTest, DirBaseSplit ) {
    const Pathname pathname{"/"};
    EXPECT_STREQ( "/", pathname.basename( ).toString( ).c_str( ) );
    EXPECT_STREQ( "/", pathname.dirname( ).toString( ).c_str( ) );

    const Pathname pathname1{"foo"};
    EXPECT_STREQ( "foo", pathname1.basename( ).toString( ).c_str( ) );
    EXPECT_STREQ( "foo", pathname1.dirname( ).toString( ).c_str( ) );

    const Pathname pathname2{"/home/joe/programming"};
    EXPECT_STREQ( "programming", pathname2.basename( ).toString( ).c_str( ) );
    EXPECT_STREQ( "/home/joe", pathname2.dirname( ).toString( ).c_str( ) );

    const Pathname pathname3{"/home/joe/programming/"};
    EXPECT_STREQ( "programming", pathname3.basename( ).toString( ).c_str( ) );
    EXPECT_STREQ( "/home/joe", pathname3.dirname( ).toString( ).c_str( ) );
}

TEST( PathnameTest, AbsoluteOrRelative ) {
    const Pathname pathname{"/"};
    EXPECT_TRUE( pathname.isAbsolute( ) );
    EXPECT_FALSE( pathname.isRelative( ) );

    const Pathname pathname1{"foo"};
    EXPECT_TRUE( pathname1.isRelative( ) );

    const Pathname pathname2{"/home/joe/programming/"};
    EXPECT_TRUE( pathname2.isAbsolute( ) );

    const Pathname pathname3{"joe/programming/"};
    EXPECT_TRUE( pathname3.isRelative( ) );
}

TEST( PathnameTest, PathAddition ) {
    const Pathname root1{"/usr"};
    const Pathname rel{"include/"};
    Pathname test = root1 + rel;
    EXPECT_STREQ( "/usr/include", test.toString( ).c_str( ) );

    const Pathname root2{"/usr/include/linux"};
    test = root1 + root2;
    EXPECT_STREQ( "/usr/include/linux", test.toString( ).c_str( ) );

    const Pathname rel1{"linux/usb"};
    test = rel + rel1;
    EXPECT_STREQ( "include/linux/usb", test.toString( ).c_str( ) );

    const Pathname root{"/"};
    test = root + rel;
    EXPECT_STREQ( "/include", test.toString( ).c_str( ) );

    test = root + std::string{"bim/bam/baz"};
    EXPECT_STREQ( "/bim/bam/baz", test.toString( ).c_str( ) );

    test = std::string{"/usr/include"} + Pathname{"/home/joe"};
    EXPECT_STREQ( "/home/joe", test.toString( ).c_str( ) );
}

TEST( PathnameTest, Operations ) {
    const Pathname p1{"/usr/include/linux/usb/video.h"};
    Pathname p2{p1.dirname( ).dirname( )};
    EXPECT_STREQ( "/usr/include/linux", p2.toString( ).c_str( ) );
    EXPECT_STREQ( "linux", p2.basename( ).toString( ).c_str( ) );
    EXPECT_TRUE( Pathname{"/usr/include/linux"} == p2 );
    EXPECT_FALSE( p1 == p2 );
}
