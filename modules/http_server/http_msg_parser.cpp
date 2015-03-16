#include <http_server/http_msg_parser.hpp>
#include <cassert>

namespace FBi
{

/**
 * @brief Constructor for parser.
 *
 * @param[in] mode The mode can be Request_Mode or Response_Mode.
 */
HttpMsgParser::HttpMsgParser(HttpMsgParser::ParsingMode mode)
{
	ResetState(mode);
}

/**
 * @brief Constructor for parser.
 *
 * @param[in] mode The mode can be Request_Mode or Response_Mode.
 * @param[in] callbacks The callbacks to notify the observer some tokens are parsed.
 */
HttpMsgParser::HttpMsgParser(HttpMsgParser::ParsingMode mode, const HttpMsgParserCallbacks& callbacks)
{
	ResetState(mode);
	SetCallbacks(callbacks);
}

/**
 * @brief Reset the state and variables of parser.
 *
 * @param[in] mode The mode can be Request_Mode or Response_Mode.
 */
void HttpMsgParser::ResetState(const HttpMsgParser::ParsingMode mode)
{
	state_ = ((mode_ = mode) == Request_Mode) ? method_start : http_version_H;
	result_ = Parse_Processing;

	// Be careful, we need to clear them before "new" parsing.
	// Otherwise, it may be fail when parsing headers.
	current_parse_str_ = NULL;
	current_str_len_ = 0;
	current_str_index_ = 0;
	current_result_str1_.clear();
	current_result_str2_.clear();
	current_result_int1_ = 0;
	current_result_int2_ = 0;
}

/**
 * @brief To start parsing the data.
 *
 * @param[in] data The data to be parsed.
 * @param[in] len The length of the data to be parsed.
 * @param[out] last_index
 * (1) Point to the character does not be parsed when Parse_OK.
 * (2) Point to the character cause parsing failed when Parse_Fail.
 * @return
 * (1) Parse_OK: The paring are finished and success.
 * (2) Parse_Fail: The paring is failed.
 * (3) Parse_Processing: The paring is under processing. We need to feed it more data.
 */
HttpMsgParser::ParsingResult HttpMsgParser::Parse(const char *data, 
		const size_t len, size_t &last_index)
{
	assert(data != NULL);

	current_str_len_ = len;
	current_parse_str_ = data;
	current_str_index_ = 0;

	state_transition();

	// If parsing failed, do we write the field name and value out for debugging?

	// Pass the index to outside that indicate
	// 1. the character does not be parsed when Parse_OK
	// 2. the character cause parsing failed when Parse_Fail
	last_index = current_str_index_;

	return result_;
}

/**
 * @brief Helper function to call the callback function and reset temporary variable (One string parameter callback).
 *
 * @param[in] callback The callback function.
 */
void HttpMsgParser::one_str_callback_helper(const OneStrCallback& callback)
{
	if(callback)
		callback(current_result_str1_);
	current_result_str1_.clear();
}

/**
 * @brief Helper function to call the callback function and reset temporary variable (Two string parameters callback).
 *
 * @param[in] callback The callback function.
 */
void HttpMsgParser::two_str_callback_helper(const TwoStrCallback& callback)
{
	if(callback)
		callback(current_result_str1_, current_result_str2_);
	current_result_str1_.clear();
	current_result_str2_.clear();
}

/**
 * @brief Helper function to call the callback function and reset temporary variable (One integer parameter callback).
 *
 * @param[in] callback The callback function.
 */
void HttpMsgParser::one_int_callback_helper(const OneIntCallback& callback)
{
	if(callback)
		callback(current_result_int1_);
	current_result_int1_ = 0;
}

/**
 * @brief Helper function to call the callback function and reset temporary variable (Two integer parameters callback).
 *
 * @param[in] callback The callback function.
 */
void HttpMsgParser::two_int_callback_helper(const TwoIntCallback& callback)
{
	if(callback)
		callback(current_result_int1_, current_result_int2_);
	current_result_int1_ = current_result_int2_ = 0;
}

/// The main state transition function.
void HttpMsgParser::state_transition()
{
	// State transition center because we don't want to use recursive functions.
	while((current_str_index_ < current_str_len_) && (result_ == Parse_Processing))
	{
		switch(state_)
		{
			case method_start:
				parse_method_start();
				break;
			case method:
				parse_method();
				break;
			case uri_start:
				parse_uri_start();
				break;
			case uri:
				parse_uri();
				break;
			case http_version_H:
				parse_expected_one_char('H', http_version_T1);
				break;
			case http_version_T1:
				parse_expected_one_char('T', http_version_T2);
				break;
			case http_version_T2:
				parse_expected_one_char('T', http_version_P);
				break;
			case http_version_P:
				parse_expected_one_char('P', http_version_slash);
				break;
			case http_version_slash:
				parse_expected_one_char('/', http_version_major_start);
				break;
			case http_version_major_start:
				parse_http_version_number_start();
				break;
			case http_version_major:
				// Store http version major number in current_result_int1_
				parse_http_version_number(http_version_minor_start, '.',
						current_result_int1_);
				break;
			case http_version_minor_start:
				parse_http_version_number_start();
				break;
			case http_version_minor_request:
				// Store http version minor number in current_result_int2_
				parse_http_version_number(start_line_LF, '\r',
						current_result_int2_);
				break;
			case start_line_LF:
				parse_expected_one_char('\n', header_line_start);
				break;
			case header_line_start:
				parse_head_line_start();
				break;
			case header_field_name:
				parse_header_field_name();
				break;
			case header_first_lws_before_field_value:
				parse_first_lws_before_field_value();
				break;
			case header_field_value:
				parse_header_field_value();
				break;
			case header_lws_inside_field_value:
				parse_lws_inside_field_value();
				break;
			case header_line_LF:
				parse_expected_one_char('\n', header_line_start);
				break;
			case headers_LF:
				if(current_parse_str_[current_str_index_] == '\n')
				{
					result_ = Parse_OK;
					++current_str_index_;
				}
				else
				{
					result_ = Parse_Fail;
				}
				return;

			case http_version_minor_response:
				// Store http version minor number in current_result_int2_
				parse_http_version_number(status_code_1, ' ',
						current_result_int2_);
				break;
			case status_code_1:
				parse_status_code(status_code_2);
				break;
			case status_code_2:
				parse_status_code(status_code_3);
				break;
			case status_code_3:
				parse_status_code(space_after_status_code);
				break;
			case space_after_status_code:
				parse_expected_one_char(' ', reason_phrase);
				break;
			case reason_phrase:
				parse_reason_phrase();
				break;

			default:
				result_ = Parse_Fail;
				return;
		}
	}
}

/// Function to parse the first character of method name of HTTP request message.
void HttpMsgParser::parse_method_start()
{
	// We do not allow the method start "space" so we need "start" function
	// to eat first character.

	assert(state_ == method_start);

	char ch = current_parse_str_[current_str_index_];

	if (is_char(ch) && !is_ctl(ch) && !is_separator(ch))
	{
		// Add one char to method string
		current_result_str1_ += ch;
		++current_str_index_;
		state_ = method;
		return;
	}

	result_ = Parse_Fail;
}

/// Function to parse the method name of HTTP request message.
void HttpMsgParser::parse_method()
{
	assert(state_ == method);

	size_t start_index = current_str_index_;
	const char *start_str_ptr = current_parse_str_ + start_index;

	for(; current_str_index_ < current_str_len_; ++current_str_index_)
	{
		char ch = current_parse_str_[current_str_index_];

		if(ch == ' ')
		{
			// We skip this expected character and append the string
			current_result_str1_.append(start_str_ptr, current_str_index_ - start_index);

			one_str_callback_helper(callbacks_.method_callback);

			++current_str_index_; // pointer to next character
			state_ = uri_start;
			return;
		}

		if (!is_char(ch) || is_ctl(ch) || is_separator(ch))
		{
			// We still append the valid string because we need it to debug when parsing failed.
			current_result_str1_.append(start_str_ptr, current_str_index_ - start_index);
			result_ = Parse_Fail;
			return;
		}
	}

	// We do not see any space so we remember the partial string now.
	current_result_str1_.append(start_str_ptr, current_str_index_ - start_index);
}

/// Function to parse the first character of URI of HTTP request message.
void HttpMsgParser::parse_uri_start()
{
	assert(state_ == uri_start);

	char ch = current_parse_str_[current_str_index_];

	if (!is_ctl(ch))
	{
		current_result_str1_ += ch;
		++current_str_index_;
		state_ = uri;
		return;
	}

	result_ = Parse_Fail;
}

/// Function to parse the URI of HTTP request message.
void HttpMsgParser::parse_uri()
{
	assert(state_ == uri);

	size_t start_index = current_str_index_;
	const char *start_str_ptr = current_parse_str_ + start_index;

	for(; current_str_index_ < current_str_len_; ++current_str_index_)
	{
		char ch = current_parse_str_[current_str_index_];

		if(ch == ' ')
		{
			current_result_str1_.append(start_str_ptr, current_str_index_ - start_index);

			one_str_callback_helper(callbacks_.uri_callback);

			++current_str_index_;
			state_ = http_version_H;
			return;
		}

		if (is_ctl(ch))
		{
			current_result_str1_.append(start_str_ptr, current_str_index_ - start_index);
			result_ = Parse_Fail;
			return;
		}
	}

	current_result_str1_.append(start_str_ptr, current_str_index_ - start_index);
}

/**
 * @brief Function to parse one character.
 *
 * @param[in] ch The expected character.
 * @param[in] next_state The next state when parsing one character is successful.
 */
void HttpMsgParser::parse_expected_one_char(const char ch, const ParsingState next_state)
{
	if(current_parse_str_[current_str_index_] == ch)
	{
		++current_str_index_;
		state_ = next_state;
		return;
	}

	result_ = Parse_Fail;
}

/// Function to parse the first integer of HTTP version of HTTP message.
void HttpMsgParser::parse_http_version_number_start()
{
	char ch = current_parse_str_[current_str_index_];

	if(!is_digital(ch))
	{
		result_ = Parse_Fail;
		return;
	}

	if(state_ == http_version_major_start)
	{
		current_result_int1_ = static_cast<int>(ch - '0');
		++current_str_index_;
		state_ = http_version_major;
		return;
	}

	if(state_ == http_version_minor_start)
	{
		current_result_int2_ = static_cast<int>(ch - '0');
		++current_str_index_;
		state_ = (mode_ == Request_Mode) ? http_version_minor_request : http_version_minor_response;
		return;
	}

	result_ = Parse_Fail;
}

/**
 * @brief Function to parse HTTP version number.
 *
 * @param[in] next_state The next state when parsing HTTP version is successful.
 * @param[in] expect_char The expected character that indicate the end of the version number.
 * @param[out] version_number The output version number (major or minor).
 */
void HttpMsgParser::parse_http_version_number(const ParsingState next_state,
					const char expect_char,
					int &version_number)
{
	assert((state_ == http_version_major) || 
		(state_ == http_version_minor_request) ||
		(state_ == http_version_minor_response));

	for(; current_str_index_ < current_str_len_; ++current_str_index_)
	{
		char ch = current_parse_str_[current_str_index_];

		if(ch == expect_char)
		{
			if((state_ == http_version_minor_request) || (state_ == http_version_minor_response))
				two_int_callback_helper(callbacks_.http_version_callback);

			++current_str_index_;
			state_ = next_state;
			return;
		}

		if(!is_digital(ch))
		{
			result_ = Parse_Fail;
			return;
		}

		version_number = version_number*10 + static_cast<int>(ch - '0');
	}

}

/// Function to trim the space before and after the field value and transfer the field name to lower case. Then call the callback function to output one header.
void HttpMsgParser::output_field_name_value()
{
	// Store the complete field name and value
	if(!current_result_str1_.empty())
	{
		// The field name is case-insensitive, so we change them to lower case
		size_t len = current_result_str1_.length();
		size_t i;
		for(i = 0; i < len; ++i)
			current_result_str1_[i] = tolower(current_result_str1_[i]);

		// Trim the lws before the field value.
		len = current_result_str2_.length();
		for(i = 0; i < len; ++i)
		{
			if((current_result_str2_[i] != ' ') && (current_result_str2_[i] != '\t'))
				break;
		}
		current_result_str2_.erase(0, i);

		// Trim the lws after the field value.
		len = current_result_str2_.length();
		for(i = len - 1; i >= 1; --i)
		{
			if((current_result_str2_[i] != ' ') && (current_result_str2_[i] != '\t'))
				break;
		}
		if( (i == 0) && ((current_result_str2_[i] == ' ') || (current_result_str2_[i] == '\t')) )
			current_result_str2_.clear();
		else
			current_result_str2_.erase(i+1, len-1-i);


		two_str_callback_helper(callbacks_.one_header_callback);
	}

	current_result_str1_.clear();
	current_result_str2_.clear();
}

/// Function to parse the start of one header line.
void HttpMsgParser::parse_head_line_start()
{
	assert(state_ == header_line_start);

	char ch = current_parse_str_[current_str_index_];

	if(ch == '\r')
	{
		// Store complete field name and value
		output_field_name_value();

		++current_str_index_;
		state_ = headers_LF;
		return;
	}

	// If the field name is not empty
	if (!current_result_str1_.empty() && (ch == ' ' || ch == '\t'))
	{
		++current_str_index_;
		state_ = header_lws_inside_field_value;
		return;
	}
	
	if (is_char(ch) && !is_ctl(ch) && !is_separator(ch))
	{
		// Store complete field name and value
		output_field_name_value();

		// New head
		current_result_str1_ += ch;
		++current_str_index_;
		state_ = header_field_name;
		return;
	}

	result_ = Parse_Fail;
}

/// Function to parse the field name of HTTP header.
void HttpMsgParser::parse_header_field_name()
{
	assert(state_ == header_field_name);

	size_t start_index = current_str_index_;
	const char *start_str_ptr = current_parse_str_ + start_index;

	for(; current_str_index_ < current_str_len_; ++current_str_index_)
	{
		char ch = current_parse_str_[current_str_index_];

		if (ch == ':')
		{
			current_result_str1_.append(start_str_ptr, current_str_index_ - start_index);
			++current_str_index_;
			state_ = header_first_lws_before_field_value;
			return;
		}

		if (!is_char(ch) || is_ctl(ch) || is_separator(ch))
		{
			current_result_str1_.append(start_str_ptr, current_str_index_ - start_index);
			result_ = Parse_Fail;
			return;
		}
	}

	current_result_str1_.append(start_str_ptr, current_str_index_ - start_index);
}

/// Function to parse the first linear white space before the field value. 
void HttpMsgParser::parse_first_lws_before_field_value()
{
	assert(state_ == header_first_lws_before_field_value);

	// From specification: The field value MAY be preceded by any amount 
	// of LWS, though a single SP is preferred

	for(; current_str_index_ < current_str_len_; ++current_str_index_)
	{
		char ch = current_parse_str_[current_str_index_];

		if((ch == ' ') || (ch == '\t'))
			continue;
		
		state_ = header_field_value;
		return;
	}
}

/// Function to parse the field value of HTTP header.
void HttpMsgParser::parse_header_field_value()
{
	assert(state_ == header_field_value);

	size_t start_index = current_str_index_;
	const char *start_str_ptr = current_parse_str_ + start_index;

	for(; current_str_index_ < current_str_len_; ++current_str_index_)
	{
		char ch = current_parse_str_[current_str_index_];

		if (ch == '\r')
		{
			current_result_str2_.append(start_str_ptr, current_str_index_ - start_index);
			++current_str_index_;
			state_ = header_line_LF;
			return;
		}

		if((ch == ' ') || (ch == '\t'))
		{
			current_result_str2_.append(start_str_ptr, current_str_index_ - start_index);
			++current_str_index_;
			state_ = header_lws_inside_field_value;
			return;
		}

		if (is_ctl(ch))
		{
			current_result_str2_.append(start_str_ptr, current_str_index_ - start_index);
			result_ = Parse_Fail;
			return;
		}
	}

	current_result_str2_.append(start_str_ptr, current_str_index_ - start_index);
}

/// Function to parse the linear white space inside the field value of HTTP header.
void HttpMsgParser::parse_lws_inside_field_value()
{
	assert(state_ == header_lws_inside_field_value);

	for(; current_str_index_ < current_str_len_; ++current_str_index_)
	{
		char ch = current_parse_str_[current_str_index_];

		if (ch == '\r')
		{
			++current_str_index_;
			state_ = header_line_LF;
			return;
		}

		if (ch == ' ' || ch == '\t')
			continue;

		if (!is_ctl(ch))
		{
			// We may replace the lws with one space between the field contents
			current_result_str2_ += ' ';

			current_result_str2_ += ch;
			++current_str_index_;
			state_ = header_field_value;
			return;
		}

		result_ = Parse_Fail;
		return;
	}
}

/**
 * @brief Function to parse status code of HTTP response message.
 *
 * @param[in] next_state The next state when parsing one integer is successful.
 */
void HttpMsgParser::parse_status_code(const ParsingState next_state)
{
	assert((state_ == status_code_1) || (state_ == status_code_2) || (state_ == status_code_3));

	char ch = current_parse_str_[current_str_index_];
	if(is_digital(ch))
	{
		current_result_int1_ = current_result_int1_*10 + static_cast<int>(ch - '0');
		++current_str_index_;
		state_ = next_state;
		return;
	}

	result_ = Parse_Fail;
}

/// Function to parse reason phrase of HTTP response message.
void HttpMsgParser::parse_reason_phrase()
{
	assert(state_ == reason_phrase);

	one_int_callback_helper(callbacks_.status_code_callback);

	size_t start_index = current_str_index_;
	const char *start_str_ptr = current_parse_str_ + start_index;

	for(; current_str_index_ < current_str_len_; ++current_str_index_)
	{
		char ch = current_parse_str_[current_str_index_];
		if (ch == '\r')
		{
			current_result_str1_.append(start_str_ptr, current_str_index_ - start_index);

			one_str_callback_helper(callbacks_.reason_phrase_callback);

			++current_str_index_;
			state_ = start_line_LF;
			return;
		}

		if (ch == ' ' || ch == '\t')
			continue;

		if (is_ctl(ch))
		{
			current_result_str1_.append(start_str_ptr, current_str_index_ - start_index);
			result_ = Parse_Fail;
			return;
		}
	}

	current_result_str1_.append(start_str_ptr, current_str_index_ - start_index);
}

/**
 * @brief Function to determine the input is regular character or not.
 *
 * @param[in] c The input character.
 * @return true: If input is character. Otherwise, it is false.
 */
bool HttpMsgParser::is_char(int c)
{
	return c >= 0 && c <= 127;
}

/**
 * @brief Function to determine the input is control character or not.
 *
 * @param[in] c The input character.
 * @return true: If input is control character. Otherwise, it is false.
 */
bool HttpMsgParser::is_ctl(int c)
{
	return (c >= 0 && c <= 31) || (c == 127);
}

/**
 * @brief Function to determine the input is separator or not.
 *
 * @param[in] c The input character.
 * @return true: If input is separator. Otherwise, it is false.
 */
bool HttpMsgParser::is_separator(int c)
{
	switch (c)
	{
		case '(': case ')': case '<': case '>': case '@':
		case ',': case ';': case ':': case '\\': case '"':
		case '/': case '[': case ']': case '?': case '=':
		case '{': case '}': case ' ': case '\t':
			return true;
		default:
			return false;
	}
}

/**
 * @brief Function to determine the input is digital or not.
 *
 * @param[in] c The input character.
 * @return true: If input is digital. Otherwise, it is false.
 */
bool HttpMsgParser::is_digital(int c)
{
	return c >= '0' && c <= '9';
}

}
