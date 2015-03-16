#ifndef NOTIFICATION_SINGLETON_HPP
#define NOTIFICATION_SINGLETON_HPP

#include <NotificationCenter/NotificationCenter.hpp>

LIB_NOTIFICATION_CENTER_DECL bool  NotificationCenterSingletonInit();
LIB_NOTIFICATION_CENTER_DECL FBi::NotificationCenter&  NotificationCenterSingletonInstance();

#endif


