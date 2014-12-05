#include "xml_node.hpp"
#include "xml_string.hpp"
#include <libxml/xpath.h>

auto FreeXmlNode::operator( )( xmlNode *node ) const -> void { xmlFreeNode( node ); }

XmlNode::XmlNode( ) : node_{nullptr} {}
XmlNode::XmlNode( const xmlNodePtr np, int extended )
    : node_{xmlCopyNode( np, extended )} {}
XmlNode::XmlNode( const XmlNode &node ) : node_{xmlCopyNode( node.node_.get( ), 2 )} {}
XmlNode::XmlNode( XmlNode &&node ) : node_{std::move( node.node_ )} {}
auto XmlNode::operator=( const XmlNode &rhs ) -> XmlNode & {
    if ( this != &rhs ) {
        XmlNode tmp{rhs};
        node_ = std::move( tmp.node_ );
    }
    return *this;
}
auto XmlNode::operator=( XmlNode &&node ) -> XmlNode & {
    node_ = std::move( node.node_ );
    return *this;
}
XmlNode::operator bool( ) const { return node_ != nullptr; }
auto XmlNode::toString( ) const -> std::string {
    XmlString s{xmlXPathCastNodeToString( node_.get( ) )};
    return s.toString( );
}

