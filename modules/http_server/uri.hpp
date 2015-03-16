#ifndef FBi_URI_HPP
#define FBi_URI_HPP

#include <http_server/libhttp_server_config.h>

#include <string>
#include <unordered_map>

namespace FBi
{
// ============== URI ==============
/// The struct for URI.
struct LIBHTTPSERVER_DECL URI_Struct
{
public:
	typedef std::unordered_map<std::string, std::string> Query_Str_Container;
public:
	URI_Struct();
	void Clear();

public:
	std::string scheme;
	std::string username;
	std::string password;
	std::string hostname;
	unsigned int port;
	std::string path;
	Query_Str_Container query_strings;
	std::string fragment_id;
};

/// The utility class for URI.
class LIBHTTPSERVER_DECL URI_Utility
{
public:
	static unsigned int CharHex2DecUint(char ch);
	static bool Unescape(const std::string& str, std::string& result);
	static void PrintURI(const URI_Struct& uri);
};

// ============== HTTP URL ==============
/// The utility class for HTTP URL.
class LIBHTTPSERVER_DECL HTTP_URL_Utility
{
public:
	static bool String2Lower(std::string& str, size_t start_index, size_t len);
	static bool ParseQueryString(const std::string& query_str, URI_Struct::Query_Str_Container& key_value_map);
	static bool ParseHttpUrl(const std::string& http_url, URI_Struct& uri);

private:
	static bool parse_http_scheme(const std::string& http_url, size_t& unhandled_index, std::string& scheme);
};

} // namespace FBi

#endif

