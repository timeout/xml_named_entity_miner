#pragma once
#include <QTreeWidget>
#include "synonyms.hpp"
#include <QPoint>

class EntityTree : public QTreeWidget {
    Q_OBJECT
public:
    EntityTree( QWidget *parent = nullptr );
    void dictionary( const Dictionary &dictionary );
    void thesaurus( const Thesaurus &thesaurus );

public slots:
    void insertEntry( const QString &entry );

private:
    void dropEvent( QDropEvent *event );

    Dictionary dictionary_;
    Thesaurus thesaurus_;

    QPoint startPos_;
};

