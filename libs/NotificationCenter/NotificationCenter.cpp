#include <NotificationCenter/NotificationCenter.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

namespace FBi
{

/**
 * @brief The private function that is used to call the slot when we using the thread to call the slot.
 *
 * @param[in] func The registered slot.
 * @param[in] arg The argument to pass into the registered slot.
 * @return The return value of the slot.
 */
static boost::any func_invoker(const NotificationSlot& func, const boost::any& arg)
{
	return func(arg);
}

/**
 * @brief The private function that is used to call the invoker function by thread.
 *
 * @param[in] is_non_blocking true: if we don't care the return value, false: wait the return value.
 * @param[in] func The registered slot.
 * @param[in] arg The argument to pass into the registered slot.
 * @return The return value of the slot or invalid value when we don't care the return value.
 */
static boost::any run_func_in_thread(bool is_non_blocking, const boost::function<boost::any(boost::any)>& func, const boost::any& arg)
{
	if(is_non_blocking)
	{
		boost::thread task(boost::bind(&func_invoker, func, arg));
		return boost::any();
	}

	boost::packaged_task<boost::any> p_task(boost::bind(&func_invoker, func, arg));
	boost::unique_future<boost::any> ret_future = p_task.get_future();

	boost::thread task(std::move(p_task));

	ret_future.wait();
	return ret_future.get();
}

/// The constructor of notification center.
NotificationCenter::NotificationCenter()
{
}

/// The destructor of notification center.
NotificationCenter::~NotificationCenter()
{
	boost::mutex::scoped_lock lock(mutex_);

	SignalContainer::iterator iter = signals_map_.begin();
	for(; iter != signals_map_.end(); iter++)
	{
		if(iter->second)
		{
			iter->second->disconnect_all_slots();
		}
	}

	signals_map_.clear();
}

/**
 * @brief Register the signal.
 *
 * @param[in] key The key which we used to register the signal.
 * @return The pointer of signal.
 */
NotificationSignalPtr NotificationCenter::RegisterSignal(const std::string& key)
{
	boost::mutex::scoped_lock lock(mutex_);

	SignalContainer::iterator iter = signals_map_.find(key);

	if( (iter != signals_map_.end()) && (iter->second) )
		return (iter->second);

	return ( signals_map_[key] = NotificationSignalPtr(new NotificationSignal) );
}

/**
 * @brief Unregister the signal.
 *
 * @param[in] key The key which we used to register the signal.
 */
void NotificationCenter::UnregisterSignal(const std::string& key)
{
	boost::mutex::scoped_lock lock(mutex_);

	SignalContainer::iterator iter = signals_map_.find(key);

	if( iter != signals_map_.end() )
	{
		if(iter->second)
		{
			iter->second->disconnect_all_slots();
		}

		signals_map_.erase(iter);
	}
}

/**
 * @brief Connection the signal and slot by the specific mode.
 *
 * @param[in] key The key which we used to register the signal.
 * @param[in] slot The slot that we want to connect it to the signal.
 * @param[in] mode The mode of connection.
 * @return The boost::signals2 connection.
 */
NotificationConnection NotificationCenter::Connect(const std::string& key, const NotificationSlot& slot, ConnectedMode mode)
{
	if(slot.empty())
	{
		return NotificationConnection();
	}

	boost::mutex::scoped_lock lock(mutex_);

	SignalContainer::iterator iter = signals_map_.find(key);

	if( (iter == signals_map_.end()) || (!iter->second) )
	{
		signals_map_[key] = NotificationSignalPtr(new NotificationSignal);
		iter = signals_map_.find(key);
	}

	switch(mode)
	{
		case BlockingThreadCall:
			return (iter->second->connect(boost::bind(&run_func_in_thread, false, slot, _1)));
		case NonBlockingThreadCall:
			return (iter->second->connect(boost::bind(&run_func_in_thread, true, slot, _1)));
		default:
			return (iter->second->connect(slot));
	}
}

/**
 * @brief Disconnect the slot and signal which are recorded by connection.
 *
 * @param[in] con The boost::signals2 connection return by Connect function.
 */
void NotificationCenter::Disconnect(NotificationConnection& con)
{
	con.disconnect();
}

} // namespace FBi
