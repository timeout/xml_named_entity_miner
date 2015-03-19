#include "xslt_dialog.hpp"
#include "line_edit.hpp"
#include "xslt_doc.hpp"
#include <fstream>
#include <sstream>
#include <QMessageBox>
#include <QProgressBar>
#include <QFileDialog>
#include <QString>

XsltDialog::XsltDialog( const XmlDoc &xml, QWidget *parent )
    : XmlTaskDialog{xml, parent} {
    lineEdit_->setText( "Enter Path to XSLT stylesheet file" );
    progressBar_->setMaximum( 4 );
    progressBar_->setMinimum( 0 );
    layout( );
    connections( );
    setWindowTitle( "XSLT Transformation Dialog" );
}

XsltDialog::~XsltDialog( ) {}

auto XsltDialog::transformedXml( ) const -> const XmlDoc& { return trans_; }

auto XsltDialog::showError( const QString &msg ) -> void {
    QMessageBox messageBox;
    messageBox.setIcon( QMessageBox::Warning );
    messageBox.setText( "Stylesheet transformation error" );
    messageBox.setInformativeText( msg );
    messageBox.setStandardButtons( QMessageBox::Discard );
    messageBox.setDefaultButton( QMessageBox::Discard );
    messageBox.exec( );
}

auto XsltDialog::open( ) -> void {
    const QString filename = QFileDialog::getOpenFileName(
        this, tr( "Open XSLT stylesheet" ), "/usr/local/home/joe",
        tr( "XSLT stylesheet files (*.xsl)" ) );
    if ( !filename.isEmpty( ) ) {
        lineEdit_->setText( filename );
        lineEdit_->setEnabled( false );
        parse( filename );
    }
}

auto XsltDialog::parse( const QString &filename ) -> void {
    std::string f = filename.toStdString( );
    std::ifstream fin{f, std::ios::in};
    if ( fin.fail( ) ) {
        QString msg{"Could not open file"};
        emit errorRaised( makeErrorMsg( msg, std::strerror( errno ) ) );
        return;
    }
    emit progress( 1 );
    // soldier on...
    XmlDoc xmlStylesheet{fin};
    if ( xmlStylesheet.errorHandler( ).hasErrors( ) ) {
        QString msg{"XSLT file is invalid"};
        std::ostringstream handlerOs;
        handlerOs << xmlStylesheet.errorHandler( );
        emit errorRaised( makeErrorMsg( msg, handlerOs.str( ) ) );
        return;
    }
    emit progress( 2 );
    XsltDoc stylesheet{xmlStylesheet};
    if ( !stylesheet ) {
        QString msg{"Could not create XSLT stylesheet"};
        emit errorRaised( msg );
        return;
    }
    emit progress( 3 );
    trans_ = stylesheet.transform( xml_ );
    if ( !trans_ ) {
        QString msg{"Stylesheet failed"};
        emit errorRaised( msg );
        return;
    }

    std::cerr << trans_ << std::endl;
    emit progress( 4 );
}

