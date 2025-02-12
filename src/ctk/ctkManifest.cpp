#include <ctk/ctkManifest.h>
#include <ctk/impl/ctkManifestImpl.h>
#include <ctk/ctkGuard.h>
#include <ctk/impl/ctkGuardWatch.h>
#include <string>

const std::string overwriteMessage = "\n[CTK_GUARD_ERROR] Tried to overwrite a manifest which has not been freed. Either free, use a new pointer or set the current pointer to null\n";

static bool blockGuard = false;

ctkResult ctkCreateManifestFromSource_real(const char* src, ctkManifest** manifest)
{
	if (ctkIsGuarding() && !blockGuard)
	{
		if (ctkGuardIsManifestWatched(*manifest))
			ctkOutput(overwriteMessage.c_str());
	}

	*manifest = reinterpret_cast<ctkManifest*>(new ctkManifestImpl());

	ctkResult res = ctkManifestImpl::CreateManifestFromSource(src, *manifest);

	if (ctkIsGuarding() && !blockGuard)
		((ctkGuardWatch*)ctkGetGuardPtr())->AddManifest(*manifest, { nullptr, 0 });

	return res;
}

ctkResult ctkCreateManifestFromFile_real(const char* file, ctkManifest** manifest)
{
	if (ctkIsGuarding() && !blockGuard)
	{
		if (ctkGuardIsManifestWatched(*manifest))
			ctkOutput(overwriteMessage.c_str());
	}

	*manifest = reinterpret_cast<ctkManifest*>(new ctkManifestImpl());

	ctkResult res = ctkManifestImpl::CreateManifestFromFile(file, *manifest);

	if (ctkIsGuarding() && !blockGuard)
		((ctkGuardWatch*)ctkGetGuardPtr())->AddManifest(*manifest, { nullptr, 0 });

	return res;
}

void ctkDestroyManifest(ctkManifest* manifest)
{
	((ctkGuardWatch*)ctkGetGuardPtr())->RemoveManifest(manifest);

	delete reinterpret_cast<ctkManifestImpl*>(manifest);
}

ctkResult ctkCreateManifestFromSourceGuarded(const char* src, ctkManifest** manifest, const char* file, int line)
{
	if (ctkIsGuarding())
	{
		if (ctkGuardIsManifestWatched(*manifest))
		{
			std::string msg = overwriteMessage;
			if (file)
				msg += std::string("Called at file: ") + file + " at line: " + std::to_string(line) + "\n";

			ctkOutput(msg.c_str());
		}
	}

	blockGuard = true;
	ctkResult res = ctkCreateManifestFromSource_real(src, manifest);
	blockGuard = false;

	if (ctkIsGuarding())
		((ctkGuardWatch*)ctkGetGuardPtr())->AddManifest(*manifest, { file, line });

	return res;
}

ctkResult ctkCreateManifestFromFileGuarded(const char* filename, ctkManifest** manifest, const char* file, int line)
{
	if (ctkIsGuarding())
	{
		if (ctkGuardIsManifestWatched(*manifest))
		{
			std::string msg = overwriteMessage;
			if (file)
				msg += std::string("Called at file: ") + file + " at line: " + std::to_string(line) + "\n";

			ctkOutput(msg.c_str());
		}
	}

	blockGuard = true;
	ctkResult res = ctkCreateManifestFromFile_real(filename, manifest);
	blockGuard = false;

	if (ctkIsGuarding())
		((ctkGuardWatch*)ctkGetGuardPtr())->AddManifest(*manifest, { file, line });

	return res;
}