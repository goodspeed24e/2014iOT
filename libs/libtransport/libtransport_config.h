#ifndef __LIBTRANSPORTCONFIG_h__
#define __LIBTRANSPORTCONFIG_h__

#ifdef WIN32
#pragma warning(disable:4251)
//your declarations that cause 4251
#ifdef LIBTRANSPORT_EXPORTS
#define LIBTRANSPORT_DECL __declspec(dllexport)
#else
#define LIBTRANSPORT_DECL __declspec(dllimport)
#endif
#else
#define LIBTRANSPORT_DECL
#endif

#endif