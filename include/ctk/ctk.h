#pragma once
#include <ctk/ctkInstance.h>
#include <ctk/ctkManifest.h>
#include <ctk/ctkResult.h>
#include <ctk/ctkType.h>
#include <ctk/ctkValue.h>
#include <ctk/ctkGuard.h>
#include <ctk/ctkIO.h>
#include <ctk/ctkDllApi.h>

#define CTK_CALLBACK(name) static ctkResult name(ctkInstance* instance, ctkValue* parameters, size_t parameterCount, void* userdata)