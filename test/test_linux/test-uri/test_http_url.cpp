#include <http_server/uri.hpp>
#include <cstdio>

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		printf("Usage: %s HTTP_URL\n", argv[0]);
		return -1;
	}

	instek::URI_Struct uri;
	if(instek::HTTP_URL_Utility::ParseHttpUrl(argv[1], uri))
		printf("Parse Success\n");
	else
		printf("Parse Failed\n");

	instek::URI_Utility::PrintURI(uri);

	return 0;
}
