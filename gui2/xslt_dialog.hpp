#pragma once
#include "xml_task_dialog.hpp"
#include "xml_doc.hpp"

class XsltDialog : public XmlTaskDialog {
    Q_OBJECT
public:
    XsltDialog( const XmlDoc &xml, QWidget *parent = nullptr );
    virtual ~XsltDialog( );
    auto transformedXml( ) const -> const XmlDoc &;

protected slots:
    virtual void open( ) override;
    virtual void showError( const QString &msg ) override;

protected:
    virtual auto parse( const QString &filename ) -> void override;

private:
    XmlDoc trans_;
};
