#pragma once
#include <ctk/ctkInstance.h>
#include <ctk/ctkManifest.h>
#include <ctk/ctkResult.h>
#include <ctk/ctkType.h>
#include <ctk/ctkValue.h>
#include <ctk/ctkGuard.h>
#include <ctk/ctkIO.h>
#include <ctk/ctkDllApi.h>

#define CTK_CALLBACK(name) static ctkResult name(ctkInstance* name ## _instance, ctkValue* name ## _parameters, size_t name ## _parameterCount, void* name ## _userdata)