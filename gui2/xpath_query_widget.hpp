#pragma once
#include <QWidget>
#include <QStringList>
#include "xml_doc.hpp"

class LineEdit;
class QListView;
class QStringListModel;
class QToolButton;
class QModelIndex;

class XPathQueryWidget : public QWidget {
    Q_OBJECT
public:
    explicit XPathQueryWidget( QWidget *parent = nullptr );
    auto xml( const XmlDoc &xml ) -> void;
    auto clear( ) -> void;

signals:
    void queryResult( const XmlElement &element );

private slots:
    void queryText( );
    void activateQueryItem( const QModelIndex &index );

private:
    auto doQuery( const QString &query ) -> bool;
    auto layout( ) -> void;
    auto connections( ) -> void;

    QListView *queryListView_;
    QStringListModel *queryListModel_;
    LineEdit *lineEdit_;
    QToolButton *searchButton_;
    QStringList xpathQueries_;
    XmlDoc xml_;
};
