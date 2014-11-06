#include "lib/bilanz.hpp"
#include "lib/xml_doc.hpp"
#include "lib/logger.hpp"
#include <iostream>
#include <thread>

int main( ) {
    log_inst.set_thread_name( "MAIN" );
    LOG_WARN( "Starting the application..", std::this_thread::get_id( ) );
    std::cout << "Hello World!" << std::endl;
    try {
        LOG( "Opening xml document" );
        XmlDoc doc{Pathname{"test.xml"}};
        std::cout << doc << std::endl;
    } catch ( ... ) {
    }
    LOG_ERR( "All good things must come to an end." );
    log_inst.terminate_logger( );
    return 0;
}
