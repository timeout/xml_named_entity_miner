#pragma once
#include <libxml/tree.h>
#include <string>
#include <vector>
#include <map>
#include <QMetaType>

class QTreeWidgetItem;

class XmlElement {
public:
    XmlElement( ) : node_{nullptr} {} // default constructor for QVariant

    XmlElement( xmlNode *node );
    explicit operator bool( ) const;
    auto name( ) const -> std::string;
    auto name( const std::string &name ) -> std::string;
    auto hasAttributes( ) const -> bool;
    auto attribute( const std::string &key, const std::string &value ) -> void;
    // auto attributes( ) const -> std::map<std::string, std::string>;
    auto attributes( ) const -> std::vector<std::pair<std::string, std::string>>;
    auto attributes( const std::map<std::string, std::string> &attrs )
        -> std::map<std::string, std::string>;
    auto hasChild( ) const -> bool;
    auto child( ) const -> XmlElement;
    auto child( const std::string &name ) -> XmlElement;
    auto child( const std::string &name, const std::map<std::string, std::string> &attrs )
        -> XmlElement;
    auto hasSibling( ) const -> bool;
    auto sibling( ) const -> XmlElement;
    auto sibling( const std::string &name ) -> XmlElement;
    auto sibling( const std::string &name,
                  const std::map<std::string, std::string> &attrs ) -> XmlElement;
    auto hasContent( ) const -> bool;
    auto content( ) const -> std::string;
    auto content( const std::string &content ) -> void;
    // auto markupEntity(const std::string &entity) -> void;
    auto toString( ) const -> std::string;

    // dev
    auto search( const std::string &needle, const std::string &entityType ) const -> void;
    auto xpaths( ) const -> const std::vector<std::string>;
    auto children( ) const -> const std::vector<XmlElement>;
    auto tags( ) const -> const std::pair<const std::string, const std::string>;
    auto tagsRegex( ) const -> const std::pair<const std::string, const std::string>;
    auto empty( ) const -> bool;

    auto get( ) const -> const xmlNode * { return node_; }

private:
    xmlNode *node_;
};

Q_DECLARE_METATYPE( XmlElement )

