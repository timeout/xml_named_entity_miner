#include "xml_element.hpp"
// #include "xml_doc.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <map>

TEST( ElementTest, Name ) {
    // xmlNode is created and destroyed outside of class
    xmlNode *n =
        xmlNewNode( NULL, reinterpret_cast<const unsigned char *>( "test-node" ) );

    XmlElement el{n};
    std::string currentName = el.name( );
    EXPECT_STREQ( "test-node", currentName.c_str( ) );

    std::string oldName = el.name( "changed-name" );
    EXPECT_STREQ( currentName.c_str( ), oldName.c_str( ) );
    currentName = el.name( );
    EXPECT_STREQ( "changed-name", currentName.c_str( ) );

    xmlFreeNode( n );
}

TEST( ElementTest, HasAttributes ) {
    xmlNode *n =
        xmlNewNode( NULL, reinterpret_cast<const unsigned char *>( "test-node" ) );

    XmlElement el{n};
    ASSERT_FALSE( el.hasAttributes( ) );

    xmlFreeNode( n );
}

TEST( ElementTest, Attribute ) {
    xmlNode *n =
        xmlNewNode( NULL, reinterpret_cast<const unsigned char *>( "test-node" ) );
    XmlElement el{n};
    ASSERT_FALSE( el.hasAttributes( ) );
    el.attribute( "id", "test-value" );
    ASSERT_TRUE( el.hasAttributes( ) );
    xmlFreeNode( n );
}

TEST( ElementTest, SetAttributes ) {
    xmlNode *n =
        xmlNewNode( NULL, reinterpret_cast<const unsigned char *>( "test-node" ) );
    XmlElement el{n};
    ASSERT_FALSE( el.hasAttributes( ) );
    std::map<std::string, std::string> attrs{{"class", "test-class"},
                                             {"test-attr", "test-value"}};
    el.attributes( attrs );
    ASSERT_TRUE( el.hasAttributes( ) );
    xmlFreeNode( n );
}

TEST( ElementTest, GetAttributes ) {
    xmlNode *n =
        xmlNewNode( NULL, reinterpret_cast<const unsigned char *>( "test-node" ) );
    XmlElement el{n};
    ASSERT_FALSE( el.hasAttributes( ) );
    std::map<std::string, std::string> attrs{{"class", "test-class"},
                                             {"test-attr", "test-value"}};
    el.attributes( attrs );
    ASSERT_TRUE( el.hasAttributes( ) );

    std::map<std::string, std::string> props = el.attributes( );
    ASSERT_TRUE( props == attrs );

    xmlFreeNode( n );
}

TEST( ElementTest, HasChild ) {
    xmlNode *n =
        xmlNewNode( NULL, reinterpret_cast<const unsigned char *>( "test-node" ) );
    XmlElement el{n};
    ASSERT_FALSE( el.hasChild( ) );
    xmlFreeNode( n );
}

TEST( ElementTest, SetChildName ) {
    xmlNode *n =
        xmlNewNode( NULL, reinterpret_cast<const unsigned char *>( "test-node" ) );
    XmlElement el{n};
    ASSERT_FALSE( el.hasChild( ) );
    XmlElement ch = el.child( "child-test" );
    ASSERT_TRUE( el.hasChild( ) );
    ASSERT_STREQ( "child-test", ch.name( ).c_str( ) );
    xmlFreeNode( n );
}

TEST( ElementTest, SetChild ) {
    xmlNode *n =
        xmlNewNode( NULL, reinterpret_cast<const unsigned char *>( "test-node" ) );
    XmlElement el{n};
    ASSERT_FALSE( el.hasChild( ) );
    std::map<std::string, std::string> attrs{{"class", "test-class"},
                                             {"test-attr", "test-value"}};
    XmlElement ch = el.child( "child-test", attrs );
    ASSERT_TRUE( el.hasChild( ) );
    ASSERT_STREQ( "child-test", ch.name( ).c_str( ) );
    ASSERT_TRUE( ch.hasAttributes( ) );
    std::map<std::string, std::string> props = ch.attributes( );
    ASSERT_TRUE( props == attrs );
    xmlFreeNode( n );
}

TEST( ElementTest, Child ) {
    xmlNode *n =
        xmlNewNode( NULL, reinterpret_cast<const unsigned char *>( "test-node" ) );
    XmlElement el{n};
    ASSERT_FALSE( el.hasChild( ) );
    XmlElement ch = el.child( "child-test" );
    ASSERT_TRUE( el.hasChild( ) );
    ASSERT_STREQ( "child-test", ch.name( ).c_str( ) );
    el = el.child( );
    ASSERT_STREQ( "child-test", el.name( ).c_str( ) );
    xmlFreeNode( n );
}

