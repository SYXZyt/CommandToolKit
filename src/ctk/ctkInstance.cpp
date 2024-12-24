#include <ctk/ctkInstance.h>
#include <ctk/impl/ctkInstanceImpl.h>

ctkInstance* ctkCreateInstance()
{
	return reinterpret_cast<ctkInstance*>(new ctkInstanceImpl());
}

void ctkDestroyInstance(ctkInstance* instance)
{
	delete reinterpret_cast<ctkInstanceImpl*>(instance);
}

void ctkAppendManifest(ctkInstance* instance, const ctkManifest* manifest)
{
	reinterpret_cast<ctkInstanceImpl*>(instance)->AppendManifest(*manifest);
}

ctkResult ctkExecute(ctkInstance* instance, const char* cmd)
{
	return reinterpret_cast<ctkInstanceImpl*>(instance)->ProcessCommand(cmd);
}

ctkManifest* ctkGetManifest(ctkInstance* instance)
{
	return reinterpret_cast<ctkInstanceImpl*>(instance)->GetManifest();
}

ctkResult ctkRegisterCallback(ctkInstance* instance, const char* cmdName, ctkCallback callback)
{
	return reinterpret_cast<ctkInstanceImpl*>(instance)->RegisterCallback(cmdName, callback);
}

ctkResult ctkUnregisterCallback(ctkInstance* instance, const char* cmdName)
{
	return reinterpret_cast<ctkInstanceImpl*>(instance)->UnregisterCallback(cmdName);
}

void ctkSetUserData(ctkInstance* instance, const char* key, void* data)
{
	reinterpret_cast<ctkInstanceImpl*>(instance)->SetUserData(key, data);
}

void* ctkGetUserData(ctkInstance* instance, const char* key)
{
	return reinterpret_cast<ctkInstanceImpl*>(instance)->GetUserData(key);
}