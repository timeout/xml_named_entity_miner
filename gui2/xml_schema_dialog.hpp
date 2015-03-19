#pragma once
#include "xml_task_dialog.hpp"

class XmlSchemaDialog : public XmlTaskDialog {
    Q_OBJECT
public:
    XmlSchemaDialog( const XmlDoc &xmlSchemaDoc, QWidget *parent = nullptr );
    virtual ~XmlSchemaDialog( );

protected slots:
    virtual void open( ) override;
    virtual void showError( const QString &msg ) override;

protected:
    virtual auto parse( const QString &filename ) -> void override;
};
