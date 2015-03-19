#pragma once
#include "xml_schema_parser.hpp"
#include <QDialog>

class LineEdit;
class QDialogButtonBox;
class QLabel;
class QProgressBar;
class QToolButton;

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
    virtual auto parse( const QString &filename ) -> void = 0;
    auto layout( ) -> void;
    auto connections( ) -> void;

    QLabel *path_;
    LineEdit *lineEdit_;
    QToolButton *fileButton_;
    QProgressBar *progressBar_;
    QDialogButtonBox *buttonBox_;
    XmlDoc xml_;
    QString title_;
};

inline auto makeErrorMsg( QString &msg, const std::string &handlerMsg ) -> const QString
    & {
    std::cerr << "handler message " << handlerMsg << std::endl;
    if ( !handlerMsg.empty( ) ) {
        msg += "\n";
        msg += QString::fromStdString( handlerMsg );
    }
    return msg;
}

