#pragma once
#include <ctk/ctkDllApi.h>
#include <ctk/ctk.h>
#include <stdbool.h>

#ifdef CTK_GUARD_MACROS
#define ctkBeginGuard() ctkBeginGuard_Guarded(__FILE__, __LINE__)
#define ctkEndGuard() ctkEndGuard_Guarded(__FILE__, __LINE__)
#else
#define ctkBeginGuard ctkBeginGuard_real
#define ctkEndGuard ctkEndGuard_real
#endif

CTK_BEGIN_C

typedef struct ctkGuardMetadata
{
	const char* file;
	int line;
} ctkGuardMetadata;

/// @brief CTK Guard will help look for memory leaks or dangling pointers. It simply keeps track of all the pointers that are allocated and deallocated and will report any that are not deallocated when the program end
extern CTK_API void ctkBeginGuard_real();
extern CTK_API void ctkEndGuard_real();

extern CTK_API void ctkBeginGuard_Guarded(const char* file, int line);
extern CTK_API void ctkEndGuard_Guarded(const char* file, int line);

extern CTK_API bool ctkIsGuarding();

extern CTK_API bool ctkGuardIsInstanceWatched(ctkInstance* instance);
extern CTK_API bool ctkGuardIsManifestWatched(ctkManifest* manifest);

/// @brief For internal use. Get the pointer to the guard who monitors all the pointers
/// @return A pointer to the guard
extern void* ctkGetGuardPtr();

CTK_END_C