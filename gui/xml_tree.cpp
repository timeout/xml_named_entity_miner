#include "xml_tree.hpp"
#include "xml_doc.hpp"
#include "xml_element.hpp"
#include <vector>
#include <string>
#include <QWidget>
#include <libxml/tree.h>
#include <QDebug>
#include <queue>

XmlTree::XmlTree( QWidget *parent ) : QTreeWidget{parent} {
    QTreeWidgetItem *topLevelItem = new QTreeWidgetItem;
    topLevelItem->setText( 0, "root" );
    addTopLevelItem( topLevelItem );
    QTreeWidgetItem *childItem = new QTreeWidgetItem( topLevelItem );
    childItem->setText( 0, "text" );
    QTreeWidgetItem *child = new QTreeWidgetItem;
    child->setText( 0, "another child" );
    topLevelItem->addChild( child );

    setContextMenuPolicy( Qt::CustomContextMenu );
    // connect( this, SIGNAL( itemSelectionChanged( ) ), this,
    //          SLOT( onItemSelectionChanged( ) ) );

    // checkbox
    connect( this, SIGNAL( itemChanged( QTreeWidgetItem *, int )), this,
             SLOT( onItemChanged( QTreeWidgetItem *, int )) );
}

XmlTree::XmlTree( const XmlDoc &xml, QWidget *parent ) : QTreeWidget{parent} {
    auto root = xml.getRootElement( );
    auto xpaths = root.children( );

    // top item
    QTreeWidgetItem *rootItem = new QTreeWidgetItem;
    rootItem->setText( 0, QString::fromUtf8( root.name( ).c_str( ) ) );
    addTopLevelItem( rootItem );

    // add children
    auto childNodes = root.children( );
    for ( auto el : childNodes ) {
        QTreeWidgetItem *child = new QTreeWidgetItem( rootItem );
        child->setText( 0, QString::fromUtf8( el.name( ).c_str( ) ) );
    }
}

void XmlTree::xml( const XmlDoc xml ) {
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
const XmlDoc &XmlTree::xml( ) const { return xml_; }

void XmlTree::onItemSelectionChanged( ) { qDebug( ) << "item selection changed\n"; }
void XmlTree::onItemChanged( QTreeWidgetItem *item, int column ) {
    if ( item->checkState( column ) == Qt::Unchecked ) {
        qDebug( ) << item->text( 0 ) << "un-checked\n";
    }
    if ( item->checkState( column ) == Qt::Checked ) {
        qDebug( ) << item->text( 0 ) << "checked\n";
        QVariant xmlElementVar = item->data( 0, Qt::UserRole );
        XmlElement targetElement = xmlElementVar.value<XmlElement>( );

        // Debug:
        qDebug( ) << "targetElement: [ "
                  << QString::fromUtf8( targetElement.name( ).c_str( ) ) << " ]\n";
        if ( targetElement.hasContent( ) ) {
            qDebug( ) << QString::fromUtf8( targetElement.content( ).c_str( ) ) << "\n";
        } else {
            qDebug() << "Element has no content\n";
        }
    }
}
