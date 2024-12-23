#include <ctk/ctkResult.h>
#include <ctk/impl/ctkString.h>

static ctkString lastMessage;

const char* ctkLastMessage()
{
	return lastMessage.c_str();
}

void ctkSetLastMessage(ctkString* msg)
{
	lastMessage = *msg;
}

ctkResult ctkMakeResult(const char* msg, ctkResult type)
{
	lastMessage = msg;

	return type;
}