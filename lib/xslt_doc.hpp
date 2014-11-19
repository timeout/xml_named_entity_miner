#pragma once

#include "xml_doc.hpp"
#include "standard_error_handler.hpp"

#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libexslt/exslt.h>

#include <memory>

class FreeXsltStDoc {
public:
    auto operator( )( xsltStylesheet *xsltDoc ) const -> void {
        xsltFreeStylesheet( xsltDoc );
    }
};

class XsltDoc {
public:
    XsltDoc( ) : stylesheet_{nullptr} {}
    /* /!\ xsltStylesheet takes ownership of xmlDoc /!\ */
    XsltDoc( XmlDoc &xml )
        : stylesheet_{xsltParseStylesheetDoc( xml.xmlDoc_.release( ) )} {}
    XsltDoc( const XsltDoc &stylesheet ) : stylesheet_{xsltNewStylesheet( )} {
        *stylesheet_ = *stylesheet.stylesheet_;
    }
    XsltDoc( XsltDoc &&stylesheet ) : stylesheet_{std::move( stylesheet.stylesheet_ )} {}
    auto operator=( const XsltDoc &rhs ) -> XsltDoc & {
        XsltDoc tmp{rhs};
        stylesheet_ = std::move( tmp.stylesheet_ );
        return *this;
    }
    auto operator=( XsltDoc &&rhs ) -> XsltDoc & {
        stylesheet_ = std::move( rhs.stylesheet_ );
        return *this;
    }
    explicit operator bool( ) const { return ( stylesheet_ != nullptr ); }
    /* /!\ xsltStylesheet takes ownership of xmlDoc /!\ */
    friend auto operator>>( XmlDoc &xml, XsltDoc &stylesheet ) -> XsltDoc & {
        stylesheet.stylesheet_.reset( xsltParseStylesheetDoc( xml.xmlDoc_.release( ) ) );
        return stylesheet;
    }
    auto transform( const XmlDoc &xml ) const -> XmlDoc {
        XmlDoc res;
        res.xmlDoc_.reset(
            xsltApplyStylesheet( stylesheet_.get( ), xml.get( ), nullptr ) );
        return res;
    }

private:
    using XsltDocT = std::unique_ptr<xsltStylesheet, FreeXsltStDoc>;
    XsltDocT stylesheet_;
};

