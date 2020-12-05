#pragma once
#include"Window.h"
#include"Shape.h"

class Prefab {
public:
	virtual void Update(Window& wnd) = 0;
	virtual void Draw(Graphics& gfx) = 0;
	Shape* shape;
};

class FireBall : public Prefab {
public:
	FireBall(Graphics& gfx, FVec3 posXYZ);
	~FireBall() = default;

	virtual void Update(Window& wnd) override;
	virtual void Draw(Graphics& gfx) override;
public:
private:
	FVec3 rotation = {0.0f, 0.0f, 0.0f};
	FVec3 position;
private:
};

class Player : public Prefab{
public:
	Player(Graphics& gfx);
	~Player() = default;

	virtual void Update(Window& wnd) override;
	virtual void Draw(Graphics& gfx) override;
public:
private:
	std::vector<FireBall> fireBalls;
	float shotCoolDown = 1.0f;
private:
};
