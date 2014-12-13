#include "xml_doc.hpp"
#include "pathname.hpp"
#include "dir.hpp"

#include <stdexcept>
#include <regex>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

#include "gtest/gtest.h"
#include <limits.h>

static const Pathname buildDir_{Dir::getInstance( )->pwd( )};
const std::string books_xml{
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

const std::string menu_xml{
    R"(<?xml version="1.0"?>
<menu revision="2134" owner="Chief">
    <food id="1">
        <name>Belgian Waffles</name>
        <price discount="15" currency="USD">5.95</price>
        <description>two of our famous Belgian Waffles with plenty of real maple syrup</description>
        <calories>650</calories>
    </food>
    <food id="2">
        <name>Strawberry Belgian Waffles</name>
        <photo><![CDATA[data:image/jpg;base64,/9j/4AAQSkZJRgABAQEASA...]]></photo>
        <price currency="USD">7.95</price>
        <description>light Belgian waffles covered with strawberries and whipped cream</description>
        <calories variance="0.05">900</calories>
    </food>
    <food id="3">
        <name>Berry-Berry Belgian Waffles</name>
        <price currency="EUR">8.95</price>
        <description>light Belgian waffles covered with an assortment of fresh berries and whipped cream</description>
        <calories>900</calories>
    </food>
    <food id="4">
        <name>French Toast</name>
        <price currency="USD">4.50</price>
        <description>thick slices made from our homemade sourdough bread</description>
        <calories>600</calories>
    </food>
    <food id="5">
        <name>Homestyle Breakfast</name>
        <price currency="EUR">6.95</price>
        <description>two eggs, bacon or sausage, toast, and our ever-popular hash browns</description>
        <calories>950</calories>
        <customer>
            <name>Jimi Hendrix</name>
            <time>15:30</time>
            <billing>credit</billing>
        </customer>
    </food>
</menu>)"};

class XmlDocTests : public ::testing::Test {
protected:
    virtual auto SetUp( ) -> void { Dir::getInstance( )->chdir( buildDir_ ); }
    auto testable( const XmlDoc &xml ) const -> bool { return xml ? true : false; }
    auto cp_f( const XmlDoc doc ) -> XmlDoc { return doc; }
};

TEST_F( XmlDocTests, DefaultCtor ) {
    const XmlDoc xml;
    EXPECT_FALSE( xml.errorHandler( ).hasErrors( ) );
    EXPECT_STREQ( "", xml.toString( ).c_str( ) );
    ASSERT_EQ( false, testable( xml ) );
}

TEST_F( XmlDocTests, ValidPathnameConstructor ) {
    Dir::getInstance( )->chdir( Pathname{"tests/xmltest/valid/sa"} );
    std::regex glob{".*\\.xml"};
    auto ls = Dir::getInstance( )->read( ).entries( );
    std::vector<Pathname> xmlValidEntries{filter( ls, glob )};
    std::sort( xmlValidEntries.begin( ), xmlValidEntries.end( ) );

    for ( auto &p : xmlValidEntries ) {
        if ( p.basename( ) == Pathname{"012.xml"} )
            continue;
        if ( p.basename( ) == Pathname{"045.xml"} )
            continue;
        if ( p.basename( ) == Pathname{"095.xml"} )
            continue;
        XmlDoc xml;
        std::ifstream f;
        try {
            f = std::ifstream{p.toString( ), std::ios::in};
        } catch ( ... ) {
            std::cerr << "ERROR: could not open '" << p << "'" << std::endl;
            throw;
        }
        f >> xml;
        EXPECT_FALSE( xml.errorHandler( ).hasErrors( ) );
        if ( xml.errorHandler( ).hasErrors( ) ) {
            std::cerr << p << std::endl;
            std::cerr << xml.errorHandler( ) << std::endl;
        }
        ASSERT_EQ( true, testable( xml ) );
    }
}

TEST_F( XmlDocTests, NotWellFormedXml ) {
    Dir::getInstance( )->chdir( Pathname{"tests/xmltest/not-wf/sa"} );
    std::regex glob{".*\\.xml"};
    auto ls = Dir::getInstance( )->read( ).entries( );
    std::vector<Pathname> xmlNotWFEntries{filter( ls, glob )};
    std::sort( xmlNotWFEntries.begin( ), xmlNotWFEntries.end( ) );
    for ( auto &p : xmlNotWFEntries ) {
        if ( p.basename( ) == Pathname{"050.xml"} )
            continue;
        if ( p.basename( ) == Pathname{"140.xml"} )
            continue;
        if ( p.basename( ) == Pathname{"141.xml"} )
            continue;
        XmlDoc xml;
        std::ifstream f;
        try {
            f = std::ifstream{p.toString( ), std::ios::in};
        } catch ( ... ) {
            std::cerr << "ERROR: could not open '" << p << "'" << std::endl;
            throw;
        }
        f >> xml;
        EXPECT_TRUE( xml.errorHandler( ).hasErrors( ) );
        if ( !xml.errorHandler( ).hasErrors( ) ) {
            std::cerr << p << std::endl;
        }
    }
}

TEST_F( XmlDocTests, Ctor ) {
    std::istringstream is{books_xml};
    XmlDoc xml{is};
    ASSERT_EQ( true, testable( xml ) );
    EXPECT_FALSE( xml.errorHandler( ).hasErrors( ) );
}

TEST_F( XmlDocTests, CopyCtor ) {
    // copy a null doc
    XmlDoc nullDoc;
    ASSERT_EQ( false, testable( nullDoc ) );

    XmlDoc cpNullDoc( nullDoc );
    ASSERT_EQ( false, testable( cpNullDoc ) );
    EXPECT_FALSE( cpNullDoc.errorHandler( ).hasErrors( ) );

    // copy xml
    std::istringstream booksIs{books_xml};
    XmlDoc booksDoc{booksIs};
    ASSERT_EQ( true, testable( booksDoc ) );

    XmlDoc cpBooksDoc{booksDoc};
    ASSERT_EQ( true, testable( cpBooksDoc ) );
    EXPECT_FALSE( cpBooksDoc.errorHandler( ).hasErrors( ) );
}

TEST_F( XmlDocTests, CopyAssignment ) {
    // null doc
    XmlDoc RHSNullDoc;
    ASSERT_EQ( false, testable( RHSNullDoc ) );

    XmlDoc LHSNullDoc;
    ASSERT_EQ( false, testable( LHSNullDoc ) );

    // assign null to null
    RHSNullDoc = LHSNullDoc;
    ASSERT_EQ( false, testable( RHSNullDoc ) );
    EXPECT_FALSE( RHSNullDoc.errorHandler( ).hasErrors( ) );

    // xml
    std::istringstream booksIs{books_xml};
    XmlDoc booksDoc{booksIs};
    ASSERT_EQ( true, testable( booksDoc ) );

    std::istringstream menuIs{menu_xml};
    XmlDoc menuDoc{menuIs};
    ASSERT_EQ( true, testable( menuDoc ) );

    booksDoc = menuDoc;
    ASSERT_EQ( true, testable( booksDoc ) );
    EXPECT_FALSE( booksDoc.errorHandler( ).hasErrors( ) );

    // assign xml to null
    RHSNullDoc = booksDoc;
    ASSERT_EQ( true, testable( RHSNullDoc ) );
    EXPECT_FALSE( RHSNullDoc.errorHandler( ).hasErrors( ) );

    // assign null to xml
    menuDoc = LHSNullDoc;
    ASSERT_EQ( false, testable( menuDoc ) );
    EXPECT_FALSE( menuDoc.errorHandler( ).hasErrors( ) );
}

TEST_F( XmlDocTests, CopyMoveCtor ) {
    XmlDoc nullDoc;
    ASSERT_EQ( false, testable( nullDoc ) );

    XmlDoc cpMvNullDoc{cp_f( nullDoc )};
    ASSERT_EQ( false, testable( cpMvNullDoc ) );

    std::istringstream menuIs{menu_xml};
    XmlDoc menuDoc{menuIs};
    ASSERT_EQ( true, testable( menuDoc ) );

    XmlDoc cpMvMenuDoc{cp_f( menuDoc )};
    ASSERT_EQ( true, testable( menuDoc ) );
    EXPECT_FALSE( cpMvMenuDoc.errorHandler( ).hasErrors( ) );
}

TEST_F( XmlDocTests, CopyMoveAssignment ) {
    // null doc
    XmlDoc RHSNullDoc;
    ASSERT_EQ( false, testable( RHSNullDoc ) );

    XmlDoc LHSNullDoc;
    ASSERT_EQ( false, testable( LHSNullDoc ) );

    // assign null to null
    RHSNullDoc = cp_f( LHSNullDoc );
    ASSERT_EQ( false, testable( RHSNullDoc ) );
    EXPECT_FALSE( RHSNullDoc.errorHandler( ).hasErrors( ) );

    // xml
    std::istringstream booksIs{books_xml};
    XmlDoc booksDoc{booksIs};
    ASSERT_EQ( true, testable( booksDoc ) );

    std::istringstream menuIs{menu_xml};
    XmlDoc menuDoc{menuIs};
    ASSERT_EQ( true, testable( menuDoc ) );

    booksDoc = cp_f( menuDoc );
    ASSERT_EQ( true, testable( booksDoc ) );
    EXPECT_FALSE( booksDoc.errorHandler( ).hasErrors( ) );

    // assign xml to null
    RHSNullDoc = cp_f( booksDoc );
    ASSERT_EQ( true, testable( RHSNullDoc ) );
    EXPECT_FALSE( RHSNullDoc.errorHandler( ).hasErrors( ) );

    // assign null to xml
    menuDoc = cp_f( LHSNullDoc );
    ASSERT_EQ( false, testable( menuDoc ) );
    EXPECT_FALSE( menuDoc.errorHandler( ).hasErrors( ) );
}

TEST_F( XmlDocTests, ToString ) {
    std::istringstream menuIs{menu_xml};
    XmlDoc menuDoc{menuIs};
    ASSERT_EQ( true, testable( menuDoc ) );
    EXPECT_STREQ( menu_xml.c_str( ), menuDoc.toString( ).c_str( ) );

    XmlDoc cpMenuDoc{menuDoc};
    EXPECT_STREQ( cpMenuDoc.toString( ).c_str( ), menuDoc.toString( ).c_str( ) );

    std::istringstream booksIs{books_xml};
    XmlDoc booksDoc{booksIs};
    EXPECT_STRNE( booksDoc.toString( ).c_str( ), menuDoc.toString( ).c_str( ) );
}
