/**********************************************/
/* THIS IS FOR INTERNAL CTK USE. DO NOT USE! */
/********************************************/

#pragma once
#include <ctk/impl/Interpreter/ctkITokeniser.h>

class ctkCmdTokeniser final : public ctkITokeniser
{
private:
	ctkToken CreateIdentifierToken();
	ctkToken CreateStringToken(char quoteType);
	ctkToken CreateFloatToken();

public:
	std::vector<ctkToken>&& Tokenise() final override;

	ctkCmdTokeniser(std::string source) : ctkITokeniser(std::move(source)) {}
};