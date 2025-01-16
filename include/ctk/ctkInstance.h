#pragma once
#include <ctk/ctkDllApi.h>
#include <ctk/ctkManifest.h>
#include <ctk/ctkResult.h>
#include <ctk/ctkType.h>
#include <ctk/ctkValue.h>
#include <stdlib.h>

CTK_BEGIN_C

/// @brief Instance of the ctk library. Use it to create, execute and destroy commands
typedef struct ctkInstance ctkInstance;

/// @brief Callback to call when the associated command is called
/// @param instance The instance that called the command
/// @param parameters The parameters passed to the command. Guarenteed to be the same count and types as defined in the manifest
/// @param parameterCount The number of parameters passed to the command
/// @param userdata Any user data that was passed to the callback
typedef ctkResult(*ctkCallback)(ctkInstance* instance, ctkValue* parameters, size_t parameterCount, void* userdata);

/// @brief Create a new instance of the ctk library
/// @return A pointer to the new instance
extern CTK_API ctkInstance* ctkCreateInstance();

/// @brief Destroy an instance of the ctk library
/// @param instance The instance which should be deleted
extern CTK_API void ctkDestroyInstance(ctkInstance* instance);

/// @brief Add a new manifest of commands to the instance
/// @param instance Which instance should have the commands added
/// @param manifest The manifest of commands to add. The instance WILL NOT take ownership, rather copying the data to its own buffer. Make sure you delete the manifest after appending to the last instance
extern CTK_API void ctkAppendManifest(ctkInstance* instance, const ctkManifest* manifest);

extern CTK_API void ctkRemoveManifest(ctkInstance* instance, const ctkManifest* manifest);

extern CTK_API void ctkRemoveCommand(ctkInstance* instance, const char* cmdName);

/// @brief Execute a command on the instance
/// @param instance Which instance should parse and run the callbacks
/// @param cmd The command to execute
/// @return OK if no errors occurred. Check the last message as some commands MAY return a message in that buffer rather than using stdio
extern CTK_API ctkResult ctkExecute(ctkInstance* instance, const char* cmd);

/// @brief Get the internal manifest used by an instance
/// @param instance The instance whose manifest to retrieve
/// @return A pointer to the internal manifest. Do not delete this pointer
extern CTK_API ctkManifest* ctkGetManifest(ctkInstance* instance);

/// @brief Register a callback which will be executable once it's associated command is called, assuming it has the correct parameters supplied
/// @param instance Which instance should have the callback registered
/// @param cmdName The of the command to listen for, for example to register a help command, this should be "help"
/// @param callback The function which should be called. The parameters passed to this function will be the same as the ones defined in the manifest and are guarenteed to be the same types, so you can be lax with your error checking
/// @return OK if the callback was registered ok
extern CTK_API ctkResult ctkRegisterCallback(ctkInstance* instance, const char* cmdName, ctkCallback callback);

/// @brief Deregister a callback so that it will no longer be used by the instance. A non OK value will be returned if the callback was not found
/// @param instance Which instance to remove the callback from
/// @param cmdName The name of the command to remove. For example "help" will remove the help command if it has been loaded
/// @return OK if the callback was successfully removed
extern CTK_API ctkResult ctkUnregisterCallback(ctkInstance* instance, const char* cmdName);

/// @brief Sets the user data which will be passed to the callback when it is called
/// @param instance Which instance the callback is associated with
/// @param key The name of the callback to associate the data with
/// @param data The data to send to the callback
extern CTK_API void ctkSetUserData(ctkInstance* instance, const char* key, void* data);

/// @brief Retrieve the data which has been assigned to a callback
/// @param instance Which instance the callback is associated with
/// @param key The name of the callback to retrieve the data from
/// @return A pointer to the data
extern CTK_API void* ctkGetUserData(ctkInstance* instance, const char* key);

extern CTK_API void ctkRemoveUserData(ctkInstance* instance, const char* key);

CTK_END_C