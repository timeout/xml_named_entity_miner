#include "gtest/gtest.h"
#include <libxml/tree.h>
#include "lib/xml_element.hpp"
#include <iostream>
#include <map>
#include <vector>

TEST( XmlElementTest, Ctor ) {
    xmlNode *node =
        xmlNewNode( nullptr, reinterpret_cast<const unsigned char *>( "test-node" ) );
    XmlElement element{node};
    ASSERT_STREQ( "test-node", element.name( ).c_str( ) );
    std::cerr << element << std::endl;
}

TEST( XmlElementTest, Ctor2 ) {
    XmlElement element{"test-node"};
    ASSERT_STREQ( "test-node", element.name( ).c_str( ) );
}

TEST( XmlElementTest, CopyCtor ) {
    XmlElement element{"test-node"};
    XmlElement copy{element};
    element.name( "element" );
    ASSERT_TRUE( std::string{"element"} == copy.name( ) );
    ASSERT_TRUE( std::string{"element"} == element.name( ) );
    std::cerr << copy << std::endl;
    std::cerr << element << std::endl;
}

TEST( XmlElementTest, CopyAssignment ) {
    XmlElement first{"first"};
    XmlElement second{"second"};
    second = first;
    ASSERT_STREQ( "first", second.name( ).c_str( ) );
    ASSERT_TRUE( first == second );
}

TEST( XmlElementTest, SetName ) {
    XmlElement element{"test-node"};
    ASSERT_STREQ( "test-node", element.name( ).c_str( ) );
    element.name( "renamed" );
    ASSERT_STREQ( "renamed", element.name( ).c_str( ) );
}

TEST( XmlElementTest, HasntAttribute ) {
    XmlElement element{"test-node"};
    ASSERT_FALSE( element.hasAttributes( ) );
}

TEST( XmlElementTest, HasAttribute ) {
    XmlElement element{"test-node"};
    element.attribute( "id", "42" );
    ASSERT_TRUE( element.hasAttributes( ) );
}

TEST( XmlElementTest, RemoveAttribute ) {
    XmlElement element{"test-node"};
    element.attribute( "id", "42" );
    ASSERT_TRUE( element.hasAttributes( ) );
    element.removeAttribute( "id" );
    ASSERT_FALSE( element.hasAttributes( ) );
}

TEST( XmlElementTest, GetAttributeMap ) {
    XmlElement element{"test-node"};
    element.attribute( "id", "42" );
    element.attribute( "class", "answer" );
    element.attribute( "author", "Douglas Adams" );
    auto attrs = element.attributes( );
    auto val = attrs.find( "class" );
    ASSERT_TRUE( val->second == std::string{"answer"} );
    val = attrs.find( "author" );
    ASSERT_TRUE( val->second == std::string{"Douglas Adams"} );
}

TEST( XmlElementTest, SetAttributeMap ) {
    XmlElement element{"test-node"};
    element.attributes(
        {{"id", "42"}, {"class", "answer"}, {"question", "meaning of life"}} );
    auto attrs = element.attributes( );
    auto val = attrs.find( "id" );
    ASSERT_TRUE( std::string{"42"} == val->second );
    val = attrs.find( "class" );
    ASSERT_TRUE( std::string{"answer"} == val->second );
    val = attrs.find( "question" );
    ASSERT_TRUE( std::string{"meaning of life"} == val->second );
    std::cerr << element << std::endl;
}

TEST( XmlElementTest, RemoveMoreAttribute ) {
    XmlElement element{"test-node"};
    element.attributes( {{"id", "42"}, {"class", "answer"}, {"person", "author"}} );
    ASSERT_TRUE( element.hasAttributes( ) );
    element.removeAttribute( "person" );
    element.removeAttribute( "id" );
    ASSERT_TRUE( element.hasAttributes( ) );
    ASSERT_STREQ( "answer", element.attribute( "class" ).c_str( ) );
    std::cerr << element << std::endl;
}

TEST( XmlElementTest, HasntContent ) {
    XmlElement element{"test-node"};
    ASSERT_STREQ( "test-node", element.name( ).c_str( ) );
    ASSERT_FALSE( element.hasContent( ) );
}

TEST( XmlElementTest, HasContent ) {
    XmlElement element{"test-node"};
    element.content(
        "Once you do know what the question actually is, you'll know what the answer "
        "means..." );
    ASSERT_TRUE( element.hasContent( ) );
}

TEST( XmlElementTest, GetNonRecursiveContent ) {
    XmlElement element{"test-node"};
    element.content(
        "Once you do know what the question actually is, you'll know what the answer "
        "means..." );
    ASSERT_TRUE( element.hasContent( ) );
    std::cerr << element.content( ) << std::endl;
}

TEST( XmlElementTest, HasntChild ) {
    XmlElement element{"test-node"};
    ASSERT_FALSE( element.hasChild( ) );
}

TEST( XmlElementTest, HasChild ) {
    XmlElement element{"test-node"};
    XmlElement child{"child-node"};
    element.child( child );
    ASSERT_TRUE( element.hasChild( ) );
}

TEST( XmlElementTest, AddChild ) {
    XmlElement element{"root"};
    element.child( XmlElement{"child"} );
    std::cerr << element << std::endl;
    ASSERT_TRUE( std::string{"root"} == element.name( ) );
    auto child = element.child( );
    ASSERT_TRUE( std::string{"child"} == child.name( ) );
}

TEST( XmlElementTest, AddSibling ) {
    XmlElement element{"root"};
    XmlElement child{"child"};
    XmlElement sibling{"sibling"};
    element.child( child );
    child.sibling( sibling );
    auto next = child.sibling( );
    std::cerr << element << std::endl;
    std::cerr << sibling << std::endl;
    std::cerr << next << std::endl;
    ASSERT_TRUE( std::string{"sibling"} == next.name( ) );
}

TEST( XmlElementTest, SwapTest1 ) {
    XmlElement element{"original"};
    XmlElement swapee{"swapped"};
    element.swap( swapee );
    ASSERT_TRUE( std::string{"swapped"} == element.name( ) );
}

TEST( XmlElementTest, ConstructElement ) {
    // The Hitchhiker's Guide to the Galaxy was a concept for a science-fiction comedy
    // radio
    // series pitched by Adams and radio producer Simon Brett to BBC Radio 4 in 1977.

    XmlElement element{"section"};
    element.content(
        "The Hitchiker's Guide to the Galaxy was a concept for a science-fiction comedy "
        "raido series pitched by " );

    XmlElement name{"named-entity"};
    name.attribute( "type", "person" );
    name.content( "Adams" );

    element.child( name );
    element.content( " and radio produced " );

    XmlElement name1{"named-entity"};
    name1.attribute( "type", "person" );
    name1.content( "Simon Brett" );

    element.child( name1 );
    element.content( " to BBC Radio 4 in 1977." );

    // std::cerr << element.toString( ) << std::endl;
    if ( element.hasChild( ) ) {
        auto ch = element.child( );
        while ( ch ) {
            if ( ch.content( ) == std::string{"Adams"} ) {
                ch.name( "ENTITY" );
                ch.attribute( "key", "value" );
            }
            ch = ch.sibling( );
        }
    }
    std::cerr << element.toString( ) << std::endl;
}

