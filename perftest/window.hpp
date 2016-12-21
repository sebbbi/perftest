#pragma once

#include "dataTypes.hpp"
#include <windows.h>

enum class MessageStatus
{
	Default,
	Exit
};

HWND createWindow(uint2 resolution/*, WNDPROC windowProc*/);
MessageStatus messagePump();
