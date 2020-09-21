#include "StringMethods.h"
#include "RESTServer.h"

using namespace std;
using namespace nlohmann;

const string RESTServer::HTTP_GET = "GET";
const string RESTServer::HTTP_PUT = "PUT";
const string RESTServer::HTTP_POST = "POST";
const string RESTServer::HTTP_DELETE = "DELETE";

/**
 * Constructor.
 */
RESTServer::RESTServer(const string & _host, const int _port, const int _threads)
    : port(_port), host(_host), threads(_threads)
{
	cout << "RESTServer::RESTServer()\n";

    htmlHeaders.insert(std::pair<string,string>("Content-Type", "text/html"));
    textHeaders.insert(std::pair<string,string>("Content-Type", "text/plain"));
    jsonHeaders.insert(std::pair<string,string>("Content-Type", "application/json"));
}

/**
 * Destructor.
 */
RESTServer::~RESTServer() {
}


/**
 * Provide an error return.
 */
void
RESTServer::returnError(std::shared_ptr<HttpServerResponse> response, const string &error) {
	cout << "Returning error: " << error << endl;

	// Build the pt and then write it to a string stream.
	json propertyTree;
	propertyTree["error"] = error;

    // Output to the response.
    response->write(SimpleWeb::StatusCode::client_error_bad_request, propertyTree.dump(2));
}

/**
 * Provide an error return.
 */
void
RESTServer::returnError(std::shared_ptr<HttpServerResponse> response, const char *error) {
	string errorStr(error);
	returnError(response, errorStr);
}

/**
 * Provide an error return.
 */
void
RESTServer::returnError(std::shared_ptr<HttpServerResponse> response, const char *errorFormat, const string &argument) {
	char *	errorPtr;

	asprintf(&errorPtr, errorFormat, argument.c_str());
	string errorStr(errorPtr);

	returnError(response, errorStr);
}

/**
 * We're returning success. We need to update the response with a success JSON.
 */
void
RESTServer::returnSuccess(
    std::shared_ptr<HttpServerResponse> response,
    const std::string &msg,
    const std::string &key,
    const std::string &value,
    const JSON_Serializable *createdObject,
    const std::string createdObjectKey)
{
	json propertyTree;

	propertyTree["success"] = msg;
    if (key.length() > 0) {
        propertyTree[key] = value;
    }
    if (createdObject != nullptr) {
		json child;
		createdObject->toJSON(child);
        propertyTree[createdObjectKey.length() > 0 ? createdObjectKey : "object"] = child;
	}

    response->write(SimpleWeb::StatusCode::success_ok, propertyTree.dump(2) + "\n", jsonHeaders);
}

/**
 * Fill in a successful return.
 */
void
RESTServer::returnSuccess(
    std::shared_ptr<HttpServerResponse> response,
    const std::string &msg,
    const JSON_Serializable *createdObject,
    const std::string createdObjectKey)
{
    returnSuccess(response, msg, string(), string(), createdObject, createdObjectKey);
}

/**
 * A wrapper.
 */
void
RESTServer::returnSuccess(
    std::shared_ptr<HttpServerResponse> response,
    const char *msg,
    const JSON_Serializable *createdObject,
    const std::string createdObjectKey)
{
	string msgStr(msg);
    returnSuccess(response, msgStr, createdObject, createdObjectKey);
}

/**
 * This version just takes the values, not burying anything.
 */
void
RESTServer::returnSuccess(std::shared_ptr<HttpServerResponse> response, const JSON_Serializable &values) {
    json propertyTree;
    values.toJSON(propertyTree);
    response->write(SimpleWeb::StatusCode::success_ok, propertyTree.dump(2) + "\n", jsonHeaders);
}

void
RESTServer::returnHTML(HttpServerResponsePtr response, const std::string &body) {
    response->write(SimpleWeb::StatusCode::success_ok, body, htmlHeaders);
}

