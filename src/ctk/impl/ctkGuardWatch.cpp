#include <ctk/impl/ctkGuardWatch.h>

void ctkGuardWatch::Report()
{
	for (ctkGuardWatch::Instance& instance : instances)
	{
		fprintf(stderr, "\n[CTK_GUARD_ERROR] Instance not destroyed: %p\n", instance.instance);
		if (instance.metadata.file)
			fprintf(stderr, "Created at file: %s at line: %d\n\n", instance.metadata.file, instance.metadata.line);
	}

	for (ctkGuardWatch::Manifest& manifest : manifests)
	{
		fprintf(stderr, "\n[CTK_GUARD_ERROR] Manifest not destroyed: %p\n", manifest.manifest);
		if (manifest.metadata.file)
			fprintf(stderr, "Created at file: %s at line: %d\n\n", manifest.metadata.file, manifest.metadata.line);
	}
}

void ctkGuardWatch::Clear()
{
	instances.clear();
	manifests.clear();
}

void ctkGuardWatch::AddInstance(ctkInstance* instance, ctkGuardMetadata metadata)
{
	Instance inst = { instance, metadata };
	instances.push_back(inst);
}

void ctkGuardWatch::AddManifest(ctkManifest* manifest, ctkGuardMetadata metadata)
{
	Manifest mani = { manifest, metadata };
	manifests.push_back(mani);
}

bool ctkGuardWatch::IsInstanceWatched(ctkInstance* instance)
{
	for (const Instance& inst : instances)
	{
		if (inst.instance == instance)
			return true;
	}

	return false;
}

bool ctkGuardWatch::IsManifestWatched(ctkManifest* manifest)
{
	for (const Manifest& mani : manifests)
	{
		if (mani.manifest == manifest)
			return true;
	}

	return false;
}

void ctkGuardWatch::RemoveInstance(ctkInstance* instance)
{
	for (size_t i = 0; i < instances.size(); ++i)
	{
		if (instances[i].instance == instance)
		{
			instances.erase(instances.begin() + i);
			return;
		}
	}
}

void ctkGuardWatch::RemoveManifest(ctkManifest* manifest)
{
	for (size_t i = 0; i < manifests.size(); ++i)
	{
		if (manifests[i].manifest == manifest)
		{
			manifests.erase(manifests.begin() + i);
			return;
		}
	}
}