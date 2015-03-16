#!/usr/bin/python

import sys
import MySQLdb
#import time
import datetime
import calendar
import os
import stat
import shlex
import subprocess
import shutil
import ConfigParser
import tempfile
import fcntl
import logging
import xmlrpclib

import loggingplugin


# ./DownloadIavf 172.16.2.13 60006 1 1340482642 1340484642 ./output_iavf ./output_idb test 104857600 8 admin id

################################################
#path_of_configuration_file_for_this_script = './downloader.ini'
#path_of_log_file = '/var/log'
path_of_configuration_file_for_this_script = '/usr/local/id/streaming_pb_downloader/configs/downloader.ini'
path_of_log_file = '/var/log/streaming_pb_downloader'
log_base_filename = path_of_log_file + '/' + os.path.splitext(os.path.basename(__file__))[0] + '.log'

################################################
# Prepare Logger
# We need open this comment when we use this script in the product because some handlers may throw exception.
logging.raiseExceptions = 0
instek_logger = logging.getLogger('InstekLogger')
instek_logger.setLevel(logging.DEBUG)

# Add the log message handler to the logger
file_handler = loggingplugin.CompressedRotatingFileHandler(log_base_filename, maxBytes=67108864, backupCount=6)
file_handler.setLevel(logging.INFO)

file_formatter = logging.Formatter('[%(levelname)s][%(asctime)s][%(process)d]: %(message)s')
file_handler.setFormatter(file_formatter)

instek_logger.addHandler(file_handler)

console_handler = logging.StreamHandler()
console_handler.setLevel(logging.DEBUG)

console_formatter = logging.Formatter('[%(levelname)s][%(asctime)s][%(process)d][%(funcName)s]: %(message)s')
console_handler.setFormatter(console_formatter)

instek_logger.addHandler(console_handler)

try:
	instek_logger.info('Read log mail configuration.')

	config = ConfigParser.RawConfigParser()
	config.read(path_of_configuration_file_for_this_script)

	smtp_server = config.get('LogMail', 'Mail Server')
	smtp_port = 0
	try:
		smtp_port = config.getint('LogMail', 'Port')
	except ValueError, err:
		instek_logger.error('When convert SMTP port to integer. We use default port. %s', str(err))

	smtp_is_ssl = True
	try:
		smtp_is_ssl = config.getboolean('LogMail', 'Is SSL')
	except ValueError, err:
		instek_logger.error('When convert SMTP SSL mode to boolean. We use SSL mode. %s', str(err))

	smtp_fromaddr = config.get('LogMail', 'From Address')
	splitat=';,'
	smtp_toaddr = ''.join([ s if s not in splitat else ' ' for s in config.get('LogMail', 'To Address')]).split()
	smtp_subject = config.get('LogMail', 'Subject')
	smtp_username = config.get('LogMail', 'Username')
	smtp_password = config.get('LogMail', 'Password')

except ConfigParser.Error, err:
	instek_logger.error('When we read log mail configurations. %s', str(err))
	pass
except:
	instek_logger.error('When we read log mail configurations. Unexpected error: %s', sys.exc_info()[0])
	pass

if smtp_port != 0:
	smtp_mail_host = (smtp_server, smtp_port)
else:
	smtp_mail_host = smtp_server

# SMTP default port = 25, default SSL port = 465
smtp_handler = loggingplugin.BufferingSMTPHandler(32768, smtp_mail_host, smtp_fromaddr, smtp_toaddr, smtp_subject, smtp_is_ssl, (smtp_username, smtp_password))
smtp_handler.setLevel(logging.ERROR)
smtp_handler.setFormatter(file_formatter)
instek_logger.addHandler(smtp_handler)

################################################
# Lock for single instance (non Windows version).
lockfile = os.path.normpath(tempfile.gettempdir() + '/' + os.path.splitext(os.path.basename(__file__))[0] + '.lock')

