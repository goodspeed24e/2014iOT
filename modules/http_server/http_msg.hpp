#ifndef HTTP_MSG_HPP
#define HTTP_MSG_HPP

#include <http_server/libhttp_server_config.h>

#include <string>
#include <map>
#include <vector>

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
//#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>

#include <http_server/uri.hpp>

namespace FBi
{

typedef std::map<std::string, std::string> HTTPMsgHeadersType;
typedef std::map<std::string, std::string>::iterator HTTPMsgHeadersIter;
typedef std::map<std::string, std::string>::const_iterator HTTPMsgHeadersConstIter;

/// Class for partial HTTP message
class LIBHTTPSERVER_DECL HttpMsg
{
public:
	HttpMsg() { http_version_major = http_version_minor = 0; private_user_data = NULL; }
	virtual ~HttpMsg() {}
	/// Function to reset the partial (common) HTTP message.
	virtual void Reset()
	{
		partial_msg_reset();
	}
protected:
	void partial_msg_reset()
	{
		http_version_major = http_version_minor = 0;
		headers.clear();
		entity_body.clear();
		private_user_data = NULL;
	}
public:
	int http_version_major;
	int http_version_minor;
	std::string entity_body;
	HTTPMsgHeadersType headers;
	/// Pointer to user data for private use.
	void* private_user_data;
};

/// Class for HTTP request message
class LIBHTTPSERVER_DECL HttpRequestMsg : public HttpMsg
{
public:
	/// Function to reset the HTTP request message.
	virtual void Reset()
	{
		partial_msg_reset();
		partial_request_line_reset();
	}
protected:
	void partial_request_line_reset()
	{
		method.clear();
		uri.Clear();
	}
public:
	std::string method;
	URI_Struct uri;
};

/// Class for HTTP response message
class LIBHTTPSERVER_DECL HttpResponseMsg : public HttpMsg
{
public:
	HttpResponseMsg() { status_code = 0; }
	/// Function to reset the HTTP response message.
	virtual void Reset()
	{
		partial_msg_reset();
		partial_status_line_reset();
	}
protected:
	void partial_status_line_reset()
	{
		status_code = 0;
		reason_phrase.clear();
	}
public:
	int status_code;
	std::string reason_phrase;
};

/// Class for Accept-Encoding of HTTP request message.
class LIBHTTPSERVER_DECL HttpEncodingWithQualityValue
{
public:
	HttpEncodingWithQualityValue() { qvalue = 1.0; }
public:
	/// Quality values
	float qvalue;
	std::string content_coding;
};

/// Utility class for HTTP message
class LIBHTTPSERVER_DECL HttpMsgUtility
{
public:
	static void ComposeHttpRequestMsg(const HttpRequestMsg&, std::string&);

	template <typename InputContainer, typename InputContainerConstIter>
	static void ComposeHttpRequestMsg(const std::string&, const std::string&, int, int, const InputContainer&, const std::string&, std::string&);

	static void ComposeHttpResponseMsg(const HttpResponseMsg&, std::string&);

	template <typename InputContainer, typename InputContainerConstIter>
	static void ComposeHttpResponseMsg(int, int, int, const std::string&, const InputContainer&, const std::string&, std::string&);

	static void SetContentLength(HttpMsg&);
	static void SetGeneralHttpResponseHeaders(HttpResponseMsg&);
	static bool SetDefaultStatusCode(HttpResponseMsg&, int);
	static std::string GetReasonPhrase(int);

	static float ParseHttpQualityValue(const std::string);
	static void ParseEncodingWithQualityValue(const std::string&, std::vector<HttpEncodingWithQualityValue>&);
	static void ParseEnconding(const std::string&, std::vector<std::string>&);

	/// The result of compression.
	enum CompressedResult { CompressedGzip, /*CompressedBzip2,*/ CompressedDeflate, CompressedIdentity, CompressedUnsupported, CompressedFailed, CompressedSuccess, NoCompressed };

	static CompressedResult CompressString(const std::string&, const std::string&, std::string&);
	static CompressedResult CompressString(const std::string&, HttpMsg&);
	static CompressedResult DecompressString(const std::string&, const std::string&, std::string&);

	static CompressedResult CompressBodyOfResponseMsg(HttpRequestMsg&, HttpResponseMsg&);
	static CompressedResult DecompressBodyOfHttpMsg(const std::vector<std::string>&, HttpMsg&);

	// For server push.
	static const std::string GenerateServerPushBoundary();
	static void ComposeServerPushBody(std::map<std::string, std::string>& push_headers, std::string& data, const std::string& boundary_str, bool is_end, std::string& result);

private:

	/**
	 * @brief Private function to use boost iostream to compress data.
	 *
	 * @param[in] compressor The boost compressor.
	 * @param[in] input The input original data.
	 * @param[out] output The output compressed data.
	 * @return true, if it is successful and false, if it is failed.
	 */
	template<typename CompressorT, typename InputIterT, typename OutputIterT>
	static bool compress(const CompressorT& compressor, InputIterT input, OutputIterT output)
	{
		try
		{
			boost::iostreams::filtering_streambuf<boost::iostreams::output> out_stream;
			out_stream.push(compressor);
			out_stream.push(output);
			boost::iostreams::copy(input, out_stream);
		}
		catch(...)
		{
			return false;
		}

		return true;
	}

	/**
	 * @brief Private function to use boost iostream to decompress data.
	 *
	 * @param[in] decompressor The boost decompressor.
	 * @param[in] input The input compressed data.
	 * @param[out] output The output decompressed data.
	 * @return true, if it is successful and false, if it is failed.
	 */
	template<typename DecompressorT, typename InputIterT, typename OutputIterT>
	static bool decompress(const DecompressorT& decompressor, InputIterT input, OutputIterT output)
	{
		try
		{
			boost::iostreams::filtering_streambuf<boost::iostreams::input> in_stream;
			in_stream.push(decompressor);
			in_stream.push(input);
			boost::iostreams::copy(in_stream, output);
		}
		catch(...)
		{
			return false;
		}

		return true;
	}
};

}

#endif
