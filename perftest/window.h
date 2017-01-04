#pragma once

#include "dataTypes.h"
#include <windows.h>

enum class MessageStatus
{
	Default,
	Exit
};

HWND createWindow(uint2 resolution/*, WNDPROC windowProc*/);
MessageStatus messagePump();
