#include"Component.h"
#include"Entity.h"

Component::Component(){}
Component::~Component() {
	entity = nullptr;
}

void Shape::UpdateTranslation(Graphics& gfx) {
	VertexConstBuffer vcb = { {
			DirectX::XMMatrixTranspose(
				DirectX::XMMatrixScaling(entity->trans.Scaling.x, entity->trans.Scaling.y, entity->trans.Scaling.z) *
				DirectX::XMMatrixRotationX(entity->trans.Rotation.x) *
				DirectX::XMMatrixRotationY(entity->trans.Rotation.y) *
				DirectX::XMMatrixRotationZ(entity->trans.Rotation.z) *
				DirectX::XMMatrixTranslation(entity->trans.Position.x, entity->trans.Position.y, entity->trans.Position.z) *
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
	case UsingTexture::FireBall:
		fileName = (wchar_t*)L"FireBall.bmp";
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

Cuboid::Cuboid(Graphics& gfx, Entity* ent, FVec3 sizeXYZ, bool isWrapped, UsingTexture ut) : isWrapped(isWrapped), tex(gfx, ut){
	entity = ent;
	CreateBuffers(gfx);
	CreateShadersAndInputLayout(gfx);
}
void Cuboid::CreateBuffers(Graphics& gfx){
	std::vector<Vertex> verts; 
	std::vector<unsigned short> inds;
	if (isWrapped) {
		verts = std::vector<Vertex>{
			{ 1.0,  1.0,  1.0,  0.0,  0.0},	//RUB 0
			{ 1.0,  1.0, -1.0,  1.0,  0.0},	//RUF 1
			{ 1.0, -1.0,  1.0,  0.0,  1.0},	//RDB 2
			{ 1.0, -1.0, -1.0,  1.0,  1.0},	//RDF 3
			{-1.0,  1.0,  1.0,  1.0,  0.0},	//LUB 4
			{-1.0,  1.0, -1.0,  0.0,  0.0},	//LUF 5
			{-1.0, -1.0,  1.0,  1.0,  1.0},	//LDB 6
			{-1.0, -1.0, -1.0,  0.0,  1.0},	//LDF 7
			};
		inds = std::vector<unsigned short> {
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
		NumOfInds = inds.size();
	}
	else {
		verts = std::vector<Vertex> {
			{-1.0, -1.0, -1.0, 0.0 , 0.5}, //LDF 0
			{-1.0,  1.0, -1.0, 0.0 , 0.0}, //LUF 1
			{ 1.0, -1.0, -1.0, 0.25, 0.5}, //RDF 2
			{ 1.0,  1.0, -1.0, 0.25, 0.0}, //RUF 3

			{ 1.0, -1.0,  1.0, 0.5 , 0.5}, //RDB 4
			{ 1.0,  1.0,  1.0, 0.5 , 0.0}, //RUB 5

			{-1.0, -1.0,  1.0, 0.75, 0.5}, //LDB 6
			{-1.0,  1.0,  1.0, 0.75, 0.0}, //LUB 7

			{-1.0, -1.0, -1.0, 1.0 , 0.5}, //LDF 8
			{-1.0,  1.0, -1.0, 1.0 , 0.0}, //LDF 9

			{-1.0,  1.0, -1.0, 0.0 , 1.0}, //RUF 10
			{-1.0,  1.0,  1.0, 0.0 , 0.5}, //RUB 11
			{ 1.0,  1.0, -1.0, 0.25, 1.0}, //LUF 12
			{ 1.0,  1.0,  1.0, 0.25, 0.5}, //LUB 13

			{-1.0, -1.0,  1.0, 0.25, 1.0}, //LUB 14
			{-1.0, -1.0, -1.0, 0.25, 0.5}, //LUB 15
			{ 1.0, -1.0,  1.0, 0.5 , 1.0}, //LUB 16
			{ 1.0, -1.0, -1.0, 0.5 , 0.5}, //LUB 17
		};
		inds = std::vector<unsigned short> {
			0, 1, 2,
			2, 1, 3,

			2, 3, 4,
			4, 3, 5,

			4, 5, 6,
			6, 5, 7,

			6, 7, 8,
			8, 7, 9,

			10, 11, 12,
			12, 11, 13,

			14, 15, 16,
			16, 15, 17,
		};
		NumOfInds = inds.size();
	}
	D3D11_BUFFER_DESC vDesc = CD3D11_BUFFER_DESC(sizeof(Vertex)*verts.size(), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0u, 0u, sizeof(Vertex));
	D3D11_SUBRESOURCE_DATA vData = { 0 };
	vData.pSysMem = verts.data();
	gfx.GetDevice()->CreateBuffer(&vDesc, &vData, &pVertexBuffer);
	
	D3D11_BUFFER_DESC iDesc = CD3D11_BUFFER_DESC(sizeof(short) * inds.size(), D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0u, 0u, sizeof(short));
	D3D11_SUBRESOURCE_DATA iData = { 0 };
	iData.pSysMem = inds.data();
	gfx.GetDevice()->CreateBuffer(&iDesc, &iData, &pIndexBuffer);
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

Square::Square(Graphics& gfx, Entity* ent, FVec3 sizeXY, UsingTexture ut) : tex(gfx, ut) {
	entity = ent;
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