#include <ctk/Interpreter/ctkManifestTokeniser.h>

static bool IsIdentifierChar(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

static bool IsWhitespaceChar(char c)
{
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

ctkToken ctkManifestTokeniser::CreateIdentifierToken()
{
	std::string lexeme = "";

	do
	{
		lexeme += currentChar;
		Advance();
	} while (IsIdentifierChar(currentChar));

	ctkToken token{};
	token.lexeme = lexeme.c_str();
	token.type = ctkTokenType::IDENTIFIER;

	if (lexeme == "float")
		token.type = ctkTokenType::KWD_FLOAT;
	else if (lexeme == "string")
		token.type = ctkTokenType::KWD_STRING;

	return token;
}

std::vector<ctkToken>&& ctkManifestTokeniser::Tokenise()
{
	while (currentChar != '\0')
	{
		if (IsWhitespaceChar(currentChar))
		{
			Advance();
			continue;
		}

		if (IsIdentifierChar(currentChar))
		{
			tokens.push_back(CreateIdentifierToken());
			continue;
		}
		else if (currentChar == '#')
		{
			while (currentChar != '\n' && currentChar != '\0')
				Advance();
		}
		else if (currentChar == ',')
		{
			ctkToken token{};
			token.lexeme = ctkString(",");
			token.type = ctkTokenType::COMMA;

			tokens.push_back(token);
			Advance();
		}
		else if (currentChar == ':')
		{
			ctkToken token{};
			token.lexeme = ctkString(":");
			token.type = ctkTokenType::COLON;

			tokens.push_back(token);
			Advance();
		}
		else if (currentChar == ';')
		{
			ctkToken token{};
			token.lexeme = ctkString(";");
			token.type = ctkTokenType::SEMI;

			tokens.push_back(token);
			Advance();
		}
		else
		{
			std::string message = std::string("Unknown character '") + currentChar + "'";

			ctkToken token{};
			token.lexeme = ctkString(message.c_str());
			token.type = ctkTokenType::TOKENISE_ERROR;

			tokens.push_back(token);

			Advance();
		}
	}

	//Add a null token so the parser knows when to stop
	ctkToken token("", ctkTokenType::NONE);
	tokens.push_back(token);

	return std::move(tokens);
}