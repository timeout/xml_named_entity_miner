#pragma once
#include <map>
#include <string>
#include <cstdlib>
#include <vector>
#include <QMetaType>

class Dictionary {
public:
    auto insert( const std::string &word ) -> std::size_t;
    auto remove( const std::string &word ) -> void;
    auto count( const std::string &word ) const -> std::size_t;
    auto exists( const std::string &word ) const -> bool;
    auto size( ) const -> std::map<std::string, size_t>::size_type;
    auto get() const -> const std::map<std::string, std::size_t>&;

private:
    std::map<std::string, std::size_t> words_;
};

Q_DECLARE_METATYPE( Dictionary )

class Thesaurus {
public:
    auto insert( const std::string &word, const std::string &canonical ) -> void;
    auto remove( const std::string &word ) -> void;
    auto canonical( const std::string &word ) const -> const std::string &;
    auto synonyms( const std::string &canonical ) const -> const std::vector<std::string>;

private:
    std::map<std::string, std::string> canon_;
    std::multimap<std::string, std::string> synonyms_;
};
