#pragma once

#include "xml_parser_ctxt.hpp"
#include "pathname.hpp"
// #include "xml_error_handler.hpp"

#include <string>
#include <iosfwd>
#include <memory>

#include <libxml/tree.h>

class FreeXmlDoc {
public:
    auto operator( )( xmlDoc *xml ) const -> void { xmlFreeDoc( xml ); }
};

class XmlDoc {
public:
    XmlDoc( );
    explicit XmlDoc( const Pathname& pathname );
    explicit XmlDoc( xmlDocPtr xml );
    XmlDoc( const XmlDoc &doc );

    auto operator=( const XmlDoc &xml ) -> XmlDoc &;
    auto release( ) -> xmlDocPtr { return xmlDoc_.release( ); }
    auto get( ) const -> xmlDocPtr { return xmlDoc_.get( ); }
    auto pathname() const -> const Pathname& { return pathname_; }
    auto swap(XmlDoc & other) -> void;
    auto toString() const -> std::string;

private:
    typedef std::unique_ptr<xmlDoc, FreeXmlDoc> XmlDocT;
    XmlDocT xmlDoc_;
    Pathname pathname_;
    XmlParserCtxt parserCtxt_;
    // XmlErrorHandler handler_;
};

auto fileToString( const std::string &path ) -> std::string;

auto operator<<( std::ostream &os, const XmlDoc &doc ) -> std::ostream &;

namespace std {
    template <>
    void swap( XmlDoc &lhs, XmlDoc &rhs );
}

