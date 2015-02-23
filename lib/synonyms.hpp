#pragma once
#include "element_selections.hpp"
#include <QObject>
#include <QHash>
#include <QMultiHash>
#include <QVector>
#include <QString>
#include <cstddef>
#include <memory>
#include <vector>

class Ontologies;
class XmlElement;
class Selection;
class Dictionary {
    friend class Ontologies;

public:
    Dictionary( );
    auto insert( const QString &word ) -> std::size_t;
    auto remove( const QString &word ) -> void;
    auto exists( const QString &word ) const -> bool;
    auto entries() const ->const QList<QString>;
    auto count( const QString &word ) const -> std::size_t;
    auto size( ) const -> std::size_t;

private:
    QHash<QString, std::size_t> words_;
};

class List;
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

class Ontology {
public:
    Ontology( ) {}
    Dictionary &dictionary( ) { return dictionary_; }
    const Dictionary &dictionary( ) const { return dictionary_; }
    Thesaurus &thesaurus( ) { return thesaurus_; }
    const Thesaurus &thesaurus( ) const { return thesaurus_; }

private:
    Dictionary dictionary_;
    Thesaurus thesaurus_;
};

class Ontologies : public QObject {
    Q_OBJECT
public:
    Ontologies( QObject *parent = nullptr );

    int currentIndex( );
    int size( );
    bool empty( );
    Dictionary &dictionary( );
    const Dictionary &dictionary( ) const;
    Thesaurus &thesaurus( );
    const Thesaurus &thesaurus( ) const;

    const Selection generateSelection( const XmlElement &element );

    // QVector<Ontology *>::iterator begin( );
    // QVector<Ontology *>::iterator end( );

public slots:
    void setCurrentIndex( int idx );
    void addOntology( );
    void removeOntology( int index );

    void insertDictionary( const QString &entry );
    void removeDictionary( const QString &entry );
    void insertThesaurus( const QString &entry, const QString &canonical );
    void removeThesaurus( const QString &entry );

signals:
    void indexChanged( int index );
    void invalidIndex( );
    void dictionaryChanged( const Dictionary &dictionary );
    void thesaurusChanged( const Thesaurus &thesaurus );
    void noOntologySet( );
    void entryAddedToDictionary( const QString &entry, int count );

private:
    std::vector<std::unique_ptr<Ontology>> ontologies_;
    std::vector<std::unique_ptr<Ontology>>::iterator idx_;
};

