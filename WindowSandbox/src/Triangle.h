#pragma once
#include"Graphics.h"

class Triangle {
public:
	Triangle();
	Triangle(Graphics& gfx, const wchar_t* vertexShaderPath, const wchar_t* pixelShaderPath);
	void Draw(Graphics& gfx);

	void CreateBuffers(Graphics& gfx);
	void CreateShadersAndInputLayout(Graphics& gfx, const wchar_t* vertexShaderPath, const wchar_t* pixelShaderPath);
	void CreateViewPort(Graphics&, int TLX, int TLY, int x, int y);
public:
	struct Vertex {
		float 
			x, y,
			r, g, b;
	};
private:
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex = nullptr;
	D3D11_VIEWPORT viewPort = {0};
	UINT numOfInds;
};