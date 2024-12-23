#include <ctk/ctkManifest.h>
#include <ctk/impl/ctkManifestImpl.h>

ctkResult ctkCreateManifestFromSource(const char* src, ctkManifest** manifest)
{
	*manifest = reinterpret_cast<ctkManifest*>(new ctkManifestImpl());

	ctkResult res = ctkManifestImpl::CreateManifestFromSource(src, *manifest);

	return res;
}

ctkResult ctkCreateManifestFromFile(const char* file, ctkManifest** manifest)
{
	*manifest = reinterpret_cast<ctkManifest*>(new ctkManifestImpl());

	ctkResult res = ctkManifestImpl::CreateManifestFromFile(file, *manifest);

	return res;
}

void ctkDestroyManifest(ctkManifest* manifest)
{
	delete reinterpret_cast<ctkManifestImpl*>(manifest);
}