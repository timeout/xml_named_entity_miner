#pragma once
#include "xml_doc.hpp"
#include <xml_schema_parser.hpp>
#include <QDialog>
#include <QString>

class QLabel;
class LineEdit;
class QToolButton;
class QProgressBar;
class QDialogButtonBox;

class XmlTaskDialog : public QDialog {
    Q_OBJECT
public:
    XmlTaskDialog( const XmlDoc &xml, QWidget *parent = nullptr );
    virtual ~XmlTaskDialog( );

protected slots:
    virtual void open( ) = 0;
    virtual void showError( const QString &msg ) = 0;

signals:
    void errorRaised( const QString &msg );
    void progress( int step );

protected:
    virtual void parse( const QString &filename ) = 0;
    void layout( );
    void connections( );

    QLabel *path_;
    LineEdit *lineEdit_;
    QToolButton *fileButton_;
    QProgressBar *progressBar_;
    QDialogButtonBox *buttonBox_;
    XmlDoc xml_;
    QString title_;
};

class XmlSchemaDialog : public XmlTaskDialog {
    Q_OBJECT
public:
    XmlSchemaDialog( const XmlDoc &xmlSchema, QWidget *parent = nullptr );
    virtual ~XmlSchemaDialog( );

protected slots:
    virtual void open( ) override;
    virtual void showError( const QString &msg ) override;

protected:
    virtual void parse( const QString &filename ) override;
};

class XsltDialog : public XmlTaskDialog {
    Q_OBJECT
public:
    XsltDialog( const XmlDoc &stylesheet, QWidget *parent = nullptr );
    virtual ~XsltDialog( );

protected slots:
    virtual void open( ) override;
    virtual void showError( const QString &msg ) override;

protected:
    virtual void parse( const QString &filename ) override;

private:
    // XmlDoc transDoc_;
};

