#ifndef SRC_LIB_RESTSERVER_H_
#define SRC_LIB_RESTSERVER_H_

#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <simple-web-server/server_http.hpp>

#include "JSON_Serializable.h"

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;
using HttpServerRequest = SimpleWeb::ServerBase<SimpleWeb::HTTP>::Request;
using HttpServerResponse = SimpleWeb::ServerBase<SimpleWeb::HTTP>::Response;

using HttpServerRequestPtr = std::shared_ptr<HttpServerRequest>;
using HttpServerResponsePtr = std::shared_ptr<HttpServerResponse>;


using HTTPFunction = void(*)(void *, HttpServerResponsePtr response, HttpServerRequestPtr request);
#define HTTPFUNCTION_LAMBDA(C, M) [](void *obj, std::shared_ptr<HttpServerResponse> response, std::shared_ptr<HttpServerRequest> request) { static_cast<C *>(obj)->M(response, request); }

/**
 * To implement a REST Server, subclas from this.
 */
class RESTServer {
private:
	static const std::string HTTP_GET;
	static const std::string HTTP_PUT;
	static const std::string HTTP_POST;
	static const std::string HTTP_DELETE;

public:
    /**
     * This is one route, such as a GET method called "index".
     */
	class Route {
	private:
		const std::string methodType;
		const std::string matchString;
		const std::regex matchRE;
		HTTPFunction function;

	public:
        /** Constructor. */
		Route(const std::string & _type, const std::string & _match, HTTPFunction _function)
			: methodType(_type), matchString(_match), matchRE(_match), function(_function) {
		}

		bool matches(const std::string &_method, const std::string & path);

        /** GET, PUT, etc. */
		const std::string & getMethod() { return methodType; }

        /** The path to match. */
		const std::string & getMatchString() { return matchString; }

        /** The callback. */
		HTTPFunction getFunction() { return function; }
	};

    /**
     * Container for all our routes.
     */
	class Router {
	private:
		void *	object;
		std::vector<Route *> routes;

	public:
        /** Constructor. */
		Router(void *forObject) : object(forObject) {}

        /** Set up a route of this type (GET, etc), this path, to this callback. */
		void addRoute(const std::string & _type, const std::string &match, HTTPFunction function) {
			Route * route = new Route(_type, match, function);
			routes.push_back(route);
		}

        /** Shortcut to add a GET operation. */
		void get(const std::string &match, HTTPFunction function) { addRoute(HTTP_GET, match, function); }

        /** Shortcut to add a PUT operation. */
        void put(const std::string &match, HTTPFunction function) { addRoute(HTTP_PUT, match, function); }

        /** Shortcut to add a POST operation. */
        void post(const std::string &match, HTTPFunction function) { addRoute(HTTP_POST, match, function); }

        /** Shortcut to add a DELETE operation. */
        void del(const std::string &match, HTTPFunction function) { addRoute(HTTP_DELETE, match, function); }

		// Returns true if it was handled.
		bool route(std::shared_ptr<HttpServerResponse> response, std::shared_ptr<HttpServerRequest> request);
	};

protected:
	/** The port we'll listen on. */
	int port;

	/** The host name we'll listen on. */
	std::string host;

	/** How many threads we'll run as. */
	int threads;

    /** The connection to the server. */
	SimpleWeb::Server<SimpleWeb::HTTP> server;

    /** The routers we use for dispatching requests. */
    std::vector<std::shared_ptr<Router>> routers;

    SimpleWeb::CaseInsensitiveMultimap htmlHeaders;
    SimpleWeb::CaseInsensitiveMultimap jsonHeaders;
    SimpleWeb::CaseInsensitiveMultimap textHeaders;

    //--------------------------------------------------
    // These are various ways to return errors.
    //--------------------------------------------------

    /** Error return. */
    void returnError(HttpServerResponsePtr response);

    void returnError(HttpServerResponsePtr response, const std::string &error);
    void returnError(HttpServerResponsePtr response, const char *error);
    void returnError(HttpServerResponsePtr response, const char *errorFormat, const std::string &argument);

    //--------------------------------------------------
    // These are various methods for returning JSON
    //--------------------------------------------------
    void returnSuccess(HttpServerResponsePtr response,
            const std::string &msg,
            const std::string &key,
            const std::string &value,
            const JSON_Serializable *createdObject = nullptr,
            const std::string createdObjectKey = "object");

    void returnSuccess(HttpServerResponsePtr response,
        const std::string &msg,
        const JSON_Serializable *createdObject = nullptr,
        const std::string createdObjectKey = "object");

    void returnSuccess(HttpServerResponsePtr response,
        const char *msg,
        const JSON_Serializable *createdObject = nullptr,
        const std::string createdObjectKey = "object");

    void returnSuccess(HttpServerResponsePtr response, const JSON_Serializable &values);

    void returnSuccess(
        HttpServerResponsePtr response,
        const std::string &msg,
        const std::shared_ptr<JSON_Serializable> ptr,
        const std::string createdObjectKey = "object")
    {
        if (ptr == nullptr) {
            returnSuccess(response, msg, nullptr, createdObjectKey);
        }
        else {
            returnSuccess(response, msg, &(*ptr), createdObjectKey);
        }
    }

    //--------------------------------------------------
    // And these are for HTML.
    //--------------------------------------------------
    void returnHTML(HttpServerResponsePtr response, const std::string &body);
    void returnText(HttpServerResponsePtr response, const std::string &body);

public:
	RESTServer();
	RESTServer(const std::string & _host, const int _port, const int _threads);
	virtual ~RESTServer();

	virtual void setupRoutes();

    /** Use this router for handling requests. */
    void addRouter(std::shared_ptr<Router>router) { routers.push_back(router); }

	void run();

    void route(HttpServerResponsePtr response, HttpServerRequestPtr request);

    bool parseAuthorizationHeader(HttpServerRequestPtr request, std::string &username, std::string &password);
};

#endif /* SRC_LIB_RESTSERVER_H_ */
