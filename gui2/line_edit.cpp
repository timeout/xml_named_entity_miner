#include "line_edit.hpp"
#include <QToolButton>
#include <QStyle>

LineEdit::LineEdit( QWidget *parent ) : QLineEdit{parent} {
    clearButton_ = new QToolButton( this );
    QIcon icon = QIcon::fromTheme( "edit-clear", QIcon( ":/edit-clear" ) );
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

void LineEdit::updateCloseButton( const QString &text ) {
    clearButton_->setVisible( !text.isEmpty( ) );
}

void LineEdit::resizeEvent( QResizeEvent *event ) {
    QSize sz = clearButton_->sizeHint( );
    int frameWidth = style( )->pixelMetric( QStyle::PM_DefaultFrameWidth );
    clearButton_->move( rect( ).right( ) - frameWidth - sz.width( ),
                        ( rect( ).bottom( ) + 1 - sz.height( ) ) / 2 );
    QLineEdit::resizeEvent( event );
}

