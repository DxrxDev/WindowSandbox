#pragma once
#include"Graphics.h"

class Entity;

struct Vertex {
	float x, y, z;
	float u, v;
};
struct VertexConstBuffer {
	DirectX::XMMATRIX trans;
};
struct FVec3 {
	float x, y, z;
};
struct Translation {
	FVec3 Position, Rotation, Scaling;
};

//Shape Stuff
class Component {
public:
	Component();
	virtual ~Component();

	Entity* entity = nullptr;
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

class Shape : public Component{
public:
	Shape() = default;
	virtual ~Shape() = default;
	virtual void Draw(Graphics& gfx) = 0;
	virtual void UpdateTranslation(Graphics& gfx);
protected:
	virtual void CreateBuffers(Graphics& gfx) = 0;
	virtual void CreateShadersAndInputLayout(Graphics& gfx) = 0;
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
	Cuboid(Graphics& gfx, Entity* ent, FVec3 sizeXYZ, bool isWrapped = true, UsingTexture ut = UsingTexture::MissingTex);
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
	Square(Graphics& gfx, Entity* ent, FVec3 sizeXY, UsingTexture ut = UsingTexture::MissingTex);
	~Square() = default;

	virtual void Draw(Graphics& gfx) override;
private:
	virtual void CreateBuffers(Graphics& gfx) override;
	virtual void CreateShadersAndInputLayout(Graphics& gfx) override;
private:
	Texture tex;
};

//Collider Stuff