class SingleInstanceObject:
	"""This object is used for preventing multiple instace running at the same time."""

	def __init__(self, filename):
		"""
		The constructor.
		@param The object pointer.
		@param filename The filename of lock file.
		"""
		self.fp = None
		self.fp = open(filename, 'w')
		# For this lock file can be opened by other users.
		try:
			os.chmod(filename, stat.S_IRUSR | stat.S_IWUSR | stat.S_IRGRP | stat.S_IWGRP | stat.S_IROTH | stat.S_IWOTH)
		except:
			instek_logger.info('When chmod file %s. Unexpected error: %s', filename, sys.exc_info()[0])

		try:
			fcntl.lockf(self.fp, fcntl.LOCK_EX | fcntl.LOCK_NB)
		except IOError, err:
			instek_logger.warning('Another instance is already running, quitting. %s', str(err))
			sys.exit(0)

	def __del__(self):
		"""
		The destroyer.
		@param The object pointer.
		"""
		if self.fp is not None:
			self.fp.close()

try:
	single_instance = SingleInstanceObject(lockfile)
except SystemExit:
	sys.exit(0)
except IOError, err:
	instek_logger.error('When open lock file %s. %s', lockfile, str(err))
	sys.exit(1)
except:
	instek_logger.error('When open lock file %s. Unexpected error: %s', lockfile, sys.exc_info()[0])
	sys.exit (1)

################################################
# For Alarm
try:
	instek_logger.info('Read alarm configurations.')

	config = ConfigParser.RawConfigParser()
	config.read(path_of_configuration_file_for_this_script)

	# Alarm
	alarm_remote_ip = "127.0.0.1"
	alarm_control_port = config.get('Alarm', 'Control Port')
	alarm_username = config.get('Alarm', 'Username')
	alarm_password = config.get('Alarm', 'Password')
	alarm_server_url = 'https://' + alarm_remote_ip + ':' + alarm_control_port

except ConfigParser.Error, err:
	instek_logger.error('When we read alarm configurations. %s', str(err))
	# FixMe: We need to trigger alarm??
	pass
except:
	instek_logger.error('When we read alarm configurations. Unexpected error: %s', sys.exc_info()[0])
	# FixMe: We need to trigger alarm??
	pass

alarm_proxy = xmlrpclib.ServerProxy(alarm_server_url)

def trigger_alarm(event_subject_arg='DOWNLOADER-127.0.0.1_0_00000000_000000_00000000_000000-ERROR'):
	"""
	The function is used for triggering alarm to HDVR.
	@param event_subject_arg The event subject of alarm.
	"""
	try:
		instek_logger.info('Trigger event: event subject = %s.', event_subject_arg)
		#session_id = alarm_proxy.server.login(alarm_username, alarm_password, "client")
		alarm_proxy.event.triggerSysAlarm(event_subject_arg, 1)
	except xmlrpclib.Fault, err:
		instek_logger.critical('When we trigger event: event subject = %s. Fault %d: %s', event_subject_arg, err.faultCode, err.faultString)
	except xmlrpclib.ProtocolError, err:
		instek_logger.critical('Protocol error of alarm. URL: %s, HTTP/HTTPS headers: %s, Error code: %d, Error message: %s', err.url, err.headers, err.errcode, err.errmsg)
	except:
		instek_logger.critical('When we trigger event: event subject = %s. Unexpected error: %s', event_subject_arg, sys.exc_info()[0])

################################################
instek_logger.info('Start Processing...')

