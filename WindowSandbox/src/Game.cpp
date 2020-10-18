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
		timer.Wait(16);

		
	}
}

void Game::DoFrame() {
	const float colour = sin(timer.Peek()) / 2.0f + 0.5f;
	window.Gfx().ClearBuffer(colour, 1.0f, 1.0f);
	window.Gfx().EndFrame();
}