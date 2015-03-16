#include <http_server/http_msg.hpp>
#include <sstream>
#include <ctime>
#include <boost/tokenizer.hpp>
#include <algorithm>

namespace FBi
{

/**
 * @brief Function to transfer the HTTP request message to c++ standard string.
 *
 * @param[in] msg The HTTP request message.
 * @param[out] result The string contains all HTTP request message.
 */
void HttpMsgUtility::ComposeHttpRequestMsg(const HttpRequestMsg& msg, std::string& result)
{
	std::stringstream sstream;
	sstream << "HTTP/" << msg.http_version_major << "." << msg.http_version_minor;

	result = msg.method;
	result += " ";
	result += msg.uri.path;

	if(!msg.uri.query_strings.empty())
	{
		URI_Struct::Query_Str_Container::const_iterator iter = msg.uri.query_strings.begin();
		if(iter != msg.uri.query_strings.end())
			result += "?";
		for(; iter != msg.uri.query_strings.end(); ++iter)
		{
			result += iter->first;
			result += "=";
			result += iter->second;
		}
	}

	if(!msg.uri.fragment_id.empty())
	{
		result += "#";
		result += msg.uri.fragment_id;
	}

	result += " ";
	result += sstream.str();
	result += "\r\n";

	HTTPMsgHeadersConstIter iter = msg.headers.begin();
	for(; iter != msg.headers.end(); iter++)
	{
		result += iter->first;
		result += ": ";
		result += iter->second;
		result += "\r\n";
	}

	result += "\r\n";
	result += msg.entity_body;
}

/**
 * @brief Function to transfer the HTTP request message to c++ standard string.
 *
 * @param[in] method The method name of HTTP request message.
 * @param[in] uri The URI of HTTP request message.
 * @param[in] http_version_major The major number of HTTP version of HTTP request message.
 * @param[in] http_version_minor The minor number of HTTP version of HTTP request message.
 * @param[in] headers The headers. Each header has already contain the field name and field value.
 * @param[in] body The entity body of HTTP request message.
 * @param[out] result The string contains all HTTP request message.
 */
template <typename InputContainer, typename InputContainerConstIter>
void HttpMsgUtility::ComposeHttpRequestMsg(const std::string& method, 
		const std::string& uri, 
		int http_version_major, 
		int http_version_minor, 
		const InputContainer& headers,
		const std::string& body,
		std::string& result)
{
	std::stringstream sstream;
	sstream << "HTTP/" << http_version_major << "." << http_version_minor;

	result = method;
	result += " ";
	result += uri;
	result += " ";
	result += sstream.str();
	result += "\r\n";

	InputContainerConstIter iter = headers.begin();
	for(; iter != headers.end(); iter++)
	{
		result += (*iter);
		result += "\r\n";
	}

	result += "\r\n";
	result += body;

}

/**
 * @brief Function to transfer the HTTP response message to c++ standard string.
 *
 * @param[in] msg The HTTP response message.
 * @param[out] result The string contains all HTTP response message.
 */
void HttpMsgUtility::ComposeHttpResponseMsg(const HttpResponseMsg& msg, std::string& result)
{
	std::stringstream sstream;
	sstream << "HTTP/" << msg.http_version_major << "." << msg.http_version_minor << " " << msg.status_code;

	result = sstream.str();
	result += " ";
	result += msg.reason_phrase;
	result += "\r\n";

	HTTPMsgHeadersConstIter iter = msg.headers.begin();
	for(; iter != msg.headers.end(); iter++)
	{
		result += iter->first;
		result += ": ";
		result += iter->second;
		result += "\r\n";
	}

	result += "\r\n";
	result += msg.entity_body;

}

/**
 * @brief Function to transfer the HTTP response message to c++ standard string.
 *
 * @param[in] http_version_major The major number of HTTP version of HTTP response message.
 * @param[in] http_version_minor The minor number of HTTP version of HTTP response message.
 * @param[in] status_code The status code of HTTP response message.
 * @param[in] reason_phrase The reason phrase of HTTP response message.
 * @param[in] headers The headers. Each header has already contain the field name and field value.
 * @param[in] body The entity body of HTTP response message.
 * @param[out] result The string contains all HTTP response message.
 */
template <typename InputContainer, typename InputContainerConstIter>
void HttpMsgUtility::ComposeHttpResponseMsg(int http_version_major, 
		int http_version_minor, 
		int status_code, 
		const std::string& reason_phrase, 
		const InputContainer& headers, 
		const std::string& body,
		std::string& result)
{
	std::stringstream sstream;
	sstream << "HTTP/" << http_version_major << "." << http_version_minor << " " << status_code;

	result = sstream.str();
	result += " ";
	result += reason_phrase;
	result += "\r\n";

	InputContainerConstIter iter = headers.begin();
	for(; iter != headers.end(); iter++)
	{
		result += (*iter);
		result += "\r\n";
	}

	result += "\r\n";
	result += body;
}

/**
 * @brief Function to calculate the length of the entity body of HTTP message then set the Content-Length header into the HTTP message.
 *
 * @param[in] msg The HTTP message.
 */
void HttpMsgUtility::SetContentLength(HttpMsg& msg)
{
	std::stringstream sstream;
	sstream << msg.entity_body.length();
	msg.headers["Content-Length"] = sstream.str();
}

/**
 * @brief Function to set HTTP version and date into the HTTP response message.
 *
 * @param[out] response The HTTP response message.
 */
void HttpMsgUtility::SetGeneralHttpResponseHeaders(HttpResponseMsg& response)
{
	response.http_version_major = 1;
	response.http_version_minor = 1;

	std::time_t result = std::time(NULL);
	struct tm* timeinfo = NULL;
	char buffer[128] = {'\0'};
	timeinfo = std::gmtime(&result);

	strftime(buffer, 128, "%a, %d %b %Y %X GMT", timeinfo);

	response.headers["Date"] = buffer;
}

/**
 * @brief Function to set the default status code and reason phrase of the HTTP response message.
 *
 * @param[out] msg The HTTP response message.
 * @param[in] status_code The status code of HTTP response message.
 * @return true: If the status code can be found in the default list. Otherwise, it is false.
 */
bool HttpMsgUtility::SetDefaultStatusCode(HttpResponseMsg& msg, int status_code)
{
	msg.reason_phrase = HttpMsgUtility::GetReasonPhrase(status_code);
	msg.status_code = status_code;

	return (msg.reason_phrase.empty()) ? false : true;
}

/**
 * @brief Function to get the reason phrase of HTTP response message corresponds with the input status code.
 *
 * @param[in] status_code The status code of HTTP response message.
 * @return The reason phrase of HTTP response message.
 */
std::string HttpMsgUtility::GetReasonPhrase(int status_code)
{
	switch(status_code)
	{
	case 100:
		return "Continue";
	case 101:
		return "Switching Protocols";
	case 200:
		return "OK";
	case 201:
		return "Created";
	case 202:
		return "Accepted";
	case 203:
		return "Non-Authoritative Information";
	case 204:
		return "No Content";
	case 205:
		return "Reset Content";
	case 206:
		return "Partial Content";
	case 300:
		return "Multiple Choices";
	case 301:
		return "Moved Permanently";
	case 302:
		return "Found";
	case 303:
		return "See Other";
	case 304:
		return "Not Modified";
	case 305:
		return "Use Proxy";
	case 306:
		return "";
	case 307:
		return "Temporary Redirect";
	case 400:
		return "Bad Request";
	case 401:
		return "Unauthorized";
	case 402:
		return "Payment Required";
	case 403:
		return "Forbidden";
	case 404:
		return "Not Found";
	case 405:
		return "Method Not Allowed";
	case 406:
		return "Not Acceptable";
	case 407:
		return "Proxy Authentication Required";
	case 408:
		return "Request Timeout";
	case 409:
		return "Conflict";
	case 410:
		return "Gone";
	case 411:
		return "Length Required";
	case 412:
		return "Precondition Failed";
	case 413:
		return "Request Entity Too Large";
	case 414:
		return "Request-URI Too Long";
	case 415:
		return "Unsupported Media Type";
	case 416:
		return "Requested Range Not Satisfiable";
	case 417:
		return "Expectation Failed";
	case 500:
		return "Internal Server Error";
	case 501:
		return "Not Implemented";
	case 502:
		return "Bad Gateway";
	case 503:
		return "Service Unavailable";
	case 504:
		return "Gateway Timeout";
	case 505:
		return "HTTP Version Not Supported";
	default:
		return "";
	}
}

/**
 * @brief Function to parse the quality value of HTTP.
 *
 * @param[in] qvalue_str The string of quality value of HTTP (ex: q=0.4).
 * @return The quality value.
 */
float HttpMsgUtility::ParseHttpQualityValue(const std::string qvalue_str)
{
	float retval = 0.0;
	if( (qvalue_str.length() >= 3) || (qvalue_str.length() <= 7))
	{
		int integer = 0;
		int decimal = 0;
		bool is_need_break = false;
		for(size_t i = 0; i < qvalue_str.length(); ++i)
		{
			switch(i)
			{
			case 0:
				if(qvalue_str[i] != 'q')
					is_need_break = true;
				break;
			case 1:
				if(qvalue_str[i] != '=')
					is_need_break = true;
				break;
			case 2:
				if((qvalue_str[i] == '0') || (qvalue_str[i] == '1'))
					integer = qvalue_str[i] - '0';
				else
					is_need_break = true;
				break;
			case 3:
				break;
			case 4:
			case 5:
			case 6:
				if((qvalue_str[i] >= '0') && (qvalue_str[i] <= '9'))
					decimal = decimal*10 + (qvalue_str[i] - '0');
				else
					is_need_break = true;
				break;
			default:
				is_need_break = true;
				break;
			}
			if(is_need_break)
			{
				retval = 0.0;
				break;
			}
		}

		if(!is_need_break)
		{
			retval = (float)integer + (float)decimal / 1000.0;
		}
	}
	else
	{
		retval = 0.0;
	}

	return retval;
}

/// Private compare function for sorting the encoding and quality value in HTTP headers.
bool compare_http_accpet_encoding(const HttpEncodingWithQualityValue& i, const HttpEncodingWithQualityValue& j)
{
	return (i.qvalue > j.qvalue);
}

/**
 * @brief Function to parse the encoding and quality value in HTTP headers.
 *
 * @param[in] coding_str The string contains enconding and quality value. (ex: gzip;q=0.4, bzip2; q=0.74).
 * @param[out] out_vector The vector contains string of enconding and quality value.
 */
void HttpMsgUtility::ParseEncodingWithQualityValue(const std::string& coding_str, std::vector<HttpEncodingWithQualityValue>& out_vector)
{
	// Separate each pair of encoding and quality value.
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer_t;
	boost::char_separator<char> sep(",");
	tokenizer_t tokens(coding_str, sep);

	// Get each encodings of HTTP message and parse them.
	for(tokenizer_t::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
	{
		// Separate each encoding and quality value.
		boost::char_separator<char> sep2("; ");
		tokenizer_t tokens2(*tok_iter, sep2);
		int i = 0;
		HttpEncodingWithQualityValue coding;
		for(tokenizer_t::iterator tok_iter2 = tokens2.begin(); tok_iter2 != tokens2.end(); ++tok_iter2, ++i)
		{
			if(i==0)
			{
				// Get the encoding string.
				coding.content_coding = *tok_iter2;
				// The content codings are case-insensitive.
				for(size_t i = 0; i < coding.content_coding.length(); ++i)
					coding.content_coding[i] = tolower(coding.content_coding[i]);
			}
			else if (i == 1)
			{
				// Get the quality value.
				coding.qvalue = ParseHttpQualityValue(*tok_iter2);
			}
		}
		out_vector.push_back(coding);
	}

	// Sorting the accepted encodings by quality values.
	stable_sort(out_vector.begin(), out_vector.end(), compare_http_accpet_encoding);
}

/**
 * @brief Function to parse the encoding without quality value.
 *
 * @param[in] coding_str The string contains enconding (ex: gzip, bzip2).
 * @param[out] encoding_vector The vector contains string of enconding and quality value.
 */
void HttpMsgUtility::ParseEnconding(const std::string& coding_str, std::vector<std::string>& encoding_vector)
{
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer_t;
	boost::char_separator<char> sep(", ");
	tokenizer_t tokens(coding_str, sep);

	// Get each encodings of HTTP message and parse them.
	for(tokenizer_t::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
	{
		// The codings are case-insensitive.
		std::string temp_str;
		for(size_t i = 0; i < tok_iter->length(); ++i)
			temp_str.push_back(tolower(tok_iter->at(i)));

		encoding_vector.push_back(temp_str);
	}
}

/**
 * @brief Function to compress string.
 *
 * @param[in] mode The compression mode.
 * @param[in] input The input string.
 * @param[out] output The output compressed string.
 * @return The result of compression (mode, failed or unsupported).
 */
HttpMsgUtility::CompressedResult HttpMsgUtility::CompressString(const std::string& mode, const std::string& input, std::string& output)
{
	bool is_successful;
	if( (mode.compare("gzip") == 0) || (mode.compare("*") == 0) )
	{
		is_successful = HttpMsgUtility::compress(boost::iostreams::gzip_compressor(boost::iostreams::zlib::best_compression),
				boost::make_iterator_range(input),
				boost::iostreams::back_inserter(output));
		if(is_successful)
			return HttpMsgUtility::CompressedGzip;
		else
			return HttpMsgUtility::CompressedFailed;
	}
	/*else if(mode.compare("bzip2") == 0)
	{
		is_successful = HttpMsgUtility::compress(boost::iostreams::bzip2_compressor(boost::iostreams::zlib::best_compression),
				boost::make_iterator_range(input),
				boost::iostreams::back_inserter(output));
		if(is_successful)
			return HttpMsgUtility::CompressedBzip2;
		else
			return HttpMsgUtility::CompressedFailed;
	}*/
	else if(mode.compare("deflate") == 0)
	{
		is_successful = HttpMsgUtility::compress(boost::iostreams::zlib_compressor(boost::iostreams::zlib::deflated | boost::iostreams::zlib::best_compression),
				boost::make_iterator_range(input),
				boost::iostreams::back_inserter(output));
		if(is_successful)
			return HttpMsgUtility::CompressedDeflate;
		else
			return HttpMsgUtility::CompressedFailed;
	}
	else if(mode.compare("identity") == 0)
	{
		output = input;
		return HttpMsgUtility::CompressedIdentity;
	}

	return HttpMsgUtility::CompressedUnsupported;
}

/**
 * @brief Function to compress string.
 *
 * @param[in] mode The compression mode.
 * @param[out] output_msg The output HTTP message.
 * @return The result of compression (mode, failed or unsupported).
 */
HttpMsgUtility::CompressedResult HttpMsgUtility::CompressString(const std::string& mode, HttpMsg& output_msg)
{
	bool is_successful;
	std::string output;
	if( (mode.compare("gzip") == 0) || (mode.compare("*") == 0) )
	{
		is_successful = HttpMsgUtility::compress(boost::iostreams::gzip_compressor(boost::iostreams::zlib::best_compression),
				boost::make_iterator_range(output_msg.entity_body),
				boost::iostreams::back_inserter(output));
		if(is_successful)
		{
			// Set the Content-Encoding header.
			output_msg.headers["Content-Encoding"] = "gzip";
			output_msg.entity_body = output;
			return HttpMsgUtility::CompressedGzip;
		}
		else
			return HttpMsgUtility::CompressedFailed;
	}
	/*else if(mode.compare("bzip2") == 0)
	{
		is_successful = HttpMsgUtility::compress(boost::iostreams::bzip2_compressor(boost::iostreams::zlib::best_compression),
				boost::make_iterator_range(output_msg.entity_body),
				boost::iostreams::back_inserter(output));
		if(is_successful)
		{
			output_msg.headers["Content-Encoding"] = "bzip2";
			output_msg.entity_body = output;
			return HttpMsgUtility::CompressedBzip2;
		}
		else
			return HttpMsgUtility::CompressedFailed;
	}*/
	else if(mode.compare("deflate") == 0)
	{
		is_successful = HttpMsgUtility::compress(boost::iostreams::zlib_compressor(boost::iostreams::zlib::deflated | boost::iostreams::zlib::best_compression),
				boost::make_iterator_range(output_msg.entity_body),
				boost::iostreams::back_inserter(output));
		if(is_successful)
		{
			output_msg.headers["Content-Encoding"] = "deflate";
			output_msg.entity_body = output;
			return HttpMsgUtility::CompressedDeflate;
		}
		else
			return HttpMsgUtility::CompressedFailed;

	}
	else if(mode.compare("identity") == 0)
	{
		return HttpMsgUtility::CompressedIdentity;
	}

	return HttpMsgUtility::CompressedUnsupported;
}

/**
 * @brief Function to decompress string.
 *
 * @param[in] mode The compression mode.
 * @param[in] input The input compressed string.
 * @param[out] output The output deconpressed string.
 * @return The result of compression (mode, failed or unsupported).
 */
HttpMsgUtility::CompressedResult HttpMsgUtility::DecompressString(const std::string& mode, const std::string& input, std::string& output)
{
	bool is_successful;
	if(mode.compare("gzip") == 0)
	{
		is_successful = HttpMsgUtility::compress(boost::iostreams::gzip_decompressor(),
				boost::make_iterator_range(input),
				boost::iostreams::back_inserter(output));
		if(is_successful)
			return HttpMsgUtility::CompressedGzip;
		else
			return HttpMsgUtility::CompressedFailed;
	}
	/*else if(mode.compare("bzip2") == 0)
	{
		is_successful = HttpMsgUtility::compress(boost::iostreams::bzip2_decompressor(),
				boost::make_iterator_range(input),
				boost::iostreams::back_inserter(output));
		if(is_successful)
			return HttpMsgUtility::CompressedBzip2;
		else
			return HttpMsgUtility::CompressedFailed;
	}*/
	else if(mode.compare("deflate") == 0)
	{
		is_successful = HttpMsgUtility::compress(boost::iostreams::zlib_decompressor(),
				boost::make_iterator_range(input),
				boost::iostreams::back_inserter(output));
		if(is_successful)
			return HttpMsgUtility::CompressedDeflate;
		else
			return HttpMsgUtility::CompressedFailed;
	}
	else if(mode.compare("identity") == 0)
	{
		output = input;
		return HttpMsgUtility::CompressedIdentity;
	}

	return HttpMsgUtility::CompressedUnsupported;

}

/**
 * @brief Function to compress the entity body of response message.
 *
 * @param[in] request_msg The input HTTP request message.
 * @param[out] response_msg The output HTTP response message.
 * @return The result of compression (success, failed, unsupported or no compressed).
 */
HttpMsgUtility::CompressedResult HttpMsgUtility::CompressBodyOfResponseMsg(HttpRequestMsg& request_msg, HttpResponseMsg& response_msg)
{
	HTTPMsgHeadersConstIter iter = request_msg.headers.find("accept-encoding");
	if(iter != request_msg.headers.end())
	{
		// Parse Accept-Encoding
		std::vector<HttpEncodingWithQualityValue> codings_vector;
		HttpMsgUtility::ParseEncodingWithQualityValue((request_msg.headers["accept-encoding"]), codings_vector);

		// Prepare to compress the entity body of HTTP response message.
		if(codings_vector.size() >= 1)
		{
			HttpMsgUtility::CompressedResult compressed_result = HttpMsgUtility::NoCompressed;
			for(size_t i = 0; i < codings_vector.size(); ++i)
			{
				// Skip the encoding with zero quality value.
				if(codings_vector[i].qvalue == 0.0)
					continue;

				compressed_result = HttpMsgUtility::CompressString(codings_vector[i].content_coding, response_msg);
				if((compressed_result != HttpMsgUtility::CompressedFailed) && (compressed_result != HttpMsgUtility::CompressedUnsupported))
					return HttpMsgUtility::CompressedSuccess;
			}

			// Set response message of error.
			response_msg.Reset();
			if(compressed_result == HttpMsgUtility::CompressedFailed)
			{
				// Internal Server Error
				HttpMsgUtility::SetDefaultStatusCode(response_msg, 500);
				return HttpMsgUtility::CompressedFailed;
			}

			// Not Acceptable
			if(compressed_result == HttpMsgUtility::CompressedUnsupported)
			{
				HttpMsgUtility::SetDefaultStatusCode(response_msg, 406);
				return HttpMsgUtility::CompressedUnsupported;
			}
		}
	}

	return HttpMsgUtility::NoCompressed;
}

/**
 * @brief Function to decompress the entity body of HTTP message.
 *
 * @param[in] encoding_vector The input encoding vector without quality value.
 * @param[out] msg The output HTTP message with decompressed body.
 * @return The result of compression (mode, failed or unsupported).
 */
HttpMsgUtility::CompressedResult HttpMsgUtility::DecompressBodyOfHttpMsg(const std::vector<std::string>& encoding_vector, HttpMsg& msg)
{
	std::string temp_str;
	std::vector<std::string>::const_reverse_iterator r_iter;
	HttpMsgUtility::CompressedResult compressed_result = HttpMsgUtility::CompressedSuccess;

	// We decompress the body of HTTP message according to the "reverse order" of Content-Encoding.
	for(r_iter = encoding_vector.rbegin() ; r_iter < encoding_vector.rend(); ++r_iter)
	{
		compressed_result = HttpMsgUtility::DecompressString(*r_iter, msg.entity_body, temp_str);

		if((compressed_result == HttpMsgUtility::CompressedFailed) || (compressed_result == HttpMsgUtility::CompressedUnsupported))
			return compressed_result;

		if(compressed_result != HttpMsgUtility::CompressedIdentity)
			msg.entity_body.swap(temp_str);

		temp_str.clear();
	}

	return compressed_result;
}

/**
 * @brief Function to generate the boundary string for HTTP server push mode.
 *
 * @return The boundary string for HTTP server push mode.
 */
const std::string HttpMsgUtility::GenerateServerPushBoundary()
{
	return "XD/E?5F_=,Q2-Xi(52+f";
}

/**
 * @brief Function to compose the body and headers for HTTP server push mode.
 *
 * @param[in] push_headers MIME headers for HTTP server push mode.
 * @param[in] data body for HTTP server push mode.
 * @param[in] boundary_str boundary string for HTTP server push mode.
 * @param[in] is_end The flag to indicate whether it is end of push mode. 
 * @param[out] result The composed result data.
 */
void HttpMsgUtility::ComposeServerPushBody(std::map<std::string, std::string>& push_headers, std::string& data, const std::string& boundary_str, bool is_end, std::string& result)
{
	std::stringstream sstream;
	sstream << data.length();
	push_headers["Content-Length"] = sstream.str();

	std::map<std::string, std::string>::iterator iter = push_headers.begin();
	for(; iter != push_headers.end(); iter++)
	{
		result += iter->first;
		result += ": ";
		result += iter->second;
		result += "\r\n";
	}

	result += "\r\n";
	result += data;
	result += "\r\n\r\n";

	result += "--";
	result += boundary_str;
	if(!is_end)
		result += "\r\n";
	else
		result += "--\r\n";
}

} // namespace FBi
