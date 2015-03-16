#include <http_server/uri.hpp>
#include <cstdio>
#include <cctype> // For tolower
#include <boost/tokenizer.hpp>

namespace FBi
{
// ============== URI ==============

/// The constructor of URI_Struct.
URI_Struct::URI_Struct()
	: port(0)
{
}

/// Function to clear all data in the URI_Struct.
void URI_Struct::Clear()
{
	scheme.clear();
	username.clear();
	password.clear();
	hostname.clear();
	port = 0;
	path.clear();
	query_strings.clear();
	fragment_id.clear();
}

/// Function to print all data in the URI_Struct.
void URI_Utility::PrintURI(const URI_Struct& uri)
{
	printf("--------- URI start ---------\n");

	printf("Scheme: %s\n", uri.scheme.c_str());
	printf("User name: %s\n", uri.username.c_str());
	printf("Password: %s\n", uri.password.c_str());
	printf("Hostname: %s\n", uri.hostname.c_str());
	printf("Port: %u\n", uri.port);
	printf("Path: %s\n", uri.path.c_str());

	printf("Query:\n");

	URI_Struct::Query_Str_Container::const_iterator iter = uri.query_strings.begin();
	for(; iter!= uri.query_strings.end(); ++iter)
	{
		printf("( %s , %s )\n", iter->first.c_str(), iter->second.c_str());
	}

	printf("Fragment ID: %s\n", uri.fragment_id.c_str());

	printf("--------- URI end ---------\n");
}

/**
 * @brief Function to convert hexadecimal character to decimal unsigned integer.
 *
 * @param[in] ch The input hexadecimal character.
 * @return The output decimal unsigned integer.
 */
unsigned int URI_Utility::CharHex2DecUint(char ch)
{
	if( (ch >= '0') && (ch <= '9') )
	{
		return (unsigned int)(ch - '0');
	}

	if(ch >= 'A' && ch <= 'F')
	{
		return ( (unsigned int) (ch - 'A') + 10 );
	}

	if(ch >= 'a' && ch <= 'f')
	{
		return ( (unsigned int) (ch - 'a') + 10 );
	}

	return 16;
}

/**
 * @brief Function to unescape (decode) the URI string.
 *
 * @param[in] str The input URI string.
 * @param[in,out] result The decoded URI string.
 * @return true: Success to decode the URI string. false: Failed to decode the URI string.
 */
bool URI_Utility::Unescape(const std::string& str, std::string& result)
{
	// SPACE is encoded as '+' or %20
	// If we don't replace them, + won't be unescaped properly and we get validation errors.
	std::string temp_str = str;
	size_t temp_index = 0;
	while( (temp_index = temp_str.find_first_of('+')) != std::string::npos )
	{
		temp_str.replace(temp_index, 1, "%20");
	}

	for(size_t i = 0; i < temp_str.length(); )
	{
		if(temp_str[i] != '%')
		{
			// The character is normal data.
			result.push_back(temp_str[i]);
			++i;
			continue;
		}

		// Check it has one byte encoded data (There is one % followed by two encoded hex characters).
		if( (i + 2) >= temp_str.length() )
			return false;

		// Decode the data.
		unsigned int high_part = URI_Utility::CharHex2DecUint(temp_str[i + 1]);
		unsigned int low_part = URI_Utility::CharHex2DecUint(temp_str[i + 2]);

		if( (high_part > 15) || (low_part > 15) )
			return false;

		result.push_back( (char) ( (high_part << 4) | low_part) );

		i += 3;

	}

	return true;
}

// ============== HTTP URL ==============

/**
 * @brief Function to convert some upper case characters of the string to lower case.
 *
 * @param[in, out] str The string need to be converted.
 * @param[in] start_index The start index for the substring of input string.
 * @param[in] len The length of substring need to be converted.
 * @return true: Success to convert the string to lower case. false: Failed to convert the string.
 */
bool HTTP_URL_Utility::String2Lower(std::string& str, size_t start_index, size_t len)
{
	size_t end_index = start_index + len;
	if(end_index > str.length())
		return false;

	for(size_t i = start_index; i < end_index; ++i)
	{
		str[i] = ::tolower(str[i]);
	}

	return true;
}

/**
 * @brief Function to parse http:// or https:// string.
 *
 * @param[in] http_url The string need to be parsed.
 * @param[in, out] unhandled_index The index to point the unhandled position of the input string.
 * @param[in, out] scheme The output scheme (http or https).
 * @return true: The input string is valid. false: The input string is invalid.
 */
bool HTTP_URL_Utility::parse_http_scheme(const std::string& http_url, size_t& unhandled_index, std::string& scheme)
{
	std::string temp_str = http_url.substr(unhandled_index, 8);
	HTTP_URL_Utility::String2Lower(temp_str, 0, 5);

	size_t temp_internal_unhandled_index = 0;

	if(temp_str[temp_internal_unhandled_index] != 'h')
	{
		unhandled_index += temp_internal_unhandled_index;
		return false;
	}

	++temp_internal_unhandled_index;
	if(temp_str[temp_internal_unhandled_index] != 't')
	{
		unhandled_index += temp_internal_unhandled_index;
		return false;
	}

	++temp_internal_unhandled_index;
	if(temp_str[temp_internal_unhandled_index] != 't')
	{
		unhandled_index += temp_internal_unhandled_index;
		return false;
	}

	++temp_internal_unhandled_index;
	if(temp_str[temp_internal_unhandled_index] != 'p')
	{
		unhandled_index += temp_internal_unhandled_index;
		return false;
	}

	++temp_internal_unhandled_index;

	scheme = "http";
	if(temp_str[temp_internal_unhandled_index] == 's')
	{
		++temp_internal_unhandled_index;
		scheme.push_back('s');
	}

	if(temp_str[temp_internal_unhandled_index] != ':')
	{
		unhandled_index += temp_internal_unhandled_index;
		return false;
	}

	++temp_internal_unhandled_index;
	if(temp_str[temp_internal_unhandled_index] != '/')
	{
		unhandled_index += temp_internal_unhandled_index;
		return false;
	}

	++temp_internal_unhandled_index;
	if(temp_str[temp_internal_unhandled_index] != '/')
	{
		unhandled_index += temp_internal_unhandled_index;
		return false;
	}

	++temp_internal_unhandled_index;

	unhandled_index += temp_internal_unhandled_index;
	return true;
}

/**
 * @brief Function to parse the query string of HTTP URL.
 *
 * @param[in] query_str The string need to be parsed.
 * @param[in, out] key_value_map The key and value pairs of input query string.
 * @return true: The input string is valid. false: The input string is invalid.
 */
bool HTTP_URL_Utility::ParseQueryString(const std::string& query_str, URI_Struct::Query_Str_Container& key_value_map)
{
	// Separate each pair of key and value.
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer_t;
	boost::char_separator<char> pair_sep("&;");
	tokenizer_t pair_tokens(query_str, pair_sep);

	for(tokenizer_t::iterator tok_iter = pair_tokens.begin(); tok_iter != pair_tokens.end(); ++tok_iter)
	{
		// Try to get the key and value.
		size_t index = tok_iter->find('=');
		if(index == std::string::npos)
			return false;

		key_value_map[tok_iter->substr(0, index)] = ( (index + 1) < tok_iter->length() ) ? tok_iter->substr(index + 1) : "";
	}

	return true;
}

/**
 * @brief Function to parse the HTTP URL.
 *
 * @param[in] http_url The input HTTP URL string.
 * @param[in, out] uri The output URI struct.
 * @return true: Success to parse string. false: Failed to parse string.
 */
bool HTTP_URL_Utility::ParseHttpUrl(const std::string& http_url, URI_Struct& uri)
{
	// Decode the HTTP URL.
	std::string decoded_url;
	if(!URI_Utility::Unescape(http_url, decoded_url) || decoded_url.empty())
		return false;

	// If it is * URL in the HTTP specification.
	if(decoded_url[0] == '*')
	{
		uri.path = "*";
		return true;
	}

	size_t unhandled_index = 0;

	if(decoded_url[0] != '/')
	{
		// http://example.org/absolute/URI/with/absolute/path/to/resource.txt
		// Parse absolute path.
		if(decoded_url.length() < 8)
			return false;

		// Scheme
		if(!HTTP_URL_Utility::parse_http_scheme(decoded_url, unhandled_index, uri.scheme))
			return false;

		if(unhandled_index >= decoded_url.length())
			return false;

		// Skip username, password, hostname, port
		unhandled_index = decoded_url.find("/", unhandled_index);

		if(unhandled_index == std::string::npos)
		{
			uri.path = "/";
			return true;
		}

		++unhandled_index;
	}
	else
	{
		// The string may be relative path or not have scheme.
		++unhandled_index;

		if( (decoded_url.length() >= 2) && (decoded_url[1] == '/') )
		{
			// //example.org/scheme-relative/URI/with/absolute/path/to/resource.txt
			// Parse the URL string which do not have scheme.
			unhandled_index = 2;

			if(unhandled_index >= decoded_url.length())
				return false;

			// Skip username, password, hostname, port
			unhandled_index = decoded_url.find("/", unhandled_index);

			if(unhandled_index == std::string::npos)
			{
				// Only path, no query string.
				uri.path = "/";
				return true;
			}

			++unhandled_index;
		}
	}

	// Handle path
	uri.path = "/";

	// Only root path
	if(unhandled_index >= decoded_url.length())
		return true;


	size_t temp_index = decoded_url.find_last_of('?');
	size_t temp_index2 = decoded_url.find_last_of('#');
	bool has_query = false;
	bool has_fragment = false;
	if( (temp_index == std::string::npos) && (temp_index2 == std::string::npos) )
	{
		// Only path, no query string.
		uri.path.append(decoded_url.substr(unhandled_index));
		return true;
	}

	if(temp_index != std::string::npos)
		has_query = true;
	if(temp_index2 != std::string::npos)
		has_fragment = true;

	size_t temp_index3 = temp_index2;
	if(has_query)
		temp_index3 = temp_index;

	uri.path.append(decoded_url.substr(unhandled_index, temp_index3 - unhandled_index));

	unhandled_index = temp_index3 + 1;

	if(unhandled_index >= decoded_url.length())
		return true;

	if(has_query)
	{
		// Query String.
		std::string temp_query_str;

		temp_query_str = (!has_fragment) ? decoded_url.substr(unhandled_index) : decoded_url.substr(unhandled_index, temp_index2 - unhandled_index);

		if(!HTTP_URL_Utility::ParseQueryString(temp_query_str, uri.query_strings))
			return false;

		unhandled_index = temp_index2 + 1;

	}

	if(unhandled_index >= decoded_url.length())
		return true;

	if(has_fragment)
	{
		// Fragment ID
		uri.fragment_id = decoded_url.substr(unhandled_index);
	}

	return true;
}

} // namespace FBi

