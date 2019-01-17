#pragma once

#ifdef OS_ANDROID
#	define	DLLAPI
#	define	CALLAPI
#elif defined OS_WIN32
#	include <SDKDDKVer.h>
#	define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#	include <windows.h>

#	ifdef DLLEXPORT
#		define	DLLAPI	__declspec(dllexport)
#	else
#		define DLLAPI	__declspec(dllimport)
#	endif
#	define	CALLAPI	__cdecl
#endif

#define NS			Core
#define START_NS	namespace Core {
#define END_NS		}
#define USE_NS		using namespace Core