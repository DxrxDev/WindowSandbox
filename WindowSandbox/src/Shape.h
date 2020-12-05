#pragma once
#include"Graphics.h"

struct Vertex {
	float x, y, z;
	float u, v;
};
struct VertexConstBuffer {
	DirectX::XMMATRIX trans;
};

enum class UsingTexture {
	MissingTex, Player, FireBall
};

class Texture {
public:
	Texture(Graphics& gfx, UsingTexture usingTex);
	unsigned long* GetPixels() const;
	int GetWidth();
	int GetHeight();
public:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pResView = nullptr;
private:
	void FillPixels(const wchar_t* file);
	void CreateSampler(Graphics& gfx);
	void CreateTexResView(Graphics& gfx);
private:
	std::unique_ptr<unsigned long[]> pPixels;
	int width, height;
};

struct FVec3 {
	float x, y, z;
};
struct Translation{
	FVec3 Position, Rotation, Scaling;
};
class Shape {
public:
	Shape() = default;
	~Shape() = default;
	void UpdatePosition(FVec3 position);
	void UpdateRotation(FVec3 rotation);
	void UpdateScaling(FVec3 scaling);

	FVec3 GetPosition();
	FVec3 GetRotation();
	FVec3 GetScaling();

	virtual void Draw(Graphics& gfx) = 0;
protected:
	Translation trans = {
		{0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0},
		{1.0, 1.0, 1.0}
	};

	virtual void CreateBuffers(Graphics& gfx) = 0;
	virtual void CreateShadersAndInputLayout(Graphics& gfx) = 0;
	virtual void UpdateTranslation(Graphics& gfx);
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstVertBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout = nullptr;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
	int NumOfInds = 0;
};

class Cuboid : public Shape {
public:
	Cuboid(Graphics& gfx, FVec3 sizeXYZ, bool isWrapped = true, UsingTexture ut = UsingTexture::MissingTex);
	~Cuboid() = default;

	virtual void Draw(Graphics& gfx) override;
private:
	virtual void CreateBuffers(Graphics& gfx) override;
	virtual void CreateShadersAndInputLayout(Graphics& gfx) override;
private:
	bool isWrapped = true;
	Texture tex;
};

class Square : public Shape {
public:
	Square(Graphics& gfx, FVec3 sizeXY, UsingTexture ut = UsingTexture::MissingTex);
	~Square() = default;

	virtual void Draw(Graphics& gfx) override;
private:
	virtual void CreateBuffers(Graphics& gfx) override;
	virtual void CreateShadersAndInputLayout(Graphics& gfx) override;
private:
	Texture tex;
};

/*
class UniqueShape : public Shape {
public:
	template<size_t vSize, size_t iSize>
	UniqueShape(Graphics& gfx, std::array<Vertex, vSize> vertices, std::array<unsigned short, iSize> indeces);
};
*/