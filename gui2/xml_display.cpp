#include "xml_display.hpp"
#include "xml_display_highlighter.hpp"
#include "xml_doc.hpp"

XmlDisplay::XmlDisplay( QWidget *parent ) : QPlainTextEdit{parent} {

    // TODO:
    setTabStopWidth( 20 );
    xmlDisplayHighlighter_ = new XmlDisplayHighlighter( document( ) );
}

auto XmlDisplay::setXml( const XmlDoc &xml ) -> void {
    xmlDisplayHighlighter_->generateTagSet( xml );
    setPlainText( QString::fromStdString( xml.toString( ) ) );
}

