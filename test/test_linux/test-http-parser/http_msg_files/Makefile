.PHONY: all clean

CC = g++ -g -Wall -Wextra -pedantic -Werror -std=c++11

all_targets=gen-file

all: $(all_targets)

gen-file: gen-file.cpp
	$(CC) $^ -o $@

clean:
	-rm -f $(all_targets)

