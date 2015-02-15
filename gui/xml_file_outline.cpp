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

const XmlDoc &XmlFileOutline::xml( ) const { return xml_; }

void XmlFileOutline::onItemSelectionChanged( ) { qDebug( ) << "item selection changed\n"; }

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
