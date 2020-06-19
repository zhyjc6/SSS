/**
 * "main.cpp"
 *
 * @author zhyjc6
 * @date 2020.6.16
 *
 */

#include "httpd.h"


int main() {
    httpserver::HttpServer server;
    server.startup(0);
    server.run();

    return 0;
}

