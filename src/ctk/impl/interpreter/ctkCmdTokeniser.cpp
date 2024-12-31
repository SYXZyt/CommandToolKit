#include <ctk/impl/Interpreter/ctkCmdTokeniser.h>

static bool IsIdentifierChar(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

static bool IsNumeric(char c)
{
	return c >= '0' && c <= '9' || c == '.';
}

static bool IsWhitespaceChar(char c)
{
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

ctkToken ctkCmdTokeniser::CreateIdentifierToken()
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

	return token;
}

ctkToken ctkCmdTokeniser::CreateStringToken(char quoteType)
{
	std::string value = "";

	Advance();

	while (currentChar != quoteType)
	{
		if (currentChar == '\0')
		{
			std::string message = std::string("Unterminated string");
			ctkToken token{};
			token.lexeme = ctkString(message.c_str());
			token.type = ctkTokenType::TOKENISE_ERROR;
			tokens.push_back(token);
			return token;
		}

		value += currentChar;
		Advance();
	}

	Advance();

	ctkToken token{};
	token.lexeme = value.c_str();
	token.type = ctkTokenType::STRING_LIT;

	return token;
}

ctkToken ctkCmdTokeniser::CreateFloatToken()
{
	std::string stringRepr;
	int decimals = 0;

	if (currentChar == '-')
	{
		stringRepr += currentChar;
		Advance();

		if (!IsNumeric(currentChar))
		{
			std::string message = std::string("Invalid float '") + stringRepr + "'";
			ctkToken token{};
			token.lexeme = ctkString(message.c_str());
			token.type = ctkTokenType::TOKENISE_ERROR;
			tokens.push_back(token);
			return token;
		}
	}

	do
	{
		if (currentChar == '.')
			++decimals;

		stringRepr += currentChar;
		Advance();
	} while (IsNumeric(currentChar));

	if (decimals > 1)
	{
		std::string message = std::string("Invalid float '") + stringRepr + "'";
		ctkToken token{};
		token.lexeme = ctkString(message.c_str());
		token.type = ctkTokenType::TOKENISE_ERROR;
		tokens.push_back(token);
		return token;
	}

	ctkToken token{};
	token.lexeme = stringRepr.c_str();
	token.type = ctkTokenType::FLOAT_LIT;

	return token;
}

std::vector<ctkToken>&& ctkCmdTokeniser::Tokenise()
{
	while (currentChar != '\0')
	{
		if (IsWhitespaceChar(currentChar))
		{
			Advance();
			continue;
		}

		if (IsNumeric(currentChar) || currentChar == '-')
			tokens.push_back(CreateFloatToken());
		else if (IsIdentifierChar(currentChar))
			tokens.push_back(CreateIdentifierToken());
		else if (currentChar == '\'')
			tokens.push_back(CreateStringToken('\''));
		else if (currentChar == '"')
			tokens.push_back(CreateStringToken('"'));
		else if (currentChar == '`')
			tokens.push_back(CreateStringToken('`'));
		else if (currentChar == ',')
			Advance();
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