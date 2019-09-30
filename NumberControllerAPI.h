//
// Created by zombibober on 01.10.18.
//

#ifndef NUMBERCONTROLLERAPI_H
#define NUMBERCONTROLLERAPI_H


#include "cpprest/json.h"
#include "cpprest/http_listener.h"
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"
#include "cpprest/http_client.h"
#include <SQLiteCpp/SQLiteCpp.h>

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;
using namespace web::http;
using namespace web::http::client;

class NumberControllerAPI {
public:
    explicit NumberControllerAPI(string_t url,  SQLite::Database *  database);
    pplx::task<void> open() { return  listener.open();}
    pplx::task<void> close() { return listener.close();}
private:
    void handle_post(http_request message);
    http_listener listener;
     SQLite::Database * db = nullptr;
};


#endif