# Load or get some variables for configuration.
try:
	instek_logger.info('Read or get configurations.')

	config = ConfigParser.RawConfigParser()
	config.read(path_of_configuration_file_for_this_script)

	# Downloader
	downloader = config.get('Downloader', 'Application')
	remote_ip = subprocess.check_output("/sbin/ifconfig eth0 | grep \"inet addr\" | awk -F: \'{ print $2 }\' | awk \'{ print $1 }\'", shell=True).strip()
	#remote_ip = "127.0.0.1"
	playback_port = config.getint('Downloader', 'Playback Port')
	if playback_port < 0:
		instek_logger.warning('The playback port = %d is invalid. We change it to 60006.', playback_port)
		playback_port = 60006

	#iavf_output_path = config.get('Downloader', 'IAVF Output Folder') # We comment out this line because we use NFS mount point now.
	idb_output_path = config.get('Downloader', 'IDB Output Folder')
	split_file_size = config.getint('Downloader', 'Max Output File Size (bytes)')
	if split_file_size < 0:
		instek_logger.warning('The split file size = %d is invalid. We change it to 104857600.', split_file_size)
		split_file_size = 104857600

	time_zone = config.getint('Downloader', 'Time Zone')
	if (time_zone < -12) or (time_zone > 14):
		instek_logger.warning('The time zone = %d is invalid. We change it to 0.', time_zone)
		time_zone = 0

	remote_username = config.get('Downloader', 'Username')
	remote_password = config.get('Downloader', 'Password')

	# Remote Datebase.
	datebase_host = config.get('Database', 'Host')
	datebase_username = config.get('Database', 'Username')
	datebase_password = config.get('Database', 'Password')
	datebase_name = config.get('Database', 'Database Name')

	# Mount Point of NFS.
	mount_point = config.get('NFS', 'Mount Point')

	# Configuration files.
	event_name_file = config.get('Event Name List', 'Filename')

	# Reserved days
	reserved_days = config.getint('Recycle Mechanism', 'Reserved Days')
	if reserved_days < 1:
		instek_logger.warning('The reserved days = %d is invalid. We change it to 1.', reserved_days)
		reserved_days = 1

	# Reserved days for the limitation of available space of storage.
	reserved_days_for_not_enough_space = config.getint('Recycle Mechanism', 'Minimum Reserved Days When Space Of Storage Is Not Enough')
	if reserved_days_for_not_enough_space < 1:
		instek_logger.warning('The minimum reserved days = %d for space of storage is not enough is invalid. We change it to 1.', reserved_days_for_not_enough_space)
		reserved_days_for_not_enough_space = 1

	# The minimum available size of mount point.
	min_available_mount_point_size = config.getint('Recycle Mechanism', 'Minimum Limitation Of Available Storage Size (KB)')
	if min_available_mount_point_size < 1:
		instek_logger.warning('The minimum limitation of available storage size = %d KB is invalid. We change it to 1048576 KB.', min_available_mount_point_size)
		min_available_mount_point_size = 1048576

	# Which seconds ago event we need to check
	seconds_ago_event = config.getint('Backup Mechanism', 'Check Seconds Ago Event')
	if seconds_ago_event < 1:
		instek_logger.warning('The check seconds ago event = %d is invalid. We change it to 6.', seconds_ago_event)
		seconds_ago_event = 6

	# When we download data and errors occur, how many times we will retry to download data.
	retry_times_for_download = config.getint('Backup Mechanism', 'Retry Times')
	if retry_times_for_download < 0:
		instek_logger.warning('The retry times = %d is invalid. We change it to 0.', retry_times_for_download)
		retry_times_for_download = 0

	# The filename of the file which to record the last check datetime of event.
	last_check_event_datetime_filename = 'last_check_event_datetime.txt'

	# The date for removing overdue data and removing old files when available space is not enough.
	min_date_for_check = datetime.date(2000, 1, 1)

except ConfigParser.Error, err:
	instek_logger.error('When we read configurations. %s', str(err))
	trigger_alarm()
	sys.exit (1)
except ValueError, err:
	instek_logger.error('When we convert some value from configurations. %s', str(err))
	trigger_alarm()
	sys.exit (1)
except subprocess.CalledProcessError, err:
	instek_logger.error('When we try to get remote IP. %s', str(err))
	trigger_alarm()
	sys.exit (1)
except:
	instek_logger.error('When we read configurations. Unexpected error: %s', sys.exc_info()[0])
	trigger_alarm()
	sys.exit (1)

################################################

