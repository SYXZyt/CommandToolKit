#include <ctk/impl/ctkManifestImpl.h>
#include <ctk/impl/ctkInstanceImpl.h>
#include <ctk/impl/ctkString.h>
#include <ctk/impl/Interpreter/ctkCmdTokeniser.h>
#include <algorithm>

ctkCallback ctkInstanceImpl::GetMatchingCallback(const ParseInfo& info)
{
	mutex.lock();

	if (!callbacks.count(info.cmdName))
	{
		mutex.unlock();
		return nullptr;
	}

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
	{
		mutex.unlock();
		return nullptr;
	}

	mutex.unlock();
	return callback;
}

void ctkInstanceImpl::SetUserData(const char* key, void* data)
{
	mutex.lock();
	userdata[key] = data;
	mutex.unlock();
}

void* ctkInstanceImpl::GetUserData(const char* key)
{
	mutex.lock();
	if (userdata.count(key))
	{
		mutex.unlock();
		return userdata[key];
	}

	mutex.unlock();
	return nullptr;
}

void ctkInstanceImpl::RemoveUserData(const char* key)
{
	mutex.lock();

	if (userdata.count(key))
		userdata.erase(key);

	mutex.unlock();
}

ctkResult ctkInstanceImpl::RegisterCallback(const char* cmdName, ctkCallback callback)
{
	mutex.lock();

	if (!cmdName || !callback)
	{
		mutex.unlock();
		return ctkMakeResult("Name or callback were nullptr", ctkResult::CTK_INVALID_ARG);
	}

	if (callbacks.count(cmdName))
	{
		mutex.unlock();
		return ctkMakeResult("Callback already registered", ctkResult::CTK_CALLBACK_ALREADY_REGISTERED);
	}

	callbacks[cmdName] = callback;
	mutex.unlock();

	return ctkResult::CTK_OK;
}

ctkResult ctkInstanceImpl::UnregisterCallback(const char* cmdName)
{
	if (!cmdName)
		return ctkMakeResult("Name was nullptr", ctkResult::CTK_INVALID_ARG);

	if (!callbacks.count(cmdName))
		return ctkMakeResult("Callback not found", ctkResult::CTK_CALLBACK_NOT_FOUND);

	mutex.lock();
	callbacks.erase(cmdName);
	mutex.unlock();

	return ctkResult::CTK_OK;
}

ctkResult ctkInstanceImpl::ProcessCommand(const char* cmd)
{
	mutex.lock();

	ctkCmdTokeniser tokeniser(cmd);

	std::vector<ctkToken> tokens = std::move(tokeniser.Tokenise());
	
	auto it = std::find_if(tokens.begin(), tokens.end(), [](const ctkToken& token)
		{
			return token.type == ctkTokenType::TOKENISE_ERROR;
		});

	if (it != tokens.end())
	{
		mutex.unlock();
		return ctkMakeResult(it->lexeme.c_str(), ctkResult::CTK_TOKENISE_CMD_ERROR);
	}

	ParseInfo info;

	ctkToken* current = tokens.data() + 1;

	//The minimum valid size is 2 which contains the name and eol, so if only one token is loaded, then no command was given. Likely a blank line.
	//We should probably just ignore this since it isn't an error to submit a blank line.
	if (tokens.size() == 1)
	{
		mutex.unlock();
		return ctkResult::CTK_OK;
	}
	
	//Now that we know we actually have a command, we can start parsing it
	if (tokens[0].type != ctkTokenType::IDENTIFIER)
	{
		mutex.unlock();
		return ctkMakeResult("Expected command name", ctkResult::CTK_TOKENISE_CMD_ERROR);
	}

	info.cmdName = tokens[0].lexeme.c_str();

	while (current->type != ctkTokenType::NONE)
	{
		//We expect either a string or a float
		if (current->type != ctkTokenType::STRING_LIT && current->type != ctkTokenType::FLOAT_LIT)
		{
			mutex.unlock();
			return ctkMakeResult("Invalid parameter", ctkResult::CTK_TOKENISE_CMD_ERROR);
		}

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
	{
		mutex.unlock();
		return ctkMakeResult("No matching command found, or no callback found", ctkResult::CTK_NO_MATHCHING_CALLBACK);
	}

	ctkResult res = callback(reinterpret_cast<ctkInstance*>(this), info.args.data(), info.args.size(), GetUserData(info.cmdName.c_str()));

	//If the type is ok then we should our own success message, otherwise any other code indicates the user submitted their own message
	if (res == ctkResult::CTK_OK)
	{
		mutex.unlock();
		return ctkMakeResult("Ok", ctkResult::CTK_OK);
	}

	//If the code was OK with a message, then change the type to just OK to prevent any error catches
	if (res == ctkResult::CTK_OK_MSG)
	{
		mutex.unlock();
		return ctkResult::CTK_OK;
	}

	mutex.unlock();
	return res;
}

void ctkInstanceImpl::AppendManifest(const ctkManifest& manifest)
{
	mutex.lock();

	ctkManifestImpl* manifestToAppend = reinterpret_cast<ctkManifestImpl*>(const_cast<ctkManifest*>(&manifest));
	ctkManifestImpl* appendTo = reinterpret_cast<ctkManifestImpl*>(this->manifest);

	if (!manifestToAppend)
	{
		mutex.unlock();
		return;
	}

	if (!appendTo)
	{
		appendTo = new ctkManifestImpl();
		this->manifest = reinterpret_cast<ctkManifest*>(appendTo);
	}

	appendTo->entries.insert(appendTo->entries.end(), manifestToAppend->entries.begin(), manifestToAppend->entries.end());
	mutex.unlock();
}

void ctkInstanceImpl::RemoveManifest(const ctkManifest& manifest)
{
	ctkManifestImpl* manifestToRemove = reinterpret_cast<ctkManifestImpl*>(const_cast<ctkManifest*>(&manifest));
	ctkManifestImpl* removeFrom = reinterpret_cast<ctkManifestImpl*>(this->manifest);

	if (!manifestToRemove || !removeFrom)
		return;

	mutex.lock();
	for (const ctkManifestImpl::ctkEntry& entry : manifestToRemove->entries)
		RemoveCommand(entry.cmdName.c_str());

	mutex.unlock();
}

void ctkInstanceImpl::RemoveCommand(const char* cmdName)
{
	if (!cmdName)
		return;

	mutex.lock();
	auto it = std::find_if(reinterpret_cast<ctkManifestImpl*>(manifest)->entries.begin(), reinterpret_cast<ctkManifestImpl*>(manifest)->entries.end(), [cmdName](const ctkManifestImpl::ctkEntry& entry)
		{
			return entry.cmdName == cmdName;
		});
	if (it != reinterpret_cast<ctkManifestImpl*>(manifest)->entries.end())
		reinterpret_cast<ctkManifestImpl*>(manifest)->entries.erase(it);
	mutex.unlock();
}