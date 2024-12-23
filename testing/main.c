#include <ctk/ctk.h>
#include <stdio.h>

#ifndef __cplusplus
#define nullptr NULL
#endif

static ctkResult OnHelp(ctkInstance* instance, ctkValue* parameters, size_t parameterCount)
{
	printf("Help command\n");
	return ctkMakeResult("I should be in the last message buffer", CTK_OK_MSG);
}

static ctkResult OnMoveTo(ctkInstance* instance, ctkValue* parameters, size_t parameterCount)
{
	printf("MoveTo command\n");
	printf("Name: %s\n", parameters[0].s);
	printf("X: %f\n", parameters[1].f);
	printf("Y: %f\n", parameters[2].f);

	return CTK_OK;
}

void main()
{
	ctkInstance* instance = ctkCreateInstance();

	const char* manifestSrc = "help; moveto name: string, x: float, y: float;";

	ctkManifest* manifest;
	ctkResult res = ctkCreateManifestFromSource(manifestSrc, &manifest);

	if (res != CTK_OK)
		fprintf(stderr, "Error creating manifest: %s\n", ctkLastMessage());

	ctkAppendManifest(instance, manifest);

	ctkRegisterCallback(instance, "help", OnHelp);
	ctkRegisterCallback(instance, "moveto", OnMoveTo);

	ctkExecute(instance, "help");
	printf("%s\n\n", ctkLastMessage());
	res = ctkExecute(instance, "moveto 'hi' 0 0");
	if (res != CTK_OK)
		fprintf(stderr, "Error executing command: %s\n", ctkLastMessage());

	ctkDestroyManifest(manifest);
	ctkUnregisterCallback(instance, "moveto");
	ctkUnregisterCallback(instance, "help");
	ctkDestroyInstance(instance);
}