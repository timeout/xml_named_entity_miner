#include "synonyms.hpp"
#include <QString>
#include <QList>
#include "gtest/gtest.h"
#include <limits.h>

#include <QDebug>

TEST( SynonymsTest, DictionaryCtor ) {
    Dictionary dict;
    ASSERT_EQ( 0, dict.size( ) );
}

TEST( SynonymsTest, InsertTest ) {
    Dictionary dict;
    dict.insert( "first word" );
    dict.insert( "second word" );
    dict.insert( "third word" );
    ASSERT_EQ( 3, dict.size( ) );
    dict.insert( "third word" );
    ASSERT_EQ( 3, dict.size( ) );
}

TEST( SynonymsTest, RemoveTest ) {
    Dictionary dict;
    dict.insert( "first word" );
    dict.insert( "second word" );
    dict.insert( "third word" );
    dict.remove( "no entry" );
    ASSERT_EQ( 3, dict.size( ) );
    dict.remove( "third word" );
    ASSERT_EQ( 2, dict.size( ) );
}

TEST( SynonymsTest, CountTest ) {
    Dictionary dict;
    ASSERT_EQ( 0, dict.count( "no entry" ) );
    dict.insert( "first word" );
    ASSERT_EQ( 1, dict.count( "first word" ) );
    dict.insert( "first word" );
    dict.insert( "first word" );
    dict.insert( "first word" );
    ASSERT_EQ( 4, dict.count( "first word" ) );
}

TEST( SynonymsTest, ExistsTest ) {
    Dictionary dict;
    ASSERT_FALSE( dict.exists( "no entry" ) );
    dict.insert( "first word" );
    ASSERT_TRUE( dict.exists( "first word" ) );
    dict.remove( "first word" );
    ASSERT_FALSE( dict.exists( "first word" ) );
}

TEST( SynonymsTest, Thesaurus ) {
    Thesaurus thes;
    thes.insert( QString{"equal"}, QString{"identical"} );
    thes.insert( QString{"equivalent"}, QString{"identical"} );
    ASSERT_STREQ( "identical", thes.canonical( "equivalent" ).toStdString( ).c_str( ) );
    ASSERT_STREQ( "identical", thes.canonical( "equal" ).toStdString( ).c_str( ) );
    ASSERT_STREQ( "", thes.canonical( "no entry" ).toStdString( ).c_str( ) );

    auto synonyms = thes.synonyms( thes.canonical( "equal" ) );
    QList<QString> control;
    control << "equivalent"
            << "equal";
    ASSERT_EQ( control, synonyms );

    // thes has identical => equivalent, identical => equal
    thes.remove( "equivalent" );
    synonyms = thes.synonyms( thes.canonical( "equal" ) );
    const QList<QString> control2 = {"equal"};
    ASSERT_EQ( control2, synonyms );
}

