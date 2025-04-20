#include <ctk/cpp/ctk.hpp>

ctkManifest* ctk::Instance::GetInternalManifest() const
{
	return ctkGetManifest(instance);
}

void ctk::Instance::AppendManifest(const Manifest& manifest)
{
	ctkAppendManifest(instance, manifest.GetManifest(), false);
}

void ctk::Instance::RemoveManifest(const Manifest& manifest)
{
	ctkRemoveManifest(instance, manifest.GetManifest());
}

void ctk::Instance::RemoveCommand(const std::string& cmdName)
{
	ctkRemoveCommand(instance, cmdName.c_str());
}

ctkResult ctk::Instance::Execute(const std::string& cmd)
{
	return ctkExecute(instance, cmd.c_str());
}

ctkResult ctk::Instance::RegisterCallback(const std::string& cmdName, ctkCallback callback, void* userdata)
{
	ctkResult result = ctkRegisterCallback(instance, cmdName.c_str(), callback);
	if (result != CTK_OK)
		return result;

	result = ctkSetUserData(instance, cmdName.c_str(), userdata);
	return result;
}

ctkResult ctk::Instance::UnregisterCallback(const std::string& cmdName)
{
	return ctkUnregisterCallback(instance, cmdName.c_str());
}

void* ctk::Instance::GetUserData(const std::string& cmdName)
{
	return ctkGetUserData(instance, cmdName.c_str());
}

ctk::Instance::Instance()
{
	instance = ctkCreateInstance();
}

ctk::Instance::~Instance()
{
	ctkDestroyInstance(instance);
	instance = nullptr;
}

ctk::Manifest::Manifest(const std::string& data, Source source)
{
	if (source == Source::FILE)
		creationResult = ctkCreateManifestFromFile(data.c_str(), &manifest);
	else
		creationResult = ctkCreateManifestFromSource(data.c_str(), &manifest);
}

ctk::Manifest::~Manifest()
{
	ctkDestroyManifest(manifest);
	manifest = nullptr;
}

void ctk::SetOutputCallback(ctkOutputCallback callback)
{
	ctkSetOutputCallback(callback);
}

void ctk::ResetOutputCallback()
{
	ctkResetOutputCallback();
}

void ctk::Output(const std::string& msg)
{
	ctkOutput(msg.c_str());
}

ctkOutputCallback ctk::GetOutputCallback()
{
	return ctkGetOutputCallback();
}

ctkOutputCallback ctk::GetDefaultOutputCallback()
{
	return ctkGetDefaultOutputCallback();
}

#include <ctk/impl/ctkString.h>
void ctk::SetLastMessage(const std::string& msg)
{
	ctkString msgStr = ctkString(msg.c_str());
	ctkSetLastMessage(&msgStr);
}

ctkResult ctk::MakeResult(const std::string& msg, ctkResult type)
{
	return ctkMakeResult(msg.c_str(), type);
}