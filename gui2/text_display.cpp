#include "text_display.hpp"
#include <QString>
#include <QDebug>
#include <vector>
#include <algorithm>
#include <QApplication>

#include <iostream>

TextDisplay::TextDisplay( const XmlElement &element, QWidget *parent )
    : QPlainTextEdit{parent}, lock_{true} {

    xmlElement_.clone( element );
    text_ = QString::fromStdString( xmlElement_.content( ) );
    setPlainText( text_ );

    setReadOnly( true ); // lock_
    highlighter_ = new TextDisplayHighlighter( document( ) );
    connections( );
}

TextDisplay::~TextDisplay( ) {}

auto TextDisplay::text( ) const -> const QString & { return text_; }

auto TextDisplay::addHighlightRule( const QString &entity, const QColor &color ) -> void {
    highlighter_->addRule( entity, color );
}

auto TextDisplay::scanKeyword( const QString &ontologyName, const QString &entity,
                               const QColor &color ) -> void {
    std::cerr << "scanning for keyword: " << entity.toStdString( ) << std::endl;
    auto text = toPlainText( ).toStdString( );
    auto search = text.find( entity.toStdString( ) );
    if ( search != std::string::npos ) {
        keywords_.insert(
            std::make_pair( entity.toStdString( ), ontologyName.toStdString( ) ) );
        addHighlightRule( entity, color );
    }
}

auto TextDisplay::removeHighlightRule( const QString &entity ) -> void {
    auto search = keywords_.find( entity.toStdString( ) );
    if ( search != std::end( keywords_ ) ) {
        keywords_.erase( search );
    }
    highlighter_->removeRule( entity );
}

auto TextDisplay::removeAllHighlightRules( const QColor &color ) -> void {
    qDebug( ) << "removing all rules associated with the color: " << color;
    highlighter_->removeAll( color );
}

auto TextDisplay::unlock( ) -> void {
    qDebug( ) << "unlocking text display";
    lock_ = false;
    setReadOnly( lock_ );
    // could be edits
    // caught via signal QPlainTextEdit#modificationChanged(bool)
}

auto TextDisplay::lock( ) -> void {
    qDebug( ) << "locking text display";
    lock_ = true;
    setReadOnly( lock_ );
    text_ = toPlainText( );

    // rescan
    auto text = text_.toStdString( );
    for ( auto iter = std::begin( keywords_ ); iter != std::end( keywords_ ); ) {
        auto search = text.find( iter->first );
        if ( search == std::string::npos ) {
            iter = keywords_.erase( iter );
            highlighter_->removeRule( QString::fromStdString( iter->first ) );
        } else {
            ++iter;
        }
    }

    // DEBUG:
    // QApplication::setOverrideCursor( Qt::WaitCursor );
    // auto element = xmlElement( );

    // qDebug( ) << QString::fromStdString( element.toString( ) );
    // std::cerr << element << std::endl;
    // QApplication::restoreOverrideCursor( );
}

auto TextDisplay::isLocked( ) const -> bool { return lock_; }

