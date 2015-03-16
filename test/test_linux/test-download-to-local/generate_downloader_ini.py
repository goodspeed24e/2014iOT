#!/usr/bin/python

from __future__ import with_statement
import ConfigParser
import sys
import os

default_path = '/usr/local/id/streaming_pb_downloader/current/configs'
default_filename = 'downloader.ini'

if len(sys.argv) > 2:
	print('Usage:\n{0}\nor\n{0} output_path'.format(sys.argv[0]))
	sys.exit(0)

if len(sys.argv) == 2:
	default_path=sys.argv[1]

output_file = os.path.join(os.path.normpath(default_path), default_filename)

config = ConfigParser.RawConfigParser()

config.add_section('LogMail')
config.set('LogMail', 'Mail Server', 'mail.instekdigital.com')
config.set('LogMail', 'Port', '465')
config.set('LogMail', 'Is SSL', 'True')
config.set('LogMail', 'From Address', 'test1@instekdigital.com')
config.set('LogMail', 'To Address', 'test1@instekdigital.com,test2@instekdigital.com')
config.set('LogMail', 'Subject', 'Error Log for streaming_pb_downloader')
config.set('LogMail', 'Username', 'test1@instekdigital.com')
config.set('LogMail', 'Password', '')

config.add_section('Downloader')
#config.set('Downloader', 'Application', './DownloadIavf_NoAutoCreateIAVFFolder')
config.set('Downloader', 'Application', '/usr/local/id/streaming_pb_downloader/current/bin/streaming_pb_downloader')
config.set('Downloader', 'Playback Port', '60006')
#config.set('Downloader', 'IAVF Output Folder', '/var/lib/hdvr/repos/NFS')
config.set('Downloader', 'IDB Output Folder', '/dev/shm/output_idb')
config.set('Downloader', 'Max Output File Size (bytes)', '104857600')
config.set('Downloader', 'Time Zone', '8')
config.set('Downloader', 'Username', 'admin')
config.set('Downloader', 'Password', 'id')

config.add_section('Alarm')
config.set('Alarm', 'Control Port', '60009')
config.set('Alarm', 'Username', 'admin')
config.set('Alarm', 'Password', 'id')

config.add_section('Database')
config.set('Database', 'Host', 'localhost')
config.set('Database', 'Username', 'root')
config.set('Database', 'Password', '')
config.set('Database', 'Database Name', 'ApolloDB')

config.add_section('NFS')
config.set('NFS', 'Mount Point', '/var/lib/hdvr/repos/NFS')

config.add_section('Event Name List')
#config.set('Event Name List', 'Filename', './event_name_list.txt')
config.set('Event Name List', 'Filename', '/usr/local/id/streaming_pb_downloader/current/configs/event_name_list.txt')

config.add_section('Recycle Mechanism')
config.set('Recycle Mechanism', 'Reserved Days', '14')
config.set('Recycle Mechanism', 'Minimum Reserved Days When Space Of Storage Is Not Enough', '7')
config.set('Recycle Mechanism', 'Minimum Limitation Of Available Storage Size (KB)', '1048576')

config.add_section('Backup Mechanism')
config.set('Backup Mechanism', 'Check Seconds Ago Event', '6')
config.set('Backup Mechanism', 'Retry Times', '2')

#with open('downloader.ini', 'wb') as configfile:
with open(output_file, 'wb') as configfile:
	config.write(configfile)

print 'The configurate file is generated in ', output_file

