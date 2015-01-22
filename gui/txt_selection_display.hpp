#pragma once
#include <QPlainTextEdit>

class XmlElement;

class TxtSelectionDisplay : public QPlainTextEdit {
    Q_OBJECT
public:
    TxtSelectionDisplay( QWidget *parent = nullptr );

protected:
    void mousePressEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );

public slots:
    void setXmlTxt( const XmlElement &element );

private:
    void cursorSelection( );
    const QList<QTextEdit::ExtraSelection> findAll( const QString &textSelection );

    struct cursorSelection {
        QTextCursor begin_, end_;
    } cursorSelection_;
    QList<QTextEdit::ExtraSelection> extraSelections_;
};