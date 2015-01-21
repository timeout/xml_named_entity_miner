#include "synonyms.hpp"

static std::string nullStr;

auto Dictionary::insert( const std::string &word ) -> std::size_t {
    return ++words_[word];
}
auto Dictionary::remove( const std::string &word ) -> void {
    auto search = words_.find( word );
    if ( search != std::end( words_ ) ) {
        words_.erase( search );
    }
}
auto Dictionary::count( const std::string &word ) const -> std::size_t {
    auto search = words_.find( word );
    if ( search != std::end( words_ ) ) {
        return search->second;
    }
    return 0;
}
auto Dictionary::exists( const std::string &word ) const -> bool {
    auto search = words_.find( word );
    if ( search != std::end( words_ ) ) {
        return true;
    }
    return false;
}
auto Dictionary::size( ) const -> std::map<std::string, size_t>::size_type {
    return words_.size( );
}

auto Thesaurus::insert( const std::string &word, const std::string &canonical ) -> void {
    canon_[word] = canonical;
    synonyms_.insert( std::make_pair( canonical, word ) );
}
auto Thesaurus::remove( const std::string &word ) -> void {
    auto search = canon_.find( word );
    if ( search != std::end( canon_ ) ) {
        auto canonical = search->second;
        auto range = synonyms_.equal_range( canonical );
        if ( range.first != range.second ) {
            for ( auto i = range.first; i != range.second; ++i ) {
                if ( i->second == word ) {
                    synonyms_.erase( i );
                    break;
                }
            }
        }
        canon_.erase( search );
    }
}
auto Thesaurus::canonical( const std::string &word ) const -> const std::string & {
    auto search = canon_.find( word );
    if ( search != std::end( canon_ ) ) {
        return search->second;
    }
    return nullStr;
}
auto Thesaurus::synonyms(
    const std::string &canonical ) const -> const std::vector<std::string> {
    std::vector<std::string> ret;
    auto range = synonyms_.equal_range( canonical );
    if ( range.first != range.second ) {
        for ( auto i = range.first; i != range.second; ++i ) {
            ret.push_back( i->second );
        }
    }
    return ret;
}

