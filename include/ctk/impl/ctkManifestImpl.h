/**********************************************/
/* THIS IS FOR INTERNAL CTK USE. DO NOT USE! */
/********************************************/

#pragma once
#include <ctk/ctkType.h>
#include <stddef.h>
#include <ctk/ctkDllApi.h>
#include <ctk/impl/Interpreter/ctkToken.h>
#include <ctk/ctkResult.h>
#include <ctk/impl/ctkString.h>
#include <ctk/ctkManifest.h>
#include <vector>

struct ctkManifestImpl final
{
private:
	ctkResult Load(ctkToken* tokens, size_t tokenCount);

public:

	struct ctkEntry
	{
		ctkString cmdName;

		struct ctkArgTemplate
		{
			ctkString argName;
			ctkType argType = ctkType::CTK_TYPE_FLOAT;
		};

		std::vector<ctkArgTemplate> args;

		ctkEntry() = default;
		ctkEntry(const ctkEntry& other);

	};

	std::vector<ctkEntry> entries;

	ctkManifestImpl() = default;
	ctkManifestImpl(const ctkManifest& other);

	static ctkResult CreateManifestFromSource(const ctkString& source, ctkManifest* manifest);
	static ctkResult CreateManifestFromFile(const ctkString& path, ctkManifest* manifest);
};