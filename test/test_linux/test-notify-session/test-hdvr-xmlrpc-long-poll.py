#!/usr/bin/python

import sys
import xmlrpclib

conn = xmlrpclib.ServerProxy("https://172.16.3.4:60009")

try:
	session_id = conn.server.login("admin", "id", "client")
	result = conn.server.getProps("cam:*:name cam:*:status", True, session_id)
	print result, '\n'

	timeout_sec = 30

	while True:
		result = conn.server.notify(timeout_sec, session_id)
		print result, '\n'

except xmlrpclib.Fault, err:
	print 'Fault ', err.faultCode, ': ', err.faultString
	sys.exit(1)


sys.exit(0)


