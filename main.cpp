#include "lib/bilanz.hpp"
#include "lib/xml_doc.hpp"
#include <iostream>
#include <thread>
#include <fstream>

int main( ) {
    std::cout << "Hello World!" << std::endl;
    XmlDoc xml;
    std::cerr << "opening file..." << std::endl;
    std::ifstream f( "test.xml", std::ios::in );
    f >> xml;
    std::cout << xml << std::endl;

    std::ifstream g( "test.xml", std::ios::in );
    XmlDoc xml1{g};
    if (xml1.errorHandler().hasErrors()) {
        std::cerr << xml1.errorHandler() << std::endl;
    }
    std::cerr << xml1 << std::endl;

    return 0;
}
