#include "ontology_view.hpp"
#include "synonyms.hpp"
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>
#include <QMouseEvent>
#include <QDragMoveEvent>
#include <QApplication>
#include <QMimeData>
#include <QDrag>
#include <QMenu>
#include <QAction>

#include <iostream>
#include <QDebug>

static QTreeWidgetItem *findItem( QTreeWidget *tw, const QString &name );

OntologyView::OntologyView( QWidget *parent )
    : QTreeWidget{parent}, selectedItem_{nullptr} {
    setColumnCount( 2 );

    setSelectionMode( QAbstractItemView::SingleSelection );
    setDragEnabled( true );
    viewport( )->setAcceptDrops( true );
    setDropIndicatorShown( true );
    setDragDropMode( QAbstractItemView::InternalMove );

    removeEntryAction_ = new QAction{tr( "&Delete" ), this};
    removeEntryAction_->setShortcut( QKeySequence::Delete );
    removeEntryAction_->setIcon( QIcon::fromTheme( "remove" ) );
    connect( removeEntryAction_, SIGNAL( triggered( ) ), SLOT( removeEntry( ) ) );

    ontologyViewContextMenu_ = new QMenu{this};
    ontologyViewContextMenu_->addAction( removeEntryAction_ );

    connect( this, SIGNAL( customContextMenuRequested( const QPoint & )),
             SLOT( contextMenuRequest( const QPoint & )) );
}
void OntologyView::insertEntry( const QString &entry, int count ) {
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText( 0, entry );
    item->setData( 1, Qt::DisplayRole, QVariant{count} );
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled |
                    Qt::ItemIsDropEnabled );
    addTopLevelItem( item );
}

void OntologyView::contextMenuRequest( const QPoint &pos ) {
    QModelIndex idx = indexAt( pos );
    if ( idx.isValid( ) ) {
        selectedItem_ = itemFromIndex( idx );
        ontologyViewContextMenu_->exec( mapToGlobal( pos ) );
    }
}

void OntologyView::removeEntry( ) {
    const QString entry = selectedItem_->text( 0 );
    if ( selectedItem_->parent( ) ) {
        QTreeWidgetItem *parent = selectedItem_->parent( );
        parent->removeChild( selectedItem_ );
    } else {
        delete selectedItem_;
    }
    selectedItem_ = nullptr;
    emit removeEntry( entry );
}

void OntologyView::dropEvent( QDropEvent *event ) {
    // there are 3 possibilities,
    // 1 create a synonym
    // 2 remove a synonym
    // 3 move to another synonym
    QTreeWidgetItem *movedItem =
        qobject_cast<OntologyView *>( event->source( ) )->currentItem( );
    QModelIndex droppedIndex = indexAt( event->pos( ) );
    std::cerr << "item dropped at index: " << droppedIndex.row( ) << std::endl;
    if ( !droppedIndex.isValid( ) ) {
        return;
    }

    QTreeWidgetItem *item = itemFromIndex( indexAt( event->pos( ) ) );
    if ( item->parent( ) ) {
        std::cerr << "item: " << item->text( 0 ).toStdString( ) << " has a parent"
                  << std::endl;
        return;
    }
    if ( movedItem->childCount( ) ) {
        std::cerr << "moved item: " << movedItem->text( 0 ).toStdString( )
                  << " has children" << std::endl;
        return;
    }

    std::cerr << "adding child: " << movedItem->text( 0 ).toStdString( )
              << " to parent: " << item->text( 0 ).toStdString( ) << std::endl;

    if ( movedItem->parent( ) ) {
        emit removeSynonym( movedItem->text( 0 ) );
    }

    int movedItemCount = movedItem->data( 1, Qt::DisplayRole ).toInt( );
    // item->setData( 1, Qt::DisplayRole, QVariant{itemCount + movedItemCount} );
    item->addChild( movedItem );
    emit itemChanged( item, 1 );

    emit synonymCreated( movedItem->text( 0 ), item->text( 0 ) );

    QTreeWidget::dropEvent( event );
}

static QTreeWidgetItem *findItem( QTreeWidget *tw, const QString &name ) {
    QTreeWidgetItemIterator it{tw};
    while ( *it ) {
        if ( ( *it )->text( 0 ) == name ) {
            return *it;
        }
    }
    return nullptr;
}
