.PHONY: all clean

CC = g++ -g -Wall -Wextra -pedantic -Werror -std=c++11
CFLAGS = -I./ -I../../../libs -I../../../modules
LIBS= -L../../../buildtemp

all_targets=test-notification

all: $(all_targets)

test-notification: test-notification.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -lnotification_center -lboost_thread-mt

clean:
	-rm -f $(all_targets)

