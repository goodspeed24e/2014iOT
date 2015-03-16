CC = g++ -g -Wall -Wextra -pedantic -Werror -std=c++0x
CFLAGS = -I../../../libs -I../../../modules
LIBS = -L../../../buildtemp

.PHONY: all clean

all_targets=test_http_url

all: $(all_targets)

test_http_url: test_http_url.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -lcommon -lboost_thread-mt -lssl -lhttp_server -lboost_system-mt
clean:
	-rm -f $(all_targets)

