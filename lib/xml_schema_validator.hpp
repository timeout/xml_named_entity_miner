#pragma once

#include "xml_schema.hpp"
#include "xml_doc.hpp"
#include "libxml2_error_handlers.hpp"

#include <libxml/xmlschemas.h>

#include <string>
#include <vector>

class FreeXmlSchemaValidator {
public:
    auto operator( )( xmlSchemaValidCtxt *validator ) const -> void {
        xmlSchemaFreeValidCtxt( validator );
    }
};

class XmlSchemaValidator {
public:
    XmlSchemaValidator( ) : schemaValidator_{nullptr} {}
    explicit XmlSchemaValidator( const XmlSchema &schema ) {
        schemaValidator_ = loadSchema( schema, validatorHandler_ );
    }

    XmlSchemaValidator( const XmlSchemaValidator & ) = delete;
    XmlSchemaValidator &operator=( const XmlSchemaValidator & ) = delete;

    friend auto operator>>( const XmlSchema &schema, XmlSchemaValidator &validator )
        -> XmlSchemaValidator & {
        validator.schemaValidator_ = loadSchema( schema, validator.validatorHandler_ );
        return validator;
    }
    auto validate( const XmlDoc &doc ) const -> bool {
        return ( xmlSchemaValidateDoc( schemaValidator_.get( ), doc.get( ) ) == 0 );
    }
    auto errorHandler( ) const -> const IErrorHandler & { return validatorHandler_; }

private:
    using XmlSchemaValidatorT =
        std::unique_ptr<xmlSchemaValidCtxt, FreeXmlSchemaValidator>;
    static auto loadSchema( const XmlSchema &schema,
                            XmlSchemaValidatorErrorHandler &errorHandler )
        -> XmlSchemaValidatorT {
        XmlSchemaValidatorT tmp{xmlSchemaNewValidCtxt( schema.get( ) )};
        errorHandler.registerHandler( tmp.get() ); // moved into class
        return tmp;
    }

    XmlSchemaValidatorT schemaValidator_;
    XmlSchemaValidatorErrorHandler validatorHandler_;
};