# Check NFS mount point.
def check_mount_point(mount_point_arg):
	"""
	The function to check the NFS mount point.
	@param mount_point_arg The path of mount point of NFS.
	@return True: Success. False: Failed.
	"""
	temp_ret = False;
	try:
		instek_logger.info('Check NFS mount point: %s', mount_point_arg)
		check_mount_out = subprocess.check_output("mount -l -t nfs,nfs4 | grep -w " + mount_point_arg, shell=True).strip()
		if len(check_mount_out) <= 0:
			return temp_ret

		temp_ret = True
	except subprocess.CalledProcessError, err:
		instek_logger.error('The NFS does not be mounted. %s', str(err))
	except:
		instek_logger.error('When we check NFS mount point %s. Unexpected error: %s', mount_point_arg, sys.exc_info()[0])

	return temp_ret;

# Check the free space of mount point.
def check_free_space(mount_point_arg, limited_min_space_arg):
	"""
	The function to check the free space of mount point.
	@param mount_point_arg The path of mount point of NFS.
	@param limited_min_space_arg The limited free space size (KB)
	@return [first_element, second_element], first_element => True: Success. False: Failed. second_element => True: It has more space than limited space. False: The space is not enough.
	"""
	temp_ret = [False, False];
	try:
		instek_logger.info('Check available space of mount point: %s', mount_point_arg)
		free_space_out = subprocess.check_output("df -P | grep -w " + mount_point_arg + ' | awk \'{print $4}\'', shell=True).strip()
		# The NFS may not be mounted.
		if len(free_space_out) <= 0:
			instek_logger.info('The NFS may not be mounted.')
			return temp_ret

		instek_logger.info('The limitation of available space: %d. The available space: %d.', int(limited_min_space_arg), int(free_space_out))
		if int(limited_min_space_arg) < int(free_space_out):
			temp_ret[1] = True

		temp_ret[0] = True
	except subprocess.CalledProcessError, err:
		instek_logger.error('Can not check the available space of mount point. %s', str(err))
	except:
		instek_logger.error('When we check the available space of mount point %s. Unexpected error: %s', mount_point_arg, sys.exc_info()[0])

	return temp_ret;

# Remove overdue data (month or day).
def remove_overdue_data(reserved_days_arg, parent_folder_arg, min_check_date):
	"""
	The function to remove the overdue data.
	@param reserved_days_arg The reserved days of the data.
	@param parent_folder_arg The parent path of the folder which these data are stored.
	@param min_check_date The minimum date for checking (for performance reason).
	"""
	utc_today = datetime.datetime.utcnow().date()
	overdue_date = utc_today - datetime.timedelta(days=reserved_days_arg)
	temp_org_overdue_date = overdue_date
	min_overdue_date = min_check_date

	while(overdue_date >= min_overdue_date):
		overdue_day_folder = parent_folder_arg + overdue_date.strftime("/%Y-%m/%d")
		if os.path.exists(overdue_day_folder):
			try:
				instek_logger.info('Remove overdue data => Remove day folder %s', overdue_day_folder)
				shutil.rmtree(overdue_day_folder)
			except OSError, err:
				instek_logger.error('When remove folder %s. %s', overdue_day_folder, str(err))
			except:
				instek_logger.error('When remove folder %s. Unexpected error: %s', overdue_day_folder, sys.exc_info()[0])

		if (overdue_date.day == 1) or (overdue_date == min_overdue_date):
			overdue_month_folder = parent_folder_arg + overdue_date.strftime("/%Y-%m")
			if os.path.exists(overdue_month_folder):
				try:
					if (overdue_date.year != temp_org_overdue_date.year) or (overdue_date.month != temp_org_overdue_date.month) or \
							(temp_org_overdue_date.day == calendar.monthrange(temp_org_overdue_date.year, temp_org_overdue_date.month)[1]):
						instek_logger.info('Remove overdue data => Remove month folder %s', overdue_month_folder)
						shutil.rmtree(overdue_month_folder)
				except OSError, err:
					instek_logger.error('When remove folder %s. %s', overdue_month_folder, str(err))
				except:
					instek_logger.error('When remove folder %s. Unexpected error: %s', overdue_month_folder, sys.exc_info()[0])

		overdue_date -= datetime.timedelta(days=1)

	return;

