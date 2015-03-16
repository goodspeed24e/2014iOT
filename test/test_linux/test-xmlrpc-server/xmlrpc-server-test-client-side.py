import xmlrpclib;
import sys;
import datetime;

if len(sys.argv) != 3:
	print('Usage: {0} <Socket mode>(0: TCP, 1: SSL) <StopServer>(0: false, 1: true)'.format(sys.argv[0]));
	sys.exit(0);

if int(sys.argv[1]) == 0:
	proxy=xmlrpclib.ServerProxy("http://127.0.0.1:60000");
elif int(sys.argv[1]) == 1:
	proxy=xmlrpclib.ServerProxy("https://127.0.0.1:60000");
else:
	print('Usage: Unknown socket mode {0}'.format(sys.argv[1]));
	print('Use 0 for TCP, 1 for SSL');
	sys.exit(0);


print proxy.example.TestInt(45);
print proxy.example.TestBool(False);
print proxy.example.TestString('C++');
print proxy.example.TestDouble(0.93);
temp_struct = dict(string = 'test String', double = 0.375, integer = 12345, boolean = True, struct = dict(Apple = 'steve jobs', OMG = False));
print proxy.example.TestStruct(temp_struct);
print proxy.example.TestTwoParams(54.96, 14);

list1 = [];
list1.append(95);
list1.append(True);
list1.append('Pig');
list1.append(-0.96);
list1.append(dict(camera1 = 'dog', camera2 = 'cat'));
list2 = [1234, 'one two three four'];
list1.append(list2);
print proxy.example.TestArray(list1);


print proxy.system.listMethods();

print proxy.system.methodSignature('example.TestTwoParams');
print proxy.system.methodSignature('system.methodSignature');
print proxy.system.methodSignature('example.TestXMLRPCDateTime');

print proxy.system.methodHelp('example.TestString');
print proxy.system.methodHelp('system.methodHelp');

# Test Error and private etc.
#print proxy.system.methodSignature('example.Undefined');
#print proxy.system.methodSignature('example.UnPublic');
#print proxy.system.methodSignature();
#print proxy.system.methodHelp('example.Undefined');
#print proxy.system.methodHelp('example.UnPublic');
#print proxy.system.methodHelp();

try:
	print proxy.example.TestThrowFault();
except xmlrpclib.Fault, err:
	print "A fault occurred"
	print "Fault code: %d" % err.faultCode
	print "Fault string: %s" % err.faultString

print proxy.example.TestEmptyStruct();
print proxy.example.TestEmptydArray();


temp_struct2 = dict(string = 'test complicated struct', struct = dict(string = 'struct in struct', Array = ['array in struct', dict(string = 'struct in array'), ['array in array', 75] ]));
temp_list2 = ['test complicated array', ['array in array', dict(string = 'struct in array', Array = ['array in struct'], Struct = dict(string = 'struct in struct'))]];
print proxy.example.TestComplicatedStruct(temp_struct2);
print proxy.example.TestComplicatedArray(temp_list2);
print proxy.example.TestAddValue();

today = datetime.datetime.today();
ret_today = proxy.example.TestXMLRPCDateTime(xmlrpclib.DateTime(today));
# convert the ISO8601 string to a datetime object
converted = datetime.datetime.strptime(ret_today.value, "%Y%m%dT%H:%M:%S");
print converted;

print proxy.example.TestGetConnectionInfo();

if int(sys.argv[2]) != 0:
	print proxy.example.StopServer();

