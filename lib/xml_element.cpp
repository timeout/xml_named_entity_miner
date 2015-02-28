#include "xml_element.hpp"
#include <iostream>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <functional>
#include <vector>
#include <stack>
#include <utility>
#include <numeric>
#include <sstream>

#include <iostream>

static auto replaceAndDestroy( xmlNode *old, xmlNode *nn ) -> void;
template <typename T>
static auto xmlTreeMap( xmlNode *node, const std::function<void( xmlNode *, T * )> &f,
                        T *arg ) -> void;
static auto catContent( xmlNode *node, std::string *str ) -> void;
static auto findContent( xmlNode *node,
                         std::pair<const std::string &, std::vector<xmlNode *>> *cl )
    -> void;

XmlElement::XmlElement( xmlNode *node ) : node_{node} {}
XmlElement::operator bool( ) const { return nullptr != node_; }
auto XmlElement::name( ) const -> std::string {
    return ( node_ ) ? std::string{reinterpret_cast<const char *>( node_->name )}
                     : std::string{};
}
auto XmlElement::name( const std::string &name ) -> std::string {
    if ( node_ ) {
        std::string ret = this->name( );
        xmlNodeSetName( node_, reinterpret_cast<const unsigned char *>( name.c_str( ) ) );
        return ret;
    }
    return std::string{};
}
auto XmlElement::hasAttributes( ) const -> bool {
    return ( nullptr != node_->properties );
}

auto XmlElement::attribute( const std::string &key, const std::string &value ) -> void {
    if ( node_ ) {
        xmlNewProp( node_, reinterpret_cast<const unsigned char *>( key.c_str( ) ),
                    reinterpret_cast<const unsigned char *>( value.c_str( ) ) );
    }
}
// auto XmlElement::attributes( ) const -> std::map<std::string, std::string> {
//     std::map<std::string, std::string> ret;
//     if ( node_ && node_->properties ) {
//         for ( xmlAttr *attr = node_->properties; nullptr != attr; attr = attr->next ) {
//             // TODO: replace with XmlString
//             xmlChar *valp = xmlGetProp( node_, attr->name );
//             const std::string value = reinterpret_cast<const char *>( valp );
//             xmlFree( valp );
//             ret[reinterpret_cast<const char *>( attr->name )] = value;
//         }
//     }
//     return ret;
// }

auto XmlElement::attributes( ) const -> std::vector<std::pair<std::string, std::string>> {
    std::vector<std::pair<std::string, std::string>> ret;
    if ( node_ && node_->properties ) {
        for ( xmlAttr *attr = node_->properties; nullptr != attr; attr = attr->next ) {
            // TODO: replace with XmlString
            xmlChar *valp = xmlGetProp( node_, attr->name );
            const std::string value = reinterpret_cast<const char *>( valp );
            xmlFree( valp );
            ret.push_back(
                std::make_pair( reinterpret_cast<const char *>( attr->name ), value ) );
        }
    }
    return ret;
}

auto XmlElement::attributes( const std::map<std::string, std::string> &attrs )
    -> std::map<std::string, std::string> {
    // get current properties
    std::map<std::string, std::string> ret;
    auto attrPairs = this->attributes( );
    for ( auto attr : attrPairs ) {
        ret[attr.first] = attr.second;
    }
    // add properties
    for ( auto it = attrs.cbegin( ); it != attrs.end( ); ++it ) {
        this->attribute( it->first, it->second );
    }
    return ret;
}
auto XmlElement::hasChild( ) const -> bool { return ( node_ && node_->children ); }
auto XmlElement::child( ) const -> XmlElement {
    return ( node_ && node_->children ) ? XmlElement{node_->children}
                                        : XmlElement{nullptr};
}
auto XmlElement::child( const std::string &name ) -> XmlElement {
    // create a new node
    xmlNode *nn{
        xmlNewNode( nullptr, reinterpret_cast<const unsigned char *>( name.c_str( ) ) )};
    // if node_ or nn is null xmlAddChild returns null
    xmlNode *child = xmlAddChild( node_, nn );
    if ( !child && nn ) {
        xmlFreeNode( nn );
    }
    return XmlElement{child};
}
auto XmlElement::child( const std::string &name,
                        const std::map<std::string, std::string> &attrs ) -> XmlElement {
    XmlElement child = this->child( name );
    child.attributes( attrs );
    return child;
}
auto XmlElement::hasSibling( ) const -> bool { return ( node_ && node_->next ); }
auto XmlElement::sibling( const std::string &name ) -> XmlElement {
    if ( !node_ ) {
        return {nullptr};
    }
    xmlNode *nn{
        xmlNewNode( nullptr, reinterpret_cast<const unsigned char *>( name.c_str( ) ) )};
    xmlNode *sibling = xmlAddNextSibling( node_, nn );
    if ( !sibling && nn ) {
        xmlFreeNode( nn );
    }
    return sibling;
}
auto XmlElement::sibling( const std::string &name,
                          const std::map<std::string, std::string> &attrs )
    -> XmlElement {
    if ( !node_ ) {
        return {nullptr};
    }
    XmlElement sibling = this->sibling( name );
    sibling.attributes( attrs );
    return sibling;
}
auto XmlElement::hasContent( ) const -> bool {
    if ( nullptr == node_ ) {
        return false;
    }
    const xmlNode *tmp = node_;
    while ( true ) {
        if ( tmp->type == XML_TEXT_NODE ) {
            return true;
        }
        if ( tmp->children ) {
            tmp = tmp->children;
        } else {
            while ( !tmp->next ) {
                if ( tmp == node_ ) {
                    return false;
                }
                tmp = tmp->parent;
            }
            tmp = tmp->next;
        }
    }
}
auto XmlElement::content( ) const -> std::string {
    if ( node_ ) {
        std::string ret;
        xmlTreeMap<std::string>( node_, catContent, &ret );
        return ret;
    }
    return {};
}
auto XmlElement::content( const std::string &content ) -> void {
    if ( node_ ) {
        xmlNodeAddContent( node_,
                           reinterpret_cast<const unsigned char *>( content.c_str( ) ) );
    }
}
auto XmlElement::toString( ) const -> std::string {
    if ( node_ ) {
        xmlBufferPtr buff = nullptr;
        if ( ( buff = xmlBufferCreate( ) ) == nullptr ) {
            // TODO:
            std::cerr << "Could not allocate xml buffer" << std::endl;
            return std::string{};
        }
        int flag = 0;
        if ( ( flag = xmlNodeDump( buff, nullptr, node_, 0, 1 ) ) < 0 ) {
            // TODO:
            std::cerr << "Node could not be dumped" << std::endl;
            return std::string{};
        }
        std::string ret{reinterpret_cast<const char *>( buff->content )};
        xmlBufferFree( buff );
        return ret;
    }
    return {};
}

