#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <boost/bind.hpp>
#include <http_server/http_msg_parser.hpp>
#include <http_server/http_msg.hpp>
#include <http_server/uri.hpp>

#define BUFFER_SIZE 4

class TestParser
{
public:
	TestParser(instek::HttpMsgParser::ParsingMode mode)
		: parser_(mode)
	{
		// Prepare callback functions for http message parser
		callbacks_.method_callback = boost::bind(&TestParser::method_callback, this, _1);
		callbacks_.uri_callback = boost::bind(&TestParser::uri_callback, this, _1);
		callbacks_.http_version_callback = boost::bind(&TestParser::http_version_callback, this, _1, _2);
		callbacks_.status_code_callback = boost::bind(&TestParser::status_code_callback, this, _1);
		callbacks_.reason_phrase_callback = boost::bind(&TestParser::reason_phrase_callback, this, _1);
		callbacks_.one_header_callback = boost::bind(&TestParser::one_header_callback, this, _1, _2);

		// Regsiter callbacks
		parser_.SetCallbacks(callbacks_);

		// Use message pointer point to request or response message
		if(mode == instek::HttpMsgParser::Request_Mode)
			msg_ = &request_msg_;
		else
			msg_ = &response_msg_;
	}

	~TestParser()
	{
	}

	instek::HttpMsgParser::ParsingResult Parse(const char* data, size_t len, size_t &last_index)
	{
		return parser_.Parse(data, len, last_index);
	}

	instek::HttpMsg* GetHttpMsg()
	{
		return msg_;
	}

	void PrintHttpMsg()
	{
		printf("\nStart Line ====>\n");
		instek::HttpMsgParser::ParsingMode mode = parser_.GetMode();
		if(mode == instek::HttpMsgParser::Request_Mode)
		{
			// For size_t in C99, we can use %zu specification in the printf
			printf("Method = %s\\n\n", request_msg_.method.c_str());
			instek::URI_Utility::PrintURI(request_msg_.uri);
		}
		printf("version = %d.%d\\n\n",msg_->http_version_major, msg_->http_version_minor);
		if(mode == instek::HttpMsgParser::Response_Mode)
		{
			printf("Status Code = %d\\n\n", response_msg_.status_code);
			printf("Reason Phrase = %s\\n\n", response_msg_.reason_phrase.c_str());
		}

		std::map<std::string, std::string>::iterator iter;
		printf("\nHeaders =======>\n");
		for (iter = msg_->headers.begin(); iter != msg_->headers.end(); iter++)
			printf("%s:%s\\n\n", (*iter).first.c_str(), (*iter).second.c_str());
	}
private:
	void method_callback(const std::string& method)
	{
		request_msg_.method = method;
	}

	void uri_callback(const std::string& uri)
	{
		printf("Raw URI = %s\n", uri.c_str());

		if(!instek::HTTP_URL_Utility::ParseHttpUrl(uri, request_msg_.uri))
			printf("HttpConnection::uri_callback => Parse HTTP URL Failed.\n");
	}

	void http_version_callback(int major, int minor)
	{
		msg_->http_version_major = major;
		msg_->http_version_minor = minor;
	}

	void status_code_callback(int status_code)
	{
		response_msg_.status_code = status_code;
	}

	void reason_phrase_callback(const std::string& reason_phrase)
	{
		response_msg_.reason_phrase = reason_phrase;
	}

	void one_header_callback(const std::string& field_name, const std::string& field_value)
	{
		(msg_->headers)[field_name] = field_value;
	}
private:
	instek::HttpMsgParser::HttpMsgParserCallbacks callbacks_;
	instek::HttpMsgParser parser_;

	instek::HttpRequestMsg request_msg_;
	instek::HttpResponseMsg response_msg_;
	instek::HttpMsg* msg_;
};


int main(int argc, char **argv)
{
	FILE *inFile;
	bool is_request;

	if (argc != 3) {
		printf("Usage: %s <pathname or filename> <mode: odd_num: Request, even_num: Response>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	inFile = fopen(argv[1], "r");
	if(inFile == NULL)
	{
		printf("Cna not open file %s.\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	TestParser *tester;
	is_request = (atoi(argv[2]) & 1) ? true : false;
	if(is_request)
		tester = new TestParser(instek::HttpMsgParser::Request_Mode);
	else
		tester = new TestParser(instek::HttpMsgParser::Response_Mode);

	if(!tester)
	{
		printf("Cna not create tester.\n");
		fclose(inFile);
		exit(EXIT_FAILURE);
	}

	size_t len = 0;
	char buf[BUFFER_SIZE] = {'\0'};
	std::string content;
	size_t last_index = 0;
	instek::HttpMsgParser::ParsingResult result = instek::HttpMsgParser::Parse_Fail;

	if(!feof(inFile))
	{
		// Read the content of file until the end of file or the parser is stopping
		do
		{
			len = fread(buf, 1, BUFFER_SIZE, inFile);
			if(len > 0)
				result = tester->Parse(buf, len, last_index);
		} while(!feof(inFile) && (result == instek::HttpMsgParser::Parse_Processing));


		if(result == instek::HttpMsgParser::Parse_OK)
		{
			// Extract body of http message
			content.append(buf + last_index, len - last_index);

			while(!feof(inFile))
			{
				len = fread(buf, 1, BUFFER_SIZE, inFile);
				content.append(buf, len);
			}
		}
		else if(result == instek::HttpMsgParser::Parse_Fail)
		{
			printf("Parse Fail!\n");
		}
		else
		{
			printf("Processing????\n");
		}
		tester->PrintHttpMsg();
		printf("\nBody ==========>\n");
		printf("%s\nBody len = %lu\n\n", content.c_str(), (unsigned long)content.length());
	}
	else
	{
		printf("EOF\n");
	}


	fclose(inFile);

	exit(EXIT_SUCCESS);
}
