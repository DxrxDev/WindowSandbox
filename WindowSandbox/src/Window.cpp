#include"Window.h"
#include"resource.h"
#include"tools/WndExcMacros.h"
#include<sstream>

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr)){
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass(){
	UnregisterClass(wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName () noexcept{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept{
	return wndClass.hInst;
}


// Window Stuff
Window::Window(int width, int height, const char* name) : width(width), height(height) {
	// create window & get hWnd
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0){
		throw WND_LAST_EXCEPT();
	};

	hWnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);
	
	if (hWnd == nullptr) {
		throw WND_LAST_EXCEPT();
	}

	// show window
	ShowWindow(hWnd, SW_SHOWDEFAULT);

	pGfx = std::make_unique<Graphics>(hWnd);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

Graphics& Window::Gfx() { 
	if (!pGfx) { 
		throw std::exception("Graphics = nullptr"); 
	} 
	return *pGfx; 
}

std::optional<int> Window::ProcessMessages() {
	MSG msg;
	while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)){
		if (msg.message == WM_QUIT){
			return (int)msg.wParam;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}


void Window::ChangeTitle(const char* str) {
	if (SetWindowText(hWnd, str) == 0) {
		throw WND_LAST_EXCEPT();
	}
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept{
	POINTS pt = MAKEPOINTS(lParam);
	switch (msg) {
		// we don't want the DefProc to handle this message because
		// we want our destructor to destroy the window, so return 0 instead of break
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		break;

	case WM_KILLFOCUS:
		kbd.ClearState();
		break;

		//---------------[KEYBOARD]---------------//
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) {
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;

	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;

		//---------------[MOUSE]---------------//
	case WM_LBUTTONDOWN: 
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	
	case WM_LBUTTONUP:
		// POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	
	case WM_RBUTTONDOWN:
		// POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	
	case WM_RBUTTONUP:
		// POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	
	case WM_MBUTTONDOWN:
		// POINTS pt = MAKEPOINTS(lParam);
		mouse.OnMiddlePressed(pt.x, pt.y);
		break;
	
	case WM_MBUTTONUP:
		// POINTS pt = MAKEPOINTS(lParam);
		mouse.OnMiddleReleased(pt.x, pt.y);
		break;

	case WM_MOUSEWHEEL: 
		// POINTS pt = MAKEPOINTS(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
			mouse.OnWheelUp(pt.x, pt.y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
			mouse.OnWheelDown(pt.x, pt.y);
		}
		break;
	
	case WM_MOUSEMOVE: 
		mouse.OnMouseMove(pt.x, pt.y);
		break;
	
	case WM_SIZE:
		this->width = pt.x;
		this->height = pt.y;
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::WindowException::WindowException(int line, const char* file, HRESULT hr) : Exception(line, file), hr(hr){}

const char* Window::WindowException::what() const noexcept{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code]" << GetErrorCode() << std::endl
		<< "[Description]" << GetErrorString() << std::endl
		<< GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::WindowException::GetType() const noexcept{
	return "Window Exception";
}

std::string Window::WindowException::TranslateErrorCode(HRESULT hr) noexcept {
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);
	if (nMsgLen == 0) {
		return "Undefined Error Code";
	}
	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Window::WindowException::GetErrorCode() const noexcept {
	return hr;
}

std::string Window::WindowException::GetErrorString() const noexcept {
	return TranslateErrorCode(hr);
}