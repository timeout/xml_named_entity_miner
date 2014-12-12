#include "xpath_query.hpp"

auto FreeXPathQuery::operator( )( xmlXPathObject *queryObject ) const -> void {
    xmlXPathFreeObject( queryObject );
};

XPathQuery::XPathQuery( ) : query_{nullptr} {}
XPathQuery::XPathQuery( const XPathCtxt &ctxt )
    : query_{nullptr}, queryCtxt_{ctxt} {}
XPathQuery::XPathQuery( const XPathQuery &query ) : queryCtxt_{query.queryCtxt_} {
    if ( query.query_ ) {
        query_.reset( xmlXPathObjectCopy( query.query_.get( ) ) );
    } else {
        query_.reset( nullptr );
    }
}
XPathQuery::XPathQuery( XPathQuery &&query )
    : query_{std::move( query.query_ )}, queryCtxt_{std::move( query.queryCtxt_ )} {}
auto XPathQuery::operator=( const XPathQuery &rhs ) -> XPathQuery & {
    std::cerr << "assigning with the assignment operator= XPathQuery" << std::endl;
    if ( this != &rhs ) {
        queryCtxt_ = rhs.queryCtxt_;
        if ( rhs.query_ ) {
            // query_.reset( xmlXPathObjectCopy( rhs.query_.get( ) ) );
        } else {
            // query_.reset( nullptr );
        }
    }
    return *this;
}
auto XPathQuery::operator=( XPathQuery &&rhs ) -> XPathQuery & {
    std::cerr << "moving with the assignment operator= XPathQuery" << std::endl;
    query_ = std::move( rhs.query_ );
    queryCtxt_ = std::move( rhs.queryCtxt_ );
    return *this;
}
XPathQuery::operator bool( ) const { return ( query_ != nullptr ); }
auto XPathQuery::query( const std::string &xpath ) -> void {
    if ( queryCtxt_ ) {
        const xmlChar *expr = ( xmlChar * )xpath.c_str( );
        if ( query_ ) {
            query_.reset( xmlXPathEvalExpression( expr, queryCtxt_.xpathCtxt_.get( ) ) );
        } else {
            XPathQueryT tmp{xmlXPathEvalExpression( expr, queryCtxt_.xpathCtxt_.get( ) )};
            query_ = std::move( tmp );
        }
    }
}

