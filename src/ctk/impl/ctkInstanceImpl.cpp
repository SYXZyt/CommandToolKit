#include <ctk/impl/ctkManifestImpl.h>
#include <ctk/impl/ctkInstanceImpl.h>
#include <ctk/impl/ctkString.h>
#include <ctk/Interpreter/ctkCmdTokeniser.h>

ctkCallback ctkInstanceImpl::GetMatchingCallback(const ParseInfo& info)
{
	if (!callbacks.count(info.cmdName))
		return nullptr;

	ctkCallback callback = callbacks[info.cmdName];

	ctkManifestImpl* manifestImpl = reinterpret_cast<ctkManifestImpl*>(manifest);
	auto it = std::find_if(manifestImpl->entries.begin(), manifestImpl->entries.end(), [&info](const ctkManifestImpl::ctkEntry& entry)
		{
			if (entry.cmdName != info.cmdName.c_str())
				return false;
			if (entry.args.size() != info.args.size())
				return false;
			for (size_t i = 0; i < entry.args.size(); i++)
			{
				if (entry.args[i].argType == ctkType::CTK_TYPE_FLOAT && info.args[i].type != ctkType::CTK_TYPE_FLOAT)
					return false;
				if (entry.args[i].argType == ctkType::CTK_TYPE_STRING && info.args[i].type != ctkType::CTK_TYPE_STRING)
					return false;
			}
			return true;
		});

	if (it == manifestImpl->entries.end())
		return nullptr;

	return callback;
}

void ctkInstanceImpl::SetUserData(const char* key, void* data)
{
	userdata[key] = data;
}

void* ctkInstanceImpl::GetUserData(const char* key)
{
	if (userdata.count(key))
		return userdata[key];

	return nullptr;
}

ctkResult ctkInstanceImpl::RegisterCallback(const char* cmdName, ctkCallback callback)
{
	if (!cmdName || !callback)
		return ctkMakeResult("Name or callback were nullptr", ctkResult::CTK_INVALID_ARG);

	if (callbacks.count(cmdName))
		return ctkMakeResult("Callback already registered", ctkResult::CTK_CALLBACK_ALREADY_REGISTERED);

	callbacks[cmdName] = callback;

	return ctkResult::CTK_OK;
}

ctkResult ctkInstanceImpl::UnregisterCallback(const char* cmdName)
{
	if (!cmdName)
		return ctkMakeResult("Name was nullptr", ctkResult::CTK_INVALID_ARG);

	if (!callbacks.count(cmdName))
		return ctkMakeResult("Callback not found", ctkResult::CTK_CALLBACK_NOT_FOUND);

	callbacks.erase(cmdName);

	return ctkResult::CTK_OK;
}

ctkResult ctkInstanceImpl::ProcessCommand(const char* cmd)
{
	ctkCmdTokeniser tokeniser(cmd);

	std::vector<ctkToken> tokens = std::move(tokeniser.Tokenise());
	
	auto it = std::find_if(tokens.begin(), tokens.end(), [](const ctkToken& token)
		{
			return token.type == ctkTokenType::TOKENISE_ERROR;
		});

	if (it != tokens.end())
	{
		return ctkMakeResult(it->lexeme.c_str(), ctkResult::CTK_TOKENISE_CMD_ERROR);
	}

	ParseInfo info;

	ctkToken* current = tokens.data() + 1;

	//The minimum valid size is 2 which contains the name and eol, so if only one token is loaded, then no command was given. Likely a blank line.
	//We should probably just ignore this since it isn't an error to submit a blank line.
	if (tokens.size() == 1)
		return ctkResult::CTK_OK;
	
	//Now that we know we actually have a command, we can start parsing it
	if (tokens[0].type != ctkTokenType::IDENTIFIER)
		return ctkMakeResult("Expected command name", ctkResult::CTK_TOKENISE_CMD_ERROR);

	info.cmdName = tokens[0].lexeme.c_str();

	while (current->type != ctkTokenType::NONE)
	{
		//We expect either a string or a float
		if (current->type != ctkTokenType::STRING_LIT && current->type != ctkTokenType::FLOAT_LIT)
			return ctkMakeResult("Invalid parameter", ctkResult::CTK_TOKENISE_CMD_ERROR);

		ctkValue value{};
		value.type = current->type == ctkTokenType::STRING_LIT ? ctkType::CTK_TYPE_STRING : ctkType::CTK_TYPE_FLOAT; //Thanks C for the stupidly long names
		
		switch (value.type)
		{
			case ctkType::CTK_TYPE_STRING:
				value.s = current->lexeme.c_str();
				break;
			case ctkType::CTK_TYPE_FLOAT:
				value.f = std::stof(current->lexeme.c_str());
				break;
		}

		info.args.push_back(value);

		++current;
	}

	//Now we have successfully parsed, we now need to check for any loaded commands which match
	//It may be that we have the right name, but the args don't match what have been loaded in the manifest
	ctkCallback callback = GetMatchingCallback(info);

	if (!callback)
		return ctkMakeResult("No matching command found", ctkResult::CTK_NO_MATHCHING_CALLBACK);

	ctkResult res = callback(reinterpret_cast<ctkInstance*>(this), info.args.data(), info.args.size(), GetUserData(info.cmdName.c_str()));

	//If the type is ok then we should our own success message, otherwise any other code indicates the user submitted their own message
	if (res == ctkResult::CTK_OK)
		return ctkMakeResult("Ok", ctkResult::CTK_OK);

	//If the code was OK with a message, then change the type to just OK to prevent any error catches
	if (res == ctkResult::CTK_OK_MSG)
		return ctkResult::CTK_OK;

	return res;
}

void ctkInstanceImpl::AppendManifest(const ctkManifest& manifest)
{
	ctkManifestImpl* manifestToAppend = reinterpret_cast<ctkManifestImpl*>(const_cast<ctkManifest*>(&manifest));
	ctkManifestImpl* appendTo = reinterpret_cast<ctkManifestImpl*>(this->manifest);

	if (!manifestToAppend)
		return;

	if (!appendTo)
	{
		appendTo = new ctkManifestImpl();
		this->manifest = reinterpret_cast<ctkManifest*>(appendTo);
	}

	appendTo->entries.insert(appendTo->entries.end(), manifestToAppend->entries.begin(), manifestToAppend->entries.end());
}