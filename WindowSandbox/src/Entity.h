#pragma once
#include"Window.h"
#include"Component.h"

class Entity {
public:
	Entity(const char* tag);
	virtual ~Entity() = default;
	virtual void Update(Window&) = 0;
public:
	unsigned short id;
	const char* tag;
	Translation trans = {
		{0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0},
		{1.0, 1.0, 1.0}
	};
	Entity* parentEntity = nullptr;
	std::vector<Entity*> childEntities;
};

class Player : public Entity {
public:
	Player(Graphics& gfx, std::vector<Shape*>* test);
	virtual void Update(Window& wnd) override;
private:
	Square sprite;
	float mouseDX = 0, mouseDY = 0;
	//Spawner barrel;
};

template<class T>
class Spawner : Entity {
public:
	Spawner(Entity* parent);
	virtual void Update(Window& wnd) override;
public:

};