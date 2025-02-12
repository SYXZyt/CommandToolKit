#pragma once
#include <vector>
#include <ctk/ctk.h>

/**********************************************/
/* THIS IS FOR INTERNAL CTK USE. DO NOT USE! */
/********************************************/

class ctkGuardWatch final
{
	struct Instance final
	{
		ctkInstance* instance;
		ctkGuardMetadata metadata;
	};

	struct Manifest final
	{
		ctkManifest* manifest;
		ctkGuardMetadata metadata;
	};

private:
	std::vector<Instance> instances;
	std::vector<Manifest> manifests;

public:
	void Report();
	void Clear();

	void AddInstance(ctkInstance* instance, ctkGuardMetadata metadata);
	void AddManifest(ctkManifest* manifest, ctkGuardMetadata metadata);

	bool IsInstanceWatched(ctkInstance* instance);
	bool IsManifestWatched(ctkManifest* manifest);

	void RemoveInstance(ctkInstance* instance);
	void RemoveManifest(ctkManifest* manifest);
};