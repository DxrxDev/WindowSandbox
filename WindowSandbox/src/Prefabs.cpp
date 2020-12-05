#include"Prefabs.h"

Player::Player(Graphics& gfx) {//: shape(new Square(gfx, {0.0f, 0.0f, 5.0f}, UsingTexture::Player)) {
	shape = new Square(gfx, {1.0f, 1.0f, 0.0f}, UsingTexture::Player);
}
void Player::Update(Window& wnd) {
	//Player Position
	float posZ = 5;
	float posX = (wnd.mouse.GetX() / (wnd.width / (posZ * 2.0f)) - posZ);
	float posY;
	if (posY = (-wnd.mouse.GetY() / (wnd.height / (posZ * 2.0f)) + posZ); posY > -0.5) {
		posY = -0.5;
	}

	//Fireball Spawn Position
	fireBallSpawn.x = posX;
	fireBallSpawn.y = posY + 1;
	fireBallSpawn.z = posZ;
	//Shooting
	if (wnd.mouse.LeftPressed() && shotCoolDown <= 0.0f) {
		fireBalls.push_back(FireBall(wnd.Gfx(), fireBallSpawn));
		shotCoolDown = 1.0f;
	}
	shotCoolDown -= 0.032;
	shape->UpdatePosition({ posX, posY, posZ });
	shape->UpdateRotation({ 0, sin(wnd.mouse.GetX() / (wnd.width / 2.0f) - 1) * (3.14159f / 2.0f), 0 });
	for (int i = 0; i < fireBalls.size(); ++i) {
		fireBalls[i].Update(wnd);
	}
}
void Player::Draw(Graphics& gfx) {
	shape->Draw(gfx);
	for (int i = 0; i < fireBalls.size(); ++i) {
		fireBalls[i].Draw(gfx);
	}
}

FireBall::FireBall(Graphics& gfx, FVec3 posXYZ) : position(posXYZ) {
	shape = new Cuboid(gfx, {0.125f, 0.125f, 0.125f}, false, UsingTexture::FireBall);
}
void FireBall::Update(Window& wnd) {
	/*
	if (wnd.kbd.KeyIsPressed('A'))
		rotY += 0.1;
	else if (wnd.kbd.KeyIsPressed('D'))
		rotY -= 0.1;
	else if (wnd.kbd.KeyIsPressed('W'))
		rotX += 0.1;
	else if (wnd.kbd.KeyIsPressed('S'))
		rotX -= 0.1;
	shape->UpdateRotation({ rotX, rotY, 0.0 });
	*/
	position.y += 0.25;
	rotation.x += 0.05;
	rotation.y += 0.5;
	shape->UpdatePosition(position);
	shape->UpdateRotation(rotation);
}
void FireBall::Draw(Graphics& gfx) {
	shape->Draw(gfx);
}