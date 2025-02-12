#pragma once
#include <ctk/ctkDllApi.h>

CTK_BEGIN_C

typedef void(*ctkOutputCallback)(const char* msg);

extern CTK_API void ctkSetOutputCallback(ctkOutputCallback callback);
extern CTK_API void ctkResetOutputCallback();
extern CTK_API void ctkOutput(const char* msg);

extern CTK_API ctkOutputCallback ctkGetOutputCallback();
extern CTK_API ctkOutputCallback ctkGetDefaultOutputCallback();

#define CTK_OUTPUT_CALLBACK(name) static void name(const char* name##_msg)

CTK_END_C