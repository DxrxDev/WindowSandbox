#pragma once
#include"tools/WinInclude.h"
#include"tools/Exception.h"
#include<d3d11.h>
#include<d3dcompiler.h>
#include<wrl.h>
#include<sstream>
#include<DirectXMath.h>
#include<vector>

namespace Gdiplus {
	using std::min;
	using std::max;
}
#include<gdiplus.h>
#pragma comment( lib,"gdiplus.lib" )

class Graphics {
private:
	class GraphicsException : public Exception {
	public:
		GraphicsException(int line, const char* file, HRESULT hr);
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
public:
	Graphics(HWND hWnd, int width, int height);
	~Graphics() = default;
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	Graphics(Graphics&&) = delete;
	Graphics& operator=(Graphics&&) = delete;

	Graphics& GetSelf();
	void DrawTriangle();
	void EndFrame();
	void ClearBuffer(float r, float g, float b);
	void DrawIndexed(size_t size);
	void CreateBindViewPort(int TLX, int TLY, int x, int y);

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();
private:
	void CreateSwapChain(HWND hWnd);
	void CreateRenderTargetView();
	void CreateDepthStencil(int width, int height);
private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
	D3D11_VIEWPORT viewPort = { 0 };
};