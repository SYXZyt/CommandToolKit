/**********************************************/
/* THIS IS FOR INTERNAL CTK USE. DO NOT USE! */
/********************************************/

#pragma once
#include <vector>
#include <ctk/Interpreter/ctkToken.h>
#include <string>

//Since both the cmd and manifest parsers and tokenisers use the same base, and just use a different tokenise check
//We can define an interface for the tokeniser to use to reduce how much code we have to re-write
class ctkITokeniser
{
protected:
	std::vector<ctkToken> tokens;
	std::string source;
	char currentChar;
	int pos;

	void Advance(unsigned int count = 1u)
	{
		pos += count;

		if (pos >= source.size())
			currentChar = '\0';
		else
			currentChar = source[pos];
	}

public:
	virtual std::vector<ctkToken>&& Tokenise() = 0;

	ctkITokeniser(std::string source) : source(std::move(source)), currentChar('\0'), pos(-1)
	{
		Advance();
	}

	virtual ~ctkITokeniser() = default;
};