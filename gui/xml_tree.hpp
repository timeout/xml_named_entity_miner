#pragma once
#include "xml_doc.hpp"
#include <QTreeWidget>

class XmlTree : public QTreeWidget {
    Q_OBJECT
public:
    XmlTree( QWidget *parent = nullptr );
    XmlTree( const XmlDoc &xml, QWidget *parent = nullptr );
    void xml( const XmlDoc xml );
    const XmlDoc &xml( ) const;

public slots:
    void onItemSelectionChanged( );
    void onItemChanged(QTreeWidgetItem *item, int column);

private:
    XmlDoc xml_;
};

