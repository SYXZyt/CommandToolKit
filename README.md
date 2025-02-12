# CommandToolKit
CommandToolKit, or CTK, is a C++ library with a C API which allows you to define commands or cheats and invoke them at anytime.
Simply define all of the commands for you to use and register the callbacks and then call the execute function. The command will be parsed and executed with the callback you registered.

## Features
- Define commands with a simple string or a manifest file
- Commands can be appended allowing for other libraries to add their own commands

## Building
CTK uses CMAKE to build so you can simply build by including it as a sub directory and using this in `CmakeLists.txt`

```cmake
add_subdirectory("CommandToolKit")
target_link_libraries(YOUR_EXE CTK)
target_include_directories(YOUR_EXE PRIVATE "CTK/include")
```

Alternatively you can use the pre-built binaries available in the releases tab.
These are built with MSVS 2022 but should work with any compiler.

## Usage
```cpp
#include <ctk/ctk.h>
#include <stdio.h>

ctkResult MyCommandCallback(ctkInstance*, ctkValue* args, size_t argc)
{
	printf("Hello, My name is %s", args[0].s);
	return CTK_OK;
}

//You can also use a macro to automatically set the params
CTK_CALLBACK(MyCommandCallback)
{
	printf("Hello, My name is %s", args[0].s);
	return CTK_OK;
}

int main()
{
	ctkInstance* instance = ctkCreateInstance();
	ctkManifest* manifest;
	manifest = ctkCreateManifestFromSource("MyCmd;", &manifest);
	ctkAppendManifest(instance, manifest, false); //Set to true to free the manifest after appending
	ctkRegisterCallback(instance, "MyCmd", MyCommandCallback);

	ctkExecute(instance, "MyCmd 'CommandToolKit'");

	//And of course don't forget to free everything
	ctkUnregisterCallback(instance, "MyCmd"); //Not necessary due to freeing the instance, but this shows you can do this at any point
	ctkFreeInstance(instance);
	ctkFreeManifest(manifest); //This can be done immediatly after appending since the instance will copy the manifest
	return 0;
}
```
### Output
`Hello, My name is CommandToolKit`

### Guard
CTK features a guard which will try to warn the user of any memory issues which occur. Guard will monitor all created objects and will inform you of undeleted memory.
Guard is completly optional and opt-in. To enable guard, you must call `ctkBeginGuard()` and `ctkEndGuard()`. Ideally this should be at the top of main and the end of main respectively.

Guard also allows you to define `CTK_GUARD_MACROS` to get more information about the memory leak. This will show you the file and line where the object was created. This may not be 100% accurate.
Guard defines a macro for certain function calls. Use these macros and do not use the functions directly. For example `ctkCreateInstance` will redirect to either `ctkCreateInstance_real` or `ctkCreateInstanceGuarded` depending on the `CTK_GUARD_MACROS` define.

Below is an example guard in action
```c
#define CTK_GUARD_MACROS
#include <ctk/ctk.h>
CTK_CALLBACK(Help)
{
	printf("Help!\n");

	return CTK_OK;
}

void main()
{
	const char* manifest = "help;";

	ctkBeginGuard();

	ctkInstance* instance = ctkCreateInstance();
	ctkManifest* manifestPtr;
	ctkCreateManifestFromSource(manifest, &manifestPtr);
	ctkAppendManifest(instance, manifestPtr, false); //False means we have to manually free the manifest, which we won't to test guard
	ctkRegisterCallback(instance, "help", Help);
	ctkExecute(instance, "help");

	//Create a dangling pointer by overwriting the manifest without freeing it
	ctkCreateManifestFromSource(manifest, &manifestPtr);

	//We never freed the instance, or either of the manifests we created
	//So guard should report an error

	ctkEndGuard();
}
```

And the console output
```
Help!

[CTK_GUARD_ERROR] Tried to overwrite a manifest which has not been freed. Either free, use a new pointer or set the current pointer to null
Called at file: C:\Users\xxjak\source\repos\CTK\testing\main.c at line: 58


[CTK_GUARD_ERROR] Instance not destroyed: 0000022497A58AF0
Created at file: C:\Users\xxjak\source\repos\CTK\testing\main.c at line: 50


[CTK_GUARD_ERROR] Manifest not destroyed: 0000022497A539E0
Created at file: C:\Users\xxjak\source\repos\CTK\testing\main.c at line: 52


[CTK_GUARD_ERROR] Manifest not destroyed: 0000022497A53B00
Created at file: C:\Users\xxjak\source\repos\CTK\testing\main.c at line: 58
```

It is recommended to enable guard for non-release builds to help catch memory leaks