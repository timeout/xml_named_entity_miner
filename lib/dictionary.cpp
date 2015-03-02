#include "dictionary.hpp"

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