auto TextDisplay::xmlElement( ) const -> XmlElement {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
    auto text = text_.toStdString( );
    auto length = text.length( );
    std::vector<std::pair<std::string::size_type, std::string::size_type>> spans;
    for ( auto keyword : keywords_ ) {
        std::string::size_type index = 0;
        while ( index != std::string::npos ) {
            index = text.find( keyword.first, index );
            if ( index != std::string::npos ) {
                spans.push_back( std::make_pair( index, keyword.first.length( ) ) );
                index += keyword.first.length( );
            }
        }
    }

    XmlElement ret{xmlElement_.name( )};
    if ( !ret ) {
        // TODO: error
    }

    if ( spans.empty( ) ) {
        ret.content( text );
        return ret;
    }
    std::sort( std::begin( spans ), std::end( spans ),
               []( const std::pair<std::string::size_type, std::string::size_type> &i,
                   const std::pair<std::string::size_type, std::string::size_type> &j ) {
        return ( i.first < j.first );
    } );

    if ( spans.size( ) > 1 ) {
        for ( auto iter = std::begin( spans ); iter != std::end( spans ) - 1; ++iter ) {
            if ( ( iter->first + iter->second ) > ( iter + 1 )->first ) {
                // TODO:
                std::cerr << "*** collision: [ " << iter->first << ", " << iter->second
                          << " ] ";
                std::cerr << text.substr( iter->first, iter->second ) << std::endl;
                if ( iter->second >= ( iter + 1 )->second ) {
                    std::cerr << "consuming" << std::endl;
                    // consumes
                    auto pos = spans.erase( iter + 1 );
                    if ( pos == std::end( spans ) ) {
                        break;
                    }
                } else {
                    std::cerr << "booom!" << std::endl;
                }
            }
        }
    }

    std::cerr << "---------------------" << std::endl;
    for ( auto span : spans ) {
        std::cerr << "span: [ " << span.first << ", " << span.second << " ]" << std::endl;
        std::cerr << "\tkeyword: ( " << text.substr( span.first, span.second ) << " )"
                  << std::endl;
    }
    std::cerr << "---------------------" << std::endl;

    std::string::size_type index = 0;
    for ( auto span : spans ) {
        auto start = index;
        auto length = ( span.first - start );

        // std::cerr << "content:" << std::endl;
        // std::cerr << "\t" << text.substr( start, length ) << std::endl;

        // content
        ret.content( text.substr( start, length ) );

        // std::cerr << "element:" << std::endl;
        // std::cerr << "\t" << text.substr( span.first, span.second ) << std::endl;

        // entity element
        auto entity = text.substr( span.first, span.second );
        XmlElement markup{"entity"};
        markup.attribute( "type", keywords_.at( entity ) );
        markup.content( entity );
        ret.child( markup );
        index += ( length + span.second );
    }
    if ( index != std::string::npos ) {
        // std::cerr << "\t" << text.substr( index ) << std::endl;
        ret.content( text.substr( index ) );
    }
    return ret;
}

void TextDisplay::mousePressEvent( QMouseEvent *event ) {
    if ( event->button( ) == Qt::LeftButton ) {
        QPoint startPos = event->pos( );
        cursorSelection_.begin_ = cursorForPosition( startPos );
    }
    QPlainTextEdit::mousePressEvent( event );
}

void TextDisplay::mouseReleaseEvent( QMouseEvent *event ) {
    if ( event->button( ) == Qt::LeftButton ) {
        QPoint endPos = event->pos( );
        cursorSelection_.end_ = cursorForPosition( endPos );
        cursorSelection( );
    }
    QPlainTextEdit::mouseReleaseEvent( event );
}

auto TextDisplay::cursorSelection( ) -> void {
    if ( lock_ == false ) {
        qDebug( ) << "Text edit locked, cursor selection disabled";
        return;
    }
    if ( cursorSelection_.begin_.blockNumber( ) !=
         cursorSelection_.end_.blockNumber( ) ) {
        return;
    }
    if ( cursorSelection_.begin_.position( ) > cursorSelection_.end_.position( ) ) {
        std::swap( cursorSelection_.begin_, cursorSelection_.end_ );
    }
    QTextEdit::ExtraSelection selection;
    if ( cursorSelection_.begin_ == cursorSelection_.end_ ) {
        selection.cursor = cursorSelection_.begin_;
        selection.cursor.select( QTextCursor::WordUnderCursor );
    } else {
        cursorSelection_.begin_.movePosition( QTextCursor::StartOfWord );
        cursorSelection_.end_.movePosition( QTextCursor::EndOfWord );
        selection.cursor = textCursor( );
        selection.cursor.setPosition( cursorSelection_.begin_.position( ) );
        selection.cursor.setPosition( cursorSelection_.end_.position( ),
                                      QTextCursor::KeepAnchor );
    }
    QString textSelection = selection.cursor.selectedText( );
    if ( textSelection.isEmpty( ) ) {
        return;
    }

    qDebug( ) << "text selection: " << textSelection;
    emit entrySelected( textSelection );
}

auto TextDisplay::connections( ) -> void {
    connect( this, &TextDisplay::modificationChanged, highlighter_,
             &TextDisplayHighlighter::rehighlightModification );
}

