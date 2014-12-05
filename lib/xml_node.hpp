#pragma once

#include <memory>
#include <string>
#include <iosfwd>
#include <libxml/tree.h>

class FreeXmlNode {
public:
    auto operator( )( xmlNode *node ) const -> void;
};

class XmlNode {
public:
    XmlNode( );
    XmlNode( const xmlNodePtr np, int extended = 1 );
    XmlNode( const XmlNode &node );
    XmlNode( XmlNode &&node );
    auto operator=( const XmlNode &rhs ) -> XmlNode &;
    auto operator=( XmlNode &&rhs ) -> XmlNode &;
    explicit operator bool( ) const;
    auto toString( ) const -> std::string;

private:
    using XmlNodeT = std::unique_ptr<xmlNode, FreeXmlNode>;
    XmlNodeT node_;
};

inline auto operator<<( std::ostream &os, const XmlNode &node ) -> std::ostream & {
    os << node.toString( );
    return os;
}

