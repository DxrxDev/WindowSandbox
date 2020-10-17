#include"Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char kc) const noexcept {
	return keyStates[kc];
}

Keyboard::Event Keyboard::ReadKey() noexcept {
	if (keyBuffer.size() > 0u) {
		Keyboard::Event e = keyBuffer.front();
		keyBuffer.pop();
		return e;
	}
	return Keyboard::Event();
}

bool Keyboard::KeyIsEmpty() const noexcept {
	return keyBuffer.empty();
}

void Keyboard::ClearKey() noexcept {
	keyBuffer = std::queue<Event>();
}

char Keyboard::ReadChar() noexcept {
	if (charBuffer.size() > 0u) {
		unsigned char charcode = charBuffer.front();
		charBuffer.pop();
		return charcode;
	}
	return 0;
}

bool Keyboard::CharIsEmpty() const noexcept {
	return charBuffer.empty();
}

void Keyboard::ClearChar() noexcept {
	charBuffer = std::queue<char>();
}

void Keyboard::Clear() noexcept {
	ClearKey();
	ClearChar();
}

void Keyboard::EnableAutorepeat() noexcept {
	autoRepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() noexcept {
	autoRepeatEnabled = false;
}

bool Keyboard::AutorepeatIsEnabled() const noexcept {
	return autoRepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char kc) noexcept {
	keyStates[kc] = true;
	keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, kc));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char kc) noexcept {
	keyStates[kc] = false;
	keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, kc));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnChar(char character) noexcept {
	charBuffer.push(character);
	TrimBuffer(charBuffer);
}

void Keyboard::ClearState() noexcept {
	keyStates.reset();
}

template<typename T>
static void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept {
	while (buffer.size() > BufferSize) {
		buffer.pop();
	}
}