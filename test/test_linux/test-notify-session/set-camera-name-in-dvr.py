import xmlrpclib;
import sys;

if len(sys.argv) != 4:
	print('Usage: {0} <server URL> <camera ID> <camera name>'.format(sys.argv[0]));
	print('ex: {0} "https://127.0.0.1:60009" "1" "cam-01 aa"'.format(sys.argv[0]))
	sys.exit(0);

proxy=xmlrpclib.ServerProxy(sys.argv[1]);
session_id=proxy.server.login("admin", "id", "client");
temp_prop = { 'cam' : { sys.argv[2] : { 'name' : sys.argv[3]} } };
proxy.server.setProps(temp_prop, session_id);

