#include <ctk/ctkIO.h>
#include <stdio.h>

CTK_OUTPUT_CALLBACK(OnOutput)
{
	printf("%s\n", OnOutput_msg);
}

static ctkOutputCallback outputCallback = OnOutput;

void ctkSetOutputCallback(ctkOutputCallback callback)
{
	outputCallback = callback;
}

void ctkResetOutputCallback()
{
	outputCallback = OnOutput;
}

void ctkOutput(const char* msg)
{
	outputCallback(msg);
}

ctkOutputCallback ctkGetOutputCallback()
{
	return outputCallback;
}

ctkOutputCallback ctkGetDefaultOutputCallback()
{
	return OnOutput;
}