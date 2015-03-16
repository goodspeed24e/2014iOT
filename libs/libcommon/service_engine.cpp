#include <libcommon/service_engine.hpp>
#include <boost/bind.hpp>

namespace FBi
{

ServiceEngine::ServiceEngine(int num)
	: RR_index_(0),
	ready_(false)
{
	ServiceContext context;
	for(int i = 0; i < num; ++i)
	{
		context.io_service = new boost::asio::io_service;
		context.work = NULL;
		context.work_thread = NULL;
		context_vec_.push_back(context);
	}

}

ServiceEngine::~ServiceEngine()
{
	this->StopService();

	size_t size = context_vec_.size();
	for(size_t i = 0; i < size; ++i)
	{
		delete context_vec_[i].io_service;
	}
}

boost::asio::io_service& ServiceEngine::GetIOService()
{
	int index = 0;
	{
		boost::mutex::scoped_lock lock(mutex_);
		index = RR_index_++;
		if(RR_index_ >= context_vec_.size())
			RR_index_ = 0;
	}

	return *(context_vec_[index].io_service);
}

bool ServiceEngine::IsRunning()
{
	return ready_;
}

void ServiceEngine::StartService()
{
	if (ready_)
		return;
	boost::mutex::scoped_lock lock(mutex_);
	if (ready_)
		return;


	size_t size = context_vec_.size();
	for(size_t i = 0; i < size; ++i)
	{
		context_vec_[i].work = new boost::asio::io_service::work(*(context_vec_[i].io_service));
		context_vec_[i].work_thread = new boost::thread(boost::bind(&boost::asio::io_service::run, context_vec_[i].io_service));
	}

	ready_ = true;
}

void ServiceEngine::StopService()
{
	if (!ready_)
		return;
	boost::mutex::scoped_lock lock(mutex_);
	if (!ready_)
		return;

	size_t size = context_vec_.size();
	for(size_t i = 0; i < size; ++i)
	{
		context_vec_[i].io_service->stop();
		delete context_vec_[i].work;
		context_vec_[i].work = NULL;
		context_vec_[i].work_thread->join();
		delete context_vec_[i].work_thread;
		context_vec_[i].work_thread = NULL;
	}

	ready_ = false;
}

}
