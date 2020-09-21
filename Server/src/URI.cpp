#include <stdlib.h>
#include <string>
#include <algorithm>
#include <cctype>
#include <functional>

#include "URI.h"

using namespace std;

/**
 * Parse the string into constituent pieces.
 */
void
URI::parse(const string& url_s)
{
    const string prot_end("://");
    string::const_iterator prot_i = search(url_s.begin(), url_s.end(), prot_end.begin(), prot_end.end());

    protocol_.reserve(distance(url_s.begin(), prot_i));
    transform(url_s.begin(), prot_i,
              back_inserter(protocol_),
              ::tolower);

    if( prot_i == url_s.end() ) {
        return;
    }

    advance(prot_i, prot_end.length());

    string::const_iterator path_i = find(prot_i, url_s.end(), '/');

    host_.reserve(distance(prot_i, path_i));

    transform(prot_i, path_i,
              back_inserter(hostWithPort_),
              ::tolower);

    string::const_iterator query_i = find(path_i, url_s.end(), '?');

    path_.assign(path_i, query_i);
    if( query_i != url_s.end() ) {
        ++query_i;
    }
    query_.assign(query_i, url_s.end());

    // Parse host and port.
    int index = static_cast<int>(hostWithPort_.find(':'));
    if (index > 0) {
    	host_ = hostWithPort_.substr(0, index);
    	port_ = atoi(hostWithPort_.substr(index + 1).c_str());
    }
}
