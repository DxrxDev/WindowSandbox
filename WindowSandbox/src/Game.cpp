#include<sstream>
#include"Game.h"

Game::Game() : window(512, 512, "Default Window"){}

Game::Game(int width, int height, const char* name) : window(width, height, name){}

int Game::Go() {
	window.Gfx().CreateBindViewPort(0, 0, window.width, window.height);

	for (int i = 0; i < 1; ++i) {
		shapes.push_back(
			new Cuboid(window.Gfx(), size)
		);
	}
	while (true){
		if (const auto ecode = Window::ProcessMessages()){
			return *ecode;
		}
		DoFrame();
	}
}

void Game::DoFrame() {
	timer.Mark();
	window.Gfx().ClearBuffer(0.0, 0.0, 0.0);

	float pos[] = { 0, 0, 5 };
	float rot[] = {0, timer2.Peek(), 0};
	for (int i = 0; i < 1; ++i) {
		shapes[i]->UpdatePosition(pos);
		shapes[i]->UpdateRotation(rot);
		shapes[i]->Draw(window.Gfx());
		pos[1] += 0.05f;
	}

	window.Gfx().EndFrame();
	title << "FPS: " << 1 / timer.Peek();
	window.ChangeTitle(title.str().c_str());
	title = std::stringstream();
}