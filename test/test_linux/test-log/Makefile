.PHONY: all clean

CC = g++ -g -Wall -Wextra -pedantic -Werror -std=c++11
CFLAGS = -I./ -I../../../libs -I../../../modules
LIBS= -L../../../buildtemp

all_targets=test-log-file

all: $(all_targets)

test-log-file: test-log-file.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -lboost_iostreams -lcommon -lboost_system-mt

clean:
	-rm -f $(all_targets)

