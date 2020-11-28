#pragma once
#include"Window.h"
#include"tools/Timer.h"
#include"Triangle.h"

class Game {
public:
	Game();
	Game(int width, int height, const char* name);
	int Go();
private:
	void DoFrame();
private:
	Window window;
	Timer timer;
	Triangle tri;
};