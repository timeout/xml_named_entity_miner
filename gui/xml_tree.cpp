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
    // hack possible function is QAbstractItemView::update(const
    //                                  QModelIndex& index);

    auto root = xml.getRootElement( );
    QTreeWidgetItem *rootItem = new QTreeWidgetItem;
    rootItem->setText( 0, QString::fromUtf8( root.name( ).c_str( ) ) );

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
            std::cerr << childElement.name( ) << std::endl;

            // create childItems
            QTreeWidgetItem *childItem = new QTreeWidgetItem( parentItem );
            childItem->setText( 0, QString::fromUtf8( childElement.name( ).c_str( ) ) );
            childItem->setCheckState( 0, Qt::Unchecked );
            childItem->setFlags( Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );

            if ( childElement.hasChild( ) ) {
                std::cerr << " -- has child(ren)" << std::endl;
                parentElementQu.push( childElement );
                parentItemQu.push( childItem );
            }
        }
    }

    addTopLevelItem( rootItem );

    /*
    rootItem->setText( 0, QString::fromUtf8( root.name( ).c_str( ) ) );
    addTopLevelItem( rootItem );

    // add children
    auto childNodes = root.children( );
    for ( auto el : childNodes ) {
        QTreeWidgetItem *child = new QTreeWidgetItem( rootItem );
        child->setText( 0, QString::fromUtf8( el.name( ).c_str( ) ) );
    }
    */
}
const XmlDoc &XmlTree::xml( ) const { return xml_; }
