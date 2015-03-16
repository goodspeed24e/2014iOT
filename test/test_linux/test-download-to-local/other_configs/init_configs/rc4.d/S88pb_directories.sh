#!/bin/bash

### BEGIN INIT INFO
# Provides: pb_downloader_directories
# Required-Start:   $network $remote_fs $syslog
# Required-Stop:    $network $remote_fs $syslog
# Default-Start:    2 3 4 5
# Default-Stop:
# Short-Description: pb downloader directories
### END INIT INFO

PATH=/sbin:/bin:/usr/sbin:/usr/bin

. /lib/lsb/init-functions

echo "Preparing pb_downloader directories..."

if [ ! -d /syshdd/log/streaming_pb_downloader ]; then
  /bin/mkdir -p /syshdd/log/streaming_pb_downloader
  /bin/chown admin:1000 /syshdd/log/streaming_pb_downloader
fi

if [ ! -d /syshdd/streaming_pb_downloader/configs ]; then
  /bin/mkdir -p /syshdd/streaming_pb_downloader/configs
  /bin/chown admin:1000 /syshdd/streaming_pb_downloader
fi
