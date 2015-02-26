#pragma once

#include "xml_doc.hpp"
#include "libxml2_error_handlers.hpp"
#include <libxml/xpath.h>
#include <memory>

class XPathQuery;

class FreeXPathCtxt {
public:
    auto operator( )( xmlXPathContext *xpathCtxt ) const -> void;
};

class XPathCtxt {
    friend class XPathQuery;

public:
    XPathCtxt( );
    explicit XPathCtxt( const XmlDoc &xml );
    XPathCtxt( const XPathCtxt &xpathCtxt );
    XPathCtxt( XPathCtxt &&xpathCtxt );
    auto operator=( const XPathCtxt &rhs ) -> XPathCtxt &;
    auto operator=( XPathCtxt &&xpathCtxt ) -> XPathCtxt &;
    explicit operator bool( ) const;
    friend auto operator>>( const XmlDoc &xml, XPathCtxt &xpathCtxt ) -> XPathCtxt &;
    auto errorHandler( ) -> IErrorHandler &;
    auto makeQuery( ) const -> XPathQuery;

private:
    using XPathCtxtT = std::unique_ptr<xmlXPathContext, FreeXPathCtxt>;
    XPathCtxtT xpathCtxt_;
    XmlDoc xml_;
    XPathErrorHandler xpathHandler_;
};

inline auto operator>>( const XmlDoc &xml, XPathCtxt &xpathCtxt ) -> XPathCtxt & {
    xpathCtxt.xml_ = xml;
    xpathCtxt.xpathCtxt_.reset( xmlXPathNewContext( xpathCtxt.xml_.get( ) ) );
    xpathCtxt.xpathHandler_.registerHandler( xpathCtxt.xpathCtxt_.get( ) );
    return xpathCtxt;
}

