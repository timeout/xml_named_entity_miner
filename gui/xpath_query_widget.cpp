#include "xpath_query_widget.hpp"
#include <QToolButton>
#include <QStyle>
#include <QListWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

QueryLineEdit::QueryLineEdit( QWidget *parent ) : QLineEdit{parent} {
    clearButton_ = new QToolButton( this );
    QIcon icon = QIcon::fromTheme( "edit-clear", QIcon( ":/images/edit-clear" ) );
    clearButton_->setIcon( icon );
    clearButton_->setCursor( Qt::ArrowCursor );
    clearButton_->setStyleSheet( "QToolButton { border: none; padding: 0px; }" );
    clearButton_->hide( );

    connect( clearButton_, SIGNAL( clicked( ) ), this, SLOT( clear( ) ) );
    connect( this, SIGNAL( textChanged( const QString & )), this,
             SLOT( updateCloseButton( const QString & )) );

    int frameWidth = style( )->pixelMetric( QStyle::PM_DefaultFrameWidth );
    setStyleSheet( QString( "QLineEdit { padding-right: %1px; } " )
                       .arg( clearButton_->sizeHint( ).width( ) + frameWidth + 1 ) );
    QSize msz = minimumSizeHint( );
    setMinimumSize(
        qMax( msz.width( ), clearButton_->sizeHint( ).height( ) + frameWidth * 2 + 2 ),
        qMax( msz.height( ), clearButton_->sizeHint( ).height( ) + frameWidth * 2 + 2 ) );
}

void QueryLineEdit::updateCloseButton( const QString &text ) {
    clearButton_->setVisible( !text.isEmpty( ) );
}

void QueryLineEdit::resizeEvent( QResizeEvent *event ) {
    QSize sz = clearButton_->sizeHint( );
    int frameWidth = style( )->pixelMetric( QStyle::PM_DefaultFrameWidth );
    clearButton_->move( rect( ).right( ) - frameWidth - sz.width( ),
                        ( rect( ).bottom( ) + 1 - sz.height( ) ) / 2 );
}

XPathQueryWidget::XPathQueryWidget( QWidget *parent )
    : QWidget{parent}, queryList_{new QListWidget}, queryLineEdit_{new QueryLineEdit},
      searchButton_{new QToolButton} {
    queryList_->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    QIcon searchIcon =
        QIcon::fromTheme( "document-preview", QIcon{":/images/document-preview"} );
    searchButton_->setIcon( searchIcon );
    searchButton_->setCursor( Qt::ArrowCursor );
    queryLineEdit_->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
    layout( );
    connections( );
}

void XPathQueryWidget::layout( ) {
    QGroupBox *groupBox = new QGroupBox{tr( "XPath Query" )};
    QHBoxLayout *controlsLayout = new QHBoxLayout;
    controlsLayout->addWidget( queryLineEdit_ );
    controlsLayout->addWidget( searchButton_ );

    QHBoxLayout *listLayout = new QHBoxLayout;
    listLayout->addWidget( queryList_ );

    QVBoxLayout *groupBoxLayout = new QVBoxLayout;
    groupBoxLayout->addLayout( controlsLayout );
    groupBoxLayout->addLayout( listLayout );

    groupBox->setLayout( groupBoxLayout );

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget( groupBox );
    mainLayout->setContentsMargins( 0, 0, 0, 0 );
    setLayout( mainLayout );
}

void XPathQueryWidget::connections( ) {}

