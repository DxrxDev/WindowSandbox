#include<Windows.h>
#include"Window.h"
#include<sstream>

INT CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR pCmdLine,
	int pCmdShow) {

	try {
		Window window(512, 512, "Sandbox");

		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			while (!window.mouse.IsEmpty()) {
				const auto e = window.mouse.Read();
				if (e.GetType() == Mouse::Event::Type::Move) {
					std::ostringstream oss;
					oss << "x: " << e.GetX() << " y: " << e.GetY();
					window.ChangeTitle(oss.str().c_str());
				}
			}
		}

		// check if GetMessage call itself borked
		if (gResult == -1)
		{
			return -1;
		}

		// wParam here is the value passed to PostQuitMessage
		return msg.wParam;
	}
	catch (const Exception& ex) {
		MessageBoxA(nullptr, ex.what(), ex.GetType(), MB_OK | MB_ICONERROR);
	}
	catch (const std::exception& ex) {
		MessageBoxA(nullptr, ex.what(), "Standard Exception", MB_OK | MB_ICONERROR);
	}
	catch (...) {
		MessageBoxA(nullptr, "No Details Available", "Unknown Exception", MB_OK | MB_ICONERROR);
	}
	return -1;
}