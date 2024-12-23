/**********************************************/
/* THIS IS FOR INTERNAL CTK USE. DO NOT USE! */
/********************************************/

#pragma once
#include <ctk/impl/ctkString.h>

enum class ctkTokenType : unsigned char
{
	NONE = 0u,
	TOKENISE_ERROR,

	IDENTIFIER,
	COLON,
	COMMA,
	SEMI,

	FLOAT_LIT,
	STRING_LIT,

	KWD_FLOAT,
	KWD_STRING,
};

struct ctkToken
{
	ctkString lexeme;
	ctkTokenType type;

	ctkToken() : lexeme(""), type(ctkTokenType::NONE) {}
	ctkToken(ctkString lexeme, ctkTokenType type) : lexeme(lexeme), type(type) {}
	ctkToken(const ctkToken& other) : lexeme(other.lexeme), type(other.type) {}
};