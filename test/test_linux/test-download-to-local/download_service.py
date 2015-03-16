#!/usr/bin/python

import sys
import MySQLdb
#import time
import datetime
import calendar
import os
import shlex
import subprocess

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

# Remote Datebase
datebase_host = "localhost"
#datebase_host = "172.16.2.13"
datebase_username = "root"
datebase_password = ""
datebase_name = "ApolloDB"

try:
	# Connect to database and get the rows those need to be downloaded
	conn = MySQLdb.connect(host=datebase_host, user=datebase_username, passwd=datebase_password, db=datebase_name)
	cursor = conn.cursor()
	cursor.execute("Select ChannelID, StartTime, EndTime From AVSlot Where State=2 or State=3")
	#result = cursor.fetchone()
	result = cursor.fetchall()
	cursor.close()
	conn.close()

	for record in result:
		#print record
		#print time.mktime(record[1].timetuple()) # local time
		#print calendar.timegm(record[1].timetuple()) # GMT

		# Construct the output filename
		temp_filename = "IP_" + remote_ip + "_Camera_" + str(record[0]) + "_Time" + record[1].strftime("%Y%m%d-%H%M%S") + "_" + record[2].strftime("%Y%m%d-%H%M%S")
		#print temp_filename

		# Calculate the epoch time of start and end time
		start_time = calendar.timegm(record[1].timetuple())
		end_time = calendar.timegm(record[2].timetuple())

		# Construct the download command
		download_cmd = downloader + " " + remote_ip + " " + str(remote_port) + " " + str(record[0]) + " " + str(start_time) + " " + str(end_time) + " " + iavf_output_path + " " + idb_output_path + " " + temp_filename + " " + str(split_file_size) + " " + str(time_zone) + " " + remote_username + " " + remote_password
		print download_cmd

		# Start download
		temp_args = shlex.split(download_cmd)
		child_p = subprocess.Popen(args=temp_args)
		child_p.communicate()

		# Get stdout or stderr.
		#child_p = subprocess.Popen(args=temp_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		#output = child_p.communicate()
		#print output[0]
		#print output[1]

		# The returncode of subprocess in the python is 0~255 when it succeed.
		print "Return Value: ", child_p.returncode
		if child_p.returncode != 0:
			print "We failed to download. The command is in the below."
			print download_cmd
			# We need to tell user

		#ret_value = os.system(download_cmd)
		#signal_type = ret_value & 0x00FF
		#exit_status = (ret_value >> 8) & 0x00FF
		#print signal_type
		#print exit_status

except MySQLdb.Error, err:
	print "Error %d: %s" % (err.args[0], err.args[1])
	sys.exit (1)
except ValueError, err:
	print "Error %d: %s" % (err.args[0], err.args[1])
	sys.exit (1)
except OSError, err:
	print "Error %d: %s" % (err.args[0], err.args[1])
	sys.exit (1)
except:
	print "Unexpected error:", sys.exc_info()[0]
	sys.exit (1)


# If there is no anything in the idb folder then we remove the folder.
try:
	os.rmdir(idb_output_path)
except OSError, err:
	print "We try to remove temporary folder."
	print "Error %d: %s" % (err.args[0], err.args[1])
except:
	print "Unexpected error:", sys.exc_info()[0]
	sys.exit (1)

sys.exit (0)

