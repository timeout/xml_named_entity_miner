#include "xml_schema_parser.hpp"

// SchemaParser::SchemaParser( const std::string &filename ) : filename_( filename ) {
//     schemapctxt_ = xmlSchemaNewParserCtxt( filename.data( ) );
//     handler_.registerHandler( schemapctxt_ );
//     if ( schemapctxt_ == NULL ) {
//         std::ostringstream out( "SchemaException: schema parser creation error: [ ",
//                                 std::ios_base::ate );
//         out << filename << " ]";
//         if ( handler_.hasErrors( ) ) {
//             handler_.stream_to( out );
//         }
//     }
// }

XmlSchemaParserCtxt::XmlSchemaParserCtxt( ) : spc_{nullptr} {}

XmlSchemaParserCtxt::XmlSchemaParserCtxt( const XmlDoc &xml )
    : spc_{xmlSchemaNewDocParserCtxt( xml.xmlDoc_.get( ) )} {
    spch_.registerHandler( spc_.get( ) );
}

auto operator>>( const XmlDoc &xml, XmlSchemaParserCtxt &spc ) -> XmlSchemaParserCtxt & {
    using SchemaParserCtxtT = std::unique_ptr<xmlSchemaParserCtxt, FreeXmlSchemaParserCtxt>;
    SchemaParserCtxtT tmp{xmlSchemaNewDocParserCtxt( xml.get( ) )};
    spc.spc_ = std::move( tmp );
    spc.spch_.registerHandler( spc.get( ) );
    return spc;
}

