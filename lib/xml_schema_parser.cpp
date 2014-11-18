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

XmlSchemaParser::XmlSchemaParser( ) : spc_{nullptr} {}

XmlSchemaParser::XmlSchemaParser( const XmlDoc &xml )
    : spc_{xmlSchemaNewDocParserCtxt( xml.get( ) )} {
    spch_.registerHandler( spc_.get( ) );
}

auto operator>>( const XmlDoc &xml, XmlSchemaParser &spc ) -> XmlSchemaParser & {
    using SchemaParserCtxtT = std::unique_ptr<xmlSchemaParserCtxt, FreeXmlSchemaParser>;
    SchemaParserCtxtT tmp{xmlSchemaNewDocParserCtxt( xml.get( ) )};
    spc.spc_ = std::move( tmp );
    spc.spch_.registerHandler( spc.get( ) );
    return spc;
}

