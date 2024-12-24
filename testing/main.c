#include <ctk/ctk.h>
#include <stdio.h>

#ifndef __cplusplus
#define nullptr NULL
#endif

static ctkResult OnHelp(ctkInstance* instance, ctkValue* parameters, size_t parameterCount, void* userdata)
{
	printf("Help command\n");
	return ctkMakeResult("I should be in the last message buffer", CTK_OK_MSG);
}

static ctkResult OnMoveTo(ctkInstance* instance, ctkValue* parameters, size_t parameterCount, void* userdata)
{
	printf("MoveTo command\n");
	printf("Name: %s\n", parameters[0].s);
	printf("X: %f\n", parameters[1].f);
	printf("Y: %f\n", parameters[2].f);

	return CTK_OK;
}

static ctkResult EchoWithUserdata(ctkInstance* instance, ctkValue* parameters, size_t parameterCount, void* userdata)
{
	printf("Echo command\n");
	printf("Userdata: %s\n", (const char*)userdata);
	return CTK_OK;
}

void main()
{
	ctkInstance* instance = ctkCreateInstance();

	const char* manifestSrc = "help; moveto name: string, x: float, y: float;echo;";

	ctkManifest* manifest;
	ctkResult res = ctkCreateManifestFromSource(manifestSrc, &manifest);

	if (res != CTK_OK)
		fprintf(stderr, "Error creating manifest: %s\n", ctkLastMessage());

	ctkAppendManifest(instance, manifest);

	ctkRegisterCallback(instance, "help", OnHelp);
	ctkRegisterCallback(instance, "moveto", OnMoveTo);
	ctkRegisterCallback(instance, "echo", EchoWithUserdata);

	ctkSetUserData(instance, "echo", "Hello, world!");

	ctkExecute(instance, "help");
	printf("%s\n\n", ctkLastMessage());
	res = ctkExecute(instance, "echo");
	if (res != CTK_OK)
		fprintf(stderr, "Error executing command: %s\n", ctkLastMessage());

	ctkDestroyManifest(manifest);
	ctkUnregisterCallback(instance, "moveto");
	ctkUnregisterCallback(instance, "help");
	ctkDestroyInstance(instance);
}