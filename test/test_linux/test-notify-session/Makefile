CC = g++ -g -Wall -Wextra -pedantic -Werror -std=c++11
CFLAGS = -I../../../libs -I../../../modules
LIBS = -L../../../buildtemp

.PHONY: all clean

all_targets=notify-session-test

all: $(all_targets)

notify-session-test: notify-session-test.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -lboost_thread-mt -lssl -lboost_iostreams-mt -lcommon -ltransport -lboost_system-mt

clean:
	-rm -f $(all_targets)

