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
    router->get("/event_templates", HTTPFUNCTION_LAMBDA(DemoServer, getEventTemplates));
    router->get("/events", HTTPFUNCTION_LAMBDA(DemoServer, getEvents));

    addRouter(router);

    // Do at end.
    RESTServer::setupRoutes();
}

bool
DemoServer::verifyLogin(HttpServerResponsePtr response, HttpServerRequestPtr request) {
    string username;
    string pw;

    if (!parseAuthorizationHeader(request, username, pw)) {
        cout << "No proper Authorization header." << endl;
        returnError(response, "Invalid login");
        return false;
    }

    User::Pointer user = db.getUser(username);
    if (user == nullptr) {
        cout << "Invalid user." << endl;
        returnError(response, "Invalid login");
        return false;
    }

    string crypted = Database::simpleSHA256(pw);
    if (crypted != user->getCryptedPassword()) {
        cout << "Invalid password." << endl;
        returnError(response, "Invalid login");
        return false;
    }

    return true;
}

/**
 *
 */
void
DemoServer::ping(HttpServerResponsePtr response, HttpServerRequestPtr ) {
    returnSuccess(response, "OK");
}

/**
 * Login verification. This actually isn't necessary for use of the other calls,
 * but it provides a login method that can be used to validate credentials.
 */
void
DemoServer::login(HttpServerResponsePtr response, HttpServerRequestPtr request) {
    if (!verifyLogin(response, request)) {
        return;
    }

    returnSuccess(response, "OK");
}

/**
 * Retrieve all Event Templates.
 */
void
DemoServer::getEventTemplates(HttpServerResponsePtr response, HttpServerRequestPtr request) {
    if (!verifyLogin(response, request)) {
        return;
    }

    EventTemplate::Vector vec;
    db.getEventTemplates(vec);
    returnSuccess(response, "OK", &vec, "eventTemplates");
}

/**
 * Retrieve all Events.
 */
void
DemoServer::getEvents(HttpServerResponsePtr response, HttpServerRequestPtr request) {
    if (!verifyLogin(response, request)) {
        return;
    }

    Event::Vector vec;
    db.getEvents(vec);
    returnSuccess(response, "OK", &vec, "events");
}
