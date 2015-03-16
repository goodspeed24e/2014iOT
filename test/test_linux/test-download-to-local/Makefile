#CC = g++ -g -Wall -Wextra -pedantic -std=c++11
CC = g++ -O3 -Wall -Wextra -pedantic -std=c++11
CFLAGS = -I../../../libs -I../../../libs/libiavfng/iavf_file_proxy -I../../../libs/libiavfng/instek_util
LIBS = -L../../../buildtemp -lcommon

.PHONY: all clean

all_targets=DownloadIavf DownloadIavf_NoAutoCreateIAVFFolder DownloadIavf_OnlyDownload

all: $(all_targets)

DownloadIavf: download-iavf.cpp streaming-downloader.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -lboost_thread-mt -liavfng -lstreaming -lssl -lboost_system-mt -lboost_filesystem-mt

DownloadIavf_NoAutoCreateIAVFFolder: download-iavf.cpp streaming-downloader.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -DDO_NOT_AUTO_CREATE_IAVF_OUTPUT_PATH -lboost_thread-mt -liavfng -lstreaming -lssl -lboost_system-mt -lboost_filesystem-mt

DownloadIavf_OnlyDownload: download-iavf.cpp streaming-downloader.cpp
	$(CC) $^ -o $@ $(CFLAGS) -DONLY_DOWNLOAD $(LIBS) -lboost_thread-mt -liavfng -lstreaming -lssl -lboost_system-mt -lboost_filesystem-mt

clean:
	-rm -f *.o $(all_targets)
