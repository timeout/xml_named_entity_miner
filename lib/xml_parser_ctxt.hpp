#pragma once

#include <memory>
#include <libxml/tree.h>
#include <libxml/parser.h>

class FreeXmlParserCtxt {
public:
    auto operator( )( xmlParserCtxt *xmlParserCtxt ) const -> void {
        xmlFreeParserCtxt( xmlParserCtxt );
    }
};

class XmlParserCtxt {
public:
    XmlParserCtxt( ) : xmlParserCtxt_{xmlNewParserCtxt( )} {}

    auto release( ) -> xmlParserCtxtPtr { return xmlParserCtxt_.release( ); }
    auto get( ) const -> xmlParserCtxtPtr { return xmlParserCtxt_.get( ); }

private:
    using XmlParserCtxtT = std::unique_ptr<xmlParserCtxt, FreeXmlParserCtxt>;
    XmlParserCtxtT xmlParserCtxt_;
};

