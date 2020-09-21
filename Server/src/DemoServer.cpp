#include <iostream>

#include "DemoServer.h"

using std::cout;
using std::endl;
using std::string;

/**
 * Constructor. We'll listen on all interfaces, this port number.
 */
DemoServer::DemoServer(int port)
    : RESTServer("*", port, 8)
{
    string url = "postgres://webdemo:webdemo-asdf@localhost:5432/webdemo";
    db.connect(url);
}


/**
 * Run forever.
 */
void DemoServer::setupRoutes() {
    // Set up the router.
    std::shared_ptr<Router> router = std::make_shared<Router>(this);

    router->get("/ping", HTTPFUNCTION_LAMBDA(DemoServer, ping));
    router->get("/login", HTTPFUNCTION_LAMBDA(DemoServer, login));

    addRouter(router);

    // Do at end.
    RESTServer::setupRoutes();
}

/**
 *
 */
void DemoServer::ping(HttpServerResponsePtr response, HttpServerRequestPtr ) {
    returnSuccess(response, "OK");
}


void DemoServer::login(HttpServerResponsePtr response, HttpServerRequestPtr request) {
    string username;
    string pw;

    if (!parseAuthorizationHeader(request, username, pw)) {
        cout << "No proper Authorization header." << endl;
        returnError(response, "Invalid login");
        return;
    }

    User::Pointer user = db.getUser(username);
    if (user == nullptr) {
        cout << "Invalid user." << endl;
        returnError(response, "Invalid login");
        return;
    }

    string crypted = Database::simpleSHA256(pw);
    if (crypted != user->getCryptedPassword()) {
        cout << "Invalid password." << endl;
        returnError(response, "Invalid login");
        return;
    }

    returnSuccess(response, "OK");
}
