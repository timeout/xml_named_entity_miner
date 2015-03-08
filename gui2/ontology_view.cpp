#include "ontology_view.hpp"
#include "ontology_view_context_menu.hpp"
#include <QDropEvent>
#include <QHeaderView>
#include <QInputDialog>
#include <QLineEdit>
#include <QModelIndex>
#include <QTreeWidgetItem>

#include <QDebug>

OntologyView::OntologyView( const QString &ontologyName, const QColor &color,
                            QWidget *parent )
    : QTreeWidget{parent}
    , selectedItem_{nullptr}
    , ontologyName_{ontologyName}
    , color_{color} {
    header( )->close( );
    setColumnCount( 1 );
    setSelectionMode( QAbstractItemView::SingleSelection );
    setDragEnabled( true );
    viewport( )->setAcceptDrops( true );
    setDropIndicatorShown( true );
    setDragDropMode( QAbstractItemView::InternalMove );
    connections( );
}

auto OntologyView::ontologyName( const QString &name ) -> void {
    ontologyName_ = name;
}

auto OntologyView::ontologyName( ) const -> const QString & {
    return ontologyName_;
}

auto OntologyView::color( const QColor &color ) -> void { color_ = color; }

auto OntologyView::color( ) const -> const QColor { return color_; }

auto OntologyView::addEntity( const QString &entity ) -> int {
    int ret = dictionary_.insert( entity );
    if ( ret == 1 ) {
        insertItem( entity );
    }
    return ret;
}

auto OntologyView::removeEntity( const QString &entity ) -> void {
    dictionary_.remove( entity );
}

auto OntologyView::addSynonym( const QString &entity, const QString &canon ) -> void {
    thesaurus_.insert( entity, canon );
}

auto OntologyView::removeSynonym( const QString &entity ) -> void {
    thesaurus_.remove( entity );
}

auto OntologyView::scanText( const QString &contents ) -> void {
    // scan text for words in dictionary
    for ( auto entity : dictionary_.entries( ) ) {
        if ( contents.contains( entity ) ) {
            if ( !contains( entity ) ) {
                insertItem( entity );
            }
            emit entityAdded( ontologyName_, entity );
        }
    }
}

auto OntologyView::removeSelectedItem( ) -> void {
    const QString entity = selectedItem_->text( 0 );
    if ( dictionary_.count( entity ) == 1 ) {
        dictionary_.remove( entity );
    }
    if ( selectedItem_->parent( ) ) {
        QTreeWidgetItem *parent = selectedItem_->parent( );
        parent->removeChild( selectedItem_ );
        if ( parent->childCount( ) == 0 ) {
            // remove parent and remove synonym
            delete parent;
            thesaurus_.remove( entity );
        }
    } else {
        delete selectedItem_;
    }
    selectedItem_ = nullptr;
    qDebug( ) << "removing entity: " << entity;
    emit entityRemoved( entity );
}

auto OntologyView::sortAscending( ) -> void { sortItems( 0, Qt::AscendingOrder ); }

auto OntologyView::sortDescending( ) -> void { sortItems( 0, Qt::DescendingOrder ); }

auto OntologyView::createSynonym( ) -> void {
    bool ok;
    QString synonym = QInputDialog::getText(
        this, tr( "New Synonym for %1" ).arg( selectedItem_->text( 0 ) ),
        tr( "Synonym:" ), QLineEdit::Normal, QString{}, &ok );
    if ( ok && !synonym.isEmpty( ) ) {
        auto childText = selectedItem_->text( 0 );
        delete selectedItem_;
        auto parent = new QTreeWidgetItem;
        parent->setText( 0, synonym );
        auto child = new QTreeWidgetItem{parent};
        child->setText( 0, childText );
        addTopLevelItem( parent );
        thesaurus_.insert( selectedItem_->text( 0 ), synonym );
    }
    selectedItem_ = nullptr;
}

void OntologyView::contextMenuRequest( const QPoint &pos ) {
    QModelIndex idx = indexAt( pos );
    if ( idx.isValid( ) ) {
        selectedItem_ = itemFromIndex( idx );
        qDebug( ) << "selected item: " << selectedItem_->text( 0 );
        // ontologyViewContextMenu_->exec( mapToGlobal( pos ) );
        auto contextMenu = new OntologyViewContextMenu{this};
        connect( contextMenu, &OntologyViewContextMenu::removeEntity, this,
                 &OntologyView::removeSelectedItem );
        connect( contextMenu, &OntologyViewContextMenu::sortAscending, this,
                 &OntologyView::sortAscending );
        connect( contextMenu, &OntologyViewContextMenu::sortDescending, this,
                 &OntologyView::sortDescending );
        connect( contextMenu, &OntologyViewContextMenu::createSynonym, this,
                 &OntologyView::createSynonym );
        contextMenu->exec( mapToGlobal( pos ) );
    }
}

auto OntologyView::dropEvent( QDropEvent *event ) -> void {
    QTreeWidgetItem *movedItem =
        qobject_cast<OntologyView *>( event->source( ) )->currentItem( );
    QModelIndex droppedIndex = indexAt( event->pos( ) );
    if ( !droppedIndex.isValid( ) ) {
        return;
    }

    QTreeWidgetItem *item = itemFromIndex( indexAt( event->pos( ) ) );
    if ( item->parent( ) ) {
        return;
    }
    if ( movedItem->childCount( ) ) {
        return;
    }

    if ( movedItem->parent( ) ) {
        thesaurus_.remove( movedItem->text( 0 ) );
        emit removeSynonym( movedItem->text( 0 ) );
    }

    item->addChild( movedItem );
    item->setExpanded( true );

    // add synonym to thesaurus
    thesaurus_.insert( movedItem->text( 0 ), item->text( 0 ) );

    emit itemChanged( item, 0 );

    QTreeWidget::dropEvent( event );
}

auto OntologyView::insertItem( const QString &entity ) -> void {
    QTreeWidgetItem *item = new QTreeWidgetItem;
    auto canon = thesaurus_.canonical( entity );
    if ( !canon.isEmpty( ) ) {
        // if a word has a synonym add synonyms to view
        item->setText( 0, canon );
        for ( auto synonym : thesaurus_.synonyms( canon ) ) {
            QTreeWidgetItem *child = new QTreeWidgetItem( item );
            child->setText( 0, synonym );
        }
    } else {
        // just add the word
        item->setText( 0, entity );
        item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled |
                        Qt::ItemIsDropEnabled );
        addTopLevelItem( item );
    }
    addTopLevelItem( item );
    // emit word so that it can be highlighted in textdisplay class
    emit entityAdded( ontologyName_, entity );
}

auto OntologyView::contains( const QString &entity ) -> bool {
    for ( auto i = topLevelItemCount( ); i; ) {
        auto item = topLevelItem( --i );
        if ( item->text( 0 ) == entity ) {
            return true;
        }
        for ( auto j = item->childCount( ); j; ) {
            auto child = item->child( --j );
            if ( child->text( 0 ) == entity ) {
                return true;
            }
        }
    }
    return false;
}

auto OntologyView::connections( ) -> void {
    connect( this, &OntologyView::customContextMenuRequested, this,
             &OntologyView::contextMenuRequest );
}