auto XmlElement::operator==( const XmlElement &lhs ) const -> bool {
    return node_ == lhs.node_;
}
auto XmlElement::operator!=( const XmlElement &lhs ) const -> bool {
    return !( *this == lhs );
}
auto XmlElement::operator<( const XmlElement &lhs ) const -> bool {
    // in terms of in-order traversal
    if ( *this == lhs ) {
        return false;
    }
    if ( node_ == nullptr && lhs.node_ ) {
        return false;
    }
    if ( lhs.node_ == nullptr && node_ ) {
        return true;
    }
    const xmlNode *tmp = node_;
    while ( true ) {
        if ( tmp->type == XML_ELEMENT_NODE && tmp == lhs.node_ ) {
            return true;
        }
        if ( tmp->children ) {
            tmp = tmp->children;
        } else {
            while ( !tmp->next ) {
                if ( !tmp->parent ) {
                    return false;
                }
                tmp = tmp->parent;
            }
            tmp = tmp->next;
        }
    }
}
auto XmlElement::operator>( const XmlElement &lhs ) const -> bool {
    if ( *this == lhs ) {
        return false;
    }
    return !( *this < lhs );
}
auto XmlElement::operator<=( const XmlElement &lhs ) const -> bool {
    if ( *this == lhs ) {
        return true;
    }
    return ( *this < lhs );
}
auto XmlElement::operator>=( const XmlElement &lhs ) const -> bool {
    if ( *this == lhs ) {
        return true;
    }
    return !( *this > lhs );
}
// TODO: xmlNodes instantiation failure?
auto XmlElement::search( const std::string &needle,
                         const std::string &entityType ) const -> void {
    std::vector<xmlNode *> nodes;
    std::pair<const std::string &, std::vector<xmlNode *>> data{needle, nodes};
    xmlTreeMap<std::pair<const std::string &, std::vector<xmlNode *>>>(
        node_, findContent, &data );

    for ( auto node : data.second ) {
        std::string content{reinterpret_cast<const char *>( node->content )};

        // split into prefix | needle | suffix
        std::pair<std::string, std::string> splstr;
        std::string::size_type n = content.find( needle );
        splstr.first = content.substr( 0, n );
        splstr.second = content.substr( n + needle.size( ), std::string::npos );

        // markup needle
        // create element
        xmlNode *entity =
            xmlNewNode( nullptr, reinterpret_cast<const unsigned char *>( "entity" ) );
        XmlElement el{entity};
        el.attribute( "type", entityType );
        el.content( needle );

        if ( splstr.first.empty( ) ) {
            replaceAndDestroy( node, el.node_ );
        } else {
            printf( "DEBUG: '%s'\n", splstr.first.c_str( ) );
            xmlNode *textNode = xmlNewText(
                reinterpret_cast<const unsigned char *>( splstr.first.c_str( ) ) );
            replaceAndDestroy( node, textNode );
            xmlAddNextSibling( textNode, el.node_ );
        }

        if ( !splstr.second.empty( ) ) {
            xmlAddNextSibling( el.node_,
                               xmlNewText( reinterpret_cast<const unsigned char *>(
                                   splstr.second.c_str( ) ) ) );
        }
    }
}

