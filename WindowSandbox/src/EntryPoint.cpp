#include<Windows.h>
#include"Window.h"

INT CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR pCmdLine,
	int pCmdShow) {
	
	Window window(512, 256, "Sandbox");

	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// check if GetMessage call itself borked
	if (gResult == -1)
	{
		return -1;
	}

	// wParam here is the value passed to PostQuitMessage
	return msg.wParam;

	return 0;
}