#pragma once
#include <memory>
#include <string>
#include <libxml/xpath.h>

#include <iostream>

class FreeXPathExpression {
public:
    auto operator( )( xmlXPathCompExpr *exprPtr ) const -> void {
        xmlXPathFreeCompExpr( exprPtr );
    }
};

class XPathExpression {
public:
    explicit XPathExpression( xmlXPathContext *xPathCtxt, const std::string &expr )
        : expr_{xmlXPathCtxtCompile(
              xPathCtxt, reinterpret_cast<const unsigned char *>( expr.c_str( ) ) )} {}

    explicit operator bool( ) const { return expr_.get( ) != nullptr; }
    auto get( ) const -> xmlXPathCompExpr * { return expr_.get( ); }

private:
    using XPathExpressionT = std::unique_ptr<xmlXPathCompExpr, FreeXPathExpression>;
    XPathExpressionT expr_;
};
