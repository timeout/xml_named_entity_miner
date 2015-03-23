#include "xml_file_outline.hpp"
#include "xml_doc.hpp"
#include "xml_element.hpp"
#include <queue>
#include <string>
#include <sstream>

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

const std::string emptyXmlDoc{
    R"(<?xml version="1.0" encoding="UTF-8"?>
        <root/>
        )"};

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

auto XmlFileOutline::writeXml( ) const -> XmlDoc {
    auto rootItem = topLevelItem( 0 );
    auto rootVar = rootItem->data( 0, Qt::UserRole );
    auto rootElement = rootVar.value<XmlElement>( );
    XmlElement root;
    root.clone( rootElement );
    std::istringstream is{emptyXmlDoc};
    XmlDoc ret{is};
    ret.setRootElement( root );
    return ret;
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
    if ( search ) {
        qDebug( ) << "search result found";
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
    std::cerr << "getting target data . . ." << std::endl;
    QVariant targetVar = target->data( 0, Qt::UserRole );
    XmlElement targetElement = targetVar.value<XmlElement>( );
    // add checked attribute to element
    targetElement.attribute( "checked", "yes" );
    if ( !targetElement ) {
        std::cerr << "targetElement is nullptr" << std::endl;
    }
    std::cerr << "xml item selected . . . " << targetElement.name( ) << std::endl;
    emit xmlItemSelected( targetElement );
}

auto XmlFileOutline::addSentiment( Sentiment sentiment ) -> void {
    if ( selectedItem_->checkState( 0 ) != Qt::Checked ) {
        return;
    }
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
        if ( selectedItem_->checkState( 0 ) != Qt::Checked ) {
            return;
        }
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
    auto value = targetElement.attribute( "checked" );
    if ( !value.empty( ) ) {
        targetElement.removeAttribute( "checked" );
    }
    auto sentiment = targetElement.attribute( "sentiment" );
    if ( !sentiment.empty( ) ) {
        targetElement.removeAttribute( "sentiment" );
    }
    emit xmlItemDeselected( targetElement );
}

auto XmlFileOutline::contextMenu( ) -> void {
    semanticAction_ = new QAction{tr( "&Add Sentiment" ), this};
    semanticAction_->setShortcut( QKeySequence{Qt::CTRL + Qt::Key_I} );
    semanticAction_->setIcon(
        QIcon::fromTheme( "configure", QIcon{":/configure"} ) );

    positiveSemanticAction_ = new QAction{tr( "Positive" ), this};
    positiveSemanticAction_->setIcon(
        QIcon::fromTheme( "add", QIcon{":/add"} ) );
    positiveSemanticAction_->setToolTip(
        "Add a positive semantic value to entities in this XML element" );
    negativeSemanticAction_ = new QAction{tr( "Negative" ), this};
    negativeSemanticAction_->setIcon(
        QIcon::fromTheme( "edit-delete", QIcon{":/edit-delete"} ) );
    positiveSemanticAction_->setToolTip(
        "Add a negative semantic value to entities in this XML element" );
    neutralSemanticAction_ = new QAction{tr( "Neutral" ), this};
    neutralSemanticAction_->setIcon(
        QIcon::fromTheme( "help", QIcon{":/help-faq"} ) );
    neutralSemanticAction_->setChecked( true );

    QActionGroup *semanticGroup = new QActionGroup{this};
    semanticGroup->addAction( positiveSemanticAction_ );
    semanticGroup->addAction( negativeSemanticAction_ );
    semanticGroup->addAction( neutralSemanticAction_ );

    fileOutlineContextMenu_ = new QMenu{"Sentiment", this};
    fileOutlineContextMenu_->setWindowFlags( Qt::Popup );
    // fileOutlineContextMenu_->addAction( "Add a Sentiment" )->setEnabled( false );
    // fileOutlineContextMenu_->addSeparator( );
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
    rootItem->setText( 0, QString::fromUtf8( root.tags( false ).first.c_str( ) ) );
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

        if ( parentElement.hasChild( ) ) {
            auto ch = parentElement.child( );
            while ( ch ) {
                QTreeWidgetItem *childItem = new QTreeWidgetItem( parentItem );
                childItem->setText( 0, QString::fromStdString( ch.tags( true ).first ) );
                childItem->setData( 0, Qt::UserRole, QVariant::fromValue( ch ) );
                childItem->setCheckState( 0, Qt::Unchecked );
                childItem->setFlags( Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
                if ( ch.hasChild( ) ) {
                    parentElementQu.push( ch );
                    parentItemQu.push( childItem );
                }
                ch = ch.sibling( );
            }
        }
    }
    return rootItem;
}

static auto findXmlElement( QTreeWidgetItem *item, const XmlElement &element )
    -> QTreeWidgetItem * {
    qDebug( ) << "searching for element: " << QString::fromStdString( element.name( ) );
    std::queue<QTreeWidgetItem *> parents;
    parents.push( item );
    while ( !parents.empty( ) ) {
        auto parent = parents.front( );
        parents.pop( );
        QVariant parentData = parent->data( 0, Qt::UserRole );
        if ( parentData.value<XmlElement>( ) == element ) {
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
