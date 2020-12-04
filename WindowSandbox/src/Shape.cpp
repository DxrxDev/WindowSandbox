#include"Shape.h"

void Shape::UpdatePosition(float* position) {
	pos[0] = *position;
	pos[1] = *(position + 1);
	pos[2] = *(position + 2);
}

void Shape::UpdateRotation(float* rotation) {
	rot[0] = *rotation;
	rot[1] = *(rotation + 1);
	rot[2] = *(rotation + 2);
}

void Shape::UpdateScaling(float* scaling) {
	scal[0] = *scaling;
	scal[1] = *(scaling + 1);
	scal[2] = *(scaling + 2);
}

void Shape::UpdateTranslation(Graphics& gfx) {
	VertexConstBuffer vcb = { {
			DirectX::XMMatrixTranspose(
				DirectX::XMMatrixScaling(scal[0], scal[1], scal[2]) *
				DirectX::XMMatrixRotationX(rot[0]) *
				DirectX::XMMatrixRotationY(rot[1]) *
				DirectX::XMMatrixRotationZ(rot[2]) *
				DirectX::XMMatrixTranslation(pos[0], pos[1], pos[2]) *
				DirectX::XMMatrixPerspectiveLH(1.0, 1.0, 0.5, 10.0)
			)
		}
	};
	D3D11_BUFFER_DESC desc = CD3D11_BUFFER_DESC(sizeof(VertexConstBuffer), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 0u, sizeof(vcb));
	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = &vcb;
	gfx.GetDevice()->CreateBuffer(&desc, &data, &pConstVertBuffer);
}

Texture::Texture(Graphics& gfx, UsingTexture usingTex) {
	wchar_t* fileName = (wchar_t*)L"";
	switch (usingTex) {
	case UsingTexture::MissingTex:
		fileName = (wchar_t*)L"MissTex.bmp";
		break;
	case UsingTexture::Player:
		fileName = (wchar_t*)L"Player.bmp";
		break;
	}
	FillPixels(fileName);
	CreateSampler(gfx);
	CreateTexResView(gfx);
}
int Texture::GetWidth(){
	return width;
}
int Texture::GetHeight() {
	return height;
}
unsigned long* Texture::GetPixels() const {
	return pPixels.get();
}
void Texture::FillPixels(const wchar_t* file) {
	ULONG_PTR gdiStartupToken;
	Gdiplus::GdiplusStartupInput gdiStartupInput = {};
	gdiStartupInput.GdiplusVersion = 1;
	gdiStartupInput.DebugEventCallback = 0;
	gdiStartupInput.SuppressBackgroundThread = 0;
	gdiStartupInput.SuppressExternalCodecs = 0;
	Gdiplus::GdiplusStartupOutput gdiStartupOutput = { 0 };
	Gdiplus::GdiplusStartup(&gdiStartupToken, &gdiStartupInput, &gdiStartupOutput);
	{
		Gdiplus::Bitmap bitmap(file, 0);
		width = bitmap.GetWidth();
		height = bitmap.GetHeight();

		std::unique_ptr<unsigned long[]> pTemp;
		pTemp = std::make_unique<unsigned long[]>(width * height);

		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				Gdiplus::Color c;
				bitmap.GetPixel(x, y, &c);
				pTemp[y * width + x] = { c.GetValue() };
			}
		}
		pPixels = std::move(pTemp);
	}
	Gdiplus::GdiplusShutdown(gdiStartupToken);
}
void Texture::CreateSampler(Graphics& gfx) {
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;

	gfx.GetDevice()->CreateSamplerState(&samplerDesc, &pSampler);
}
void Texture::CreateTexResView(Graphics& gfx) {
	D3D11_TEXTURE2D_DESC texDesc = { 0 };
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1u;
	texDesc.ArraySize = 1u;
	texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.SampleDesc.Count = 1u;
	texDesc.SampleDesc.Quality = 0u;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0u;
	texDesc.MiscFlags = 0u;

	D3D11_SUBRESOURCE_DATA texData = { 0 };
	texData.pSysMem = pPixels.get();
	texData.SysMemPitch = width * sizeof(long);

	gfx.GetDevice()->CreateTexture2D(&texDesc, &texData, &pTexture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0u;
	srvDesc.Texture2D.MipLevels = 1u;
	gfx.GetDevice()->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pResView);
}

