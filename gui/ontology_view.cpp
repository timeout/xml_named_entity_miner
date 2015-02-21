#include "ontology.hpp"
#include "synonyms.hpp"
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>
#include <QMouseEvent>
#include <QDragMoveEvent>
#include <QApplication>
#include <QMimeData>
#include <QDrag>

#include <iostream>
#include <QDebug>

static QTreeWidgetItem *findItem( QTreeWidget *tw, const QString &name );

Ontology::Ontology( QWidget *parent ) : QTreeWidget{parent} {
    setColumnCount( 2 );

    setSelectionMode( QAbstractItemView::SingleSelection );
    setDragEnabled( true );
    viewport( )->setAcceptDrops( true );
    setDropIndicatorShown( true );
    setDragDropMode( QAbstractItemView::InternalMove );
}
// void Ontology::dictionary( const Dictionary &dictionary ) { dictionary_ = dictionary; }
// void Ontology::thesaurus( const Thesaurus &thesaurus ) { thesaurus_ = thesaurus; }
// void Ontology::insertEntry( const QString &entry ) {
//     const std::string &entry_ = entry.toStdString( );
//     if ( dictionary_.exists( entry_ ) ) {
//         std::string canonical = thesaurus_.canonical( entry_ );
//         if ( canonical.empty( ) ) { // has no synonym
//             // add under root if not already added
//             int count = dictionary_.insert( entry_ );
//             QTreeWidgetItem *search = findItem( this, entry );
//             if ( !search ) {
//                 QTreeWidgetItem *item = new QTreeWidgetItem;
//                 item->setText( 0, entry );
//                 item->setData( 1, Qt::DisplayRole, QVariant{count} );
//                 item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable |
//                                 Qt::ItemIsDragEnabled );
//                 addTopLevelItem( item );
//             } else {
//                 // update count
//                 search->setData( 1, Qt::DisplayRole, QVariant{count} );
//                 emit itemChanged( search, 1 );
//             }
//         } else { // has synonym
//                  // add as child of canonical item, add canonical item if not
//                  // already added and all synonyms collapsed
//             const QString text = QString::fromUtf8( canonical.c_str( ) );
//             QTreeWidgetItem *search = findItem( this, text );
//             if ( !search ) {
//                 QTreeWidgetItem *canonicalItem = new QTreeWidgetItem;
//                 canonicalItem->setText( 0, text );
//                 auto synonyms = thesaurus_.synonyms( canonical );
//                 int canonicalCount = 0;
//                 for ( auto synonym : synonyms ) {
//                     int count = dictionary_.count( synonym );
//                     QTreeWidgetItem *item = new QTreeWidgetItem( canonicalItem );
//                     item->setText( 0, QString::fromUtf8( synonym.c_str( ) ) );
//                     item->setData( 1, Qt::DisplayRole, QVariant{count} );
//                     item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable |
//                                     Qt::ItemIsDragEnabled );
//                     canonicalCount += count;
//                 }
//             } else {
//                 for ( int i = 0; i < search->childCount( ); ++i ) {
//                     if ( search->child( i )->text( 0 ) == entry ) {
//                         // update item's count
//                         int count = dictionary_.insert( entry_ );
//                         search->child( i )
//                             ->setData( 1, Qt::DisplayRole, QVariant{count} );
//                         emit itemChanged( search->child( i ), 1 );
// 
//                         // update canonicalItem's count
//                         auto synonyms = thesaurus_.synonyms( canonical );
//                         int canonicalCount = search->data( 1, Qt::DisplayRole ).toInt( );
//                         search->setData( 1, Qt::DisplayRole, QVariant{++canonicalCount} );
//                     }
//                 }
//             }
//         }
//     } else {
//         // add under root
//         int count = dictionary_.insert( entry_ );
//         QTreeWidgetItem *item = new QTreeWidgetItem;
//         item->setText( 0, entry );
//         item->setData( 1, Qt::DisplayRole, QVariant{count} );
//         addTopLevelItem( item );
//     }
// }
// 
// void Ontology::dictionary( ) {
//     qDebug( ) << "dictionary requested ...";
//     emit dictionaryRequested( dictionary_ );
//     qDebug( ) << "and emitted\n";
// }

void Ontology::dropEvent( QDropEvent *event ) {
    QTreeWidgetItem *movedItem =
        qobject_cast<Ontology *>( event->source( ) )->currentItem( );
    QModelIndex droppedIndex = indexAt( event->pos( ) );
    QTreeWidgetItem *item = itemFromIndex( indexAt( event->pos( ) ) );
    if ( !droppedIndex.isValid( ) ) {
        return;
    }

    if ( item->parent( ) ) {
        std::cerr << "item: " << item->text( 0 ).toStdString( ) << " has a parent"
                  << std::endl;
        return;
    }
    if ( movedItem->childCount( ) ) {
        return;
    }

    // update thesaurus
    // std::string word = movedItem->text( 0 ).toStdString( );
    // std::string canonical = item->text( 0 ).toStdString( );

    // std::cerr << "word: " << word << ", canonical: " << canonical << std::endl;
    // thesaurus_.insert( word, canonical );

    // update item
    int itemCount = item->data( 1, Qt::DisplayRole ).toInt( );
    int movedItemCount = movedItem->data( 1, Qt::DisplayRole ).toInt( );
    item->setData( 1, Qt::DisplayRole, QVariant{itemCount + movedItemCount} );
    emit itemChanged( item, 1 );

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
