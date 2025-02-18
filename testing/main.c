#define CTK_GUARD_MACROS

#include <ctk/ctk.h>
#include <stdio.h>

#ifndef __cplusplus
#define nullptr NULL
#endif

CTK_CALLBACK(OnHelp)
{
	printf("Help command\n");
	return ctkMakeResult("I should be in the last message buffer", CTK_OK_MSG);
}

CTK_CALLBACK(OnMoveTo)
{
	printf("MoveTo command\n");
	printf("Name: %s\n", OnMoveTo_parameters[0].s);
	printf("X: %f\n", OnMoveTo_parameters[1].f);
	printf("Y: %f\n", OnMoveTo_parameters[2].f);

	return CTK_OK;
}

CTK_CALLBACK(EchoWithUserdata)
{
	printf("Echo command\n");
	printf("Userdata: %s\n", (const char*)EchoWithUserdata_userdata);
	return CTK_OK;
}

void main()
{
	ctkBeginGuard();

	ctkInstance* instance = ctkCreateInstance();

	const char* manifestSrc = "help; moveto name: string, x: float, y: float;echo;";

	ctkManifest* manifest;
	ctkResult res = ctkCreateManifestFromSource(manifestSrc, &manifest);

	if (res != CTK_OK)
		fprintf(stderr, "Error creating manifest: %s\n", ctkLastMessage());

	ctkAppendManifest(instance, manifest, false);

	ctkRegisterCallback(instance, "help", OnHelp);
	ctkRegisterCallback(instance, "moveto", OnMoveTo);
	ctkRegisterCallback(instance, "echo", EchoWithUserdata);

	ctkSetUserData(instance, "echo", "Hello, world!");

	ctkExecute(instance, "moveto 'hi' 'bruh' 0 ");
	printf("CTK Message Buffer: %s\n\n", ctkLastMessage());
	res = ctkExecute(instance, "echo");
	if (res != CTK_OK)
		fprintf(stderr, "Error executing command: %s\n", ctkLastMessage());

	ctkUnregisterCallback(instance, "moveto");
	ctkUnregisterCallback(instance, "help");

	ctkDestroyInstance(instance);
	ctkDestroyManifest(manifest);

	ctkEndGuard();
}