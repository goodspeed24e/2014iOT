#!/usr/bin/python

import sys
import xmlrpclib

conn = xmlrpclib.ServerProxy("https://172.16.3.4:60009")

try:
	session_id = conn.server.login("admin", "id", "client")
	result = conn.server.getProps("cam:*:name cam:*:status", False, session_id)
except xmlrpclib.Fault, err:
	print 'Fault ', err.faultCode, ': ', err.faultString
	sys.exit(1)

print result

sys.exit(0)



