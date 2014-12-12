#include "xpath_query.hpp"
#include "xpath_ctxt.hpp"
#include "xml_doc.hpp"

#include <fstream>

#include "gtest/gtest.h"
#include <limits.h>

const std::string testDoc{
    R"(<?xml version="1.0"?>
<catalog>
   <book id="bk101">
      <author>Gambardella, Matthew</author>
      <title>XML Developer's Guide</title>
      <genre>Computer</genre>
      <price>44.95</price>
      <publish_date>2000-10-01</publish_date>
      <description>An in-depth look at creating applications 
      with XML.</description>
   </book>
   <book id="bk102">
      <author>Ralls, Kim</author>
      <title>Midnight Rain</title>
      <genre>Fantasy</genre>
      <price>5.95</price>
      <publish_date>2000-12-16</publish_date>
      <description>A former architect battles corporate zombies, 
      an evil sorceress, and her own childhood to become queen 
      of the world.</description>
   </book>
   <book id="bk103">
      <author>Corets, Eva</author>
      <title>Maeve Ascendant</title>
      <genre>Fantasy</genre>
      <price>5.95</price>
      <publish_date>2000-11-17</publish_date>
      <description>After the collapse of a nanotechnology 
      society in England, the young survivors lay the 
      foundation for a new society.</description>
   </book>
   <book id="bk104">
      <author>Corets, Eva</author>
      <title>Oberon's Legacy</title>
      <genre>Fantasy</genre>
      <price>5.95</price>
      <publish_date>2001-03-10</publish_date>
      <description>In post-apocalypse England, the mysterious 
      agent known only as Oberon helps to create a new life 
      for the inhabitants of London. Sequel to Maeve 
      Ascendant.</description>
   </book>
   <book id="bk105">
      <author>Corets, Eva</author>
      <title>The Sundered Grail</title>
      <genre>Fantasy</genre>
      <price>5.95</price>
      <publish_date>2001-09-10</publish_date>
      <description>The two daughters of Maeve, half-sisters, 
      battle one another for control of England. Sequel to 
      Oberon's Legacy.</description>
   </book>
   <book id="bk106">
      <author>Randall, Cynthia</author>
      <title>Lover Birds</title>
      <genre>Romance</genre>
      <price>4.95</price>
      <publish_date>2000-09-02</publish_date>
      <description>When Carla meets Paul at an ornithology 
      conference, tempers fly as feathers get ruffled.</description>
   </book>
   <book id="bk107">
      <author>Thurman, Paula</author>
      <title>Splish Splash</title>
      <genre>Romance</genre>
      <price>4.95</price>
      <publish_date>2000-11-02</publish_date>
      <description>A deep sea diver finds true love twenty 
      thousand leagues beneath the sea.</description>
   </book>
   <book id="bk108">
      <author>Knorr, Stefan</author>
      <title>Creepy Crawlies</title>
      <genre>Horror</genre>
      <price>4.95</price>
      <publish_date>2000-12-06</publish_date>
      <description>An anthology of horror stories about roaches,
      centipedes, scorpions  and other insects.</description>
   </book>
   <book id="bk109">
      <author>Kress, Peter</author>
      <title>Paradox Lost</title>
      <genre>Science Fiction</genre>
      <price>6.95</price>
      <publish_date>2000-11-02</publish_date>
      <description>After an inadvertant trip through a Heisenberg
      Uncertainty Device, James Salway discovers the problems 
      of being quantum.</description>
   </book>
   <book id="bk110">
      <author>O'Brien, Tim</author>
      <title>Microsoft .NET: The Programming Bible</title>
      <genre>Computer</genre>
      <price>36.95</price>
      <publish_date>2000-12-09</publish_date>
      <description>Microsoft's .NET initiative is explored in 
      detail in this deep programmer's reference.</description>
   </book>
   <book id="bk111">
      <author>O'Brien, Tim</author>
      <title>MSXML3: A Comprehensive Guide</title>
      <genre>Computer</genre>
      <price>36.95</price>
      <publish_date>2000-12-01</publish_date>
      <description>The Microsoft MSXML3 parser is covered in 
      detail, with attention to XML DOM interfaces, XSLT processing, 
      SAX and more.</description>
   </book>
   <book id="bk112">
      <author>Galos, Mike</author>
      <title>Visual Studio 7: A Comprehensive Guide</title>
      <genre>Computer</genre>
      <price>49.95</price>
      <publish_date>2001-04-16</publish_date>
      <description>Microsoft Visual Studio 7 is explored in depth,
      looking at how Visual Basic, Visual C++, C#, and ASP+ are 
      integrated into a comprehensive development 
      environment.</description>
   </book>
</catalog>)"};

