#pragma once
#include <cstddef>
#include <QHash>
#include <QList>
#include <QString>


class Dictionary {
public:
    Dictionary( );
    auto insert( const QString &word ) -> std::size_t;
    auto remove( const QString &word ) -> void;
    auto exists( const QString &word ) const -> bool;
    auto entries( ) const -> const QList<QString>;
    auto count( const QString &word ) const -> std::size_t;
    auto size( ) const -> std::size_t;

private:
    QHash<QString, std::size_t> words_;
};

