#include"Graphics.h"
#include<memory>
#include<DirectXMath.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")


#define GFX_EXCEPT() Graphics::GraphicsException(__LINE__, __FILE__, hr)
#define GFX_EXCEPT_THROW(code) if(FAILED(hr = code)){throw GFX_EXCEPT();}
#define GFX_LAST_EXCEPT() Graphics::GraphicsException(__LINE__, __FILE__, GetLastError())
#define GFX_LAST_EXCEPT_THROW(code) if(FAILED(code)){throw GFX_LAST_EXCEPT();}


Graphics::Graphics(HWND hWnd) {
	HRESULT hr;

#ifdef _DEBUG
	UINT layer = D3D11_CREATE_DEVICE_DEBUG;
#else
	UINT layer = 0u;
#endif

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
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	GFX_EXCEPT_THROW(
	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		layer,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	));

	Microsoft::WRL::ComPtr<ID3D11Resource>pBackBuf;
	GFX_EXCEPT_THROW(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuf));
	GFX_EXCEPT_THROW(pDevice->CreateRenderTargetView(pBackBuf.Get(), nullptr, &pTarget));

	D3D11_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthEnable = TRUE;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSS;
	GFX_EXCEPT_THROW(pDevice->CreateDepthStencilState(&dsd, &pDSS));

	pContext->OMSetDepthStencilState(pDSS.Get(), 1u);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDS;
	D3D11_TEXTURE2D_DESC dD = {};
	dD.Width = 512;
	dD.Height = 512;
	dD.MipLevels = 1u;
	dD.ArraySize = 1u;
	dD.Format = DXGI_FORMAT_D32_FLOAT;
	dD.SampleDesc.Count = 1u;
	dD.SampleDesc.Quality = 0u;
	dD.Usage = D3D11_USAGE_DEFAULT;
	dD.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GFX_EXCEPT_THROW(pDevice->CreateTexture2D(&dD, nullptr, &pDS));

	D3D11_DEPTH_STENCIL_VIEW_DESC dDsv = {};
	dDsv.Format = DXGI_FORMAT_D32_FLOAT;
	dDsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dDsv.Texture2D.MipSlice = 0u;

	GFX_EXCEPT_THROW(pDevice->CreateDepthStencilView(pDS.Get(), &dDsv, &pDSV));

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());
}

