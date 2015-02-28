#include "xml_file_explorer.hpp"
#include "xml_element.hpp"
#include "xml_file_outline.hpp"
#include "xpath_query_widget.hpp"

#include <QGridLayout>

XmlFileExplorer::XmlFileExplorer( QWidget *parent )
    : QWidget{parent}
    , xmlFileOutline_{new XmlFileOutline{this}}
    , xPathQueryWidget_{new XPathQueryWidget{this}} {
    layout( );
    connections( );
}

auto XmlFileExplorer::clear( ) -> void {
    xml_ = XmlDoc{};
    xmlFileOutline_->clear( );
    xPathQueryWidget_->clear( );
}

auto XmlFileExplorer::setXml( const XmlDoc &xml ) -> void {
    if ( xml_ ) {
        xmlFileOutline_->clear( );
        xPathQueryWidget_->clear( );
    }
    xml_ = xml;
    xmlFileOutline_->xml( xml_ );
    xPathQueryWidget_->xml( xml_ );
}

auto XmlFileExplorer::layout( ) -> void {
    QGridLayout *layout = new QGridLayout;
    layout->addWidget( xmlFileOutline_, 0, 0 );
    layout->addWidget( xPathQueryWidget_, 1, 0 );
    layout->setRowStretch( 0, 3 );
    layout->setRowStretch( 1, 1 );
    layout->setContentsMargins( 0, 0, 0, 0 );
    setLayout( layout );
}

auto XmlFileExplorer::connections( ) -> void {
    connect( xPathQueryWidget_, &XPathQueryWidget::queryResult, xmlFileOutline_,
             &XmlFileOutline::xPathResult );
}
