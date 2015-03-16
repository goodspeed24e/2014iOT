import xmlrpclib;
import sys;

if len(sys.argv) != 3:
	print('Usage: {0} <Socket mode>(0: TCP, 1: SSL) <StopServer>(0: false, 1: true)'.format(sys.argv[0]));
	sys.exit(0);

if int(sys.argv[1]) == 0:
	proxy=xmlrpclib.ServerProxy("http://172.16.3.41:10001");
elif int(sys.argv[1]) == 1:
	proxy=xmlrpclib.ServerProxy("https://172.16.3.41:10001");
else:
	print('Usage: Unknown socket mode {0}'.format(sys.argv[1]));
	print('Use 0 for TCP, 1 for SSL');
	sys.exit(0);

print proxy.Login('admin', 'id');

