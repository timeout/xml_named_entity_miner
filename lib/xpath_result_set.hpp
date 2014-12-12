#pragma once

#include "xpath_query.hpp"
#include "xml_node.hpp"
#include "libxml/tree.h"
#include "libxml/xpath.h"
#include <limits>

class XPathResultSet {
public:
    XPathResultSet( ) : query_{}, resultSet_{nullptr}, sz_{0}, idx_{0} {}
    XPathResultSet( const XPathQuery &query )
        : query_{query},
          resultSet_{( query ) ? query.query_.get( )->nodesetval : nullptr},
          sz_{resultSet_ ? ( size_t )resultSet_->nodeNr : 0}, idx_{0} {} // ! nodeNr >= 0
    XPathResultSet( const XPathResultSet &rs )
        : query_{rs.query_},
          resultSet_{( rs.query_ ) ? query_.query_.get( )->nodesetval : nullptr},
          sz_{rs.sz_}, idx_{0} {}
    XPathResultSet( XPathResultSet &&rhs )
        : query_{std::move( rhs.query_ )}, 
          resultSet_{( query_ ) ? query_.query_.get( )->nodesetval : nullptr},
          sz_{rhs.sz_}, idx_{0} {
        rhs.resultSet_ = nullptr;
        rhs.sz_ = rhs.idx_ = 0;
    }
    auto operator=( const XPathResultSet &rhs ) -> XPathResultSet & {
        if ( this != &rhs ) {
            query_ = rhs.query_;
            resultSet_ = ( query_ ) ? query_.query_.get( )->nodesetval : nullptr;
            sz_ = rhs.sz_;
            idx_ = rhs.idx_;
        }
        return *this;
    }
    auto operator=( XPathResultSet &&rhs ) -> XPathResultSet & {

        query_ = std::move( rhs.query_ ); // leaking

        resultSet_ = ( query_ ) ? query_.query_.get( )->nodesetval : nullptr;
        sz_ = rhs.sz_;
        idx_ = rhs.idx_;
        rhs.resultSet_ = nullptr;
        rhs.sz_ = rhs.idx_ = 0;
        return *this;
    }
    auto size( ) const -> size_t { return sz_; }
    auto operator[]( size_t idx ) const -> const XmlNode {
        return XmlNode{resultSet_->nodeTab[idx]};
    }
    auto operator[]( size_t idx ) -> XmlNode { return XmlNode{resultSet_->nodeTab[idx]}; }

private:
    XPathQuery query_;
    xmlNodeSetPtr resultSet_;
    size_t sz_;
    size_t idx_;
};
