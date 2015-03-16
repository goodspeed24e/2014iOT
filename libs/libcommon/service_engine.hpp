#ifndef SERVICE_ENGINE_HPP
#define SERVICE_ENGINE_HPP

#include <libcommon/libcommon_config.h>
#include <boost/aligned_storage.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <vector>

namespace FBi
{

class ServiceContext
{
public:
	boost::asio::io_service::work* work;
	boost::thread* work_thread;
	boost::asio::io_service* io_service;
};

class LIBCOMMON_DECL ServiceEngine
{
public:
	explicit ServiceEngine(int num = 1);
	~ServiceEngine();
	
	boost::asio::io_service& GetIOService();
	void StartService();
	void StopService();
	bool IsRunning();
	
private:
	int RR_index_;
	boost::mutex mutex_;
	std::vector<ServiceContext> context_vec_;
	bool ready_;
};

// Class to manage the memory to be used for handler-based custom allocation.
// It contains a single block of memory which may be returned for allocation
// requests.
//Single Block Allocator
typedef boost::aligned_storage<1024> sb_allocator;

// Wrapper class template for handler objects to allow handler memory
// allocation to be customised. Calls to operator() are forwarded to the
// encapsulated handler.
template <typename Handler>
class custom_alloc_handler
{
public:
	custom_alloc_handler(sb_allocator& a, BOOST_ASIO_MOVE_ARG(Handler) h)
		: allocator_(a),
		handler_(BOOST_ASIO_MOVE_CAST(Handler)(h))
	{
	}
#if defined(BOOST_ASIO_HAS_MOVE)
	custom_alloc_handler(const custom_alloc_handler& other)
		: allocator_(other.allocator_),
		  handler_(other.handler_)
	{
	}

	custom_alloc_handler(custom_alloc_handler&& other)
		: allocator_(other.allocator_),
		  handler_(BOOST_ASIO_MOVE_CAST(Handler)(other.handler_))
	{
	}
#endif

	template <typename Arg1>
	void operator()(const Arg1& arg1)
	{
		handler_(arg1);
	}

	template <typename Arg1, typename Arg2>
	void operator()(const Arg1& arg1, const Arg2& arg2)
	{
		handler_(arg1, arg2);
	}

	friend void* asio_handler_allocate(std::size_t size,
			custom_alloc_handler<Handler>* this_handler)
	{
		assert(size < 1024);
		return this_handler->allocator_.address();
	}

	friend void asio_handler_deallocate(void* /*pointer*/, std::size_t /*size*/,
			custom_alloc_handler<Handler>* /*this_handler*/)
	{
		//this_handler->allocator_.deallocate(pointer);
	}

private:
	sb_allocator& allocator_;
	Handler handler_;
};

// Helper function to wrap a handler object to add custom allocation.
template <typename Handler>
inline custom_alloc_handler<Handler> make_custom_alloc_handler(
	sb_allocator& a, BOOST_ASIO_MOVE_ARG(Handler) h)
{
	return custom_alloc_handler<Handler>(a, BOOST_ASIO_MOVE_CAST(Handler)(h));
}


}

#endif //SERVICE_ENGINE_HPP
