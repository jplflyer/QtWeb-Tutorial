#pragma once

#include "RESTServer.h"
#include "Database.h"

class DemoServer: public RESTServer
{
private:
    Database db;

private:
    bool verifyLogin(HttpServerResponsePtr response, HttpServerRequestPtr request);

public:
    DemoServer(int port);

    void setupRoutes();
    void ping(HttpServerResponsePtr response, HttpServerRequestPtr request);
    void login(HttpServerResponsePtr response, HttpServerRequestPtr request);
    void getEventTemplates(HttpServerResponsePtr response, HttpServerRequestPtr request);
    void getEvents(HttpServerResponsePtr response, HttpServerRequestPtr request);

};
