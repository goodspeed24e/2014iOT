#!/bin/bash

# ./DownloadIavf 172.16.2.13 60006 1 1340482642 1340484642 ./output_iavf ./output_idb test 104857600 8 admin id

# For streaming downloader
downloader="./DownloadIavf"
#downloader="./DownloadIavf_OnlyDownload"
max_num_of_download_process=1
#remote_ip="172.16.3.4"
remote_ip=`ifconfig eth0 | grep "inet addr" | awk -F: '{ print $2 }' | awk '{ print $1 }'`
remote_port=60006
iavf_output_path="./output_iavf"
idb_output_path="/dev/shm/output_idb"
split_file_size=104857600
time_zone=8
remote_username="admin"
remote_password="id"

# For remote datebase
#datebase_host="localhost"
datebase_username="root"
#datebase_password=""
datebase_name="ApolloDB"

max_channel_id=""
min_channel_id=""

sql_cmd="select ChannelID, StartTime, EndTime from AVSlot where (State=2 or State=3)"
channel_cmd=""

if [ -n "$min_channel_id" ]; then
	channel_cmd="ChannelID >= ${min_channel_id}"
fi

if [ -n "$max_channel_id" ]; then
	if [ -n "$channel_cmd" ]; then
		channel_cmd="${channel_cmd} and ChannelID <= ${max_channel_id}"
	else
		channel_cmd="ChannelID <= ${max_channel_id}"
	fi
fi

if [ -n "$channel_cmd" ]; then
	sql_cmd="${sql_cmd} and (${channel_cmd})"
fi

sql_cmd="${sql_cmd} Order By StartTime"

echo $sql_cmd

# Connect to datebase and query data.
results=`mysql -B -u ${datebase_username} -D ${datebase_name} -e "${sql_cmd}" | sed -n '2,$p'`

# Calculate the number of rows in the data.
num_of_rows=`echo "${results}" | wc -l`

# Start download
pids=()

process_limited()
{
	local this
	while [ ${#} -gt ${max_num_of_download_process} ]; do
		this=${1}
		wait "$this"
		shift
	done

	pids=($@)
}

for (( i=1; i <= ${num_of_rows}; i++ ))
do
	process_limited ${pids[@]}

	channel_id=`echo "${results}" | sed -n "${i}p" | cut -f 1`
	temp_start_time=`echo "${results}" | sed -n "${i}p" | cut -f 2`
	temp_end_time=`echo "${results}" | sed -n "${i}p" | cut -f 3`
	start_time_str=`date --utc --date="${temp_start_time}" "+%Y%m%d-%H%M%S"`
	end_time_str=`date --utc --date="${temp_end_time}" "+%Y%m%d-%H%M%S"`
	start_time_epoch=`date --utc --date="${temp_start_time}" "+%s"`
	end_time_epoch=`date --utc --date="${temp_end_time}" "+%s"`

	filename="IP_${remote_ip}_Camera_${channel_id}_Time${start_time_str}_${end_time_str}"
	download_cmd="${downloader} ${remote_ip} ${remote_port} ${channel_id} ${start_time_epoch} ${end_time_epoch} ${iavf_output_path} ${idb_output_path} ${filename} ${split_file_size} ${time_zone} ${remote_username} ${remote_password}"


	echo ${download_cmd}

	# Execute the download command.
	eval ${download_cmd}&

	pids=( ${pids[@]} $! )
done

for pid in ${pids[@]}
do
	wait "$pid"
done

# If there is no anything in the idb folder then we remove the folder.
rmdir ${idb_output_path}

