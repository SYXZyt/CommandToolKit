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
These are built with MSVS 2022 but should work with any compiler due to the C API.

## Usage
```cpp
#include <ctk/ctk.h>
#include <stdio.h>

ctkResult MyCommandCallback(ctkInstance*, ctkValue* args, size_t argc)
{
	printf("Hello, My name is %s", args[0].s);
	return CTK_OK;
}

int main()
{
	ctkInstance* instance = ctkCreateInstance();
	ctkManifest* manifest;
	manifest = ctkCreateManifestFromSource("MyCmd;", &manifest);
	ctkAppendManifest(instance, manifest);
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