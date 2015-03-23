#include "lib/xslt_doc.hpp"
#include "lib/xml_doc.hpp"

#include <sstream>
#include <iostream>

#include "gtest/gtest.h"
#include <limits.h>

#include "test_helper.hpp"

class XsltDocTests : public testing::Test {
protected:
    auto xmlStylesheet( ) const -> const XmlDoc { return creator.stylesheet( ); }
    auto xmlBooksDoc( ) const -> const XmlDoc & { return creator.booksXml( ); }
    auto xmlMenuDoc( ) const -> const XmlDoc & { return creator.menuXml( ); }

private:
    XmlCreator creator;
};

TEST_F( XsltDocTests, DefaultCtor ) {
    XsltDoc xslt;
    ASSERT_EQ( false, testBool_f<XsltDoc>( xslt ) );
}

TEST_F( XsltDocTests, Ctor ) {
    XmlDoc stylesheet{xmlStylesheet( )};
    XsltDoc xslt{stylesheet};
    ASSERT_EQ( true, testBool_f<XsltDoc>( xslt ) );
}

TEST_F( XsltDocTests, Transform ) {
    const std::string transform{
        R"(<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="/">

    <title><xsl:value-of select="catalog/book/title"/></title>
</xsl:template>
</xsl:stylesheet>)"};
    std::istringstream transform_is{transform};
    XmlDoc trans_doc{transform_is};
    XsltDoc xslt{trans_doc};
    ASSERT_EQ( true, testBool_f<XsltDoc>( xslt ) );

    XmlDoc books{xmlBooksDoc( )};
    auto trans = xslt.transform( books );
    std::cerr << trans << std::endl;
}
