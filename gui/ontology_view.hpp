#pragma once
#include <QTreeWidget>
#include "synonyms.hpp"
#include <QPoint>

class Dictionary;
class QMenu;
class QAction;

class OntologyView : public QTreeWidget {
    Q_OBJECT
public:
    OntologyView( QWidget *parent = nullptr );

public slots:
    void insertEntry( const QString &entry, int count );
    void contextMenuRequest( const QPoint &pos );
    void removeEntry( );

signals:
    void dictionaryRequested( const Dictionary &dictionary );
    void synonymCreated( const QString &entry, const QString &canonical );
    void removeSynonym( const QString &entry );
    void removeEntry( const QString &entry );

private:
    void dropEvent( QDropEvent *event );

    QMenu *ontologyViewContextMenu_;
    QAction *removeEntryAction_;
    QTreeWidgetItem *selectedItem_;
};

