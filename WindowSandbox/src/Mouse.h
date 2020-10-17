#pragma once
#include<queue>

class Mouse {
	friend class Window;
public:
	class Event {
	public:
		enum class Type {
			LPress, LRelease,
			RPress, RRelease,
			MPress, MRelease,
			MUp   , MDown,
			Move,
			Invalid
		};
	private:
		Type type;
		bool leftPressed, rightPressed, middlePressed;
		int x, y;
	public:
		Event() noexcept : type(Type::Invalid), leftPressed(false), rightPressed(false), middlePressed(false), x(0), y(0) {}

		Event(Type type, const Mouse& parent) noexcept : type(type), leftPressed(parent.leftPressed), rightPressed(parent.rightPressed), middlePressed(parent.middlePressed), x(parent.x), y(parent.y) {}

		bool IsValid() const noexcept{
			return type != Type::Invalid;
		}
		Type GetType() const noexcept{
			return type;
		}
		std::pair<int, int> GetPos() const noexcept {
			return {x, y};
		}
		int GetX() const noexcept {
			return x;
		}
		int GetY() const noexcept {
			return y;
		}
		bool LeftPressed() const noexcept {
			return leftPressed;
		}
		bool RightPressed() const noexcept {
			return rightPressed;
		}
		bool MiddlePressed() const noexcept {
			return middlePressed;
		}
	};
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	std::pair<int, int> GetPos() const noexcept;
	int GetX() const noexcept;
	int GetY() const noexcept;
	bool LeftPressed() const noexcept;
	bool RightPressed() const noexcept;
	bool MiddlePressed() const noexcept;

	Mouse::Event Read() noexcept;
	bool IsEmpty() const noexcept {return buffer.empty();}
	void Clear() noexcept;

private:
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnMiddlePressed(int x, int y) noexcept;
	void OnMiddleReleased(int x, int y) noexcept;

	void OnMouseMove(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;

	void TrimBuffer() noexcept;

private:
	bool leftPressed = false, rightPressed = false, middlePressed = false;
	int x, y;
	static constexpr unsigned int bufferSize = 16u;
	std::queue<Event> buffer;
};