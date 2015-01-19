#pragma once
#include "xml_doc.hpp"
#include <QTreeWidget>

class XmlTree : public QTreeWidget {
    Q_OBJECT
public:
    XmlTree( QWidget *parent = nullptr );
    void xml( const XmlDoc xml );
    const XmlDoc &xml( ) const;

public slots:
    void onItemSelectionChanged( );
    void itemClicked( const QModelIndex &idx );

signals:
    void xmlItemSelected( const XmlElement &element );
    void xmlItemDeselected( const XmlElement &element );

private:
    void checkSubtree( QTreeWidgetItem *target );
    void uncheckSubtree( QTreeWidgetItem *target );
    XmlDoc xml_;
};

