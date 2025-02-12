#include <ctk/impl/ctkGuardWatch.h>
#include <ctk/ctkIO.h>
#include <string>
#include <sstream>

void ctkGuardWatch::Report()
{
	std::stringstream reportStream;

	for (ctkGuardWatch::Instance& instance : instances)
	{
		reportStream << "\n[CTK_GUARD_ERROR] Instance not destroyed: " << instance.instance << "\n";
		if (instance.metadata.file)
			reportStream << "Created at file: " << instance.metadata.file << " at line: " << instance.metadata.line << "\n\n";
	}

	for (ctkGuardWatch::Manifest& manifest : manifests)
	{
		reportStream << "\n[CTK_GUARD_ERROR] Manifest not destroyed: " << manifest.manifest << "\n";
		if (manifest.metadata.file)
			reportStream << "Created at file: " << manifest.metadata.file << " at line: " << manifest.metadata.line << "\n\n";
	}

	ctkOutput(reportStream.str().c_str());
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