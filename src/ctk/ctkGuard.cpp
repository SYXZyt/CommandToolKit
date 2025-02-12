#include <ctk/ctkGuard.h>
#include <ctk/impl/ctkGuardWatch.h>
#include <ctk/ctkIO.h>
#include <string>

static ctkGuardWatch guardWatch = {};
static bool guardEnabled = false;

void ctkBeginGuard_real()
{
	ctkBeginGuard_Guarded(nullptr, 0);
}

void ctkEndGuard_real()
{
	ctkEndGuard_Guarded(nullptr, 0);
}

void ctkBeginGuard_Guarded(const char* file, int line)
{
	if (guardEnabled)
	{
		std::string msg = "\n[CTK_GUARD_ERROR] Guard is already enabled\n";
		if (file)
			msg += std::string("Called at file: ") + file + " at line: " + std::to_string(line) + "\n";

		ctkOutput(msg.c_str());
	}

	guardEnabled = true;
}

void ctkEndGuard_Guarded(const char* file, int line)
{
	if (!guardEnabled)
	{
		std::string msg = "\n[CTK_GUARD_ERROR] Tried to end Guard when is not enabled\n";
		if (file)
			msg += std::string("Called at file: ") + file + " at line: " + std::to_string(line) + "\n";

		ctkOutput(msg.c_str());
	}

	guardEnabled = false;

	guardWatch.Report();
	guardWatch.Clear();
}

bool ctkIsGuarding()
{
	return guardEnabled;
}

bool ctkGuardIsInstanceWatched(ctkInstance* instance)
{
	return guardEnabled ? guardWatch.IsInstanceWatched(instance) : false;
}

bool ctkGuardIsManifestWatched(ctkManifest* manifest)
{
	return guardEnabled ? guardWatch.IsManifestWatched(manifest) : false;
}

void* ctkGetGuardPtr()
{
	return &guardWatch;
}