#include "xpath_ctxt.hpp"
#include "xpath_query.hpp"
#include <iostream>

auto FreeXPathCtxt::operator( )( xmlXPathContext *xpathCtxt ) const -> void {
    xmlXPathFreeContext( xpathCtxt );
}

XPathCtxt::XPathCtxt( ) : xpathCtxt_{nullptr} {}
XPathCtxt::XPathCtxt( const XmlDoc &xml )
    : xpathCtxt_{xmlXPathNewContext( xml.xmlDoc_.get( ) )}, xml_{xml} {
    xpathHandler_.registerHandler( xpathCtxt_.get( ) );
}
XPathCtxt::XPathCtxt( const XPathCtxt &xpathCtxt ) : xml_{xpathCtxt.xml_} {
    if ( xpathCtxt ) {
        xpathCtxt_.reset( xmlXPathNewContext( xpathCtxt.xml_.xmlDoc_.get( ) ) );
        xpathHandler_.registerHandler( xpathCtxt_.get( ) );
    }
}
XPathCtxt::XPathCtxt( XPathCtxt &&xpathCtxt )
    : xpathCtxt_{std::move( xpathCtxt.xpathCtxt_ )}, xml_{std::move( xpathCtxt.xml_ )},
      xpathHandler_{std::move( xpathCtxt.xpathHandler_ )} {}

auto XPathCtxt::operator=( const XPathCtxt &rhs ) -> XPathCtxt & {
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
auto XPathCtxt::operator=( XPathCtxt &&xpathCtxt ) -> XPathCtxt & {
    xml_ = std::move( xpathCtxt.xml_ );
    xpathCtxt_ = std::move( xpathCtxt.xpathCtxt_ );
    xpathHandler_ = std::move( xpathCtxt.xpathHandler_ );
    return *this;
}
XPathCtxt::operator bool( ) const {
    return ( xpathCtxt_ != nullptr && xpathCtxt_->doc != nullptr );
}
auto XPathCtxt::errorHandler( ) const -> const IErrorHandler & { return xpathHandler_; }
auto XPathCtxt::makeQuery( ) const -> XPathQuery { return XPathQuery{*this}; }

