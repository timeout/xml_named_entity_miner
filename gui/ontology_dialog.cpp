#include "ontology_dialog.hpp"
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QApplication>
#include <QSizePolicy>
#include <QGroupBox>
#include <QColorDialog>
#include <QSettings>
#include <QStringList>
#include <QStandardItem>
#include <QStandardItemModel>

#include <QDebug>

static QPixmap pixmap( const QColor &color, int width );

OntologyDialogLineEdit::OntologyDialogLineEdit( QWidget *parent )
    : QLineEdit{parent}, activePalette_{palette( )},
      inactivePalette_{QGuiApplication::palette( )}, edited_{false} {
    inactivePalette_.setColor(
        QPalette::Active, QPalette::Text,
        inactivePalette_.color( QPalette::Disabled, QPalette::WindowText ) );
    inactivePalette_.setColor(
        QPalette::Inactive, QPalette::Text,
        inactivePalette_.color( QPalette::Disabled, QPalette::WindowText ) );
    setPalette( inactivePalette_ );
    connect( this, SIGNAL( keyPressed( bool )), SLOT( clearDefaultEntry( bool )) );
    connect( this, SIGNAL( mousePressed( bool )), SLOT( clearDefaultEntry( bool )) );
}

void OntologyDialogLineEdit::clearDefaultEntry( bool ) {
    clear( );
    setPalette( activePalette_ );
}

void OntologyDialogLineEdit::focusInEvent( QFocusEvent *event ) {
    QLineEdit::focusInEvent( event );
    emit( focussed( true ) );
}

void OntologyDialogLineEdit::keyPressEvent( QKeyEvent *event ) {
    QLineEdit::keyPressEvent( event );
    if ( !edited_ ) {
        emit( keyPressed( true ) );
    }
    edited_ = true;
}

void OntologyDialogLineEdit::mousePressEvent( QMouseEvent *event ) {
    QLineEdit::mousePressEvent( event );
    if ( !edited_ ) {
        emit( mousePressed( true ) );
    }
    edited_ = true;
}

OntologyDialog::OntologyDialog( QWidget *parent )
    : QDialog{parent}, configGroupBox_{new QGroupBox{tr( "New Ontology:" )}},
      label_{new QLabel{tr( "&Name:" )}}, ontologyName_{new QComboBox},
      chooseColor_{new QPushButton},
      buttonBox_{new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel )},
      color_{0xb5, 0x89, 0x0}, defaultEntryCleared_{false} {

    label_->setBuddy( ontologyName_ );
    label_->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

    // comboBox
    ontologyName_->setLineEdit( new OntologyDialogLineEdit );
    ontologyName_->setMaxCount( 5 );
    ontologyName_->setEditable( true );
    ontologyName_->setInsertPolicy( QComboBox::InsertAtTop );
    QFontMetrics metrics{QApplication::font( )};
    ontologyName_->setMinimumWidth( metrics.averageCharWidth( ) * 24 );
    ontologyName_->lineEdit( )->setSizePolicy( QSizePolicy::Expanding,
                                               QSizePolicy::Fixed );
    // ontologyName_->setToolTip( tr( "Enter an ontology name" ) );

    QSettings settings;
    QStringList categories = settings.value( "recentOntologyNames" ).toStringList( );
    ontologyName_->addItems( categories );

    ontologyName_->lineEdit( )->setText( "Enter ontology name" );

    // color button
    chooseColor_->setFixedSize( 24, 24 );
    chooseColor_->setToolTip( tr( "Choose a colour" ) );
    setChooseColor( );

    layout( );
    connections( );

    setWindowTitle( tr( "Ontology Dialog" ) );
}

const QString OntologyDialog::name( ) const {
    QString name{ontologyName_->lineEdit( )->text( )};

    QSettings settings;
    QStringList categories = settings.value( "recentOntologyNames" ).toStringList( );
    categories.removeAll( name );
    categories.prepend( name );
    while ( categories.size( ) > 5 ) {
        categories.removeLast( );
    }
    settings.setValue( "recentOntologyNames", categories );

    return name;
}

const QColor OntologyDialog::color( ) const { return color_; }

void OntologyDialog::startColourDialog( ) {
    // yellow	#b58900
    // orange	#cb4b16
    // red	    #dc322f
    // magenta	#d33682
    // violet	#6c71c4
    // blue	    #268bd2
    // cyan	    #2aa198
    // green	#859900
    QColorDialog *colorDialog = new QColorDialog{this};
    colorDialog->setCustomColor( 0, QColor{0xb5, 0x89, 0x00} );
    colorDialog->setCustomColor( 1, QColor{0xcb, 0x4b, 0x16} );
    colorDialog->setCustomColor( 2, QColor{0xdc, 0x32, 0x2f} );
    colorDialog->setCustomColor( 3, QColor{0xd3, 0x36, 0x82} );
    colorDialog->setCustomColor( 4, QColor{0x6c, 0x71, 0xc4} );
    colorDialog->setCustomColor( 5, QColor{0x26, 0x8b, 0xd2} );
    colorDialog->setCustomColor( 6, QColor{0x2a, 0xa1, 0x98} );
    colorDialog->setCustomColor( 7, QColor{0x85, 0x99, 0x00} );
    color_ = colorDialog->getColor( color_, this );
    setChooseColor( );
    emit colorChanged( color_ );
}

void OntologyDialog::setChooseColor( ) {
    QIcon colorIcon( pixmap( color_, chooseColor_->height( ) ) );
    chooseColor_->setIcon( colorIcon );
}

void OntologyDialog::layout( ) {
    QHBoxLayout *controlsLayout = new QHBoxLayout;
    controlsLayout->addWidget( label_ );
    controlsLayout->addWidget( ontologyName_ );
    controlsLayout->addWidget( chooseColor_ );

    configGroupBox_->setLayout( controlsLayout );

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget( buttonBox_ );

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget( configGroupBox_ );
    layout->addLayout( bottomLayout );

    setLayout( layout );
}

void OntologyDialog::connections( ) {
    connect( chooseColor_, SIGNAL( clicked( ) ), this, SLOT( startColourDialog( ) ) );
    connect( buttonBox_, SIGNAL( accepted( ) ), this, SLOT( accept( ) ) );
    connect( buttonBox_, SIGNAL( rejected( ) ), this, SLOT( reject( ) ) );
}

static QPixmap pixmap( const QColor &color, int width ) {
    QPixmap ret( width, width );
    ret.fill( color );
    return ret;
}
