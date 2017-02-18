#include "Error.h"

void Error(const char* message)
{
	MessageBoxA(0, message, "Error", MB_OK);

	if (IsDebuggerPresent())
	{
		_asm int 3;
	}
	else
	{
		_exit(1);
	}
}