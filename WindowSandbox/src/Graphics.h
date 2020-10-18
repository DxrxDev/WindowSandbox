#pragma once
#include"tools/WinInclude.h"
#include"tools/Exception.h"
#include<d3d11.h>

class Graphics {
public:
	class GfxException : Exception {
	public:
		GfxException(int line, const char* file, HRESULT hr);
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
public:
	Graphics(HWND hWnd);
	~Graphics();
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	Graphics(Graphics&&) = delete;
	Graphics& operator=(Graphics&&) = delete;

	void EndFrame();
	void ClearBuffer(float r, float g, float b) noexcept;
private:
	ID3D11Device* pDevice = 0;
	IDXGISwapChain* pSwap = 0;
	ID3D11DeviceContext* pContext = 0;
	ID3D11RenderTargetView* pTarget = 0;
};

#define GFX_EXCEPT(hr) Graphics::GfxException(__LINE__, __FILE__, hr)
#define GFX_LAST_EXCEPT() Graphics::GfxException(__LINE__, __FILE__, GetLastError())