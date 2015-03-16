#include <NotificationCenter/NotificationSingleton.hpp>

FBi::NotificationCenter& NotificationCenterSingletonInstance()
{
	static FBi::NotificationCenter instance_;
	return instance_;
}

bool NotificationCenterSingletonInitImpl()
{
	NotificationCenterSingletonInstance();
	return true;
}

bool NotificationCenterSingletonInit()
{
	static const bool ret = NotificationCenterSingletonInitImpl();
	return ret;
}