#pragma once
#include <ctk/ctkDllApi.h>

typedef void(*ctkOutputCallback)(const char* msg);

extern CTK_API void ctkSetOutputCallback(ctkOutputCallback callback);
extern CTK_API void ctkResetOutputCallback();
extern CTK_API void ctkOutput(const char* msg);

#define CTK_OUTPUT_CALLBACK(name) void name(const char* msg)