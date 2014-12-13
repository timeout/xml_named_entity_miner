#include "xpath_ctxt.hpp"
#include "xml_doc.hpp"
#include "pathname.hpp"

#include <fstream>
#include <sstream>

#include "gtest/gtest.h"
#include <limits.h>

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

class XPathCtxtTest : public testing::Test {
protected:
    auto testable( const XPathCtxt &ctxt ) const -> bool { return ctxt ? true : false; }
    auto cp_f( XPathCtxt xpc ) -> XPathCtxt { return xpc; }

private:
    XmlDoc xml_;
};

TEST_F( XPathCtxtTest, DefaultCtor ) {
    XPathCtxt xpc;
    ASSERT_EQ( false, testable( xpc ) );
    EXPECT_FALSE( xpc.errorHandler( ).hasErrors( ) );
}

TEST_F( XPathCtxtTest, NullCtor ) {
    // empty xml
    const XmlDoc nullXml;
    XPathCtxt xpc{nullXml};
    ASSERT_EQ( false, testable( xpc ) );
    EXPECT_FALSE( xpc.errorHandler( ).hasErrors( ) );
}

TEST_F( XPathCtxtTest, Ctor ) {
    std::istringstream booksIs{books_xml};
    XmlDoc booksDoc{booksIs};
    XPathCtxt xpc;
    booksDoc >> xpc;
    ASSERT_EQ( true, testable( xpc ) );
    EXPECT_FALSE( xpc.errorHandler( ).hasErrors( ) );
}

TEST_F( XPathCtxtTest, CopyCtor ) {
    // null XPathCtxt
    XPathCtxt nullXpc;
    ASSERT_EQ( false, testable( nullXpc ) );

    XPathCtxt cpNullXpc{nullXpc};
    ASSERT_EQ( false, testable( cpNullXpc ) );
    EXPECT_FALSE( cpNullXpc.errorHandler( ).hasErrors( ) );

    // XPathCtxt with null Xml
    const XmlDoc nullDoc;
    nullDoc >> nullXpc;
    ASSERT_EQ( false, testable( nullXpc ) );

    XPathCtxt cpNullDocXpc{nullXpc};
    ASSERT_EQ( false, testable( cpNullDocXpc ) );
    EXPECT_FALSE( cpNullDocXpc.errorHandler( ).hasErrors( ) );

    // XPathCtxt
    std::istringstream booksIs{books_xml};
    XmlDoc booksDoc{booksIs};
    XPathCtxt xpc;
    booksDoc >> xpc;
    ASSERT_EQ( true, testable( xpc ) );

    XPathCtxt cpXpc{xpc};
    ASSERT_EQ( true, testable( cpXpc ) );
    EXPECT_FALSE( cpXpc.errorHandler( ).hasErrors( ) );
}

TEST_F( XPathCtxtTest, CopyAssignment ) {
    // assign null to null
    XPathCtxt nullXpc;
    ASSERT_EQ( false, testable( nullXpc ) );

    XPathCtxt nullXpc1;
    ASSERT_EQ( false, testable( nullXpc1 ) );

    nullXpc = nullXpc1;
    ASSERT_EQ( false, testable( nullXpc ) );
    EXPECT_FALSE( nullXpc.errorHandler( ).hasErrors( ) );

    // assign xpc to xpc
    std::istringstream booksIs{books_xml};
    XmlDoc booksDoc{booksIs};
    XPathCtxt xpc;
    booksDoc >> xpc;
    ASSERT_EQ( true, testable( xpc ) );

    std::istringstream menuIs{menu_xml};
    XmlDoc menuDoc{menuIs};
    XPathCtxt xpc1;
    booksDoc >> xpc1;
    ASSERT_EQ( true, testable( xpc1 ) );

    xpc = xpc1;
    ASSERT_EQ( true, testable( xpc ) );
    EXPECT_FALSE( xpc.errorHandler( ).hasErrors( ) );

    // assign null to xpc
    xpc = nullXpc;
    ASSERT_EQ( false, testable( xpc ) );
    EXPECT_FALSE( xpc.errorHandler( ).hasErrors( ) );

    // assign xpc to null
    xpc1 = nullXpc1;
    ASSERT_EQ( false, testable( nullXpc1 ) );
    EXPECT_FALSE( nullXpc1.errorHandler( ).hasErrors( ) );
}

