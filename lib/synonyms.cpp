#include "synonyms.hpp"
#include <QDebug>
#include <QList>

Dictionary::Dictionary( QObject *parent ) : QObject{parent} {}
auto Dictionary::insert( const QString &word ) -> std::size_t { return ++words_[word]; }
auto Dictionary::remove( const QString &word ) -> void { words_.remove( word ); }
auto Dictionary::exists( const QString &word ) const -> bool {
    return words_.contains( word );
}
auto Dictionary::count( const QString &word ) const -> std::size_t {
    return words_.value( word );
}
auto Dictionary::size( ) const -> std::size_t { return words_.size( ); }

Thesaurus::Thesaurus( QObject *parent ) : QObject{parent} {}
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
