#ifndef __LIB_NOTIFICATION_CENTER_CONFIG_h__
#define __LIB_NOTIFICATION_CENTER_CONFIG_h__

#ifdef WIN32
#pragma warning(disable:4251)
//your declarations that cause 4251
#ifdef LIB_NOTIFICATION_CENTER_EXPORTS
#define LIB_NOTIFICATION_CENTER_DECL __declspec(dllexport)
#else
#define LIB_NOTIFICATION_CENTER_DECL __declspec(dllimport)
#endif
#else
#define LIB_NOTIFICATION_CENTER_DECL
#endif

#endif
