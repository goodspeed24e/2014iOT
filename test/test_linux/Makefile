CC = g++ -g -Wall -std=c++0x
CFLAGS = -I../../libs -I../../libs/libiavfng/iavf_file_proxy -I../../libs/libiavfng/instek_util -I../../modules -I/usr/include/ffmpeg -I/usr/include/SDL
LIBS = -L../../buildtemp -lcommon -ltransport

QT_PKG_CONFIG=`pkg-config --libs --cflags QtGui`

.PHONY: all clean

all_targets=StreamingDecoder player-test session-test ptree-method-settings-test buf-mgr-test qt-player-test

all: $(all_targets)

StreamingDecoder: streaming-decoder.cpp
	$(CC) $(CFLAGS) -o StreamingDecoder streaming-decoder.cpp $(LIBS) -lplayer -lboost_thread-mt -liavfng -lstreaming -lssl -lav -lboost_system-mt

player-test: player-test.cpp
	$(CC) $(CFLAGS) -o player-test player-test.cpp $(LIBS) -lplayer -lboost_thread-mt -lstreaming -lssl -liavfng -lav

session-test: session-test.cpp
	$(CC) $(CFLAGS) -o session-test session-test.cpp $(LIBS) -lboost_thread-mt -liavfng -lssl -lboost_iostreams-mt -lboost_system-mt

ptree-method-settings-test: ptree-method-settings-test.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -lcommon -ltransport -lssl -lboost_system-mt

buf-mgr-test: buf-mgr-test.cpp
	$(CC) $^ -o $@ $(CFLAGS) -L../../buildtemp -lcommon -lav -lboost_system-mt


qt-player-test: qt-player-test.cpp
	$(CC) $^ -o $@ $(CFLAGS) -L../../buildtemp $(QT_PKG_CONFIG) -lplayer -lboost_thread-mt -lstreaming -lssl -liavfng -lav $(LIBS) -lboost_system-mt -I/usr/include/QtMultimediaKit -I/usr/include/QtMobility

clean:
	-rm -f *.o $(all_targets)
