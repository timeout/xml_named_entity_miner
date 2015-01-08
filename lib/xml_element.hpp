#pragma once
#include <libxml/tree.h>
#include <string>
#include <map>

class XmlElement {
public:
    XmlElement( xmlNode *node );
    explicit operator bool( ) const;
    auto name( ) const -> std::string;
    auto name( const std::string &name ) -> std::string;
    auto hasAttributes( ) const -> bool;
    auto attribute( const std::string &key, const std::string &value ) -> void;
    auto attributes( ) const -> std::map<std::string, std::string>;
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

    auto search( const std::string &needle, const std::string& entityType ) const -> void;

private:
    xmlNode *node_;
};