# Remove old data (month or day).
def remove_old_data_for_available_space(reserved_days_arg, parent_folder_arg, min_check_date, mount_point_arg, limited_min_space_arg):
	"""
	The function to remove the old data when the space of storage is not enough.
	@param reserved_days_arg The reserved days of the data.
	@param parent_folder_arg The parent path of the folder which these data are stored.
	@param min_check_date The minimum date for checking (for performance reason).
	@param mount_point_arg The path of mount point of NFS.
	@param limited_min_space_arg The limited free space size (KB)
	"""
	utc_today = datetime.datetime.utcnow().date()
	delete_date_upper_bound = utc_today - datetime.timedelta(days=reserved_days_arg)
	delete_date_lower_bound = min_check_date
	temp_org_delete_date_lower_bound = min_check_date
	is_removed = False

	# We check the available space first.
	temp_free_space_ret = check_free_space(mount_point_arg, limited_min_space_arg)
	# If we can not check the free space or the free space is large than the minimum limitation then we break the loop.
	if (temp_free_space_ret[0] == False) or (temp_free_space_ret[1] == True):
		return

	# Delete the old data until the available space is enough or we encounter the reserved days for checking the available space.
	while(delete_date_lower_bound <= delete_date_upper_bound):
		delete_day_folder = parent_folder_arg + delete_date_lower_bound.strftime("/%Y-%m/%d")
		if os.path.exists(delete_day_folder):
			try:
				instek_logger.info('Remove old data => Remove day folder %s', delete_day_folder)
				shutil.rmtree(delete_day_folder)
				is_removed = True
			except OSError, err:
				instek_logger.error('When remove folder %s. %s', delete_day_folder, str(err))
			except:
				instek_logger.error('When remove folder %s. Unexpected error: %s', delete_day_folder, sys.exc_info()[0])

		if (delete_date_lower_bound.day == calendar.monthrange(delete_date_lower_bound.year, delete_date_lower_bound.month)[1]):
			delete_month_folder = parent_folder_arg + delete_date_lower_bound.strftime("/%Y-%m")
			if os.path.exists(delete_month_folder):
				try:
					instek_logger.info('Remove old data => Remove month folder %s', delete_month_folder)
					shutil.rmtree(delete_month_folder)
					is_removed = True
				except OSError, err:
					instek_logger.error('When remove folder %s. %s', delete_month_folder, str(err))
				except:
					instek_logger.error('When remove folder %s. Unexpected error: %s', delete_month_folder, sys.exc_info()[0])

		delete_date_lower_bound += datetime.timedelta(days=1)

		if is_removed == True:
			is_removed = False
			temp_free_space_ret = check_free_space(mount_point_arg, limited_min_space_arg)
			# If we can not check the free space or the free space is large than the minimum limitation then we break the loop.
			if (temp_free_space_ret[0] == False) or (temp_free_space_ret[1] == True):
				break

	return

# Read the specified event name from file then construct the SQL condition of the event names.
def generate_sql_condition_of_event_name(filename_of_event_name_file_arg):
	"""
	The function to generate SQL condition of specified event name from file.
	@param filename_of_event_name_file_arg The path of the file which store specified event name.
	@return The string of SQL condition.
	"""
	# Read event name from configuration file.
	event_name_list = []
	try:
		instek_logger.info('Read event names from file %s', filename_of_event_name_file_arg)
		with open(filename_of_event_name_file_arg, "r") as fileobj:
			event_name_list = fileobj.read().splitlines()
	except IOError, err:
		instek_logger.error('When read event names from file %s. %s', filename_of_event_name_file_arg, str(err))
	except:
		instek_logger.error('When read event names from file %s. Unexpected error: %s', filename_of_event_name_file_arg, sys.exc_info()[0])

	# Construct the condition of event name for SQL.
	sql_event_name_condition = ""
	i = 0
	for temp_event in event_name_list:
		if temp_event != "":
			if i != 0:
				sql_event_name_condition += (" OR event_name = \"" + temp_event + "\"")
			else:
				sql_event_name_condition = ("event_name = \"" + temp_event + "\"")
			i = i + 1

	if sql_event_name_condition != "":
		sql_event_name_condition = "(" + sql_event_name_condition + ")"

	return sql_event_name_condition;


