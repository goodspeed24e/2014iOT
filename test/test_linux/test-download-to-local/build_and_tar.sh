#!/bin/bash

if [ "$#" != "2" ]; then
	echo "Usage: $0 ap_version use_init"
	exit 1
fi

# Build the binary.
make -j4

ap_version=$1
use_init=$2
svn_version=`export LANGUAGE="en_US";svn info | grep "Last Changed Rev" | awk '{print $4}'`
version="${ap_version}-${svn_version}"


# Create folders.
mkdir -p ./build/usr/local/id/streaming_pb_downloader/${version}/bin
mkdir -p ./build/usr/local/id/streaming_pb_downloader/${version}/lib
mkdir -p ./build/usr/local/id/streaming_pb_downloader/${version}/scripts
mkdir -p ./build/etc/ld.so.conf.d
mkdir -p ./build/etc/cron.d
echo ${version} > ./build/usr/local/id/streaming_pb_downloader/${version}/VERSION

if [ "$2" == "0" ]; then
	mkdir -p ./build/var/log/streaming_pb_downloader
	mkdir -p ./build/var/lib/hdvr/streaming_pb_downloader/configs
fi

# Create links.
cd ./build
ln -s /var/lib/hdvr/streaming_pb_downloader/configs usr/local/id/streaming_pb_downloader/${version}/configs
cd usr/local/id/streaming_pb_downloader
ln -s ${version} current
cd ../../../../..

# Copy boost library files
tar xjf ./lib/boost_filesystem.tbz2 --directory=./lib/
cp -a ./lib/libboost_filesystem.so* ./build/usr/local/id/streaming_pb_downloader/${version}/lib

# Copy files.
cp -a ./DownloadIavf_NoAutoCreateIAVFFolder ./build/usr/local/id/streaming_pb_downloader/${version}/bin/streaming_pb_downloader
cp -a ../../../buildtemp/libstreaming.so ../../../buildtemp/libcommon.so ./build/usr/local/id/streaming_pb_downloader/${version}/lib
cp -a ./download_service_for_archive.py ./loggingplugin.py ./generate_downloader_ini.py ./generate_event_name_list.py ./build/usr/local/id/streaming_pb_downloader/${version}/scripts
cp -a ./streaming_pb_downloader_libs.conf ./build/etc/ld.so.conf.d
cp -a ./pb_downloader ./build/etc/cron.d

if [ "$2" != "0" ]; then
	cp -ar ./other_configs/init_configs/* ./build/etc
fi

# tar them
cd ./build
if [ "$2" == "0" ]; then
	tar --owner=root --group=root -jcpf streaming_pb_downloader.tar.bz2 usr var etc
else
	tar --owner=root --group=root -jcpf streaming_pb_downloader.tar.bz2 usr etc
fi

cd ..


