#include <ctk/ctkResult.h>
#include <ctk/impl/ctkString.h>
#include <mutex>

static ctkString lastMessage;
static std::mutex mutex;

const char* ctkLastMessage()
{
	return lastMessage.c_str();
}

void ctkSetLastMessage(ctkString* msg)
{
	mutex.lock();
	lastMessage = *msg;
	mutex.unlock();
}

ctkResult ctkMakeResult(const char* msg, ctkResult type)
{
	mutex.lock();
	lastMessage = msg;
	mutex.unlock();

	return type;
}