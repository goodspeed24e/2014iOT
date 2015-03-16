CC = g++ -g -Wall -Wextra -pedantic -Werror -std=c++0x
CFLAGS = -I../../../libs -I../../../modules
LIBS = -L../../../buildtemp

.PHONY: all clean

all_targets=xmlrpc-server-test session-ssl-test ulxmlrpc_client iqxmlrpc_client

all: $(all_targets)

xmlrpc-server-test: xmlrpc-server-test.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -lboost_thread-mt -lssl -lcommon -ltransport -lhttp_server -lxmlrpc_server -lboost_system-mt

session-ssl-test: session-ssl-test.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -lboost_thread-mt -lssl -lcommon -ltransport -lboost_system-mt

ulxmlrpc_client: ulxmlrpc_client.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -lulxmlrpcpp

iqxmlrpc_client: iqxmlrpc_client.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -liqxmlrpc -liqxmlrpc-client

clean:
	-rm -f $(all_targets)