Cuboid::Cuboid(Graphics& gfx, float* sizeXYZ, UsingTexture ut) : tex(gfx, ut){
	UpdateScaling(sizeXYZ);
	CreateBuffers(gfx);
	CreateShadersAndInputLayout(gfx);
}
void Cuboid::CreateBuffers(Graphics& gfx){
	Vertex verts[] = {
		{ 1.0,  1.0,  1.0,  0.0,  0.0},	//RUB 0
		{ 1.0,  1.0, -1.0,  1.0,  0.0},	//RUF 1
		{ 1.0, -1.0,  1.0,  0.0,  1.0},	//RDB 2
		{ 1.0, -1.0, -1.0,  1.0,  1.0},	//RDF 3
		{-1.0,  1.0,  1.0,  1.0,  0.0},	//LUB 4
		{-1.0,  1.0, -1.0,  0.0,  0.0},	//LUF 5
		{-1.0, -1.0,  1.0,  1.0,  1.0},	//LDB 6
		{-1.0, -1.0, -1.0,  0.0,  1.0},	//LDF 7
	};
	D3D11_BUFFER_DESC vDesc = CD3D11_BUFFER_DESC(sizeof(verts), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0u, 0u, sizeof(Vertex));
	D3D11_SUBRESOURCE_DATA vData = { 0 };
	vData.pSysMem = &verts;
	gfx.GetDevice()->CreateBuffer(&vDesc, &vData, &pVertexBuffer);
	
	unsigned short inds[] = {
		7, 5, 3, //FRONT
		3, 5, 1,

		2, 0, 6, //BACK
		6, 0, 4,

		3, 1, 2, //RIGHT
		2, 1, 0,

		6, 4, 7, //LEFT
		7, 4, 5,

		5, 4, 1, //TOP
		1, 4, 0,

		6, 7, 2, //BOTTOM
		2, 7, 3,
	};
	D3D11_BUFFER_DESC iDesc = CD3D11_BUFFER_DESC(sizeof(inds), D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0u, 0u, sizeof(short));
	D3D11_SUBRESOURCE_DATA iData = { 0 };
	iData.pSysMem = &inds;
	gfx.GetDevice()->CreateBuffer(&iDesc, &iData, &pIndexBuffer);
	NumOfInds = 36;
}
void Cuboid::CreateShadersAndInputLayout(Graphics& gfx) {
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	gfx.GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), 0u, &pPixelShader); 
	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	gfx.GetDevice()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), 0u, &pVertexShader);

	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	gfx.GetDevice()->CreateInputLayout(ied, 2u, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);
}
void Cuboid::Draw(Graphics& gfx){
	UpdateTranslation(gfx);
	UINT strides = sizeof(Vertex);
	UINT offsets = 0u;
	gfx.GetContext()->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &strides, &offsets);
	gfx.GetContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	gfx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx.GetContext()->IASetInputLayout(pInputLayout.Get());
	gfx.GetContext()->VSSetConstantBuffers(0u, 1u, pConstVertBuffer.GetAddressOf());
	gfx.GetContext()->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	gfx.GetContext()->PSSetSamplers(0u, 1u, tex.pSampler.GetAddressOf());
	gfx.GetContext()->PSSetShaderResources(0u, 1u, tex.pResView.GetAddressOf());
	gfx.GetContext()->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	gfx.DrawIndexed(NumOfInds);
}

Square::Square(Graphics& gfx, float* sizeXY, UsingTexture ut) : tex(gfx, ut) {
	UpdateScaling(sizeXY);
	CreateBuffers(gfx);
	CreateShadersAndInputLayout(gfx);
}
void Square::CreateBuffers(Graphics& gfx) {
	Vertex verts[] = {
		{-1, -1, 0,  0.0,  1.0},	//BL 0
		{-1,  1, 0,  0.0,  0.0},	//TL 1
		{ 1, -1, 0,  1.0,  1.0},	//BR 2
		{ 1,  1, 0,  1.0,  0.0},	//TR 3
	};
	D3D11_BUFFER_DESC vDesc = CD3D11_BUFFER_DESC(sizeof(verts), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0u, 0u, sizeof(Vertex));
	D3D11_SUBRESOURCE_DATA vData = { 0 };
	vData.pSysMem = &verts;
	gfx.GetDevice()->CreateBuffer(&vDesc, &vData, &pVertexBuffer);

	unsigned short inds[] = {
		0, 1, 2,
		2, 1, 3
	};
	D3D11_BUFFER_DESC iDesc = CD3D11_BUFFER_DESC(sizeof(inds), D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0u, 0u, sizeof(short));
	D3D11_SUBRESOURCE_DATA iData = { 0 };
	iData.pSysMem = &inds;
	gfx.GetDevice()->CreateBuffer(&iDesc, &iData, &pIndexBuffer);
	NumOfInds = 6;
}
void Square::CreateShadersAndInputLayout(Graphics& gfx) {
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	gfx.GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), 0u, &pPixelShader);
	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	gfx.GetDevice()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), 0u, &pVertexShader);

	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	gfx.GetDevice()->CreateInputLayout(ied, 2u, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);
}
void Square::Draw(Graphics& gfx) {
	UpdateTranslation(gfx);
	UINT strides = sizeof(Vertex);
	UINT offsets = 0u;
	gfx.GetContext()->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &strides, &offsets);
	gfx.GetContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	gfx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx.GetContext()->IASetInputLayout(pInputLayout.Get());
	gfx.GetContext()->VSSetConstantBuffers(0u, 1u, pConstVertBuffer.GetAddressOf());
	gfx.GetContext()->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	gfx.GetContext()->PSSetSamplers(0u, 1u, tex.pSampler.GetAddressOf());
	gfx.GetContext()->PSSetShaderResources(0u, 1u, tex.pResView.GetAddressOf());
	gfx.GetContext()->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	gfx.DrawIndexed(NumOfInds);
}