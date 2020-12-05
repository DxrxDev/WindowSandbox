#pragma once
#include<queue>
#include"Window.h"
#include"tools/Timer.h"
#include"Shape.h"
#include"Prefabs.h"

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
	std::vector<Prefab*> prefabs;

	Window window;
	Timer timer;
	Timer timer2;
	std::stringstream title;
};