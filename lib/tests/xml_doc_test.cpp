#include "lib/xml_doc.hpp"
#include "lib/xml_element.hpp"

#include <stdexcept>
#include <regex>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

#include "gtest/gtest.h"
#include <limits.h>

#include "test_helper.hpp"

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


TEST( XmlDocTests, DefaultCtor ) {
    const XmlDoc xml;
    EXPECT_FALSE( xml.errorHandler( ).hasErrors( ) );
    EXPECT_STREQ( "", xml.toString( ).c_str( ) );
    ASSERT_EQ( false, testBool_f<XmlDoc>( xml ) );
}


TEST( XmlDocTests, Ctor ) {
    std::istringstream is{books_xml};
    XmlDoc xml{is};
    ASSERT_EQ( true, testBool_f<XmlDoc>( xml ) );
    EXPECT_FALSE( xml.errorHandler( ).hasErrors( ) );
}

TEST( XmlDocTests, CopyCtor ) {
    // copy a null doc
    XmlDoc nullDoc;
    ASSERT_EQ( false, testBool_f<XmlDoc>( nullDoc ) );

    XmlDoc cpNullDoc( nullDoc );
    ASSERT_EQ( false, testBool_f<XmlDoc>( cpNullDoc ) );
    EXPECT_FALSE( cpNullDoc.errorHandler( ).hasErrors( ) );

    // copy xml
    std::istringstream booksIs{books_xml};
    XmlDoc booksDoc{booksIs};
    ASSERT_EQ( true, testBool_f<XmlDoc>( booksDoc ) );

    XmlDoc cpBooksDoc{booksDoc};
    ASSERT_EQ( true, testBool_f<XmlDoc>( cpBooksDoc ) );
    EXPECT_FALSE( cpBooksDoc.errorHandler( ).hasErrors( ) );
}

TEST( XmlDocTests, CopyAssignment ) {
    // null doc
    XmlDoc RHSNullDoc;
    ASSERT_EQ( false, testBool_f<XmlDoc>( RHSNullDoc ) );

    XmlDoc LHSNullDoc;
    ASSERT_EQ( false, testBool_f<XmlDoc>( LHSNullDoc ) );

    // assign null to null
    RHSNullDoc = LHSNullDoc;
    ASSERT_EQ( false, testBool_f<XmlDoc>( RHSNullDoc ) );
    EXPECT_FALSE( RHSNullDoc.errorHandler( ).hasErrors( ) );

    // xml
    std::istringstream booksIs{books_xml};
    XmlDoc booksDoc{booksIs};
    ASSERT_EQ( true, testBool_f<XmlDoc>( booksDoc ) );

    std::istringstream menuIs{menu_xml};
    XmlDoc menuDoc{menuIs};
    ASSERT_EQ( true, testBool_f<XmlDoc>( menuDoc ) );

    booksDoc = menuDoc;
    ASSERT_EQ( true, testBool_f<XmlDoc>( booksDoc ) );
    EXPECT_FALSE( booksDoc.errorHandler( ).hasErrors( ) );

    // assign xml to null
    RHSNullDoc = booksDoc;
    ASSERT_EQ( true, testBool_f<XmlDoc>( RHSNullDoc ) );
    EXPECT_FALSE( RHSNullDoc.errorHandler( ).hasErrors( ) );

    // assign null to xml
    menuDoc = LHSNullDoc;
    ASSERT_EQ( false, testBool_f<XmlDoc>( menuDoc ) );
    EXPECT_FALSE( menuDoc.errorHandler( ).hasErrors( ) );
}

TEST( XmlDocTests, CopyMoveCtor ) {
    XmlDoc nullDoc;
    ASSERT_EQ( false, testBool_f<XmlDoc>( nullDoc ) );

    XmlDoc cpMvNullDoc{cp_f<XmlDoc>( nullDoc )};
    ASSERT_EQ( false, testBool_f<XmlDoc>( cpMvNullDoc ) );

    std::istringstream menuIs{menu_xml};
    XmlDoc menuDoc{menuIs};
    ASSERT_EQ( true, testBool_f<XmlDoc>( menuDoc ) );

    XmlDoc cpMvMenuDoc{cp_f<XmlDoc>( menuDoc )};
    ASSERT_EQ( true, testBool_f<XmlDoc>( menuDoc ) );
    EXPECT_FALSE( cpMvMenuDoc.errorHandler( ).hasErrors( ) );
}

TEST( XmlDocTests, CopyMoveAssignment ) {
    // null doc
    XmlDoc RHSNullDoc;
    ASSERT_EQ( false, testBool_f<XmlDoc>( RHSNullDoc ) );

    XmlDoc LHSNullDoc;
    ASSERT_EQ( false, testBool_f<XmlDoc>( LHSNullDoc ) );

    // assign null to null
    RHSNullDoc = cp_f<XmlDoc>( LHSNullDoc );
    ASSERT_EQ( false, testBool_f<XmlDoc>( RHSNullDoc ) );
    EXPECT_FALSE( RHSNullDoc.errorHandler( ).hasErrors( ) );

    // xml
    std::istringstream booksIs{books_xml};
    XmlDoc booksDoc{booksIs};
    ASSERT_EQ( true, testBool_f<XmlDoc>( booksDoc ) );

    std::istringstream menuIs{menu_xml};
    XmlDoc menuDoc{menuIs};
    ASSERT_EQ( true, testBool_f<XmlDoc>( menuDoc ) );

    booksDoc = cp_f<XmlDoc>( menuDoc );
    ASSERT_EQ( true, testBool_f<XmlDoc>( booksDoc ) );
    EXPECT_FALSE( booksDoc.errorHandler( ).hasErrors( ) );

    // assign xml to null
    RHSNullDoc = cp_f<XmlDoc>( booksDoc );
    ASSERT_EQ( true, testBool_f<XmlDoc>( RHSNullDoc ) );
    EXPECT_FALSE( RHSNullDoc.errorHandler( ).hasErrors( ) );

    // assign null to xml
    menuDoc = cp_f<XmlDoc>( LHSNullDoc );
    ASSERT_EQ( false, testBool_f<XmlDoc>( menuDoc ) );
    EXPECT_FALSE( menuDoc.errorHandler( ).hasErrors( ) );
}

TEST( XmlDocTests, ToString ) {
    std::istringstream menuIs{menu_xml};
    XmlDoc menuDoc{menuIs};
    ASSERT_EQ( true, testBool_f<XmlDoc>( menuDoc ) );
    EXPECT_STREQ( menu_xml.c_str( ), menuDoc.toString( ).c_str( ) );

    XmlDoc cpMenuDoc{menuDoc};
    EXPECT_STREQ( cpMenuDoc.toString( ).c_str( ), menuDoc.toString( ).c_str( ) );

    std::istringstream booksIs{books_xml};
    XmlDoc booksDoc{booksIs};
    EXPECT_STRNE( booksDoc.toString( ).c_str( ), menuDoc.toString( ).c_str( ) );
}

TEST( XmlDocTests, ElementSearch ) {
    std::istringstream menuIs{menu_xml};
    XmlDoc menuDoc{menuIs};
    ASSERT_EQ( true, testBool_f<XmlDoc>( menuDoc ) );
    XmlElement root{menuDoc.getRootElement( )};
}

TEST( XmlDocTests, SetRootElement ) {
    XmlDoc doc;
    XmlElement root{"root-element"};
    root.attributes( {{"checked", "yes"}, {"sentiment", "positive"}} );
    doc.setRootElement( root );

    std::cerr << doc << std::endl;
}