# Read the old event time from file then construct the SQL condition of the min and max event time.
def generate_sql_condition_of_event_time(last_check_event_datetime_filename_arg, seconds_ago_event_arg):
	"""
	The function to generate SQL condition of last check event time from file and new event time.
	@param last_check_event_datetime_filename_arg The path of the file which store the last check event time.
	@param seconds_ago_event_arg The specified seconds ago time from current time.
	@return The string of SQL condition.
	"""
	# Construct the min and max check event time.

	# Get min check event time.
	min_check_event_datetime = datetime.datetime(1900, 1, 1)
	try:
		instek_logger.info('Read old last check event time from file %s', last_check_event_datetime_filename_arg)
		with open(last_check_event_datetime_filename_arg, "r") as fileobj:
			event_time_list = fileobj.read().splitlines()
			if len(event_time_list) > 0:
				temp_datetime = datetime.datetime.strptime(event_time_list[0], "%Y-%m-%d %H:%M:%S")
				if temp_datetime > min_check_event_datetime:
					min_check_event_datetime = temp_datetime
	except IOError, err:
		instek_logger.warning('When read event time from file %s. %s', last_check_event_datetime_filename_arg, str(err))
	except ValueError, err:
		instek_logger.error('The time format in the file %s is incorrect. %s', last_check_event_datetime_filename_arg, str(err))
	except:
		instek_logger.error('When read event time from file %s. Unexpected error: %s', last_check_event_datetime_filename_arg, sys.exc_info()[0])

	# Get max check event time and write to file.
	instek_logger.info('Get new last check event time')
	max_check_event_datetime = datetime.datetime.utcnow() - datetime.timedelta(seconds=seconds_ago_event_arg)

	if min_check_event_datetime > max_check_event_datetime:
		instek_logger.error('The min and max check event datatime is invalid. min_check_event_datetime = %s, max_check_event_datetime = %s', min_check_event_datetime.strftime("%Y-%m-%d %H:%M:%S"), max_check_event_datetime.strftime("%Y-%m-%d %H:%M:%S"))
		return "";

	try:
		instek_logger.info('Write new last check event time %s to file %s', max_check_event_datetime.strftime("%Y-%m-%d %H:%M:%S"), last_check_event_datetime_filename_arg)
		with open(last_check_event_datetime_filename_arg, "w") as fileobj:
			fileobj.write(max_check_event_datetime.strftime("%Y-%m-%d %H:%M:%S"))
	except IOError, err:
		instek_logger.error('When write new last check event time to file %s. %s', last_check_event_datetime_filename_arg, str(err))
		return "";
	except:
		instek_logger.error('When write last check event time to file %s. Unexpected error: %s', last_check_event_datetime_filename_arg, sys.exc_info()[0])
		return "";


	# Construct the condition of event time for SQL.
	sql_event_time_condition = "(event_time > \"" + min_check_event_datetime.strftime("%Y-%m-%d %H:%M:%S") + "\" and event_time <= \"" + max_check_event_datetime.strftime("%Y-%m-%d %H:%M:%S") + "\")"
	return sql_event_time_condition;


# Generate complete SQL query command.
def generate_sql_query_cmd(sql_event_name_condition_arg, sql_event_time_condition_arg):
	"""
	The function to generate completed SQL query command.
	@param sql_event_name_condition_arg The SQL condition of event name.
	@param sql_event_time_condition_arg The SQL condition of event time.
	@return The completed SQL query command string.
	"""
	sql_query_cmd = "SELECT ChannelID, StartTime, EndTime FROM (SELECT ClipID, ChannelID, StartTime, EndTime FROM AVClip UNION SELECT ClipID, ChannelID, StartTime, EndTime FROM RecentAVClip) AS AVClipsTable WHERE ClipID IN (SELECT clip_id FROM EventClip WHERE event_his_id IN (SELECT event_his_id FROM EventHistory WHERE " + sql_event_time_condition_arg + " and " + sql_event_name_condition_arg + "))"
	return sql_query_cmd;


