#pragma once
#include"Window.h"
#include"tools/Timer.h"
#include"Shape.h"

class Game {
public:
	Game();
	Game(int width, int height, const char* name);
	int Go();
public:
private:
	void DoFrame();
private:
	float size[3] = { 1.0, 1.0, 1.0 };
	std::vector<Shape*> shapes;

	Window window;
	Timer timer;
	Timer timer2;
	std::stringstream title;
};