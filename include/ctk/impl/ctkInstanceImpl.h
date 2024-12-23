/**********************************************/
/* THIS IS FOR INTERNAL CTK USE. DO NOT USE! */
/********************************************/

#pragma once
#include <ctk/ctkInstance.h>
#include <ctk/ctkManifest.h>
#include <unordered_map>
#include <string>
#include <ctk/ctkValue.h>

class ctkInstanceImpl final
{
private:
	struct ParseInfo
	{
		std::string cmdName;
		std::vector<ctkValue> args;
	};

	ctkCallback GetMatchingCallback(const ParseInfo& info);

protected:
	ctkManifest* manifest;
	std::unordered_map<std::string, ctkCallback> callbacks;

public:
	inline ctkManifest* GetManifest() { return manifest; }

	ctkResult RegisterCallback(const char* cmdName, ctkCallback callback);
	ctkResult UnregisterCallback(const char* cmdName);

	ctkResult ProcessCommand(const char* cmd);
	void AppendManifest(const ctkManifest& manifest);
};