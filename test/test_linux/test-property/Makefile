CC = g++ -g -Wall -Wextra -pedantic -Werror -std=c++0x
CFLAGS = -I../../../libs -I../../../modules
LIBS = -L../../../buildtemp

.PHONY: all clean

all_targets=test-limitation test-new-properties test-array-properties test-xmlrpc-date-time test-empty-struct-array test-get-sub-properties test-copy

all: $(all_targets)

test-limitation: test-limitation.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -lcommon -lboost_system-mt

test-new-properties: test-new-properties.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -lcommon -lboost_system-mt

test-array-properties: test-array-properties.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -lcommon -lboost_system-mt

test-xmlrpc-date-time: test-xmlrpc-date-time.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -lcommon -lboost_system-mt

test-empty-struct-array: test-empty-struct-array.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -lcommon -ltransport -lssl -lboost_system-mt

test-get-sub-properties: test-get-sub-properties.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -lcommon -lboost_system-mt -lboost_filesystem-mt

test-copy: test-copy.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -lcommon -lboost_system-mt -lboost_filesystem-mt

clean:
	-rm -f $(all_targets) test-array-properties-out.xml
	-rm -rf sub_properties_output_dir sub_properties_merge_output_dir copy_output_dir

