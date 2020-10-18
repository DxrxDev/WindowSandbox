#include"tools/WinInclude.h"
#include<sstream>
#include"Game.h"

INT CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR pCmdLine,
	int pCmdShow) {

	try {
		Game sandbox(512, 512, "Sandbox");
		sandbox.Go();
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