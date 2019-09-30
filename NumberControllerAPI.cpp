//
// Created by zombibober on 01.10.18.
//

#include "NumberControllerAPI.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;
using namespace web::http;
using namespace web::http::client;



NumberControllerAPI::NumberControllerAPI(string_t url, SQLite::Database * database) : listener(url), db(database){
    db = database;
    listener.support(methods::POST, std::bind(&NumberControllerAPI::handle_post,this,std::placeholders::_1));
}


void NumberControllerAPI::handle_post(http_request message) {

    string_t body = message.extract_string().get();
    int val = std::stoi(body);
    if(val < 0) {
        BOOST_LOG_TRIVIAL(warning) << "Number must be positive " << val << std::endl;
        message.reply(status_codes::BadRequest, "number must be positive");
    }
    SQLite::Statement select(*db, "SELECT * FROM number_storage WHERE key = ?");
    select.bind(1, val);
    if (select.executeStep()) {
        //int value = select.getColumn(0).getInt();
        BOOST_LOG_TRIVIAL(warning) << "Handle exception #1(number already exists):" << val;
        message.reply(status_codes::BadRequest, "Handle exception #1(number already exists):" + std::to_string(val));
        return;
    }
    select.reset();

    select.bind(1, val + 1);
    if(select.executeStep()) {
        //int value = select.getColumn(0);
        BOOST_LOG_TRIVIAL(warning) << "Handle exception #2(incremented number already exists): " << (val+1);
        message.reply(status_codes::BadRequest, "Handle exception #2(incremented number already exists):" +std::to_string(val + 1));
        return;
    }
    SQLite::Statement insert(*db, "INSERT INTO number_storage VALUES (?)");
    insert.bind(1,val);
    insert.exec();
    message.reply(status_codes::OK, std::to_string(val + 1));
}

