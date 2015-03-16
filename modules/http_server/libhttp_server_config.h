#ifndef __LIBHTTPSERVERCONFIG_h__
#define __LIBHTTPSERVERCONFIG_h__

#ifdef WIN32
#pragma warning(disable:4251)
//your declarations that cause 4251
#ifdef LIBHTTPSERVER_EXPORTS
#define LIBHTTPSERVER_DECL __declspec(dllexport)
#else
#define LIBHTTPSERVER_DECL __declspec(dllimport)
#endif
#else
#define LIBHTTPSERVER_DECL
#endif

#endif