auto XmlElement::xpaths( ) const -> const std::vector<std::string> {
    if ( nullptr == node_ ) {
        return {};
    }
    std::vector<std::string> base;
    std::vector<std::string> fullPath;
    const xmlNode *tmp = node_;
    while ( true ) {
        if ( tmp->type == XML_ELEMENT_NODE ) {
            std::string basePath =
                std::accumulate( base.begin( ), base.end( ), std::string{} );
            fullPath.push_back( basePath + "/" +
                                reinterpret_cast<const char *>( tmp->name ) );
        }

        if ( tmp->children ) {
            base.push_back( std::string{"/"} +
                            reinterpret_cast<const char *>( tmp->name ) );
            tmp = tmp->children;
        } else {
            while ( !tmp->next ) {
                if ( tmp == node_ ) {
                    return fullPath;
                }
                base.pop_back( );
                tmp = tmp->parent;
            }
            tmp = tmp->next;
        }
    }
}

auto XmlElement::children( ) const -> const std::vector<XmlElement> {
    if ( node_->children == nullptr ) {
        return {};
    }
    std::vector<XmlElement> ret;
    for ( xmlNode *tmp = node_->children; tmp != nullptr; tmp = tmp->next ) {
        if ( tmp->type == XML_ELEMENT_NODE ) {
            ret.push_back( XmlElement{tmp} );
        }
    }
    return ret;
}

auto XmlElement::tags( ) const -> const std::pair<const std::string, const std::string> {
    if ( !node_ ) {
        return {};
    }
    auto nm = name( );
    std::string attrRep;
    if ( hasAttributes( ) ) {
        std::ostringstream attrsOut;
        const auto &attrs = attributes( );
        for ( const auto &attr : attrs ) {
            attrsOut << " " << attr.first << "='" << attr.second << "'"; // prepend space
        }
        attrRep = attrsOut.str( );
    }
    if ( empty( ) ) {
        return std::make_pair( "<" + nm + attrRep + "/>", std::string{} );
    }
    return std::make_pair( "<" + nm + attrRep + ">", "</" + nm + ">" );
}

auto XmlElement::tagsRegex( ) const
    -> const std::pair<const std::string, const std::string> {
    if ( !node_ ) {
        return {};
    }
    auto nm = name( );
    std::string attrRep;
    if ( hasAttributes( ) ) {
        std::ostringstream attrsOut;
        attrsOut << "\\b";
        const auto &attrs = attributes( );
        for ( const auto &attr : attrs ) {
            attrsOut << "\\s+\\b" << attr.first // key literal string
                     << "\\b\\s*"       // word border plus 0 or more whitespace chars
                     << "="             // literal char match on '='
                     << "\\s*('|\")\\b" // 0 or more whitespace chars
                                        // followed by either ' or "
                     << attr.second     // value literal string
                     << "\\b('|\")";    // word border, close quotes, 0 or more
                                        // whitespace chars, prepend space
        }
        attrRep = attrsOut.str( );
    }
    if ( empty( ) ) {
        return std::make_pair( "<" + nm + attrRep + "/>", std::string{} );
    }
    std::pair<std::string, std::string> ret( "<" + nm + attrRep + ">", "</" + nm + ">" );
    return ret;
}

auto XmlElement::empty( ) const -> bool {
    if ( node_ ) {
        return ( node_->children == nullptr );
    }
    return false;
}

static auto replaceAndDestroy( xmlNode *old, xmlNode *nn ) -> void {
    xmlReplaceNode( old, nn );
    xmlFreeNode( old );
    old = nn;
}

template <typename T>
static auto xmlTreeMap( xmlNode *node, const std::function<void( xmlNode *, T * )> &f,
                        T *arg ) -> void {
    assert( f );
    if ( nullptr == node ) {
        return;
    }
    xmlNode *tmp = node;

    while ( true ) {
        f( tmp, arg );
        if ( tmp->children ) {
            tmp = tmp->children;
        } else {
            while ( !tmp->next ) {
                if ( tmp == node || !tmp->parent ) {
                    return;
                }
                tmp = tmp->parent;
            }
            if ( tmp == node ) // target nodes that have siblings
                return;
            tmp = tmp->next;
        }
    }
}

static auto catContent( xmlNode *node, std::string *str ) -> void {
    if ( node->type == XML_TEXT_NODE ) {
        *str += reinterpret_cast<const char *>( node->content );
    }
}

static auto findContent( xmlNode *node,
                         std::pair<const std::string &, std::vector<xmlNode *>> *cl )
    -> void {
    if ( node->type == XML_TEXT_NODE ) {
        std::string content{reinterpret_cast<const char *>( node->content )};
        std::string::size_type n = content.find( cl->first );
        if ( n != std::string::npos ) {
            cl->second.push_back( node );
        }
    }
}

