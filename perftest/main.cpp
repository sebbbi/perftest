#include "window.hpp"
#include "directx.hpp"

int main()
{
	int2 resolution = int2(1280, 720);
	HWND window = createWindow(resolution);
	DirectXDevice dx(window, resolution);

	MessageStatus status = MessageStatus::Default;
	do
	{
		dx.presentFrame();
		status = messagePump();
	}
	while (status != MessageStatus::Exit);

	return 0;
}