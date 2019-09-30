#include <iostream>
#include "NumberControllerAPI.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>


void init(){
    boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");
    boost::log::add_console_log(std::cout, boost::log::keywords::format = "[%TimeStamp%] >> %Message%");
    boost::log::add_file_log(
            boost::log::keywords::file_name="ExceptionLog.log",
            boost::log::keywords::format="[%TimeStamp%] %Message%",
            boost::log::keywords::auto_flush = true);
    boost::log::core::get()->set_filter(
            boost::log::trivial::severity >= boost::log::trivial::info
            );
    boost::log::add_common_attributes();
}


int main() {
    init();
    string_t port(U("8080"));
    SQLite::Database db("number.db3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
    try {
        db.exec("CREATE TABLE number_storage(key INTEGER PRIMARY KEY)");
    }
    catch(SQLite::Exception  & exception){
        BOOST_LOG_TRIVIAL(warning) << exception.what();
    }
    std::unique_ptr<NumberControllerAPI> numberAPI = std::unique_ptr<NumberControllerAPI>(
            new NumberControllerAPI("http://0.0.0.0:" + port + "/v1/increment/", &db));
    numberAPI->open().wait();
    std::string line;
    do {
        std::getline(std::cin, line);
    }
    while (line!="exit");

    numberAPI->close().wait();
    return 0;
}