#pragma once
#include<queue>
#include"Window.h"
#include"tools/Timer.h"
#include"Entity.h"

enum EventType {
	EventUpdatePhysics
};

class Game {
public:
	Game();
	Game(int width, int height, const char* name);
	int Go();
public:
private:
	void DoFrame();
	void UpdatePhysics();
private:
	std::vector<Entity*> entities;

	std::vector<Shape*> shapes;

	Window window;
	Timer timer;
	std::stringstream title;
};