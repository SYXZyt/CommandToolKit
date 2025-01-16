/**********************************************/
/* THIS IS FOR INTERNAL CTK USE. DO NOT USE! */
/********************************************/

#pragma once
#include <ctk/ctkInstance.h>
#include <ctk/ctkManifest.h>
#include <unordered_map>
#include <string>
#include <ctk/ctkValue.h>
#include <mutex>

class ctkInstanceImpl final
{
private:
	std::recursive_mutex mutex;

	struct ParseInfo
	{
		std::string cmdName;
		std::vector<ctkValue> args;
	};

	ctkCallback GetMatchingCallback(const ParseInfo& info);

protected:
	ctkManifest* manifest;
	std::unordered_map<std::string, ctkCallback> callbacks;
	std::unordered_map<std::string, void*> userdata;

public:
	inline ctkManifest* GetManifest() { return manifest; }

	void SetUserData(const char* key, void* data);
	void* GetUserData(const char* key);

	ctkResult RegisterCallback(const char* cmdName, ctkCallback callback);
	ctkResult UnregisterCallback(const char* cmdName);

	ctkResult ProcessCommand(const char* cmd);
	void AppendManifest(const ctkManifest& manifest);
};