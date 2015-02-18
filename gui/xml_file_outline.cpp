#include "xml_file_outline.hpp"
#include "xml_doc.hpp"
#include "xml_element.hpp"
#include <vector>
#include <string>
#include <QWidget>
#include <QTreeWidgetItemIterator>
#include <libxml/tree.h>
#include <QDebug>
#include <queue>

XmlFileOutline::XmlFileOutline( QWidget *parent ) : QTreeWidget{parent} {
    setContextMenuPolicy( Qt::CustomContextMenu );
    connect( this, SIGNAL( clicked( const QModelIndex & )),
             SLOT( itemClicked( const QModelIndex & )) );
}

void XmlFileOutline::xml( const XmlDoc &xml ) {
    xml_ = xml;
    clear( ); // public slot of QTreeWidget

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

    addTopLevelItem( rootItem );
}

const XmlDoc &XmlFileOutline::xml( ) const { return xml_; }

void XmlFileOutline::onItemSelectionChanged( ) {
    qDebug( ) << "item selection changed\n";
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
