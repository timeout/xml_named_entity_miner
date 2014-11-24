#pragma once

#include "xml_doc.hpp"
#include "libxml2_error_handlers.hpp"
#include <libxml/xpath.h>
#include <memory>
#include <iostream>

class FreeXPathCtxt {
public:
    auto operator( )( xmlXPathContext *xpathCtxt ) const -> void {
        xmlXPathFreeContext( xpathCtxt );
    }
};

class XPathCtxt {
    friend class XPathQuery;

public:
    XPathCtxt( ) : xpathCtxt_{nullptr} {}
    explicit XPathCtxt( const XmlDoc &xml )
        : xpathCtxt_{xmlXPathNewContext( xml.xmlDoc_.get( ) )} {
        xpathHandler_.registerHandler( xpathCtxt_.get( ) );
    }
    XPathCtxt( const XPathCtxt &xpathCtxt ) {
        xpathCtxt_.reset( xmlXPathNewContext( xpathCtxt.xpathCtxt_->doc ) );
        xpathHandler_.registerHandler( xpathCtxt_.get( ) );
    }
    XPathCtxt( XPathCtxt &&xpathCtxt )
        : xpathCtxt_{std::move( xpathCtxt.xpathCtxt_ )},
          xpathHandler_{std::move( xpathCtxt.xpathHandler_ )} {}
    auto operator=( const XPathCtxt &rhs ) -> XPathCtxt & {
        if ( this != &rhs ) {
            xpathCtxt_.reset( xmlXPathNewContext( rhs.xpathCtxt_->doc ) );
            xpathHandler_ = rhs.xpathHandler_;
        }
        return *this;
    }
    auto operator=( XPathCtxt &&xpathCtxt ) -> XPathCtxt & {
        xpathCtxt_ = std::move( xpathCtxt.xpathCtxt_ );
        xpathHandler_ = std::move( xpathCtxt.xpathHandler_ );
        return *this;
    }
    explicit operator bool( ) const {
        return ( xpathCtxt_ != nullptr && xpathCtxt_->doc != nullptr );
    }
    friend auto operator>>( const XmlDoc &xml, XPathCtxt &xpathCtxt ) -> XPathCtxt & {
        xpathCtxt.xpathCtxt_.reset( xmlXPathNewContext( xml.xmlDoc_.get( ) ) );
        xpathCtxt.xpathHandler_.registerHandler(
            xpathCtxt.xpathCtxt_.get( ) ); // ist this correct?
        return xpathCtxt;
    }
    auto errorHandler( ) const -> const IErrorHandler & { return xpathHandler_; }

private:
    using XPathCtxtT = std::unique_ptr<xmlXPathContext, FreeXPathCtxt>;
    XPathCtxtT xpathCtxt_;
    XPathErrorHandler xpathHandler_;
};