void Graphics::DrawTriangle(float angle, float mouseX, float mouseY) {
	HRESULT hr;

	struct Vertex {
		float x, y, z;
	};

	const Vertex vertices[] = {
		{  1.0f,  1.0f, -1.0f},//0 RUF
		{ -1.0f,  1.0f, -1.0f},//1 LUF
		{  1.0f, -1.0f, -1.0f},//2 RDF
		{ -1.0f, -1.0f, -1.0f},//3 LDF
		{  1.0f,  1.0f,  1.0f},//4 RUB
		{ -1.0f,  1.0f,  1.0f},//5 LUB
		{  1.0f, -1.0f,  1.0f},//6 RDB
		{ -1.0f, -1.0f,  1.0f},//7 LDB
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>pVBuff;
	D3D11_BUFFER_DESC vbd = {};
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.CPUAccessFlags = 0u;
	vbd.MiscFlags = 0u;
	vbd.ByteWidth = sizeof(vertices);
	vbd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA vsd = {};
	vsd.pSysMem = vertices;
	GFX_EXCEPT_THROW(pDevice->CreateBuffer(&vbd, &vsd, &pVBuff));

	const unsigned short index[] = {
		3, 2, 7,
		6, 7, 2,

		7, 5, 3,
		1, 3, 5,

		3, 1, 2,
		0, 2, 1,

		2, 0, 4,
		4, 6, 2,

		6, 4, 7,
		7, 4, 5,

		5, 0, 1,
		0, 5, 4, 
	};

	//create index buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer>pIBuff;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(index);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = index;
	GFX_EXCEPT_THROW(pDevice->CreateBuffer(&ibd, &isd, &pIBuff));

	//bind VB to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVBuff.GetAddressOf(), &stride, &offset);
	pContext->IASetIndexBuffer(pIBuff.Get(), DXGI_FORMAT_R16_UINT, 0u);

	struct ConstantBuffer2 {
		struct{
			float r, g, b, a;
		} colours[6];
	};

	const ConstantBuffer2 cb2 = { {
			{1.0, 0.0, 0.0, 1.0}, // bottom
			{0.0, 1.0, 0.0, 1.0}, // left
			{1.0, 1.0, 0.0, 1.0}, // front
			{0.0, 0.0, 1.0, 1.0}, // right
			{1.0, 1.0, 1.0, 1.0}, // back
			{1.0, 0.5, 0.0, 1.0}, // top
		}
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> pC2Buff;
	D3D11_BUFFER_DESC cb2d = {};
	cb2d.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb2d.Usage = D3D11_USAGE_DEFAULT;
	cb2d.CPUAccessFlags = 0u;
	cb2d.MiscFlags = 0u;
	cb2d.ByteWidth = sizeof(cb2);
	cb2d.StructureByteStride = sizeof(ConstantBuffer2);
	D3D11_SUBRESOURCE_DATA c2sd = {};
	c2sd.pSysMem = &cb2;
	GFX_EXCEPT_THROW(pDevice->CreateBuffer(&cb2d, &c2sd, &pC2Buff));

	pContext->PSSetConstantBuffers(0u, 1u, pC2Buff.GetAddressOf());

	//create PS then bind
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	GFX_EXCEPT_THROW(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	struct ConstantBuffer {
		DirectX::XMMATRIX transform;
	};

	const ConstantBuffer cb = { {
			DirectX::XMMatrixTranspose(
				DirectX::XMMatrixRotationY(angle) *
				DirectX::XMMatrixRotationX(angle/2) *
				DirectX::XMMatrixTranslation(0, 0, 5) *
				DirectX::XMMatrixPerspectiveLH(1.0, 1.0, 0.5, 10.0) *
				DirectX::XMMatrixTranslation(mouseX, mouseY, 0.0)
			)
		}
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> pCBuff;
	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = sizeof(ConstantBuffer);
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	GFX_EXCEPT_THROW(pDevice->CreateBuffer(&cbd, &csd, &pCBuff));

	pContext->VSSetConstantBuffers(0u, 1u, pCBuff.GetAddressOf());


	//create VS then bind
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVS;
	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	GFX_EXCEPT_THROW(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVS));

	pContext->VSSetShader(pVS.Get(), nullptr, 0u);

	//input VL (2d pos)
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pIL;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	GFX_EXCEPT_THROW(pDevice->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pIL));

	//bind vert layout
	pContext->IASetInputLayout(pIL.Get());

	//set primitive topology to triangle list (groups of 3 vertices)
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//config vp
	D3D11_VIEWPORT vp;
	vp.Width = 512;
	vp.Height = 512;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);


	pContext->DrawIndexed((UINT)std::size(index), 0u, 0u);
}

void Graphics::EndFrame() {
	HRESULT hr;
	GFX_EXCEPT_THROW(pSwap->Present(1u, 0u));
}

void Graphics::ClearBuffer(float r, float g, float b) {
	const float colour[] = { r, g, b, 1.0 };
	pContext->ClearRenderTargetView(pTarget.Get(), colour);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0, 0u);
}


//---------------[GRAPHICS EXCEPTION]---------------//
Graphics::GraphicsException::GraphicsException(int line, const char* file, HRESULT hr) : Exception(line, file), hr(hr) {}

const char* Graphics::GraphicsException::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code]" << GetErrorCode() << std::endl
		<< "[Description]" << GetErrorString() << std::endl
		<< GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::GraphicsException::GetType() const noexcept {
	return "Graphics Exception";
}

std::string Graphics::GraphicsException::TranslateErrorCode(HRESULT hr) noexcept {
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

HRESULT Graphics::GraphicsException::GetErrorCode() const noexcept {
	return hr;
}

std::string Graphics::GraphicsException::GetErrorString() const noexcept {
	return TranslateErrorCode(hr);
}