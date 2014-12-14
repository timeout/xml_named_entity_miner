#include "xml_doc.hpp"

class XmlCreator {
public:
    XmlCreator( );

    auto booksXml( ) const -> const XmlDoc & { return books_xml_; }
    auto menuXml( ) const -> const XmlDoc & { return menu_xml_; }

private:
    XmlDoc books_xml_, menu_xml_;
};

template <typename T>
auto cp_f( T t ) -> T {
    return t;
}

template <typename T>
auto testBool_f( const T &t ) -> bool {
    return ( t ) ? true : false;
}
