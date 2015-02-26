#pragma once

#include "xpath_query.hpp"
#include "xml_element.hpp"
#include "libxml/tree.h"
#include <limits>

class XPathResultSet {
public:
    XPathResultSet( );
    XPathResultSet( const XPathQuery &query );
    XPathResultSet( const XPathResultSet &other );
    XPathResultSet( XPathResultSet &&rhs );
    auto operator=( const XPathResultSet &rhs ) -> XPathResultSet &;
    auto operator=( XPathResultSet &&rhs ) -> XPathResultSet &;
    auto size( ) const -> std::size_t { return sz_; }
    auto operator[]( std::size_t idx ) -> XmlElement;

    class iterator : public std::iterator<std::random_access_iterator_tag, XmlElement> {
        xmlNode **set_;
        std::size_t idx_;
        int size_;

    public:
        iterator( xmlNodeSet *nodeSet = nullptr );
        auto operator*( ) -> XmlElement;
        auto operator++( ) -> iterator &;
        auto operator++( int ) -> iterator;
        auto operator--( ) -> iterator &;
        auto operator--( int ) -> iterator;
        auto operator[]( int index ) -> XmlElement;
        auto operator==( const iterator &other ) const -> bool;
        auto operator!=( const iterator &other ) const -> bool;
        auto operator<( const iterator &other ) const -> bool;
        auto operator>( const iterator &other ) const -> bool;
        auto operator<=( const iterator &other ) const -> bool;
        auto operator>=( const iterator &other ) const -> bool;
    };

    auto begin( ) -> iterator { return iterator{resultSet_}; }
    auto end( ) -> iterator { return iterator{}; }

private:
    XPathQuery query_;
    xmlNodeSetPtr resultSet_;
    size_t sz_;
    size_t idx_;
};

