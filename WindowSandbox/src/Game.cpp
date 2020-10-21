#include<sstream>
#include"Game.h"

Game::Game() : window(512, 512, "Default Window") {}

Game::Game(int width, int height, const char* name) : window(width, height, name) {}

int Game::Go() {
	while (true){
		if (const auto ecode = Window::ProcessMessages()){
			return *ecode;
		}
		DoFrame();
		timer.Wait(50);
	}
}

void Game::DoFrame() {
	window.Gfx().ClearBuffer(0.5f, 0.5f, 0.5f);

	window.Gfx().DrawTriangle(
		timer.Peek(),
		(float)window.mouse.GetX() / 256 - 1,
		(float)-window.mouse.GetY() / 256 + 1
	);

	window.Gfx().DrawTriangle(
		-timer.Peek(),
		0.0, 0.0
	);

	window.Gfx().EndFrame();
}