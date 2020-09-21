#ifndef SRC_LIB_URI_H_
#define SRC_LIB_URI_H_

#include <string>

/**
 * A URI broken into pieces.
 */
class URI {
private:
    std::string protocol_;          /**< http:, etc */
    std::string hostWithPort_;      /**< localhost:9999 */
    std::string path_;              /**< /foo/bar */
    std::string query_;             /**< ??? */

    std::string host_;              /**< localhost */
    int port_;                      /**< 1999 */

    void parse(const std::string& url_s);

public:
    /** Constructor. */
    URI(const std::string& url_s): port_(0) { parse(url_s); }

    /** Destructor. */
	virtual ~URI() {}

	// Protocol is https, etc.

    /** Return the protocol. */
	const std::string & protocol() const { return protocol_; }

    /** Return the host & port string. */
	const std::string & hostWithPort() const { return hostWithPort_; }

    /** Return the path. */
	const std::string & path() const { return path_; }

    /** ??? */
	const std::string & query() const { return query_; }

    /** Return just the host. */
	const std::string & host() const { return host_; }

    /** Return just the port. */
	int port() const { return port_; }
};

#endif /* SRC_LIB_URI_H_ */
