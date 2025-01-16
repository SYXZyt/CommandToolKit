#include <ctk/impl/ctkManifestImpl.h>
#include <vector>

#define EOF_CHECK do { if (current->type == ctkTokenType::NONE) return ctkMakeResult("Unexpected EOF", ctkResult::CTK_PARSE_MANIFEST_ERROR); } while (false)
#define CLN_CHECK do { if (current->type != ctkTokenType::COLON) return ctkMakeResult("Expected colon", ctkResult::CTK_PARSE_MANIFEST_ERROR); else ++current; } while (false)
#define SKIP_COMMA do { if (current->type == ctkTokenType::COMMA) ++current; } while (false)

ctkResult ctkManifestImpl::Load(ctkToken* tokens, size_t tokenCount)
{
	/*
	Parse the following
	Terminals are in caps and non-terminals are in lower. * refers to 0 or more, + refers to 1 or more. Optionals are in {}

	manifest : IDENTIFIER parameter* SEMICOLON;
	paremeter : IDENTIFIER COLON type { COMMA };
	type : "float" | "string";
	*/

	ctkToken* current = &tokens[0];
	while (current->type != ctkTokenType::NONE)
	{
		if (current->type == ctkTokenType::TOKENISE_ERROR)
			return ctkMakeResult(current->lexeme.c_str(), ctkResult::CTK_TOKENISE_MANIFEST_ERROR);

		if (current->type != ctkTokenType::IDENTIFIER)
			return ctkMakeResult("Expected identifer for command name", ctkResult::CTK_PARSE_MANIFEST_ERROR);
		
		ctkEntry entry;
		entry.cmdName = current->lexeme;
		
		++current;

		//Since the parameter is optional as it occurs 0 or more times, we should check for a semi colon here
		EOF_CHECK;

		while (current->type != ctkTokenType::SEMI)
		{
			if (current->type != ctkTokenType::IDENTIFIER)
				return ctkMakeResult("Expected identifer for parameter name", ctkResult::CTK_PARSE_MANIFEST_ERROR);

			ctkEntry::ctkArgTemplate arg;
			arg.argName = current->lexeme;

			++current;

			CLN_CHECK;
			EOF_CHECK;

			if (current->type == ctkTokenType::KWD_FLOAT)
				arg.argType = ctkType::CTK_TYPE_FLOAT;
			else if (current->type == ctkTokenType::KWD_STRING)
				arg.argType = ctkType::CTK_TYPE_STRING;
			else
				return ctkMakeResult("Unknown parameter type", ctkResult::CTK_PARSE_MANIFEST_ERROR);

			++current;
			EOF_CHECK;

			SKIP_COMMA;

			//Here, we either hit another iden and loop, or a semi and we begin the next command
			entry.args.push_back(arg);
		}

		//Skip the semicolon
		++current;

		entries.push_back(entry);
	}

	return ctkResult::CTK_OK;
}

ctkManifestImpl::ctkManifestImpl(const ctkManifest& other) : entries(reinterpret_cast<const ctkManifestImpl&>(other).entries) {}

#include <ctk/impl/Interpreter/ctkManifestTokeniser.h>
ctkResult ctkManifestImpl::CreateManifestFromSource(const ctkString& source, ctkManifest* manifest)
{
	ctkManifestTokeniser tokeniser(source.c_str());

	std::vector<ctkToken> tokens = tokeniser.Tokenise();
	return reinterpret_cast<ctkManifestImpl*>(manifest)->Load(tokens.data(), tokens.size());
}

#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
ctkResult ctkManifestImpl::CreateManifestFromFile(const ctkString& path, ctkManifest* manifest)
{
	if (!std::filesystem::exists(path.c_str()))
		return ctkMakeResult("File does not exist", ctkResult::CTK_FILE_ERROR);

	std::ifstream in(path.c_str());
	if (!in.good())
		return ctkMakeResult("Error opening file", ctkResult::CTK_FILE_ERROR);

	std::stringstream buffer;
	buffer << in.rdbuf();

	return CreateManifestFromSource(buffer.str().c_str(), manifest);
}

ctkManifestImpl::ctkEntry::ctkEntry(const ctkManifestImpl::ctkEntry& other)
{
	cmdName = other.cmdName;
	args = other.args;
}