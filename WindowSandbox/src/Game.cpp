#include<sstream>
#include<memory>
#include<deque>
#include<thread>
#include"Game.h"

Game::Game() : window(1024, 1024, "Default Window"){}

Game::Game(int width, int height, const char* name) : window(width, height, name){}

int Game::Go() {
	window.Gfx().CreateBindViewPort(0, 0, window.width, window.height);


	for (int i = 0; i < 1; ++i) {
		entities.push_back(new Player(window.Gfx(), &shapes));
	}

	while (true){
		timer.Mark();
		if (const auto ecode = Window::ProcessMessages()){
			return *ecode;
		}

		std::thread updatePhy(&Game::UpdatePhysics, this);
		DoFrame();
		updatePhy.join();
		title << "FPS: " << 1 / timer.Peek();
		window.ChangeTitle(title.str().c_str());
		title = std::stringstream();
	}
}

void Game::DoFrame() {
	window.Gfx().ClearBuffer(0.0, 0.0, 0.0);

	for (int i = 0; i < shapes.size(); ++i) {
		shapes[i]->Draw(window.Gfx());
	}

	window.Gfx().EndFrame();
}

void Game::UpdatePhysics() {
	for (int i = 0; i < shapes.size(); ++i) {
		entities[i]->Update(window);
	}
}