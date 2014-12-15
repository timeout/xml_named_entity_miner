#pragma once

#include "xml_parser_ctxt.hpp"
#include "pathname.hpp"
#include "libxml2_error_handlers.hpp"
#include "xml_string.hpp"

#include <string>
#include <iosfwd>
#include <memory>

#include <libxml/tree.h>

struct FreeXmlDoc {
public:
    auto operator( )( xmlDoc *xml ) const -> void { xmlFreeDoc( xml ); }
};

class XmlDoc {
    friend class XmlSchemaParserCtxt;
    friend class XsltDoc;
    friend class XPathCtxt;

public:
    XmlDoc( );
    explicit XmlDoc( std::istream &is );
    XmlDoc( const XmlDoc &doc );
    XmlDoc( XmlDoc &&doc );
    auto operator=( const XmlDoc &rhs ) -> XmlDoc &;
    auto operator=( XmlDoc &&rhs ) -> XmlDoc &;
    explicit operator bool( ) const;
    auto get( ) const -> xmlDoc * { return xmlDoc_.get( ); }
    friend auto operator>>( std::istream &is, XmlDoc &doc ) -> std::istream &;
    auto errorHandler( ) const -> const IErrorHandler & { return xmlHandler_; }
    auto toString( ) const -> std::string;
    auto swap( XmlDoc &other ) -> void;

private:
    using XmlDocT = std::shared_ptr<xmlDoc>;
    // std::unique_ptr<xmlDoc, FreeXmlDoc> xmlDoc_;
    XmlDocT xmlDoc_;
    XmlErrorHandler xmlHandler_;
};

auto fileToString( const std::string &path ) -> std::string;

inline auto operator<<( std::ostream &os, const XmlDoc &doc ) -> std::ostream & {
    os << doc.toString( );
    return os;
}

namespace std {
    template <>
    void swap( XmlDoc &lhs, XmlDoc &rhs );
}