void
RESTServer::returnText(HttpServerResponsePtr response, const std::string &body) {
    response->write(SimpleWeb::StatusCode::success_ok, body, textHeaders);
}


/** ??? */
void
receive(std::shared_ptr<HttpServerResponse> , std::shared_ptr<HttpServer::Request> ) {
	cout << "Got a request.\n";
}

/**
 * Set up the routes we use at the base. Our children will chain to this when they override.
 */
void
RESTServer::setupRoutes() {
	cout << "RESTServer::setupRoutes\n";
	//router.get("/", Rest::Routes::bind(&RESTServer::helloWorld, this));
	//router.get("/hello", Rest::Routes::bind(&RESTServer::helloWorld, this));
}

/**
 * Implements a simple Hello World method for testing. This is what gets called if you hit us at /.
 */
void
RESTServer::route(std::shared_ptr<HttpServerResponse> response, std::shared_ptr<HttpServer::Request> request) {
	cout << "Got request to path: " << request->path << " with method: " << request->method << "\n";
	bool handled = false;
	for (auto iter = routers.begin(); iter != routers.end(); ++iter) {
        shared_ptr<Router> router = *iter;
		if (router->route(response, request)) {
			handled = true;
			break;
		}
	}

	if (!handled) {
		returnError(response, "Illegal request");
	}
}

// This is a hack, and the system will fall apart if you run more than one server.
static RESTServer *staticServer = nullptr;

/**
 * Run forever.
 */
void RESTServer::run() {
	string hostAndPort = host + ":" + std::to_string(port);
	cout << "Starting the server as: " << hostAndPort << endl;

	setupRoutes();

	//server.config.address = host;
    server.config.port = static_cast<unsigned short>(port);
	server.config.thread_pool_size = threads;
	staticServer = this;

	server.resource["^.*$"]["GET"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
		staticServer->route(response, request);
	};
	server.resource["^.*$"]["PUT"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
		staticServer->route(response, request);
	};
	server.resource["^.*$"]["POST"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
		staticServer->route(response, request);
	};
	server.resource["^.*$"]["DELETE"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
		staticServer->route(response, request);
	};

	// Setup the end point.
	server.start();
}

/**
 * Look for the Authorization header. If found, and it's of type Basic, then decode it into username:password.
 * Returns true if we get that far, false if we don't.
 */
bool
RESTServer::parseAuthorizationHeader(HttpServerRequestPtr request, std::string &username, std::string &password) {
    auto it = request->header.find("Authorization");
    if (it == request->header.end()) {
        return false;
    }

    string fullString = it->second;
    auto pos = fullString.find(' ');
    if (pos == string::npos) {
        return false;
    }

    string type = fullString.substr(0, pos);
    if (type != "Basic") {
        return false;
    }

    string credentials = fullString.substr(pos + 1);
    string decoded = base64Decode(credentials);

    pos = decoded.find(':');
    if (pos == string::npos) {
        return false;
    }

    username = decoded.substr(0, pos);
    password = decoded.substr(pos + 1);

    return true;
}

/**
 * Our router runs through his routes and if one matches, he dispatches it.
 */
bool
RESTServer::Router::route(shared_ptr<HttpServerResponse> response, shared_ptr<HttpServer::Request> request) {
    try {
        const string & path = request->path;
        const string & method = request->method;
        cout << "Dispatch method: " << method << " Path: " << path << endl;

        for (auto iter = routes.begin(); iter != routes.end(); ++iter) {
            Route *route = *iter;
            if (route->matches(method, path)) {
                (*route->getFunction())(object, response, request);
                return true;
            }
        }
    }
    catch (exception & e) {
        cout << "Exception in Router::route(): " << e.what() << endl;
    }
    return false;
}

/**
 * Do we match this route?
 */
bool
RESTServer::Route::matches(const string & _method, const string & _path) {
	return (methodType == _method) && regex_match(_path, matchRE);
}
