#include "xml_element.hpp"
#include <iostream>
#include <cassert>
#include <sstream>

XmlElement::XmlElement( xmlNode *node ) : node_{node, FreeXmlElement( )} {}

XmlElement::XmlElement( const std::string &name )
    : node_{
          xmlNewNode( nullptr, reinterpret_cast<const unsigned char *>( name.c_str( ) ) ),
          FreeXmlElement( )} {}

XmlElement::XmlElement( const XmlElement &other ) { node_ = other.node_; }

XmlElement::XmlElement( XmlElement &&other ) : node_{std::move( other.node_ )} {
    other.node_ = nullptr;
}

XmlElement::~XmlElement( ) {}

XmlElement::operator bool( ) const { return node_ != nullptr; }

auto XmlElement::operator=( const XmlElement &rhs ) -> XmlElement & {
    if ( this != &rhs ) {
        node_ = rhs.node_;
    }
    return *this;
}

auto XmlElement::operator=( XmlElement &&rhs ) -> XmlElement & {
    node_ = std::move( rhs.node_ );
    rhs.node_ = nullptr;
    return *this;
}

auto XmlElement::operator==( const XmlElement &rhs ) const -> bool {
    return node_ == rhs.node_;
}

auto XmlElement::operator!=( const XmlElement &rhs ) const -> bool {
    return node_ != rhs.node_;
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
    const xmlNode *tmp = node_.get( );
    while ( true ) {
        if ( tmp->type == XML_ELEMENT_NODE && tmp == lhs.node_.get( ) ) {
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

auto XmlElement::name( ) const -> std::string {
    if ( node_ ) {
        return std::string{reinterpret_cast<const char *>( node_->name )};
    }
    return std::string{};
}

auto XmlElement::name( const std::string &name ) -> void {
    if ( node_ && !name.empty( ) ) {
        xmlNodeSetName( node_.get( ),
                        reinterpret_cast<const unsigned char *>( name.c_str( ) ) );
    }
}

auto XmlElement::hasAttributes( ) const -> bool { return ( node_ && node_->properties ); }

auto XmlElement::attributes( ) const -> std::map<std::string, std::string> {
    std::map<std::string, std::string> ret;
    if ( node_ ) {
        for ( auto attr = node_->properties; attr; attr = attr->next ) {
            auto prop = xmlGetProp( node_.get( ), attr->name );
            ret.emplace( std::string{reinterpret_cast<const char *>( attr->name )},
                         std::string{reinterpret_cast<const char *>( prop )} );
            xmlFree( prop ); // nasty
        }
    }
    return ret;
}

auto XmlElement::attribute( const std::string &key, const std::string &value ) -> void {
    if ( !node_ ) {
        return;
    }

    auto libxmlKey = reinterpret_cast<const unsigned char *>( key.c_str( ) );
    auto libxmlValue = reinterpret_cast<const unsigned char *>( value.c_str( ) );
    if ( node_->properties && xmlHasProp( node_.get( ), libxmlKey ) ) {
        xmlSetProp( node_.get( ), libxmlKey, libxmlValue );
    } else {
        xmlNewProp( node_.get( ), libxmlKey, libxmlValue );
    }
}

auto XmlElement::attribute( const std::string &key ) const -> std::string {
    if ( node_ && node_->properties ) {
        for ( auto attr = node_->properties; attr; attr = attr->next ) {
            if ( xmlStrEqual( attr->name, reinterpret_cast<const unsigned char *>(
                                              key.c_str( ) ) ) == 1 ) {
                auto value = xmlGetProp( node_.get( ), attr->name );
                std::string ret{reinterpret_cast<const char *>( value )};
                xmlFree( value ); // nasty
                return ret;
            }
        }
    }
    return std::string{};
}

auto XmlElement::removeAttribute( const std::string &key ) -> void {
    if ( node_ && node_->properties ) {
        xmlUnsetProp( node_.get( ),
                      reinterpret_cast<const unsigned char *>( key.c_str( ) ) );
    }
}

auto XmlElement::attributes( const std::map<std::string, std::string> &attrMap ) -> void {
    for ( auto attr = attrMap.begin( ); attr != attrMap.end( ); ++attr ) {
        attribute( attr->first, attr->second );
    }
}

auto XmlElement::hasContent( ) const -> bool {
    if ( !node_ ) {
        return false;
    }
    const xmlNode *tmp = node_.get( );
    while ( true ) {
        if ( tmp->type == XML_TEXT_NODE ) {
            return true;
        }
        if ( tmp->children ) {
            tmp = tmp->children;
        } else {
            while ( !tmp->next ) {
                if ( tmp == node_.get( ) ) {
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
        xmlTreeMap<std::string>( node_.get( ), catContent, &ret );
        return ret;
    }
    return {};
}

auto XmlElement::content( const std::string &text ) -> void {
    if ( node_ ) {
        xmlNodeAddContent( node_.get( ),
                           reinterpret_cast<const unsigned char *>( text.c_str( ) ) );
    }
}

auto XmlElement::hasChild( ) const -> bool {
    if ( node_->children ) {
        for ( auto child = node_->children; child; child = child->next ) {
            if ( child->type == XML_ELEMENT_NODE ) {
                return true;
            }
        }
    }
    return false;
}

auto XmlElement::child( ) const -> XmlElement {
    if ( node_->children ) {
        for ( auto child = node_->children; child; child = child->next ) {
            if ( child->type == XML_ELEMENT_NODE ) {
                return XmlElement{child};
            }
        }
    }
    return XmlElement{};
}

auto XmlElement::child( const XmlElement &element ) -> void {
    auto child = xmlAddChild( node_.get( ), element.node_.get( ) );
    if ( !child ) {
        std::cerr << __PRETTY_FUNCTION__ << " boom!" << std::endl;
    }
}

auto XmlElement::hasSibling( ) const -> bool {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
    if ( node_ ) {
        for ( auto sibling = node_->next; sibling; sibling = sibling->next ) {
            if ( sibling->type == XML_ELEMENT_NODE ) {
                return true;
            }
        }
    }
    return false;
}

auto XmlElement::sibling( ) const -> XmlElement {
    if ( node_ && node_->parent ) {
        for ( auto sibling = node_->next; sibling; sibling = sibling->next ) {
            if ( sibling->type == XML_ELEMENT_NODE ) {
                return XmlElement{sibling};
            }
        }
    }
    return XmlElement{};
}

auto XmlElement::sibling( const XmlElement &element ) -> void {
    auto sibling = xmlAddSibling( node_.get( ), element.node_.get( ) );
    if ( !sibling ) {
        std::cerr << __PRETTY_FUNCTION__ << ": boom!" << std::endl;
    }
}

auto XmlElement::nextSibling( const XmlElement &element ) -> void {
    auto node = xmlAddNextSibling( node_.get( ), element.node_.get( ) );
}

auto XmlElement::prevSibling( const XmlElement &element ) -> void {
    auto node = xmlAddPrevSibling( node_.get( ), element.node_.get( ) );
}

auto XmlElement::hasParent( ) const -> bool { return node_ && node_->parent; }

auto XmlElement::parent( ) const -> XmlElement {
    if ( node_ && node_->parent ) {
        return XmlElement{node_->parent};
    }
    return XmlElement{};
}

auto XmlElement::hasPrev( ) const -> bool {
    if ( node_ ) {
        for ( auto previous = node_->prev; previous; previous = previous->prev ) {
            if ( previous->type == XML_ELEMENT_NODE ) {
                return true;
            }
        }
    }
    return true;
}

auto XmlElement::prev( ) const -> XmlElement {
    if ( node_ ) {
        for ( auto previous = node_->prev; previous; previous = previous->prev ) {
            if ( previous->type == XML_ELEMENT_NODE ) {
                return XmlElement{previous};
            }
        }
    }
    return XmlElement{};
}

auto XmlElement::toString( ) const -> std::string {
    if ( node_ ) {
        xmlBufferPtr buff = nullptr;
        if ( ( buff = xmlBufferCreate( ) ) == nullptr ) {
            // TODO:
            std::cerr << "Could not allocate xml buffer" << std::endl;
            return std::string{};
        }
        std::cerr << "dumping node: " << name( ) << std::endl;
        int flag = 0;
        if ( ( flag = xmlNodeDump( buff, nullptr, node_.get( ), 0, 1 ) ) < 0 ) {
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

auto XmlElement::tags(
    bool regex ) const -> const std::pair<const std::string, const std::string> {
    if ( !node_ ) {
        return {};
    }
    auto nm = name( );
    std::string attrRep;
    if ( hasAttributes( ) ) {
        std::ostringstream attrsOut;
        const auto &attrs = attributes( );
        for ( const auto &attr : attrs ) {
            if ( regex ) {
                attrsOut << "\\b\\s+\\b" << attr.first // key literal string
                         << "\\b\\s*"       // word border plus 0 or more whitespace chars
                         << "="             // literal char match on '='
                         << "\\s*('|\")\\b" // 0 or more whitespace chars
                                            // followed by either ' or "
                         << attr.second     // value literal string
                         << "\\b('|\")";    // word border, close quotes, 0 or more
                                            // whitespace chars, prepend space
            } else {
                attrsOut << " " << attr.first << "='" << attr.second
                         << "'"; // prepend space
            }
        }
        attrRep = attrsOut.str( );
    }
    if ( !hasChild( ) ) {
        return std::make_pair( "<" + nm + attrRep + "/>", std::string{} );
    }
    std::pair<std::string, std::string> ret( "<" + nm + attrRep + ">", "</" + nm + ">" );
    return ret;
}

auto XmlElement::unlink( Unlink link ) -> XmlElement {
    switch ( link ) {
    case Unlink::Child:
        if ( hasChild( ) ) {
            for ( auto unlink = node_->children; unlink; unlink = unlink->next ) {
                if ( unlink->type == XML_ELEMENT_NODE ) {
                    xmlUnlinkNode( unlink );
                    return XmlElement{unlink};
                }
            }
        }
        return XmlElement{};
        break;
    case Unlink::Sibling:
        if ( hasSibling( ) ) {
            for ( auto unlink = node_->next; unlink; unlink = unlink->next ) {
                if ( unlink->type == XML_ELEMENT_NODE ) {
                    xmlUnlinkNode( unlink );
                    return XmlElement{unlink};
                }
            }
        }
        return XmlElement{};
        break;
    case Unlink::Prev:
        if ( hasPrev( ) ) {
            for ( auto unlink = node_->prev; unlink; unlink = unlink->prev ) {
                if ( unlink->type == XML_ELEMENT_NODE ) {
                    xmlUnlinkNode( unlink );
                    return XmlElement{unlink};
                }
            }
        }
        return XmlElement{};
        break;
    default:
        assert( 0 ); // boom!
        return XmlElement{};
    }
    if ( node_->parent ) {
        xmlUnlinkNode( node_.get( ) );
        xmlFreeNode( node_.get( ) );
        node_ = nullptr;
    }
}

auto XmlElement::swap( XmlElement &element ) -> void {
    std::swap( node_, element.node_ );
}

template <>
auto std::swap( XmlElement &lhs, XmlElement &rhs ) -> void {
    lhs.swap( rhs );
}

auto XmlElement::clone( const XmlElement &element ) -> void {
    node_.reset( xmlCopyNode( element.node_.get( ), 1 ), FreeXmlElement( ) );
}

auto XmlElement::clear( ) -> void {
    if ( node_->children ) {
        xmlNode *tmp = node_->children;
        while ( tmp ) {
            xmlNode *node = tmp;
            tmp = tmp->next;
            xmlUnlinkNode( node );
            xmlFreeNode( node );
        }
    }
}

auto XmlElement::copy( const XmlElement &other ) -> void {
    if ( other.node_->children ) {
        xmlNode *list = xmlCopyNodeList( other.node_->children );
        if ( node_->children ) {
            node_->last = list;
        } else {
            node_->children = list;
        }
    }
}

template <typename T>
auto XmlElement::xmlTreeMap( xmlNode *node,
                             const std::function<void( xmlNode *, T * )> &f, T *arg )
    -> void {
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

auto XmlElement::catContent( xmlNode *node, std::string *str ) -> void {
    if ( node->type == XML_TEXT_NODE ) {
        *str += reinterpret_cast<const char *>( node->content );
    }
}

auto XmlElement::findContent( xmlNode *node,
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

