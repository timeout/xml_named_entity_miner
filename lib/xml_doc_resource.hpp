#ifndef _XML_DOC_RESOURCE_HPP_
#define _XML_DOC_RESOURCE_HPP_

#include <memory>
#include <string>
#include <iosfwd>

#include <libxml/tree.h>
#include <libxml/parser.h>

namespace bilanzXml {

class FreeXmlDoc {
public:
    auto operator( )( xmlDoc *res ) const -> void { xmlFreeDoc( res ); }
};

class XmlDocResource {
public:
    XmlDocResource( const std::string &buf ) {
        std::string buffer(
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" );
        buffer += buf;
        xmlDoc *xml = xmlReadMemory( buffer.data( ), buffer.size( ),
                                     NULL, NULL, XML_PARSE_NONET );
        xmlDoc_ = XmlDocT( xml );
    }
    auto release( ) -> xmlDocPtr { return xmlDoc_.release( ); }
    auto get( ) const -> xmlDocPtr { return xmlDoc_.get( ); }

private:
    typedef std::unique_ptr<xmlDoc, FreeXmlDoc> XmlDocT;
    XmlDocT xmlDoc_;
};

} // namespace bilanzXml

#endif // _XML_DOC_RESOURCE_HPP_
