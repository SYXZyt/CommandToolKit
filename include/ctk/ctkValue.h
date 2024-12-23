#pragma once
#include <ctk/ctkDllApi.h>
#include <ctk/ctkType.h>

CTK_BEGIN_C

/// @brief A value which can be passed to a command
typedef struct ctkValue
{
	union
	{
		/// @brief The floating point value
		float f;

		/// @brief The string value
		const char* s;
	};

	/// @brief The type of the value
	ctkType type;
} ctkValue;

CTK_END_C