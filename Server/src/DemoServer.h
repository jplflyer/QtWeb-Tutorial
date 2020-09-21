#pragma once

#include "RESTServer.h"
#include "Database.h"

class DemoServer: public RESTServer
{
private:
    Database db;

public:
    DemoServer(int port);

    void setupRoutes();
    void ping(HttpServerResponsePtr response, HttpServerRequestPtr request);
    void login(HttpServerResponsePtr response, HttpServerRequestPtr request);

};
