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
        : xpathCtxt_{xmlXPathNewContext( xml.xmlDoc_.get( ) )}, xml_{xml} {
        xpathHandler_.registerHandler( xpathCtxt_.get( ) );
    }
    XPathCtxt( const XPathCtxt &xpathCtxt ) : xml_{xpathCtxt.xml_} {
        if ( xpathCtxt ) {
            xpathCtxt_.reset( xmlXPathNewContext( xpathCtxt.xml_.xmlDoc_.get( ) ) );
            xpathHandler_.registerHandler( xpathCtxt_.get( ) );
        }
    }
    XPathCtxt( XPathCtxt &&xpathCtxt )
        : xpathCtxt_{std::move( xpathCtxt.xpathCtxt_ )},
          xml_{std::move( xpathCtxt.xml_ )},
          xpathHandler_{std::move( xpathCtxt.xpathHandler_ )} {}
    auto operator=( const XPathCtxt &rhs ) -> XPathCtxt & {
        if ( this != &rhs ) {
            xml_ = rhs.xml_;
            if ( rhs ) {
                xpathCtxt_.reset( xmlXPathNewContext( rhs.xml_.xmlDoc_.get( ) ) );
                xpathHandler_ = rhs.xpathHandler_;
            } else {
                xpathCtxt_.reset( nullptr );
            }
        }
        return *this;
    }
    auto operator=( XPathCtxt &&xpathCtxt ) -> XPathCtxt & {
        xml_ = std::move( xpathCtxt.xml_ );
        xpathCtxt_ = std::move( xpathCtxt.xpathCtxt_ );
        xpathHandler_ = std::move( xpathCtxt.xpathHandler_ );
        return *this;
    }
    explicit operator bool( ) const {
        return ( xpathCtxt_ != nullptr && xpathCtxt_->doc != nullptr );
    }
    friend auto operator>>( const XmlDoc &xml, XPathCtxt &xpathCtxt ) -> XPathCtxt & {
        xpathCtxt.xml_ = xml;
        xpathCtxt.xpathCtxt_.reset( xmlXPathNewContext( xpathCtxt.xml_.xmlDoc_.get( ) ) );
        xpathCtxt.xpathHandler_.registerHandler(
            xpathCtxt.xpathCtxt_.get( ) ); // ist this correct?
        return xpathCtxt;
    }
    auto errorHandler( ) const -> const IErrorHandler & { return xpathHandler_; }

private:
    using XPathCtxtT = std::unique_ptr<xmlXPathContext, FreeXPathCtxt>;
    XPathCtxtT xpathCtxt_;
    XmlDoc xml_;
    XPathErrorHandler xpathHandler_;
};

