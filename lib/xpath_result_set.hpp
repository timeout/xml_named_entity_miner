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
    auto size( ) const -> size_t { return sz_; }
    auto operator[]( size_t idx ) -> XmlElement;

private:
    XPathQuery query_;
    xmlNodeSetPtr resultSet_;
    size_t sz_;
    size_t idx_;
};

