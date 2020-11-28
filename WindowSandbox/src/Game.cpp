#include<sstream>
#include"Game.h"

Game::Game() : window(512, 512, "Default Window"), tri(window.Gfx(), L"VertexShader.cso", L"PixelShader.cso") {}

Game::Game(int width, int height, const char* name) : window(width, height, name), tri(window.Gfx(), L"VertexShader.cso", L"PixelShader.cso"){}

int Game::Go() {
	while (true){
		if (const auto ecode = Window::ProcessMessages()){
			return *ecode;
		}
		DoFrame();
	}
}

void Game::DoFrame() {
	timer.Mark();
	window.Gfx().ClearBuffer(0.0f, (float)window.mouse.GetX()/512, (float)(window.mouse.GetY() / 512.0f) + 1.0f - (2.0f * window.mouse.GetY() / 512.0f));
	//window.Gfx().DrawTriangle();

	if (window.mouse.GetX() > window.width/2) {
		tri.Draw(window.Gfx());
	}

	window.Gfx().EndFrame();
	std::stringstream ss;
	ss << timer.Peek();
	window.ChangeTitle(ss.str().c_str());
}