#include "thesaurus.hpp"

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

