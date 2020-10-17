#include"Mouse.h"

std::pair<int, int> Mouse::GetPos() const noexcept {
	return { x, y };
}

int Mouse::GetX() const noexcept {
	return x;
}

int Mouse::GetY() const noexcept {
	return y;
}

bool Mouse::LeftPressed() const noexcept {
	return leftPressed;
}

bool Mouse::RightPressed() const noexcept {
	return rightPressed;
}

bool Mouse::MiddlePressed() const noexcept {
	return middlePressed;
}

Mouse::Event Mouse::Read() noexcept {
	if (buffer.size() > 0u) {
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	return Mouse::Event();
}

void Mouse::Clear() noexcept {
	buffer = std::queue<Event>();
}

void Mouse::OnLeftPressed(int x, int y) noexcept {
	leftPressed = true;
	buffer.push(Event(Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept {
	leftPressed = false;
	buffer.push(Event(Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept { 
	rightPressed = true;
	buffer.push(Event(Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept { 
	rightPressed = false;
	buffer.push(Event(Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnMiddlePressed(int x, int y) noexcept { 
	middlePressed = true;
	buffer.push(Event(Event::Type::MPress, *this));
	TrimBuffer();
}

void Mouse::OnMiddleReleased(int x, int y) noexcept { 
	middlePressed = false;
	buffer.push(Event(Event::Type::MRelease, *this));
	TrimBuffer();
}

void Mouse::OnMouseMove(int x, int y) noexcept {
	this->x = x;
	this->y = y;
	buffer.push(Event(Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept   {
	buffer.push(Event(Event::Type::MUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept {
	buffer.push(Event(Event::Type::MDown, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer() noexcept {
	while (buffer.size() > bufferSize) {
		buffer.pop();
	}
}