#include "xml_display.hpp"
#include "xml_highlighter.hpp"
#include "xml_doc.hpp"

#include <set>
#include <queue>

XmlDisplay::XmlDisplay( QWidget *parent ) : QPlainTextEdit{parent} {

    // TODO:
    setTabStopWidth( 20 );

    // TODO:
    // hookup highlighter
    xmlHighlighter = new XmlHighlighter( document( ) );
}

void XmlDisplay::setXml( const XmlDoc &xml ) {
    // send highlighter a set of xml tag names to match
    std::set<std::string> tagSet;
    std::queue<XmlElement> elementQueue;
    XmlElement root = xml.getRootElement( );
    elementQueue.push( root );
    while ( !elementQueue.empty( ) ) {
        XmlElement parent{elementQueue.front( )};
        elementQueue.pop( );
        tagSet.insert( parent.tagsRegex( ).first );
        if ( !parent.tagsRegex( ).second.empty( ) ) {
            tagSet.insert( parent.tagsRegex( ).second );
        }
        for ( auto child : parent.children( ) ) {
            elementQueue.push( child );
        }
    }
    xmlHighlighter->setTagSet( tagSet );

    // set plain text
    setPlainText( QString::fromUtf8( xml.toString( ).c_str( ) ) );
}