# Create sub directory if directory does not exist.
def create_sub_directory(dir_name_arg):
	"""
	The function to create sub-directory if the directory does not exist.
	@param dir_name_arg The path of the sub-directory which we want to create.
	@return True: The sub-directory is created. False: The sub-directory is not created or there are some errors occur.
	"""
	if os.path.exists(dir_name_arg):
		if os.path.isdir(dir_name_arg):
			return True;
		else:
			return False;

	try:
		instek_logger.info('Create folder %s', dir_name_arg)
		os.mkdir(dir_name_arg)
		return True;
	except OSError, err:
		instek_logger.error('When create folder %s. %s', dir_name_arg, str(err))
	except:
		instek_logger.error('When create folder %s. Unexpected error: %s', dir_name_arg, sys.exc_info()[0])

	# Note: If the directory exists and we try to create it. The mkdir function will throw OSError exception.
	# We can return False here because we check whether the directory exists first.
	return False;


# Launch the downloader.
def launch_downloader_to_run(download_cmd_arg):
	"""
	Function to launcher downloader to run.
	@param download_cmd_arg The command to launch downloader.
	@return True: The downloader is launched and it runs successfully. False: There are some errors occur.
	"""
	ret_value = -10009
	download_times = retry_times_for_download + 1
	temp_output = ('', '')
	try:
		for i in range(download_times):
			# Start download
			instek_logger.info('%d times to retry for downloading.', i)
			temp_args = shlex.split(download_cmd_arg)
			child_p = subprocess.Popen(args=temp_args, stderr=subprocess.PIPE)
			temp_output = child_p.communicate()
			ret_value = child_p.returncode

			if (len(temp_output[1]) == 0) and (ret_value == 0):
				break

		# Get the stderr of subprocess
		if len(temp_output[1]) > 0:
			instek_logger.error('%s', temp_output[1])

		# The returncode of subprocess in the python is 0~255 when it succeed.
		instek_logger.info('return value = %d', ret_value)

	except ValueError, err:
		instek_logger.error('When launch downloader. %s', str(err))
	except OSError, err:
		instek_logger.error('When launch downloader. %s', str(err))
	except:
		instek_logger.error('When launch downloader. Unexpected error: %s', sys.exc_info()[0])
	finally:
		if ret_value != 0:
			if ret_value == -10009:
				instek_logger.error('Error occurs when we launch the downloader in the script. The command is %s', download_cmd)
			else:
				instek_logger.error('We failed to download. Return code = %d. The command is %s', ret_value, download_cmd)
			return False;
	return True;


################################################
# Check NFS mount point.
if check_mount_point(mount_point) == False:
	trigger_alarm()
	sys.exit (1)

# Remove overdue data.
remove_overdue_data(reserved_days, mount_point, min_date_for_check)

# Generate the query command of SQL.
sql_event_name_condition = generate_sql_condition_of_event_name(event_name_file)
if sql_event_name_condition == "":
	instek_logger.error('There is no any specified event name in the configuration file %s. We do not download anything.', event_name_file)
	trigger_alarm()
	sys.exit (1)

temp_last_check_event_datetime_filename = mount_point + "/" + last_check_event_datetime_filename
sql_event_time_condition = generate_sql_condition_of_event_time(temp_last_check_event_datetime_filename, seconds_ago_event)
if sql_event_time_condition == "":
	instek_logger.error('Invalid datetime for checking event or we can not write new last check event time to file. We do not download anything.')
	trigger_alarm()
	sys.exit (1)

sql_query_cmd = generate_sql_query_cmd(sql_event_name_condition, sql_event_time_condition)
#sql_query_cmd = "Select ChannelID, StartTime, EndTime FROM AVSlot Where State = 2 Or State = 3"
instek_logger.info(sql_query_cmd)


