/**********************************************/
/* THIS IS FOR INTERNAL CTK USE. DO NOT USE! */
/********************************************/

#pragma once
#include <ctk/impl/Interpreter/ctkITokeniser.h>

class ctkManifestTokeniser final : public ctkITokeniser
{
private:
	ctkToken CreateIdentifierToken();

public:
	std::vector<ctkToken>&& Tokenise() final override;

	ctkManifestTokeniser(std::string source) : ctkITokeniser(std::move(source)) {}
};