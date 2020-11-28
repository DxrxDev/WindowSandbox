#include"Triangle.h"

Triangle::Triangle(){}

Triangle::Triangle(Graphics& gfx, const wchar_t* vertexShaderPath, const wchar_t* pixelShaderPath) {
	CreateBuffers(gfx);
	CreateShadersAndInputLayout(gfx, vertexShaderPath, pixelShaderPath);
	CreateViewPort(gfx, 0, 0, 512, 512);
}

void Triangle::Draw(Graphics& gfx) {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	gfx.GetContext()->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	gfx.GetContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	gfx.GetContext()->IASetInputLayout(pInputLayout.Get());
	gfx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx.GetContext()->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	gfx.GetContext()->RSSetViewports(1u, &viewPort);
	gfx.GetContext()->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	gfx.GetContext()->DrawIndexed(numOfInds, 0u, 0u);
}

void Triangle::CreateBuffers(Graphics& gfx){
	Vertex verts[] = {
		{-0.5, -0.5, 1.0, 0.0, 0.0},//BL 0
		{-0.5,  0.5, 0.0, 1.0, 0.0},//TL 1
		{ 0.5, -0.5, 0.0, 0.0, 1.0},//BR 2
		{ 0.5,  0.5, 0.0, 0.0, 0.0},//TR 3
	};
	auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(verts), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0u, 0u, sizeof(Vertex));
	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = verts;
	gfx.GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &pVertexBuffer);

	unsigned short inds[] = {
		0, 1, 2,
		2, 1, 3,
	};
	auto indexBufferDesc = CD3D11_BUFFER_DESC(sizeof(verts), D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0u, 0u, sizeof(short));
	D3D11_SUBRESOURCE_DATA indexData = { 0 };
	indexData.pSysMem = inds;
	gfx.GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &pIndexBuffer);
	numOfInds = std::size(inds);
}

void Triangle::CreateShadersAndInputLayout(Graphics& gfx, const wchar_t* vertexShaderPath, const wchar_t* pixelShaderPath){
	D3DReadFileToBlob(pixelShaderPath, &pBlob);
	gfx.GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	D3DReadFileToBlob(vertexShaderPath, &pBlob);
	gfx.GetDevice()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Colour", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	gfx.GetDevice()->CreateInputLayout(ied, 2u, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);
}

void Triangle::CreateViewPort(Graphics&, int TLX, int TLY, int x, int y) {
	viewPort.TopLeftX = TLX;
	viewPort.TopLeftY = TLY;
	viewPort.Width = x;
	viewPort.Height = y;
	viewPort.MaxDepth = 1;
}