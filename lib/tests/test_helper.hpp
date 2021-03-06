#pragma once

#include "lib/xml_doc.hpp"
#include "lib/xpath_ctxt.hpp"
#include "lib/xpath_query.hpp"

class XmlCreator {
public:
    XmlCreator( );

    auto booksXml( ) const -> const XmlDoc & { return books_xml_; }
    auto menuXml( ) const -> const XmlDoc & { return menu_xml_; }
    auto stylesheet( ) const -> const XmlDoc { return bilanz_xsl_; }

private:
    XmlDoc books_xml_, menu_xml_, bilanz_xsl_;
};

class XPathCtxtCreator {
public:
    XPathCtxtCreator( );

    auto booksCtxt( ) const -> const XPathCtxt & { return books_xpctx_; }
    auto menuCtxt( ) const -> const XPathCtxt & { return menu_xpctx_; }

private:
    XPathCtxt books_xpctx_, menu_xpctx_;
};

class XPathQueryCreator {
public:
    XPathQueryCreator( );

    auto booksQuery( ) const -> const XPathQuery & { return books_query_; }
    auto menuQuery( ) const -> const XPathQuery & { return menu_query_; }

private:
    XPathQuery books_query_, menu_query_;
};

template <typename T>
auto cp_f( T t ) -> T {
    return t;
}

template <typename T>
auto testBool_f( const T &t ) -> bool {
    return ( t ) ? true : false;
}
