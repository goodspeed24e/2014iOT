#include <iostream>
#include <NotificationCenter/NotificationCenter.hpp>
#include <boost/bind.hpp>
#include <cstdio>
#include <boost/thread.hpp>

#include <deque>

using namespace std;

boost::any test_int_func(const boost::any& arg)
{
	std::cout << "test_int: " << boost::any_cast<int>(arg) << std::endl;
	return -1;
}

boost::any test_int_func2(const boost::any& arg)
{
	std::cout << "test_int2: " << boost::any_cast<int>(arg) << std::endl;
	return -2;
}

boost::any test_int_ptr_func(const boost::any& arg)
{
	int* temp_int_ptr = boost::any_cast<int*>(arg);
	std::cout << "test_int_ptr: " << *temp_int_ptr << std::endl;
	*temp_int_ptr = 77;
	return -3;
}

boost::any test_thread_blocking_func(const boost::any& arg)
{
	std::cout << "test_thread_blocking_func: " << boost::any_cast<int>(arg) << std::endl;
	boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
	std::cout << "exit test_thread_blocking_func =======" << std::endl;
	return -4;
}

boost::any test_thread_nonblocking_func(const boost::any& arg)
{
	std::cout << "test_thread_nonblocking_func: " << boost::any_cast<int>(arg) << std::endl;
	boost::this_thread::sleep(boost::posix_time::milliseconds(4000));
	std::cout << "exit test_thread_nonblocking_func =======" << std::endl;
	return -5;
}

boost::any test_deque_func(const boost::any& arg)
{
	std::deque<int> vec = boost::any_cast< std::deque<int> >(arg);

	for(size_t i = 0; i < vec.size(); ++i)
	{
		std::cout << "test_deque_func: " << vec[i] << std::endl;
	}
	return -6;
}

int main()
{
	instek::NotificationCenter mgr;

	instek::NotificationSignalPtr sig = mgr.RegisterSignal("test_int");
	instek::NotificationSignalPtr sig2 = mgr.RegisterSignal("test_int_ptr");
	instek::NotificationSignalPtr sig3 = mgr.RegisterSignal("test_deque");

	//typedef boost::any (*FUNC_PTR)(const boost::any& arg);
	//FUNC_PTR func_ptr = NULL;
	//instek::NotificationConnection con1 = mgr.Connect("test_int", boost::bind(func_ptr, _1), instek::NotificationCenter::SyncCall);

	instek::NotificationConnection con5 = mgr.Connect("test_int", boost::bind(&test_thread_nonblocking_func, _1), instek::NotificationCenter::NonBlockingThreadCall);
	instek::NotificationConnection con1 = mgr.Connect("test_int", boost::bind(&test_int_func, _1), instek::NotificationCenter::SyncCall);
	instek::NotificationConnection con2 = mgr.Connect("test_int", boost::bind(&test_int_func2, _1), instek::NotificationCenter::SyncCall);
	instek::NotificationConnection con3 = mgr.Connect("test_int_ptr", boost::bind(&test_int_ptr_func, _1), instek::NotificationCenter::SyncCall);
	instek::NotificationConnection con4 = mgr.Connect("test_int", boost::bind(&test_thread_blocking_func, _1), instek::NotificationCenter::BlockingThreadCall);
	instek::NotificationConnection con6 = mgr.Connect("test_deque", boost::bind(&test_deque_func, _1), instek::NotificationCenter::SyncCall);

	//con1.disconnect();
	//mgr.Disconnect(con1);
	//mgr.UnregisterSignal("test_int");

	deque<int> que;
	que.push_back(43);
	que.push_back(44);
	que.push_back(45);
	(*sig3)(que);

	boost::optional<boost::any> ret = (*sig)(23);
	if(ret)
	{
		boost::any val = ret.get();
		if(val.type() == typeid(int))
			std::cout << "ret = " << boost::any_cast<int>(val) << std::endl;
	}

	int temp_int = 1;
	boost::optional<boost::any> ret2 = (*sig2)(&temp_int);
	if(ret2)
	{
		boost::any val = ret2.get();
		if(val.type() == typeid(int))
			std::cout << "ret2 = " << boost::any_cast<int>(val) << std::endl;
	}
	std::cout << "ref_int = " << temp_int << std::endl;

	std::cout << "main thread sleep!!!!!" << std::endl;
	boost::this_thread::sleep(boost::posix_time::milliseconds(8000));

	std::cout << "main thread exit!!!!!" << std::endl;
	return 0;
}
