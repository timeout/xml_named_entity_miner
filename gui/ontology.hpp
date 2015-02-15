#pragma once
#include <QTreeWidget>
#include "synonyms.hpp"
#include <QPoint>

class Dictionary;

class Ontology : public QTreeWidget {
    Q_OBJECT
public:
    Ontology( QWidget *parent = nullptr );
    // void dictionary( const Dictionary &dictionary );
    // void thesaurus( const Thesaurus &thesaurus );

public slots:
    // void insertEntry( const QString &entry );
    // void dictionary( );

signals:
    void dictionaryRequested(const Dictionary& dictionary);

private:
    void dropEvent( QDropEvent *event );

    // Dictionary dictionary_;
    // Thesaurus thesaurus_;

    QPoint startPos_;
};

