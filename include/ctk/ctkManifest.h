#pragma once
#include <ctk/ctkDllApi.h>
#include <ctk/ctkResult.h>

CTK_BEGIN_C

#ifdef CTK_GUARD_MACROS
#define ctkCreateManifestFromSource(src, man) ctkCreateManifestFromSourceGuarded(src, man, __FILE__, __LINE__)
#define ctkCreateManifestFromFile(file, man) ctkCreateManifestFromFileGuarded(file, man, __FILE__, __LINE__)
#else
#define ctkCreateManifestFromSource ctkCreateManifestFromSource_real
#define ctkCreateManifestFromFile ctkCreateManifestFromFile_real
#endif

/// @brief An object which contains a list of commands and their parameter names and types
typedef struct ctkManifest ctkManifest;

/// @brief Create a new manifest using a raw string literal. For example `const char manifest = "help;"` should use this function. If the manifest is in a text file, use `ctkCreateManifestFromFile` instead
/// @param src C-String containing the manifest data
/// @param manifest A pointer-ref to the manifest object to create
/// @return OK if no errors occurred during the creation of the manifest
extern CTK_API ctkResult ctkCreateManifestFromSource_real(const char* src, ctkManifest** manifest);

/// @brief Create a new manifest, but loading it from a file on disk
/// @param src A string storing the filename to load from
/// @param manifest A pointer-ref to the manifest object to create
/// @return OK if no errors occurred during the creation of the manifest
extern CTK_API ctkResult ctkCreateManifestFromFile_real(const char* file, ctkManifest** manifest);
extern CTK_API void ctkDestroyManifest(ctkManifest* manifest);

extern CTK_API ctkResult ctkCreateManifestFromSourceGuarded(const char* src, ctkManifest** manifest, const char* file, int line);
extern CTK_API ctkResult ctkCreateManifestFromFileGuarded(const char* filename, ctkManifest** manifest, const char* file, int line);

CTK_END_C