#include <ctk/ctkInstance.h>
#include <ctk/impl/ctkInstanceImpl.h>
#include <ctk/ctkGuard.h>
#include <ctk/impl/ctkGuardWatch.h>

static bool blockGuard = false;

ctkInstance* ctkCreateInstance_real()
{
	ctkInstance* instance = reinterpret_cast<ctkInstance*>(new ctkInstanceImpl());

	if (!blockGuard && ctkIsGuarding())
	{
		((ctkGuardWatch*)ctkGetGuardPtr())->AddInstance(instance, { nullptr, 0 });
	}

	return instance;
}

ctkInstance* ctkCreateInstanceGuarded(const char* file, int line)
{
	blockGuard = true;
	ctkInstance* instance = ctkCreateInstance_real();
	blockGuard = false;

	if (ctkIsGuarding())
	{
		ctkGuardMetadata metadata = { file, line };
		((ctkGuardWatch*)ctkGetGuardPtr())->AddInstance(instance, metadata);
	}
	
	return instance;
}

void ctkDestroyInstance(ctkInstance* instance)
{
	((ctkGuardWatch*)ctkGetGuardPtr())->RemoveInstance(instance);

	delete reinterpret_cast<ctkInstanceImpl*>(instance);
}

void ctkAppendManifest(ctkInstance* instance, ctkManifest* manifest, bool freeManifest)
{
	reinterpret_cast<ctkInstanceImpl*>(instance)->AppendManifest(*manifest);

	if (freeManifest)
		ctkDestroyManifest(manifest);
}

void ctkRemoveManifest(ctkInstance* instance, const ctkManifest* manifest)
{
	reinterpret_cast<ctkInstanceImpl*>(instance)->RemoveManifest(*manifest);
}

void ctkRemoveCommand(ctkInstance* instance, const char* cmdName)
{
	reinterpret_cast<ctkInstanceImpl*>(instance)->RemoveCommand(cmdName);
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

ctkResult ctkSetUserData(ctkInstance* instance, const char* key, void* data)
{
	return reinterpret_cast<ctkInstanceImpl*>(instance)->SetUserData(key, data);
}

void* ctkGetUserData(ctkInstance* instance, const char* key)
{
	return reinterpret_cast<ctkInstanceImpl*>(instance)->GetUserData(key);
}

void ctkRemoveUserData(ctkInstance* instance, const char* key)
{
	reinterpret_cast<ctkInstanceImpl*>(instance)->RemoveUserData(key);
}
