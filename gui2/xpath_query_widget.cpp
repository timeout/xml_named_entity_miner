#include "xpath_query_widget.hpp"
#include "xpath_ctxt.hpp"
#include "xpath_query.hpp"
#include "xpath_result_set.hpp"
#include "line_edit.hpp"
#include <QToolButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
// #include <QListWidget>
// #include <QListWidgetItem>
#include <QListView>
#include <QStringListModel>
#include <QStringList>
#include <QModelIndex>

#include <QDebug>

XPathQueryWidget::XPathQueryWidget( QWidget *parent )
    : QWidget{parent}
    , queryListView_{new QListView{this}}
    , queryListModel_{new QStringListModel{this}}
    , lineEdit_{new LineEdit{this}}
    , searchButton_{new QToolButton{this}} {
    queryListModel_->setStringList( xpathQueries_ );
    queryListView_->setModel( queryListModel_ );
    queryListView_->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    queryListView_->setEditTriggers( QAbstractItemView::NoEditTriggers );
    queryListView_->setSelectionMode( QAbstractItemView::SingleSelection );
    QIcon searchIcon = QIcon::fromTheme( "document-preview",
                                         QIcon{":/images/document-preview-archive.svg"} );
    searchButton_->setIcon( searchIcon );
    searchButton_->setCursor( Qt::ArrowCursor );
    lineEdit_->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
    layout( );
    connections( );
}

auto XPathQueryWidget::xml( const XmlDoc &xml ) -> void { xml_ = xml; }

auto XPathQueryWidget::clear( ) -> void {
    if ( xml_ ) {
        xml_ = XmlDoc{};
    }
}

auto XPathQueryWidget::doQuery( const QString &query ) -> bool {
    if ( query == "/" ) { // hack XmlElements do not check for document root type
        qDebug( ) << QString::fromStdString( xml_.getRootElement( ).name( ) );
        emit queryResult( xml_.getRootElement( ) );
        return true;
    }
    XPathCtxt ctxt{xml_};
    if ( ctxt.errorHandler( ).hasErrors( ) ) {
        qDebug( ) << "error handler has errors";
        return false;
    }
    XPathQuery xpq{ctxt};
    xpq.query( query.toStdString( ) );
    if ( xpq ) {
        XPathResultSet resultSet{xpq};
        for ( auto iter = resultSet.begin( ); iter != resultSet.end( ); ++iter ) {
            qDebug( ) << "emiting result: "
                      << QString::fromStdString( ( *iter ).name( ) );
            emit queryResult( *iter );
        }
    } else {
        return false;
    }
    return true;
}

void XPathQueryWidget::queryText( ) {
    const QString query = lineEdit_->text( );
    if ( !query.isEmpty( ) && doQuery( query ) ) {
        // saveQuery( query );
        if ( !xpathQueries_.contains( query ) ) {
            xpathQueries_ << query;
            queryListModel_->setStringList( xpathQueries_ );
        }
    }
}

void XPathQueryWidget::activateQueryItem( const QModelIndex &index ) {
    QVariant qVar = queryListModel_->data( index, Qt::DisplayRole );
    QString query = qVar.toString( );
    doQuery( query );
}

auto XPathQueryWidget::layout( ) -> void {
    QGroupBox *groupBox = new QGroupBox{tr( "XPath Query" )};
    QHBoxLayout *controlsLayout = new QHBoxLayout;
    controlsLayout->addWidget( lineEdit_ );
    controlsLayout->addWidget( searchButton_ );

    QHBoxLayout *listLayout = new QHBoxLayout;
    listLayout->addWidget( queryListView_ );

    QVBoxLayout *groupBoxLayout = new QVBoxLayout;
    groupBoxLayout->addLayout( controlsLayout );
    groupBoxLayout->addLayout( listLayout );

    groupBox->setLayout( groupBoxLayout );

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget( groupBox );
    mainLayout->setContentsMargins( 0, 0, 0, 0 );
    setLayout( mainLayout );
}

auto XPathQueryWidget::connections( ) -> void {
    connect( lineEdit_, &LineEdit::editingFinished, this, &XPathQueryWidget::queryText );
    connect( queryListView_, &QListView::activated, this,
             &XPathQueryWidget::activateQueryItem );
}