TEST_F( XPathCtxtTest, CopyMoveCtor ) {
    // null XPathCtxt
    XPathCtxt nullXpc;
    ASSERT_EQ( false, testable( nullXpc ) );

    XPathCtxt cpNullXpc{cp_f( nullXpc )};
    ASSERT_EQ( false, testable( cpNullXpc ) );
    EXPECT_FALSE( cpNullXpc.errorHandler( ).hasErrors( ) );

    // XPathCtxt with null Xml
    const XmlDoc nullDoc;
    nullDoc >> nullXpc;
    ASSERT_EQ( false, testable( nullXpc ) );

    XPathCtxt cpNullDocXpc{cp_f( nullXpc )};
    ASSERT_EQ( false, testable( cpNullDocXpc ) );
    EXPECT_FALSE( cpNullDocXpc.errorHandler( ).hasErrors( ) );

    // XPathCtxt
    std::istringstream booksIs{books_xml};
    XmlDoc booksDoc{booksIs};
    XPathCtxt xpc;
    booksDoc >> xpc;
    ASSERT_EQ( true, testable( xpc ) );

    XPathCtxt cpXpc{cp_f( xpc )};
    ASSERT_EQ( true, testable( cpXpc ) );
    EXPECT_FALSE( cpXpc.errorHandler( ).hasErrors( ) );
}

TEST_F( XPathCtxtTest, CopyMoveAssignment ) {
    // assign null to null
    XPathCtxt nullXpc;
    ASSERT_EQ( false, testable( nullXpc ) );

    XPathCtxt nullXpc1;
    ASSERT_EQ( false, testable( nullXpc1 ) );

    nullXpc = cp_f(nullXpc1);
    ASSERT_EQ( false, testable( nullXpc ) );
    EXPECT_FALSE( nullXpc.errorHandler( ).hasErrors( ) );

    // assign xpc to xpc
    std::istringstream booksIs{books_xml};
    XmlDoc booksDoc{booksIs};
    XPathCtxt xpc;
    booksDoc >> xpc;
    ASSERT_EQ( true, testable( xpc ) );

    std::istringstream menuIs{menu_xml};
    XmlDoc menuDoc{menuIs};
    XPathCtxt xpc1;
    booksDoc >> xpc1;
    ASSERT_EQ( true, testable( xpc1 ) );

    xpc = cp_f(xpc1);
    ASSERT_EQ( true, testable( xpc ) );
    EXPECT_FALSE( xpc.errorHandler( ).hasErrors( ) );

    // assign null to xpc
    xpc = cp_f(nullXpc);
    ASSERT_EQ( false, testable( xpc ) );
    EXPECT_FALSE( xpc.errorHandler( ).hasErrors( ) );

    // assign xpc to null
    xpc1 = cp_f(nullXpc1);
    ASSERT_EQ( false, testable( nullXpc1 ) );
    EXPECT_FALSE( nullXpc1.errorHandler( ).hasErrors( ) );
}

TEST_F(XPathCtxtTest, OperatorIn) {
    XPathCtxt nullXpc;
    ASSERT_EQ( false, testable( nullXpc ) );

    XmlDoc xml;
    xml >> nullXpc;
    ASSERT_EQ( false, testable( nullXpc ) );

    std::istringstream booksIs{books_xml};
    booksIs >> xml;
    xml >> nullXpc;
    ASSERT_EQ( true, testable( nullXpc ) );

    std::istringstream menuIs{menu_xml};
    menuIs >> xml;
    xml >> nullXpc;
    ASSERT_EQ( true, testable( nullXpc ) );

    XmlDoc nullDoc;
    nullDoc >> nullXpc;
    ASSERT_EQ( false, testable( nullXpc ) );
}
