#include"Entity.h"

Entity::Entity(const char* tag) : tag(tag) {
	static unsigned int s_id = 0;
	id = s_id;
	++s_id;
}

Player::Player(Graphics& gfx, std::vector<Shape*>* sceneShapeVec) : Entity("Player"), sprite(Square(gfx, this, { 0.125f, 0.125f, 0.0f }, UsingTexture::Player)){
	trans.Position.z = 1.0f;
	trans.Scaling = { 0.0625f, 0.0625f, 0.0f };
	sceneShapeVec->push_back(&sprite);

	//Spawner* spawner = new Spawner(this);
	//childEntities.push_back(std::move(spawner));
}
void Player::Update(Window& wnd) {
	float newMouseDX = (wnd.mouse.GetX() / (wnd.width / 2.0f) - 1.0f) - (float)trans.Position.x;
	float newMouseDY = trans.Position.y - (wnd.mouse.GetY() / -(wnd.height / 2.0f) + 1.0f);

	if (newMouseDX != mouseDX || newMouseDY != mouseDY) {
		mouseDX = newMouseDX;
		mouseDY = newMouseDY;
		if (mouseDY < 0) {
			trans.Rotation.z = atan(mouseDX / (mouseDY));
		}
		else {
			trans.Rotation.z = atan(mouseDX / (mouseDY)) + 3.14159f;
		}
	}

	if (wnd.kbd.KeyIsPressed('A')) {
		if (trans.Position.x - trans.Scaling.x > -1) {
			trans.Position.x -= 0.01f;
		}
	}
	if (wnd.kbd.KeyIsPressed('D')) {
		if (trans.Position.x + trans.Scaling.x < 1) {
			trans.Position.x += 0.01f;
		}
	}
	if (wnd.kbd.KeyIsPressed('W')) {
		if (trans.Position.y + trans.Scaling.y < 1) {
			trans.Position.y += 0.01f;
		}
	}
	if (wnd.kbd.KeyIsPressed('S')) {
		if (trans.Position.y - trans.Scaling.y > -1) {
			trans.Position.y -= 0.01f;
		}
	}
}