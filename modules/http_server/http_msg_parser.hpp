#ifndef HTTP_MSG_PARSER_HPP
#define HTTP_MSG_PARSER_HPP

#include <http_server/libhttp_server_config.h>

#include <string>
#include <boost/function.hpp>

namespace FBi
{
/// HTTP message parser (excluding the entity body).
class LIBHTTPSERVER_DECL HttpMsgParser
{
public:
	/// Mode for HTTP request or response message.
	enum ParsingMode { Request_Mode, Response_Mode };
	/// The return result for parser.
	enum ParsingResult { Parse_OK, Parse_Fail, Parse_Processing };

	typedef boost::function<void (const std::string&)> OneStrCallback;
	typedef boost::function<void (const std::string&, const std::string&)> TwoStrCallback;
	typedef boost::function<void (int)> OneIntCallback;
	typedef boost::function<void (int, int)> TwoIntCallback;

	/// Callback struct for notify the observer.
	struct HttpMsgParserCallbacks
	{
		HttpMsgParserCallbacks()
		{
			method_callback = NULL;
			uri_callback = NULL;
			http_version_callback = NULL;
			status_code_callback = NULL;
			reason_phrase_callback = NULL;
			one_header_callback = NULL;
		}
		HttpMsgParserCallbacks(const OneStrCallback& method,
					const OneStrCallback& uri,
					const TwoIntCallback& http_version,
					const OneIntCallback& status_code,
					const OneStrCallback& reason_phrase,
					const TwoStrCallback& header)
		{
			method_callback = method;
			uri_callback = uri;
			http_version_callback = http_version;
			status_code_callback = status_code;
			reason_phrase_callback = reason_phrase;
			one_header_callback = header;
		}
		OneStrCallback method_callback;
		OneStrCallback uri_callback;
		TwoIntCallback http_version_callback;
		OneIntCallback status_code_callback;
		OneStrCallback reason_phrase_callback;
		TwoStrCallback one_header_callback;
	};

private:
	// lws: linear white space
	/// Transition states for parser.
	enum ParsingState
	{
		// For request line
		method_start, method,
		uri_start, uri,
		http_version_H, http_version_T1,
		http_version_T2, http_version_P,
		http_version_slash, http_version_major_start, http_version_major,
		http_version_minor_start, 
		http_version_minor_request, http_version_minor_response,
		
		// For status line
		status_code_1, status_code_2, status_code_3, space_after_status_code, reason_phrase,

		// For start line
		start_line_LF,

		// For head
		header_line_start,
		header_first_lws_before_field_value, header_lws_inside_field_value,
		header_field_name, header_field_value,
		header_line_LF,
		headers_LF
	};

public:
	// Constructors
	HttpMsgParser(HttpMsgParser::ParsingMode);
	HttpMsgParser(HttpMsgParser::ParsingMode, const HttpMsgParserCallbacks& callbacks);

	// The function to reset the state and variable of parser.
	void ResetState(const HttpMsgParser::ParsingMode);
	// The function to parser the input data.
	HttpMsgParser::ParsingResult Parse(const char*, const size_t, size_t&);

	/// Function to get the parsing result.
	HttpMsgParser::ParsingResult GetResult() { return result_; }
	/// Function to get the parsing mode.
	HttpMsgParser::ParsingMode GetMode() { return mode_; }
	/// Function to set the callbacks for observer.
	void SetCallbacks(const HttpMsgParserCallbacks& callbacks) { callbacks_ = callbacks; }
private:
	void state_transition();
	void parse_method_start();
	void parse_method();
	void parse_uri_start();
	void parse_uri();
	void parse_expected_one_char(const char, const ParsingState);
	void parse_http_version_number_start();
	void parse_http_version_number(const ParsingState, const char, int &);
	void parse_head_line_start();
	void parse_header_field_name();
	void parse_first_lws_before_field_value();
	void parse_header_field_value();
	void parse_lws_inside_field_value();
	void output_field_name_value();

	void parse_status_code(const ParsingState);
	void parse_reason_phrase();

	bool is_char(int c);
	bool is_ctl(int c);
	bool is_separator(int c);
	bool is_digital(int c);

	void one_str_callback_helper(const OneStrCallback&);
	void two_str_callback_helper(const TwoStrCallback&);
	void one_int_callback_helper(const OneIntCallback&);
	void two_int_callback_helper(const TwoIntCallback&);
private:
	/// Member variable to store the parsing mode.
	ParsingMode mode_;
	/// Member variable to store the transition state.
	ParsingState state_;
	/// Member variable to store the parsing result.
	ParsingResult result_;
	/// Pointer for the parsing data.
	const char *current_parse_str_;
	/// Member variable to store the length of parsing string.
	size_t current_str_len_;
	/// Member variable to store the current parsing index.
	size_t current_str_index_;
	/// Member variable to store the callbacks for observer.
	HttpMsgParserCallbacks callbacks_;
	/// Member variable to store "method, uri, reason phrase, field name" temporarily.
	std::string current_result_str1_;
	/// Member variable to store "field value" temporarily.
	std::string current_result_str2_;
	/// Member variable to store "HTTP version major number, status code" temporarily. 
	int current_result_int1_;
	/// Member variable to store "HTTP version minor number" temporarily.
	int current_result_int2_;
};


}
#endif
