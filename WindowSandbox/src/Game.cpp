#include<sstream>
#include"Game.h"

Game::Game() : window(512, 512, "Default Window") {}

Game::Game(int width, int height, const char* name) : window(width, height, name) {

}

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
	const float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time Passed: " << t;
	window.ChangeTitle(oss.str().c_str());
}