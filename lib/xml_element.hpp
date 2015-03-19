#pragma once
#include <libxml/tree.h>
#include <string>
#include <iterator>
#include <map>
#include <vector>
#include <memory>
#include <QMetaType>

struct FreeXmlElement {
    auto operator( )( xmlNode *node ) const -> void {
        if ( node && !node->parent ) {
            xmlFreeNode( node );
        }
    }
};

class XmlElement {
    friend class XmlDoc;

public:
    XmlElement( xmlNode *node = nullptr );
    XmlElement( const std::string &name );
    virtual ~XmlElement( );
    XmlElement( const XmlElement &other );
    XmlElement( XmlElement &&other );

    explicit operator bool( ) const;

    enum class Unlink { Child, Sibling, Prev };

    auto operator=( const XmlElement &rhs ) -> XmlElement &;
    auto operator=( XmlElement &&rhs ) -> XmlElement &;

    auto operator==( const XmlElement &rhs ) const -> bool;
    auto operator!=( const XmlElement &rhs ) const -> bool;
    auto operator<( const XmlElement &lhs ) const -> bool;
    auto operator>( const XmlElement &lhs ) const -> bool;
    auto operator<=( const XmlElement &lhs ) const -> bool;
    auto operator>=( const XmlElement &lhs ) const -> bool;

    auto name( ) const -> std::string;
    auto name( const std::string &name ) -> void;

    auto hasAttributes( ) const -> bool;
    auto attribute( const std::string &key, const std::string &value ) -> void;
    auto attribute( const std::string &key ) const -> std::string;
    auto removeAttribute( const std::string &key ) -> void;
    auto attributes( ) const -> std::map<std::string, std::string>;
    auto attributes( const std::map<std::string, std::string> &attrMap ) -> void;

    auto hasContent( ) const -> bool;
    auto content( ) const -> std::string;
    auto content( const std::string &text ) -> void;

    auto hasChild( ) const -> bool;
    auto child( ) const -> XmlElement;
    auto child( const XmlElement &element ) -> void;

    auto hasSibling( ) const -> bool;
    auto sibling( ) const -> XmlElement;
    auto sibling( const XmlElement &element ) -> void;
    auto nextSibling( const XmlElement &element ) -> void;
    auto prevSibling( const XmlElement &element ) -> void;

    auto hasParent( ) const -> bool;
    auto parent( ) const -> XmlElement;

    auto hasPrev( ) const -> bool;
    auto prev( ) const -> XmlElement;

    auto toString( ) const -> std::string;
    auto tags(
        bool regex ) const -> const std::pair<const std::string, const std::string>;

    auto unlink( Unlink link ) -> XmlElement;
    auto swap( XmlElement &element ) -> void;
    auto clone( const XmlElement &element ) -> void;
    auto clear( ) -> void;
    auto copy( const XmlElement &other ) -> void;

protected:
    template <typename T>
    static auto xmlTreeMap( xmlNode *node, const std::function<void( xmlNode *, T * )> &f,
                            T *arg ) -> void;

private:
    static auto catContent( xmlNode *node, std::string *str ) -> void;
    static auto findContent( xmlNode *node,
                             std::pair<const std::string &, std::vector<xmlNode *>> *cl )
        -> void;
    using XmlElementT = std::shared_ptr<xmlNode>;
    XmlElementT node_;
};

inline auto operator<<( std::ostream &os, const XmlElement &element ) -> std::ostream & {
    os << element.toString( );
    return os;
}

namespace std {
    template <>
    auto swap( XmlElement &lhs, XmlElement &rhs ) -> void;
}

Q_DECLARE_METATYPE( XmlElement )

