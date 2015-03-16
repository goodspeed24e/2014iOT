#ifndef __DISCOVERY_CONFIG_h__
#define __DISCOVERY_CONFIG_h__

#ifdef WIN32
#pragma warning(disable:4251)
//your declarations that cause 4251
#ifdef DISCOVERY_EXPORTS
#define DISCOVERY_EXPORTS_DECL __declspec(dllexport)
#else
#define DISCOVERY_EXPORTS_DECL __declspec(dllimport)
#endif
#else
#define DISCOVERY_EXPORTS_DECL
#endif

#endif