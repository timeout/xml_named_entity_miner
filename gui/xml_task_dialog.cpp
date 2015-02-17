#include "xml_task_dialog.hpp"
#include "line_edit.hpp"

#include "xml_doc.hpp"
#include "xml_schema.hpp"
#include "xml_schema_validator.hpp"
#include "xslt_doc.hpp"

#include <QLabel>
#include <QToolButton>
#include <QProgressBar>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QFileDialog>
#include <QErrorMessage>

#include <fstream>
#include <sstream>

#include <QDebug>
#include <iostream>

static const QString &makeErrorMsg( QString &msg, const std::string &handlerMsg );

XmlTaskDialog::XmlTaskDialog( const XmlDoc &xml, QWidget *parent )
    : QDialog{parent}, path_{new QLabel{tr( "Path: " )}}, lineEdit_{new LineEdit},
      fileButton_{new QToolButton}, progressBar_{new QProgressBar},
      buttonBox_{new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel )},
      xml_{xml} {
    path_->setBuddy( lineEdit_ );
    path_->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    QFontMetrics metrics{QApplication::font( )};
    lineEdit_->setMinimumWidth( metrics.averageCharWidth( ) * 24 );
    QIcon icon = QIcon::fromTheme( "document-open", QIcon( ":/images/document-open" ) );
    fileButton_->setIcon( icon );
}

XmlTaskDialog::~XmlTaskDialog( ) {}

void XmlTaskDialog::layout( ) {
    QGroupBox *groupBox = new QGroupBox{title_};
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget( path_ );
    topLayout->addWidget( lineEdit_ );
    topLayout->addWidget( fileButton_ );
    QHBoxLayout *middleLayout = new QHBoxLayout;
    middleLayout->addWidget( progressBar_ );
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    QVBoxLayout *groupBoxLayout = new QVBoxLayout;
    groupBoxLayout->addLayout( topLayout );
    groupBoxLayout->addLayout( middleLayout );
    groupBox->setLayout( groupBoxLayout );

    bottomLayout->addWidget( buttonBox_ );
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget( groupBox );
    mainLayout->addLayout( bottomLayout );
    setLayout( mainLayout );
}

void XmlTaskDialog::connections( ) {
    connect( fileButton_, SIGNAL( clicked( ) ), this, SLOT( open( ) ) );
    connect( this, SIGNAL( progress( int )), progressBar_, SLOT( setValue( int )) );
    connect( this, SIGNAL( errorRaised( const QString & )),
             SLOT( showError( const QString & )) );
}

XmlSchemaDialog::XmlSchemaDialog( const XmlDoc &xmlSchema, QWidget *parent )
    : XmlTaskDialog{xmlSchema, parent} {
    lineEdit_->setText( "Enter Path to XML Schema file" );
    progressBar_->setMaximum( 6 );
    progressBar_->setMinimum( 0 );
    layout( );
    connections( );
    setWindowTitle( "XML Schema Validation Dialog" );
}

XmlSchemaDialog::~XmlSchemaDialog( ) {}

void XmlSchemaDialog::showError( const QString &msg ) {
    QErrorMessage *errorMessage = new QErrorMessage{this};
    errorMessage->setWindowTitle( "Xml Schema error" );
    errorMessage->showMessage( msg );
}

void XmlSchemaDialog::open( ) {
    const QString filename = QFileDialog::getOpenFileName(
        this, tr( "Open XML Schema file" ), "/usr/local/home/joe",
        tr( "XML Schema Files (*.xsd)" ) );
    if ( !filename.isEmpty( ) ) {
        lineEdit_->setText( filename );
        lineEdit_->setEnabled( false );
        parse( filename );
    }
}

void XmlSchemaDialog::parse( const QString &filename ) {
    qDebug( ) << "parsing Xml Schema: " << filename;
    std::string f = filename.toStdString( );
    std::ifstream fin{f, std::ios::in};
    if ( fin.fail( ) ) {
        QString msg{"Could not open file"};
        emit errorRaised( makeErrorMsg( msg, std::strerror( errno ) ) );
        return;
    }
    emit progress( 1 );
    // soldier on...
    const XmlDoc xmlSchema{fin};
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

XsltDialog::XsltDialog( const XmlDoc &stylesheet, QWidget *parent )
    : XmlTaskDialog{stylesheet, parent} {
    lineEdit_->setText( "Enter Path to XSLT stylesheet file" );
    progressBar_->setMaximum( 4 );
    progressBar_->setMinimum( 0 );
    layout( );
    connections( );
    setWindowTitle( "XSLT Transformation Dialog" );
}

XsltDialog::~XsltDialog( ) {}

void XsltDialog::showError( const QString &msg ) {
    QErrorMessage *errorMessage = new QErrorMessage{this};
    errorMessage->setWindowTitle( "XSLT stylesheet error" );
    errorMessage->showMessage( msg );
}

void XsltDialog::open( ) {
    const QString filename = QFileDialog::getOpenFileName(
        this, tr( "Open XSLT stylesheet" ), "/usr/local/home/joe",
        tr( "XSLT stylesheet files (*.xsl)" ) );
    if ( !filename.isEmpty( ) ) {
        lineEdit_->setText( filename );
        lineEdit_->setEnabled( false );
        parse( filename );
    }
}

void XsltDialog::parse( const QString &filename ) {
    qDebug( ) << "parsing XSLT stylesheet: " << filename;
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
    XmlDoc transDoc = stylesheet.transform( xml_ );
    if ( !transDoc ) {
        QString msg{"Stylesheet failed"};
        emit errorRaised( msg );
        return;
    }
    std::cerr << transDoc << std::endl;
    emit progress( 4 );
}

static const QString &makeErrorMsg( QString &msg, const std::string &handlerMsg ) {
    std::cerr << "handler message " << handlerMsg << std::endl;
    if ( !handlerMsg.empty( ) ) {
        msg += "\n";
        msg += QString::fromStdString( handlerMsg );
    }
    qDebug( ) << msg;
    return msg;
}
