#ifndef __LIBCOMMONCONFIG_h__
#define __LIBCOMMONCONFIG_h__

#ifdef WIN32
#pragma warning(disable:4251)
//your declarations that cause 4251
#ifdef LIBCOMMON_EXPORTS
#define LIBCOMMON_DECL __declspec(dllexport)
#else
#define LIBCOMMON_DECL __declspec(dllimport)
#endif
#else
#define LIBCOMMON_DECL
#endif

#endif