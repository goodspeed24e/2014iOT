#ifndef NOTIFICATION_CENTER_HPP
#define NOTIFICATION_CENTER_HPP

#include <NotificationCenter/libNotificationCenter_config.h>
#include <map>
#include <string>
#include <deque>
#include <boost/signals2.hpp>
#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

namespace FBi
{

typedef boost::signals2::signal<boost::any(const boost::any&)> NotificationSignal;
typedef boost::shared_ptr<NotificationSignal> NotificationSignalPtr;
typedef boost::function<boost::any(const boost::any&)> NotificationSlot;
typedef boost::signals2::connection NotificationConnection;

/// Class of notification center.
class LIB_NOTIFICATION_CENTER_DECL NotificationCenter
{
public:
	/// The type of signal container.
	typedef std::map<std::string, NotificationSignalPtr> SignalContainer;
	/// The mode of connection. (1) Synchronous call. (2) Call by thread and wait the result. (3) Call by thread and discard the result.
	enum ConnectedMode { SyncCall = 0, BlockingThreadCall, NonBlockingThreadCall };
public:
	NotificationCenter();
	~NotificationCenter();

	NotificationSignalPtr RegisterSignal(const std::string& key);
	void UnregisterSignal(const std::string& key);

	NotificationConnection Connect(const std::string& key, const NotificationSlot& slot, ConnectedMode mode = NonBlockingThreadCall);
	void Disconnect(NotificationConnection& con);

private:
	/// The map to store the keys and signals.
	SignalContainer signals_map_;

	/// The mutex to protect the map.
	boost::mutex mutex_;
};

} // namespace FBi

#endif

