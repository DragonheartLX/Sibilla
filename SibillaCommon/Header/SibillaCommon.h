#pragma once
#include "Macros.h"

#ifdef SBL_PLATFORM_WINDOWS
	#ifdef SBL_BUILD_SHARED
		#ifdef SBL_BUILD_DLL
			#define SBL_API __declspec(dllexport)
		#else
			#define SBL_API __declspec(dllimport)
		#endif
	#else
		#define SBL_API
	#endif
#else
	#define SBL_API
#endif

namespace scom
{

}