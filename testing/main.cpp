#include <ctk/cpp/ctk.hpp>
#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

CTK_CALLBACK(OnHelp)
{
	std::cout << "Help command" << '\n';
	return ctkMakeResult("I should be in the last message buffer", CTK_OK_MSG);
}

CTK_CALLBACK(OnMoveTo)
{
	std::cout << "MoveTo command" << '\n';
	std::cout << "Name: " << OnMoveTo_parameters[0].s << '\n';
	std::cout << "X: " << OnMoveTo_parameters[1].f << '\n';
	std::cout << "Y: " << OnMoveTo_parameters[2].f << '\n';
	return CTK_OK;
}

CTK_CALLBACK(EchoWithUserdata)
{
	std::cout << "Echo command" << '\n';
	std::cout << "Userdata: " << (const char*)EchoWithUserdata_userdata << '\n';
	return CTK_OK;
}

void main()
{
	ctk::Instance instance;
	const char* manifestSrc = "help; moveto name: string, x: float, y: float;echo;";

	ctk::Manifest manifest(manifestSrc, ctk::Manifest::Source::MEMORY);
	if (manifest.GetCreationResult() != CTK_OK)
	{
		std::cerr << "Error creating manifest: " << ctkLastMessage() << '\n';
		return;
	}

	instance.AppendManifest(manifest);

	instance.RegisterCallback("help", OnHelp);
	instance.RegisterCallback("moveto", OnMoveTo);
	instance.RegisterCallback("echo", EchoWithUserdata, "Hello, World!");

	ctkResult res = instance.Execute("moveto 'hi' 'bruh' 0 ");
	std::cout << "CTK Message Buffer: " << ctkLastMessage() << '\n';
	res = instance.Execute("echo");
	if (res != CTK_OK)
		std::cerr << "Error executing command: " << ctkLastMessage() << '\n';

	instance.UnregisterCallback("moveto");
	instance.UnregisterCallback("help");
}