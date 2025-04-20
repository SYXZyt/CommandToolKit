#pragma once
#ifndef __cplusplus
#error "ctk.hpp requires C++"
#endif

#include <string>
#include <ctk/ctkDllApi.h>
#include <ctk/ctk.h>

namespace ctk
{
	class CTK_API Manifest
	{
	public:
		enum class Source
		{
			FILE,
			MEMORY,
		};

	private:
		ctkManifest* manifest;
		ctkResult creationResult;

	public:
		inline ctkManifest* GetManifest() const
		{
			return manifest;
		}

		inline ctkResult GetCreationResult() const
		{
			return creationResult;
		}

		Manifest(const std::string& data, Source source);
		~Manifest();
	};

	class CTK_API Instance
	{
	private:
		ctkInstance* instance;

	public:
		inline ctkInstance* GetInstance() const
		{
			return instance;
		}

		ctkManifest* GetInternalManifest() const;

		void AppendManifest(const Manifest& manifest);
		void RemoveManifest(const Manifest& manifest);

		void RemoveCommand(const std::string& cmdName);

		ctkResult Execute(const std::string& cmd);

		ctkResult RegisterCallback(const std::string& cmdName, ctkCallback callback, void* userdata = nullptr);
		ctkResult UnregisterCallback(const std::string& cmdName);
		void* GetUserData(const std::string& cmdName);

		Instance();
		~Instance();
	};

	extern void CTK_API SetOutputCallback(ctkOutputCallback callback);
	extern void CTK_API ResetOutputCallback();
	extern void CTK_API Output(const std::string& msg);

	extern ctkOutputCallback CTK_API GetOutputCallback();
	extern ctkOutputCallback CTK_API GetDefaultOutputCallback();

	extern void CTK_API SetLastMessage(const std::string& msg);
	extern const char* CTK_API LastMessage();
	extern ctkResult CTK_API MakeResult(const std::string& msg, ctkResult type);
}