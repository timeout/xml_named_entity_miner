#pragma once
#include <string>

namespace Utils {
    auto ltrim( std::string &text ) -> std::string &;
    auto rtrim( std::string &text ) -> std::string &;
    auto trim( std::string &text ) -> std::string &;
    auto collapseWhiteSpace( std::string::iterator start,
                             std::string::iterator end ) -> std::string::iterator;
    auto sentenceBoundary( const std::string &text, std::string::iterator start,
                           std::string::iterator end ) -> std::string::iterator;
}
