#pragma once

#include "singleton.hpp"
#include "pathname.hpp"
#include <vector>
#include <regex>

class Dir : public Singleton<Dir> {

    friend class Singleton<Dir>;

public:
    auto pwd( ) -> Pathname;
    auto exists( const Pathname &pathname ) -> bool;
    auto mk( const Pathname &pathname ) -> void; // TODO:
    auto rm( const Pathname &pathname ) -> void; // TODO:
    auto chdir( const Pathname &pathname ) -> Dir &;
    auto read( ) -> Dir &;
    auto entries( ) const -> const std::vector<Pathname> &;

private:
    Dir( );
    Dir( const Dir & ) = delete;
    virtual ~Dir( ) {}
    auto operator=( const Dir & ) -> Dir & = delete;

    Pathname pathname_;
    std::vector<Pathname> entries_;
    bool dirty_;
    bool read_;
};

auto filter(const std::vector<Pathname>& ls, const std::regex &glob) -> std::vector<Pathname>;