class XPathQueryTest : public testing::Test {
protected:
    auto testable( const XPathQuery &query ) const -> bool {
        return query ? true : false;
    }
    auto xpc( const Pathname &path ) -> void {
        std::ifstream f{path.toString( ), std::ios::in};
        if ( !f.is_open( ) ) {
            std::cerr << "couldn't open: " << path.toString( ) << std::endl;
            ASSERT_FALSE( true );
        }
        XmlDoc xml{f};
        if ( !xml ) {
            std::cerr << "xml empty" << std::endl;
            ASSERT_FALSE( true );
        }
        if ( xml.errorHandler( ).hasErrors( ) ) {
            std::cerr << xml.errorHandler( ) << std::endl;
            ASSERT_FALSE( true ); // burn baby, burn
        }
        xml >> xpc_;
        if ( !xpc_ ) {
            std::cerr << "xpc empty" << std::endl;
            ASSERT_FALSE( true );
        }
    }
    auto xpc( ) const -> const XPathCtxt & { return xpc_; }
    auto f( XPathQuery query ) -> XPathQuery { return query; }

private:
    XPathCtxt xpc_;
};

TEST_F( XPathQueryTest, DefaultCtor ) {
    XPathQuery query;
    ASSERT_EQ( false, testable( query ) );
}

TEST_F( XPathQueryTest, Ctor ) {
    xpc( Pathname{"tests/xmltest/valid/sa/001.xml"} );
    XPathCtxt xp{xpc( )};
    XPathQuery query1;
    xp >> query1;
    ASSERT_EQ( false, testable( query1 ) );

    XPathQuery query2( xp );
    ASSERT_EQ( false, testable( query2 ) );
}

TEST_F( XPathQueryTest, CopyCtor ) {
    XPathQuery query;
    XPathQuery queryCp{query};
    ASSERT_EQ( false, testable( query ) );

    xpc( Pathname{"tests/xmltest/valid/sa/001.xml"} );
    XPathCtxt xp1{xpc( )};
    XPathQuery query1;
    xp1 >> query1;

    XPathQuery query2{query1};
    ASSERT_EQ( false, testable( query2 ) );

    std::istringstream is{testDoc};
    XmlDoc xml{is};
    XPathCtxt ctxt{xml};
    XPathQuery q{ctxt};
    q.query("//author");
    ASSERT_EQ( true, testable( q ) );

    XPathQuery cpy{q};
    ASSERT_EQ( true, testable( cpy ) );
}

TEST_F( XPathQueryTest, CopyMoveCtor ) {
    XPathQuery query;
    XPathQuery queryCp{f( query )};
    ASSERT_EQ( false, testable( queryCp ) );

    xpc( Pathname{"tests/xmltest/valid/sa/001.xml"} );
    XPathQuery query1;
    xpc( ) >> query1;
    XPathQuery query2{f( query1 )};
    ASSERT_EQ( false, testable( query2 ) );
    query2.query( "/element/leaf[1]" );
    ASSERT_EQ( true, testable( query2 ) );
    ASSERT_EQ( false, testable( query1 ) );
}

TEST_F( XPathQueryTest, CopyAssignment ) {
    xpc( Pathname{"tests/xmltest/valid/sa/001.xml"} );
    XPathQuery query1;
    xpc( ) >> query1;

    xpc( Pathname{"tests/xmltest/valid/sa/008.xml"} );
    XPathCtxt xp2{xpc( )};
    XPathQuery query2{xp2};

    query1 = query2;
    ASSERT_EQ( false, testable( query1 ) );
    query1.query( "/element/leaf[1]" );
    ASSERT_EQ( true, testable( query1 ) );
}

TEST_F( XPathQueryTest, CopyMoveAssignment ) {
    xpc( Pathname{"tests/xmltest/valid/sa/001.xml"} );
    XPathQuery query;
    xpc( ) >> query;
    query.query( "/element/sub" );

    xpc( Pathname{"tests/xmltest/valid/sa/006.xml"} );
    XPathQuery ass{xpc( )};
    ASSERT_EQ( false, testable( ass ) );
    ass = f( query );
    ASSERT_EQ( true, testable( ass ) );
}

TEST_F( XPathQueryTest, query ) {
    std::istringstream is{testDoc};
    XmlDoc testXml{is};
    XPathCtxt testCtxt{testXml};
    XPathQuery query{testCtxt};
    query.query( "//author" );
    ASSERT_EQ( true, testable( query ) );
}

