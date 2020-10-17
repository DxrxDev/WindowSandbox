#pragma once
#include<queue>
#include<bitset>

class Keyboard{
	friend class Window;
public:
	class Event {
	public:
		enum class Type {
			Press, Release, Invalid
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event() : type(Type::Invalid), code(0u){}
		Event(Type type, unsigned char code) noexcept : type(type), code(code){}
		bool IsPressed() const noexcept {
			return type == Type::Press;
		}
		bool IsReleased() const noexcept {
			return type == Type::Release;
		}
		bool IsValid() const noexcept{
			return type != Type::Invalid;
		}
		unsigned char GetCode() const noexcept {
			return code;
		}
	};
	
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	//key stuff
public:
	bool KeyIsPressed(unsigned char kc) const noexcept;
	Event ReadKey() noexcept;
	bool KeyIsEmpty() const noexcept;
	void ClearKey() noexcept;

	//char stuff
public:
	char ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void ClearChar() noexcept;
	void Clear() noexcept;

	//autorepeat stuff
public:
	void EnableAutorepeat() noexcept;
	void DisableAutorepeat() noexcept;
	bool AutorepeatIsEnabled() const noexcept;

private:
	void OnKeyPressed(unsigned char kc) noexcept;
	void OnKeyReleased(unsigned char kc) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int BufferSize = 16u;
	bool autoRepeatEnabled = false;
	std::bitset<nKeys> keyStates;
	std::queue<Event> keyBuffer;
	std::queue<char> charBuffer;
};