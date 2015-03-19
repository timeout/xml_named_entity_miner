#include "xml_task_dialog.hpp"
#include "line_edit.hpp"

#include <QApplication>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QProgressBar>
#include <QToolButton>
#include <QVBoxLayout>

XmlTaskDialog::XmlTaskDialog( const XmlDoc &xml, QWidget *parent )
    : QDialog{parent}
    , path_{new QLabel{tr( "Path: " )}}
    , lineEdit_{new LineEdit{this}}
    , fileButton_{new QToolButton{this}}
    , progressBar_{new QProgressBar{this}}
    , buttonBox_{
          new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this )}
    , xml_{xml} {
    path_->setBuddy( lineEdit_ );
    path_->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    QFontMetrics metrics{QApplication::font( )};
    lineEdit_->setMinimumWidth( metrics.averageCharWidth( ) * 32 );
    QIcon icon = QIcon::fromTheme( "document-open", QIcon( ":/images/document-open" ) );
    fileButton_->setIcon( icon );
}

XmlTaskDialog::~XmlTaskDialog( ) {}

auto XmlTaskDialog::layout( ) -> void {
    QGroupBox *groupBox = new QGroupBox{title_};
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget( path_ );
    topLayout->addWidget( lineEdit_ );
    topLayout->addWidget( fileButton_ );
    QHBoxLayout *middleLayout = new QHBoxLayout;
    middleLayout->addWidget( progressBar_ );
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget( buttonBox_ );
    QVBoxLayout *groupBoxLayout = new QVBoxLayout;
    groupBoxLayout->addLayout( topLayout );
    groupBoxLayout->addLayout( middleLayout );
    groupBox->setLayout( groupBoxLayout );
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget( groupBox );
    mainLayout->addLayout( bottomLayout );
    setLayout( mainLayout );
}

auto XmlTaskDialog::connections( ) -> void {
    connect( fileButton_, &QToolButton::clicked, this, &XmlTaskDialog::open );
    connect( this, &XmlTaskDialog::progress, progressBar_, &QProgressBar::setValue );
    connect( this, &XmlTaskDialog::errorRaised, this, &XmlTaskDialog::showError );
    connect( buttonBox_, &QDialogButtonBox::accepted, this, &XmlTaskDialog::accept );
    connect( buttonBox_, &QDialogButtonBox::rejected, this, &XmlTaskDialog::reject );
}

