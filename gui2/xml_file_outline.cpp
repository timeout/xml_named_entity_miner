#include "xml_file_outline.hpp"
#include "xml_doc.hpp"
#include "xml_element.hpp"
#include <queue>
#include <string>

#include <QAction>
#include <QActionGroup>
#include <QDebug>
#include <QHeaderView>
#include <QMenu>
#include <QPoint>
#include <QTreeWidgetItem>
#include <QWidget>

#include <libxml/tree.h>

static auto buildTreeFromXml( const XmlDoc &xml ) -> QTreeWidgetItem *;
static auto findXmlElement( QTreeWidgetItem *item, const XmlElement &element )
    -> QTreeWidgetItem *;

XmlFileOutline::XmlFileOutline( QWidget *parent ) : QTreeWidget{parent} {
    setContextMenuPolicy( Qt::CustomContextMenu );
    header( )->close( );
    contextMenu( );
    connections( );
}

auto XmlFileOutline::clear( ) -> void { QTreeWidget::clear( ); }

auto XmlFileOutline::xml( const XmlDoc &xml ) -> void {
    addTopLevelItem( buildTreeFromXml( xml ) );
    if ( invisibleRootItem( )->child( 0 ) ) {
        expandItem( invisibleRootItem( )->child( 0 ) );
    }
}

void XmlFileOutline::itemClicked( const QModelIndex &idx ) {
    QTreeWidgetItem *target = itemFromIndex( idx );
    Qt::CheckState checkedState = target->checkState( 0 );
    switch ( checkedState ) {
    case Qt::Checked:
        checkSubtree( target );
        break;
    case Qt::Unchecked:
        uncheckSubtree( target );
        break;
    default:
        return;
    }
}

void XmlFileOutline::xPathResult( const XmlElement &element ) {
    qDebug( ) << "adding xpath result...\n";
    QTreeWidgetItem *root = invisibleRootItem( );
    QTreeWidgetItem *search = findXmlElement( root, element );
    //    if ( search ) {
    //        Qt::CheckState checkedState = search->checkState( 0 );
    //        switch ( checkedState ) {
    //        case Qt::Checked:
    //            break;
    //        case Qt::Unchecked:
    //            checkSubtree( search );
    //            break;
    //        default:
    //            // return;
    //        }
    //        search->setCheckState( 0, Qt::Checked );
    //    }
    if ( search ) {
        search->setCheckState( 0, Qt::Checked );
        itemClicked( indexFromItem( search, 0 ) );
    }
}

void XmlFileOutline::clearSelections( ) {
    qDebug( ) << "clearing all selections...\n";
    QTreeWidgetItem *root = invisibleRootItem( );
    std::queue<QTreeWidgetItem *> parents;
    parents.push( root );
    while ( !parents.empty( ) ) {
        auto parent = parents.front( );
        parents.pop( );
        auto child = parent->child( 0 );
        if ( child->checkState( 0 ) == Qt::Checked ) {
            child->setCheckState( 0, Qt::Unchecked );
            uncheckSubtree( child );
        } else {
            if ( child->child( 0 ) ) {
                parents.push( child->child( 0 ) );
            }
        }
    }
}

void XmlFileOutline::checkSubtree( QTreeWidgetItem *target ) {
    qDebug( ) << "checking subtree...\n";
    if ( target->isDisabled( ) == true ) {
        return;
    }
    std::queue<QTreeWidgetItem *> parents;
    parents.push( target );
    while ( !parents.empty( ) ) {
        auto parent = parents.front( );
        parents.pop( );
        auto child = parent->child( 0 );
        while ( child ) {
            if ( child->child( 0 ) ) {
                parents.push( child );
            }
            if ( child->checkState( 0 ) == Qt::Checked &&
                 child->isDisabled( ) == false ) {
                // deselect item
                qDebug( ) << "deselcting: " << child->text( 0 ) << "\n";
                QVariant childVar = child->data( 0, Qt::UserRole );
                XmlElement childElement = childVar.value<XmlElement>( );
                emit xmlItemDeselected( childElement );
            }
            child->setCheckState( 0, Qt::Checked );
            child->setDisabled( true );
            child = parent->child( parent->indexOfChild( child ) + 1 );
        }
    }
    QVariant targetVar = target->data( 0, Qt::UserRole );
    XmlElement targetElement = targetVar.value<XmlElement>( );
    emit xmlItemSelected( targetElement );
}

auto XmlFileOutline::addSentiment( Sentiment sentiment ) -> void {
    QVariant elementVar = selectedItem_->data( 0, Qt::UserRole );
    XmlElement element = elementVar.value<XmlElement>( );
    // add attribute
    switch ( sentiment ) {
    case Sentiment::Positive:
        element.attribute( "sentiment", "positive" );
        break;
    case Sentiment::Neutral:
        element.attribute( "sentiment", "neutral" );
        break;
    case Sentiment::Negative:
        element.attribute( "sentiment", "negative" );
        break;
    }

    std::cerr << "DEBUG:" << std::endl;
    std::cerr << element.toString( ) << std::endl;
}

void XmlFileOutline::addPositiveSentiment( ) { addSentiment( Sentiment::Positive ); }

void XmlFileOutline::addNegativeSentiment( ) { addSentiment( Sentiment::Negative ); }

void XmlFileOutline::addNeutralSentiment( ) { addSentiment( Sentiment::Neutral ); }

void XmlFileOutline::customContextMenu( const QPoint &pos ) {
    QModelIndex idx = indexAt( pos );
    if ( idx.isValid( ) ) {
        selectedItem_ = itemFromIndex( idx );
        qDebug( ) << "selected item: " << selectedItem_->text( 0 );
        fileOutlineContextMenu_->exec( mapToGlobal( pos ) );
    }
}

