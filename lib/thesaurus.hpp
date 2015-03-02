#pragma once
#include <QHash>
#include <QList>
#include <QMultiHash>
#include <QString>

class Thesaurus {
public:
    Thesaurus( );
    auto insert( const QString &word, const QString &canonical ) -> void;
    auto remove( const QString &word ) -> void;
    auto canonical( const QString &word ) const -> const QString;
    auto synonyms( const QString &canonical ) const -> const QList<QString>;

private:
    QHash<QString, QString> canon_;
    QMultiHash<QString, QString> synonyms_;
};

