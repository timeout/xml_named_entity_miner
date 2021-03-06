#pragma once
#include <string>

namespace Utils {
    auto ltrim( std::string &text ) -> std::string &;
    auto rtrim( std::string &text ) -> std::string &;
    auto trim( std::string &text ) -> std::string &;
    auto collapseWhiteSpace( std::string::iterator start, std::string::iterator end )
        -> std::string::iterator;
    auto sentenceBoundary( std::string::iterator start, std::string::iterator end )
        -> std::string::iterator;
    auto paragraphBoundary( std::string::iterator start, std::string::iterator end )
        -> std::string::iterator;
}
