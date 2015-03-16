#ifndef __LIBXMLRPCSERVERCONFIG_h__
#define __LIBXMLRPCSERVERCONFIG_h__

#ifdef WIN32
#pragma warning(disable:4251)
//your declarations that cause 4251
#ifdef LIBXMLRPCSERVER_EXPORTS
#define LIBXMLRPCSERVER_DECL __declspec(dllexport)
#else
#define LIBXMLRPCSERVER_DECL __declspec(dllimport)
#endif
#else
#define LIBXMLRPCSERVER_DECL
#endif

#endif