TEST( ElementTest, HasSiblings ) {
    xmlNode *n =
        xmlNewNode( NULL, reinterpret_cast<const unsigned char *>( "test-node" ) );
    XmlElement el{n};
    ASSERT_FALSE( el.hasChild( ) );
    XmlElement ch1 = el.child( "first-child-test" );
    XmlElement ch2 = el.child( "second-child-test" );
    ASSERT_TRUE( ch1.hasSibling( ) );
    xmlFreeNode( n );
}

TEST( ElementTest, SetSiblingName ) {
    xmlNode *n =
        xmlNewNode( NULL, reinterpret_cast<const unsigned char *>( "test-node" ) );
    XmlElement el{n};
    XmlElement ch = el.child( "child" );
    XmlElement sibling = ch.sibling( "sibling-test" );
    ASSERT_TRUE( ch.hasSibling( ) );
    xmlFreeNode( n ); // only free's child nodes
}

TEST( ElementTest, SetSibling ) {
    xmlNode *n =
        xmlNewNode( NULL, reinterpret_cast<const unsigned char *>( "test-node" ) );
    XmlElement el{n};
    XmlElement ch = el.child( "child" );
    std::map<std::string, std::string> attrs{{"class", "test-class"},
                                             {"test-attr", "test-value"}};
    XmlElement sibling = ch.sibling( "sibling-test", attrs );
    ASSERT_TRUE( ch.hasSibling( ) );
    ASSERT_EQ( attrs, sibling.attributes( ) );
    xmlFreeNode( n ); // only free's child nodes
}

TEST( ElementTest, HasContent ) {
    xmlNode *n =
        xmlNewNode( NULL, reinterpret_cast<const unsigned char *>( "test-node" ) );
    XmlElement el{n};
    XmlElement ch = el.child( "child" );
    std::map<std::string, std::string> attrs{{"class", "test-class"},
                                             {"test-attr", "test-value"}};
    XmlElement sibling = ch.sibling( "sibling-test", attrs );
    ASSERT_TRUE( ch.hasSibling( ) );
    ASSERT_EQ( attrs, sibling.attributes( ) );
    ASSERT_FALSE( el.hasContent( ) );
    xmlFreeNode( n ); // only free's child nodes
}

TEST( ElementTest, Content ) {
    xmlNode *n =
        xmlNewNode( NULL, reinterpret_cast<const unsigned char *>( "test-node" ) );
    XmlElement el{n};
    el.content( "This is some test content" );
    ASSERT_TRUE( el.hasContent( ) );
    std::cerr << el.content( ) << std::endl;
    xmlFreeNode( n ); // only free's child nodes
    n = nullptr;

    n = xmlNewNode( NULL, reinterpret_cast<const unsigned char *>( "test-node" ) );
    el = XmlElement{n};
    XmlElement ch1 = el.child( "child" );
    XmlElement sibling = el.child( "sibling" );
    XmlElement ch2 = sibling.child( "child-of-sibling" );
    ch2.content( "This is some more test content" );
    ch2.child( "child-of-child-of-sibling" );
    ASSERT_TRUE( el.hasContent( ) );
    std::cerr << el.content( ) << std::endl;
    xmlFreeNode( n ); // only free's child nodes
    n = nullptr;
}

TEST( ElementTest, AddContent ) {
    xmlNode *n =
        xmlNewNode( NULL, reinterpret_cast<const unsigned char *>( "test-node" ) );
    XmlElement el{n};
    el.content( "This is some test content" );
    ASSERT_TRUE( el.hasContent( ) );
    xmlFreeNode( n ); // only free's child nodes
    n = nullptr;

    n = xmlNewNode( NULL, reinterpret_cast<const unsigned char *>( "test-node" ) );
    el = XmlElement{n};
    XmlElement ch1 = el.child( "child" );
    XmlElement sibling = el.child( "sibling" );
    XmlElement ch2 = sibling.child( "child-of-child" );
    ch2.content( "This is some test content" );
    ASSERT_TRUE( el.hasContent( ) );
    xmlFreeNode( n ); // only free's child nodes
    n = nullptr;

    n = xmlNewNode( NULL, reinterpret_cast<const unsigned char *>( "test-node" ) );
    el = XmlElement{n};
    ch1 = el.child( "child" );
    sibling = el.child( "sibling" );
    ch2 = sibling.child( "child-of-child" );
    ASSERT_FALSE( el.hasContent( ) );
    xmlFreeNode( n ); // only free's child nodes
}
