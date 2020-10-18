#include"Graphics.h"
#include<sstream>

Graphics::Graphics(HWND hWnd){
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = (HWND)69420;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	if (FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	))) {
		GFX_LAST_EXCEPT();
	}

	ID3D11Resource* pBackBuf = nullptr;
	
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuf));

	pDevice->CreateRenderTargetView(pBackBuf, nullptr, &pTarget);

	pBackBuf->Release();
}

Graphics::~Graphics() {
	if (pDevice != nullptr)
		pDevice->Release();

	if (pSwap != nullptr)
		pSwap->Release();

	if (pContext != nullptr)
		pContext->Release();

	if (pTarget != nullptr)
		pTarget->Release();
}

void Graphics::EndFrame() {
	pSwap->Present(1u, 0u);
}

void Graphics::ClearBuffer(float r, float g, float b) noexcept {
	const float colour[] = { r, g, b, 1.0 };
	pContext->ClearRenderTargetView(pTarget, colour);
}



Graphics::GfxException::GfxException(int line, const char* file, HRESULT hr) : Exception(line, file), hr(hr) {}

const char* Graphics::GfxException::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code]" << GetErrorCode() << std::endl
		<< "[Description]" << GetErrorString() << std::endl
		<< GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::GfxException::GetType() const noexcept {
	return "Window Exception";
}

std::string Graphics::GfxException::TranslateErrorCode(HRESULT hr) noexcept {
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

HRESULT Graphics::GfxException::GetErrorCode() const noexcept {
	return hr;
}

std::string Graphics::GfxException::GetErrorString() const noexcept {
	return TranslateErrorCode(hr);
}