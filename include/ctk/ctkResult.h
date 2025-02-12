#pragma once
#include <ctk/ctkDllApi.h>

CTK_BEGIN_C

/// @brief Possible failure states of the library
typedef enum ctkResult
{
	/// @brief No errors occurred
	CTK_OK = 0u,

	/// @brief No errors occurred, BUT a message was written into the last message buffer. This is only really used internally, so you don't ever need to check for it since it will be replaced with `CTK_OK` before it ever gets to the end user
	CTK_OK_MSG,

	/// @brief Something went wrong, but we don't know what. Don't use this, just make a different enum value for your error. This is here as a backup or for if an error is too obscure or hard to track down
	CTK_UNKNOWN_ERROR,

	/// @brief Something went wrong when working with a file. It may be corrupt, missing or have the wrong permissions
	CTK_FILE_ERROR,

	/// @brief An argument passed to a function was invalid such as a null pointer or a negative number
	CTK_INVALID_ARG,

	/// @brief Failed to get a matching callback for the command. This is likely because the arguments don't match the manifest or the callback hasn't been registered
	CTK_NO_MATHCHING_CALLBACK,

	/// @brief Failed to tokenise the manifest. Prehaps an invalid character was found or a keyword was misspelled
	CTK_TOKENISE_MANIFEST_ERROR,

	/// @brief Failed to tokenise the command. Prehaps an invalid character was found
	CTK_TOKENISE_CMD_ERROR,

	/// @brief Failed to parse the manifest. Likely a missing identifier or a missing semi colon, etc
	CTK_PARSE_MANIFEST_ERROR,

	/// @brief Failed to parse the command. Likely you missed the command name
	CTK_PARSE_CMD_ERROR,

	/// @brief Failed to find a callback with the given name
	CTK_CALLBACK_NOT_FOUND,

	/// @brief Tried to register a callback with a name that was already registered
	CTK_CALLBACK_ALREADY_REGISTERED,
} ctkResult;

/// @brief Get the last message which was written into the buffer 
extern CTK_API const char* ctkLastMessage();

/// @brief Set your own message into the buffer. Only for internal use. Use `ctkMakeResult` for external use
extern void ctkSetLastMessage(struct ctkString* msg);

extern CTK_API ctkResult ctkMakeResult(const char* msg, ctkResult type);

CTK_END_C