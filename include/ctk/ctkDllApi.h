#pragma once
#ifdef _WIN32
	#ifdef CTK_EXPORT
		#define CTK_API __declspec(dllexport)
	#else
		#define CTK_API __declspec(dllimport)
	#endif
#else
	#define CTK_API
#endif

#ifdef __cplusplus
#define CTK_BEGIN_C extern "C" {
#define CTK_END_C }
#else
#define CTK_BEGIN_C
#define CTK_END_C
#endif