void XmlFileOutline::uncheckSubtree( QTreeWidgetItem *target ) {
    qDebug( ) << "unchecking subtree...\n";
    std::queue<QTreeWidgetItem *> parents;
    parents.push( target );
    while ( !parents.empty( ) ) {
        auto parent = parents.front( );
        parents.pop( );
        auto child = parent->child( 0 );
        while ( child ) {
            if ( child->child( 0 ) ) {
                parents.push( child );
            }
            child->setDisabled( false );
            child->setCheckState( 0, Qt::Unchecked );
            child = parent->child( parent->indexOfChild( child ) + 1 );
        }
    }
    QVariant targetVar = target->data( 0, Qt::UserRole );
    XmlElement targetElement = targetVar.value<XmlElement>( );
    emit xmlItemDeselected( targetElement );
}

auto XmlFileOutline::contextMenu( ) -> void {
    // removeEntryAction_ = new QAction{tr( "&Delete" ), this};
    // removeEntryAction_->setShortcut( QKeySequence::Delete );
    // removeEntryAction_->setIcon( QIcon::fromTheme( "remove" ) );
    // connect( removeEntryAction_, SIGNAL( triggered( ) ), SLOT( removeEntry( ) ) );
    semanticAction_ = new QAction{tr( "&Add Sentiment" ), this};
    semanticAction_->setShortcut( QKeySequence{Qt::CTRL + Qt::Key_I} );
    semanticAction_->setIcon( QIcon::fromTheme( "configure" ) );

    positiveSemanticAction_ = new QAction{tr( "Positive" ), this};
    positiveSemanticAction_->setIcon( QIcon::fromTheme( "add" ) );
    negativeSemanticAction_ = new QAction{tr( "Negative" ), this};
    negativeSemanticAction_->setIcon( QIcon::fromTheme( "edit-delete" ) );
    neutralSemanticAction_ = new QAction{tr( "Neutral" ), this};
    neutralSemanticAction_->setIcon( QIcon::fromTheme( "help" ) );
    neutralSemanticAction_->setChecked( true );

    QActionGroup *semanticGroup = new QActionGroup{this};
    semanticGroup->addAction( positiveSemanticAction_ );
    semanticGroup->addAction( negativeSemanticAction_ );
    semanticGroup->addAction( neutralSemanticAction_ );

    fileOutlineContextMenu_ = new QMenu{this};
    fileOutlineContextMenu_->addAction( positiveSemanticAction_ );
    fileOutlineContextMenu_->addAction( neutralSemanticAction_ );
    fileOutlineContextMenu_->addAction( negativeSemanticAction_ );
}

auto XmlFileOutline::connections( ) -> void {
    connect( this, &XmlFileOutline::clicked, this, &XmlFileOutline::itemClicked );
    connect( this, &XmlFileOutline::customContextMenuRequested, this,
             &XmlFileOutline::customContextMenu );
    connect( positiveSemanticAction_, &QAction::triggered, this,
             &XmlFileOutline::addPositiveSentiment );
    connect( negativeSemanticAction_, &QAction::triggered, this,
             &XmlFileOutline::addNegativeSentiment );
    connect( neutralSemanticAction_, &QAction::triggered, this,
             &XmlFileOutline::addNeutralSentiment );
}

static auto buildTreeFromXml( const XmlDoc &xml ) -> QTreeWidgetItem * {
    auto root = xml.getRootElement( );
    QTreeWidgetItem *rootItem = new QTreeWidgetItem;
    rootItem->setText( 0, QString::fromUtf8( root.tags( ).first.c_str( ) ) );
    // Using QMetaType
    rootItem->setData( 0, Qt::UserRole, QVariant::fromValue( root ) );

    std::queue<QTreeWidgetItem *> parentItemQu;
    parentItemQu.push( rootItem );

    std::queue<XmlElement> parentElementQu;
    parentElementQu.push( root );

    while ( !parentElementQu.empty( ) && !parentItemQu.empty( ) ) {
        auto parentElement = parentElementQu.front( );
        parentElementQu.pop( );
        auto parentItem = parentItemQu.front( );
        parentItemQu.pop( );

        auto elementChildren = parentElement.children( );
        for ( auto childElement : elementChildren ) {
            // create childItems
            QTreeWidgetItem *childItem = new QTreeWidgetItem( parentItem );
            childItem->setText(
                0, QString::fromUtf8( childElement.tagsRegex( ).first.c_str( ) ) );
            childItem->setData( 0, Qt::UserRole, QVariant::fromValue( childElement ) );
            childItem->setCheckState( 0, Qt::Unchecked );
            childItem->setFlags( Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
            // breadth first creation (Elements only)
            if ( childElement.hasChild( ) ) {
                parentElementQu.push( childElement );
                parentItemQu.push( childItem );
            }
        }
    }
    return rootItem;
}

static auto findXmlElement( QTreeWidgetItem *item, const XmlElement &element )
    -> QTreeWidgetItem * {
    std::queue<QTreeWidgetItem *> parents;
    parents.push( item );
    while ( !parents.empty( ) ) {
        auto parent = parents.front( );
        parents.pop( );
        QVariant parentData = parent->data( 0, Qt::UserRole );
        const XmlElement parentElement = parentData.value<XmlElement>( );
        if ( parentElement == element ) {
            return parent;
        }
        auto child = parent->child( 0 );
        while ( child ) {
            parents.push( child );
            child = parent->child( parent->indexOfChild( child ) + 1 );
        }
    }
    return nullptr;
}
