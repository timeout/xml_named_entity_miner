#pragma once
#include <QLineEdit>

class QToolButton;

class LineEdit : public QLineEdit {
    Q_OBJECT
public:
    explicit LineEdit( QWidget *parent = nullptr );

public slots:
    void updateCloseButton( const QString &text );

protected:
    void resizeEvent( QResizeEvent *event );

private:
    QToolButton *clearButton_;
};

