#include "synonyms.hpp"
#include "xml_element.hpp"
#include "utils.hpp"
#include <QDebug>
#include <QList>

#include <iostream>
#include <string>

Dictionary::Dictionary( ) {}
auto Dictionary::insert( const QString &word ) -> std::size_t { return ++words_[word]; }
auto Dictionary::remove( const QString &word ) -> void {
    auto count = words_.value( word, 0 );
    if ( count && count == 1 ) {
        words_.remove( word );
    } else {
        words_[word] = --count;
    }
}
auto Dictionary::exists( const QString &word ) const -> bool {
    return words_.contains( word );
}
auto Dictionary::entries( ) const -> const QList<QString> { return words_.keys( ); }
auto Dictionary::count( const QString &word ) const -> std::size_t {
    return words_.value( word );
}
auto Dictionary::size( ) const -> std::size_t { return words_.size( ); }

Thesaurus::Thesaurus( ) {}
auto Thesaurus::insert( const QString &word, const QString &canonical ) -> void {
    canon_.insert( word, canonical );
    synonyms_.insert( canonical, word );
}
auto Thesaurus::remove( const QString &word ) -> void {
    synonyms_.remove( canonical( word ), word );
    canon_.remove( word );
}
auto Thesaurus::canonical( const QString &word ) const -> const QString {
    return canon_.value( word );
}
auto Thesaurus::synonyms( const QString &canonical ) const -> const QList<QString> {
    return synonyms_.values( canonical );
}

Ontologies::Ontologies( QObject *parent ) : QObject{parent} {}

int Ontologies::size( ) { return ontologies_.size( ); }

int Ontologies::currentIndex( ) { return ( idx_ - ontologies_.begin( ) ); }

bool Ontologies::empty( ) { return ontologies_.empty( ); }

Dictionary &Ontologies::dictionary( ) { return ( *idx_ )->dictionary( ); }

const Dictionary &Ontologies::dictionary( ) const { return ( *idx_ )->dictionary( ); }

Thesaurus &Ontologies::thesaurus( ) { return ( *idx_ )->thesaurus( ); }

const Thesaurus &Ontologies::thesaurus( ) const { return ( *idx_ )->thesaurus( ); }

const Selection Ontologies::generateSelection( const XmlElement &element ) {
    std::string text = element.content( );
    text = Utils::trim( text );
    QVector<QVector<QString>> keywordLists;
    for ( auto iter = ontologies_.begin( ); iter != ontologies_.end( ); ++iter ) {
        QVector<QString> keywords;
        Dictionary &dict = ( *iter )->dictionary( );
        auto words = dict.entries( );
        foreach ( const QString &word, words ) {
            auto needle = word.toStdString( );
            // search for needle
            auto pos = text.find( needle );
            if ( pos != std::string::npos ) {
                keywords.push_back( QString::fromStdString( text ) );
            }
        }
        keywordLists.push_back( keywords );
    }
    return Selection{element, QString::fromStdString( text ), keywordLists};
}

// -- slots
void Ontologies::setCurrentIndex( int idx ) {
    // no check for negative indexes application not library, still
    // checking for out of range
    if ( idx >= ontologies_.size( ) ) {
        idx_ = ontologies_.end( );
        emit invalidIndex( );
        return;
    }
    idx_ = ontologies_.begin( ) + idx;
    emit indexChanged( idx );
    emit dictionaryChanged( dictionary( ) );
    emit thesaurusChanged( thesaurus( ) );

    std::cerr << "index: " << currentIndex( ) << std::endl;
    std::cerr << "size: " << size( ) << std::endl;
}

void Ontologies::addOntology( ) {
    ontologies_.push_back( std::unique_ptr<Ontology>{new Ontology} );
    idx_ = ontologies_.end( ) - 1;
    emit indexChanged( currentIndex( ) );
    emit dictionaryChanged( dictionary( ) );
    emit thesaurusChanged( thesaurus( ) );

    std::cerr << "index: " << currentIndex( ) << std::endl;
    std::cerr << "size: " << size( ) << std::endl;
}

void Ontologies::removeOntology( int index ) {
    if ( index >= ontologies_.size( ) ) {
        emit invalidIndex( );
        return;
    }
    ontologies_.erase( ontologies_.begin( ) + index );
}

void Ontologies::insertDictionary( const QString &entry ) {
    if ( ontologies_.empty( ) ) {
        emit noOntologySet( );
        if ( ontologies_.empty( ) ) {
            return;
        }
    }
    // TODO:
    std::cerr << "inserting into dictionary: " << entry.toStdString( ) << std::endl;
    Dictionary &dict = dictionary( );
    auto count = dict.insert( entry );
    if ( count == 1 ) {
        std::cerr << "first entry" << std::endl;
    }
    entryAddedToDictionary( entry, count );
}

void Ontologies::removeDictionary( const QString &entry ) {
    Dictionary &dict = dictionary( );
    dict.remove( entry );
}

void Ontologies::insertThesaurus( const QString &entry, const QString &canonical ) {
    std::cerr << "inserting into thesaurus: " << entry.toStdString( ) << ", "
              << canonical.toStdString( ) << std::endl;
    Thesaurus &thes = thesaurus( );
    thes.insert( entry, canonical );
}

void Ontologies::removeThesaurus( const QString &entry ) {
    std::cerr << "removing from thesaurus: " << entry.toStdString( ) << std::endl;
    Thesaurus &thes = thesaurus( );
    thes.remove( entry );
}

