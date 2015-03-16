#!/usr/bin/python

import sys
#import time
import datetime
import calendar
import os
import xmlrpclib

# ./DownloadIavf 172.16.2.13 60006 1 1340482642 1340484642 ./output_iavf ./output_idb test 104857600 8 admin id
downloader = "./DownloadIavf"
remote_ip = "172.16.3.4"
#remote_ip = "172.16.2.13"
remote_port = 60006
iavf_output_path = "./output_iavf"
idb_output_path = "/dev/shm/output_idb"
split_file_size = 104857600
time_zone = 8
remote_username = "admin"
remote_password = "id"

xmlrpc_server = "https://127.0.0.1:60009"

s = xmlrpclib.ServerProxy(xmlrpc_server)
session_id = s.server.login("admin", "id", "client")
result = s.info.getMarkedDownload(session_id)

for record in result:
	#print time.mktime(record['StartTime'].timetuple()) # local time
	#print calendar.timegm(record['StartTime'].timetuple()) # GMT

	# convert the ISO8601 string to a datetime object
	converted_start_time = datetime.datetime.strptime(record['StartTime'].value, "%Y%m%dT%H:%M:%S")
	converted_end_time = datetime.datetime.strptime(record['EndTime'].value, "%Y%m%dT%H:%M:%S")

	# Construct the output filename
	temp_filename = "IP_" + remote_ip + "_Camera_" + str(record['ChannelID']) + "_Time" + converted_start_time.strftime("%Y%m%d-%H%M%S") + "_" + converted_end_time.strftime("%Y%m%d-%H%M%S")
	#print temp_filename

	# Calculate the epoch time of start and end time
	start_time = calendar.timegm(record['StartTime'].timetuple())
	end_time = calendar.timegm(record['EndTime'].timetuple())

	# Construct the download command
	download_cmd = downloader + " " + remote_ip + " " + str(remote_port) + " " + str(record['ChannelID']) + " " + str(start_time) + " " + str(end_time) + " " + iavf_output_path + " " + idb_output_path + " " + temp_filename + " " + str(split_file_size) + " " + str(time_zone) + " " + remote_username + " " + remote_password
	print download_cmd

	# Start download
	os.system(download_cmd)

# If there is no anything in the idb folder then we remove the folder.
try:
	os.rmdir(idb_output_path)
except OSError, err:
	print "Error %d: %s" % (err.args[0], err.args[1])

sys.exit(0)

