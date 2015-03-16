#!/usr/bin/python

from __future__ import with_statement
import sys
import os

default_path = '/usr/local/id/streaming_pb_downloader/current/configs'
default_filename = 'event_name_list.txt'

if len(sys.argv) > 2:
	print('Usage:\n{0}\nor\n{0} output_path'.format(sys.argv[0]))
	sys.exit(0)

if len(sys.argv) == 2:
	default_path=sys.argv[1]

output_file = os.path.join(os.path.normpath(default_path), default_filename)

with open(output_file, 'w') as fileobj:
	fileobj.write('ID-MOTION\nID-TAMPERING\nID-DI-ON\nID-DI-OFF\nID-SYS\nID-VLOSS\n')

print 'The default event name list is generated in ', output_file