try:
	# Connect to database and get the rows those need to be downloaded
	dbconn = MySQLdb.connect(host=datebase_host, user=datebase_username, passwd=datebase_password, db=datebase_name)
	cursor = dbconn.cursor(MySQLdb.cursors.DictCursor)
	cursor.execute(sql_query_cmd)
	num_of_rows = int(cursor.rowcount)

	instek_logger.info('num_of_rows = %d', num_of_rows)

	for i in range(num_of_rows):
		record = cursor.fetchone()
		#print record

		temp_channel_id = record["ChannelID"]
		temp_start_time = record["StartTime"]
		temp_end_time = record["EndTime"]

		#print time.mktime(temp_start_time.timetuple()) # local time
		#print calendar.timegm(temp_start_time.timetuple()) # GMT

		# Construct the output sub-folder. (FixMe: Use start time or end time to create output folder??)
		year_month_subfolder = mount_point + "/" + temp_start_time.strftime("%Y-%m")
		iavf_output_path = year_month_subfolder + "/" + temp_start_time.strftime("%d")

		# Create year and month sub-folder
		if create_sub_directory(year_month_subfolder) == False:
			trigger_alarm()
			continue;

		# Create day sub-folder
		if create_sub_directory(iavf_output_path) == False:
			trigger_alarm()
			continue;

		# Construct the output filename
		temp_filename = "IP_" + remote_ip + "_Camera_" + str(temp_channel_id) + "_Time" + temp_start_time.strftime("%Y%m%d-%H%M%S") + "_" + temp_end_time.strftime("%Y%m%d-%H%M%S")

		# Calculate the epoch time of start and end time
		start_time = calendar.timegm(temp_start_time.timetuple())
		end_time = calendar.timegm(temp_end_time.timetuple())

		# Construct the download command
		download_cmd = downloader + " " + remote_ip + " " + str(playback_port) + " " + str(temp_channel_id) + " " + str(start_time) + " " + str(end_time) + " " + iavf_output_path + " " + idb_output_path + " " + temp_filename + " " + str(split_file_size) + " " + str(time_zone) + " " + remote_username + " " + remote_password

		instek_logger.info('%s', download_cmd)

		# Start downloading.
		if launch_downloader_to_run(download_cmd) == False:
			temp_event_subject = 'DOWNLOADER-' + remote_ip + '_' + str(temp_channel_id) + '_' + temp_start_time.strftime("%Y%m%d_%H%M%S") + '_'+ temp_end_time.strftime("%Y%m%d_%H%M%S") + '-ERROR'
			temp_free_space_ret = check_free_space(mount_point, min_available_mount_point_size)

			if (temp_free_space_ret[0] == True) and (temp_free_space_ret[1] == False):
				instek_logger.error('The available space of mount point %s may be not enough. We delete some old files then retry command %s', mount_point, download_cmd)

				instek_logger.info('Start delete old files.')
				remove_old_data_for_available_space(reserved_days_for_not_enough_space, mount_point, min_date_for_check, mount_point, min_available_mount_point_size)

				instek_logger.info('Retry download after deleting old files.')
				if launch_downloader_to_run(download_cmd) == False:
					trigger_alarm(temp_event_subject)
			else:
				trigger_alarm(temp_event_subject)

		instek_logger.info('End of command = %s', download_cmd)

except MySQLdb.Error, err:
	instek_logger.error('When we use MySQL. %s', str(err))
	trigger_alarm()
	sys.exit (1)
except:
	instek_logger.error('When we use MySQL. Unexpected error: %s', sys.exc_info()[0])
	trigger_alarm()
	sys.exit (1)
finally:
	if cursor:
		cursor.close()
	if dbconn:
		dbconn.close()

# If there is no anything in the idb folder then we remove the folder.
try:
	if os.path.exists(idb_output_path):
		os.rmdir(idb_output_path)
except OSError, err:
	instek_logger.error('We try to remove temporary folder. %s', str(err))
	trigger_alarm()
	sys.exit (1)
except:
	instek_logger.error('When we try to remove temporary folder. Unexpected error: %s', sys.exc_info()[0])
	trigger_alarm()
	sys.exit (1)

sys.exit (0)

