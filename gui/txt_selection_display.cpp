#include "txt_selection_display.hpp"
#include "xml_element.hpp"
#include <QString>

#include <iostream>

TxtSelectionDisplay::TxtSelectionDisplay( QWidget *parent ) : QPlainTextEdit( parent ) {}

void TxtSelectionDisplay::setXmlTxt( const XmlElement &element ) {
    auto txt = element.content( );
    setPlainText( QString::fromUtf8( txt.c_str( ) ) );
}

