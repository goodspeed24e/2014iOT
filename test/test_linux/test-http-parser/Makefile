.PHONY: all clean

CC = g++ -g -Wall -Wextra -pedantic -Werror -std=c++11
CFLAGS = -I./ -I../../../libs -I../../../modules
LIBS= -L../../../buildtemp

all_targets=test-http-parser

all: $(all_targets)

test-http-parser: test-http-parser.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -lhttp_server -ltransport -lssl -lboost_system-mt

clean:
	-rm -f $(all_targets)

