CC = g++ -g -Wall -Wextra -pedantic -Werror -std=c++11
CFLAGS = -I../../../libs -I../../../modules
LIBS = -L../../../buildtemp

.PHONY: all clean

all_targets=test-multimedia test-qt-audio test-in-out

all: $(all_targets)

test-multimedia: test-multimedia.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -lboost_thread-mt -lboost_system-mt -lcommon -lmultimedia -lQtMultimediaKit -I/usr/include/QtMultimediaKit -I/usr/include/QtMobility `pkg-config --cflags --libs QtCore libavcodec libavutil libswscale libavformat`

test-qt-audio: test-qt-audio.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -lQtMultimediaKit -I/usr/include/QtMultimediaKit -I/usr/include/QtMobility `pkg-config --cflags --libs QtCore`


test-in-out: test-in-out.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS) -lboost_thread-mt -lboost_system-mt -lcommon -lmultimedia -lQtMultimediaKit -I/usr/include/QtMultimediaKit -I/usr/include/QtMobility `pkg-config --cflags --libs QtCore`

clean:
	-rm -f $(all_targets)

