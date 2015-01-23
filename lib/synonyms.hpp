#pragma once
#include <QObject>
#include <QHash>
#include <QMultiHash>
#include <QVector>
#include <QString>
#include <cstddef>

class Dictionary : public QObject {
    Q_OBJECT
public:
    Dictionary( QObject *parent = nullptr );
    auto insert( const QString &word ) -> std::size_t;
    auto remove( const QString &word ) -> void;
    auto exists( const QString &word ) const -> bool;
    auto count( const QString &word ) const -> std::size_t;
    auto size( ) const -> std::size_t;

private:
    QHash<QString, std::size_t> words_;
};

class List;
class Thesaurus : public QObject {
    Q_OBJECT
public:
    Thesaurus( QObject *parent = nullptr );
    auto insert( const QString &word, const QString &canonical ) -> void;
    auto remove( const QString &word ) -> void;
    auto canonical( const QString &word ) const -> const QString;
    auto synonyms( const QString &canonical ) const -> const QList<QString>;

private:
    QHash<QString, QString> canon_;
    QMultiHash<QString, QString> synonyms_;
};
