#include "xml_schema_dialog.hpp"

#include "xml_doc.hpp"
#include "xml_schema.hpp"
#include "xml_schema_validator.hpp"

#include <fstream>
#include <sstream>

#include "line_edit.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressBar>

XmlSchemaDialog::XmlSchemaDialog( const XmlDoc &xmlSchemaDoc, QWidget *parent )
    : XmlTaskDialog{xmlSchemaDoc, parent} {
    lineEdit_->setText( "Enter path to XML Schema file" );
    progressBar_->setMaximum( 6 );
    progressBar_->setMinimum( 0 );
    layout( );
    connections( );
    setWindowTitle( "XML Schema validation dialog" );
}

XmlSchemaDialog::~XmlSchemaDialog( ) {}

auto XmlSchemaDialog::showError( const QString &msg ) -> void {
    QMessageBox messageBox;
    messageBox.setIcon( QMessageBox::Warning );
    messageBox.setText( "Schema validation error" );
    messageBox.setInformativeText( msg );
    messageBox.setStandardButtons( QMessageBox::Discard );
    messageBox.setDefaultButton( QMessageBox::Discard );
    messageBox.exec( );
}

auto XmlSchemaDialog::open( ) -> void {
    auto filename = QFileDialog::getOpenFileName(
        this, tr( "Open XML Schema file" ), "/home", tr( "XML Schema Files (*.xsd)" ) );
    if ( !filename.isEmpty( ) ) {
        lineEdit_->setText( filename );
        lineEdit_->setEnabled( false );
        parse( filename );
    }
}

auto XmlSchemaDialog::parse( const QString &filename ) -> void {
    std::string file = filename.toStdString( );
    std::ifstream in{file};
    if ( in.fail( ) ) {
        QString msg{"Could not open file"};
        emit errorRaised( makeErrorMsg( msg, std::strerror( errno ) ) );
        return;
    }
    emit progress( 1 );
    const XmlDoc xmlSchema{in};
    if ( xmlSchema.errorHandler( ).hasErrors( ) ) {
        QString msg{"XML Schema file is invalid"};
        std::ostringstream handlerOs;
        handlerOs << xmlSchema.errorHandler( );
        emit errorRaised( makeErrorMsg( msg, handlerOs.str( ) ) );
        return;
    }
    emit progress( 2 );
    XmlSchemaParser validParser;
    xmlSchema >> validParser;
    if ( validParser.errorHandler( ).hasErrors( ) ) {
        QString msg{"Could not parse XML Schema file"};
        std::ostringstream handlerOs;
        handlerOs << validParser.errorHandler( );
        emit errorRaised( makeErrorMsg( msg, handlerOs.str( ) ) );
        return;
    }
    emit progress( 3 );
    XmlSchema schema;
    validParser >> schema;
    if ( schema.errorHandler( ).hasErrors( ) ) {
        QString msg;
        std::ostringstream handlerOs;
        handlerOs << schema.errorHandler( );
        emit errorRaised( makeErrorMsg( msg, handlerOs.str( ) ) );
        return;
    }
    emit progress( 4 );
    XmlSchemaValidator validator;
    schema >> validator;
    if ( validator.errorHandler( ).hasErrors( ) ) {
        QString msg{"Could not create XML Schema validator"};
        std::ostringstream handlerOs;
        handlerOs << validator.errorHandler( );
        emit errorRaised( makeErrorMsg( msg, handlerOs.str( ) ) );
        return;
    }
    emit progress( 5 );
    if ( validator.validate( xml_ ) ) {
        emit progress( 6 );
    } else {
        QString msg{"Could not validate"};
        std::ostringstream handlerOs;
        handlerOs << validator.errorHandler( );
        emit errorRaised( makeErrorMsg( msg, handlerOs.str( ) ) );
    }
}

