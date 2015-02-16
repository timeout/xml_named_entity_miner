#pragma once
#include <QWidget>
#include <QLineEdit>

class QListWidget;
class QToolButton;
class QLabel;

class QueryLineEdit : public QLineEdit {
    Q_OBJECT
public:
    explicit QueryLineEdit( QWidget *parent = nullptr );

public slots:
    void updateCloseButton(const QString &text);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QToolButton *clearButton_;
};

class XPathQueryWidget : public QWidget {
    Q_OBJECT
public:
    explicit XPathQueryWidget( QWidget *parent = nullptr );

private:
    void layout();
    void connections();

    QListWidget *queryList_;
    QueryLineEdit *queryLineEdit_;
    QToolButton *searchButton_;
};
