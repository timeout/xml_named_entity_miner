#include "xpath_result_set.hpp"
#include "libxml/xpath.h"

XPathResultSet::XPathResultSet( ) : query_{}, resultSet_{nullptr}, sz_{0}, idx_{0} {}

XPathResultSet::XPathResultSet( const XPathQuery &query ) : query_{query} {
    if ( query_ && query_.query_->nodesetval ) {
        resultSet_ = query_.query_->nodesetval;
        sz_ = resultSet_->nodeNr;
    } else {
        resultSet_ = nullptr;
        sz_ = 0;
    }
    idx_ = 0;
}

XPathResultSet::XPathResultSet( const XPathResultSet &other ) : query_{other.query_} {
    if ( query_ && query_.query_->nodesetval ) {
        resultSet_ = query_.query_->nodesetval;
        sz_ = resultSet_->nodeNr;
    } else {
        resultSet_ = nullptr;
        sz_ = 0;
    }
    idx_ = other.idx_;
}

XPathResultSet::XPathResultSet( XPathResultSet &&rhs )
    : query_{std::move( rhs.query_ )}, resultSet_{rhs.resultSet_}, sz_{rhs.sz_},
      idx_{rhs.idx_} {}

auto XPathResultSet::operator=( const XPathResultSet &rhs ) -> XPathResultSet & {
    if ( this != &rhs ) {
        query_ = rhs.query_;
        if ( query_ && query_.query_->nodesetval ) {
            resultSet_ = query_.query_->nodesetval;
            sz_ = resultSet_->nodeNr;
        } else {
            resultSet_ = nullptr;
            sz_ = 0;
        }
        idx_ = rhs.idx_;
    }
    return *this;
}
auto XPathResultSet::operator=( XPathResultSet &&rhs ) -> XPathResultSet & {
    query_ = std::move( rhs.query_ );
    resultSet_ = rhs.resultSet_;
    sz_ = rhs.sz_;
    idx_ = rhs.idx_;
    return *this;
}

auto XPathResultSet::operator[]( size_t idx ) -> XmlElement {
    if ( resultSet_ && ( idx_ <= sz_ ) ) {
        idx_ = idx;
        return XmlElement{resultSet_->nodeTab[idx_]};
    }
    // return XmlNode{nullptr};
}

// private:
//     XPathQuery query_;
//     xmlNodeSetPtr resultSet_;
//     size_t sz_;
//     size_t idx_;
// };

