#pragma once
#include "../utils/Vector2D.hpp"
#include <functional>
#include <unordered_map>

namespace InputManager
{
	enum class InputEvent
	{
		NONE,
		DIRECTION_UP,
		DIRECTION_DOWN,
		DIRECTION_LEFT,
		DIRECTION_RIGHT,
		PAUSE_TOGGLE
	};

	class InputManager
	{
	public:
		InputManager() { Initialize(); };

		~InputManager() { Cleanup(); }

		using DirectionCallback = std::function<void(Vector2D)>;
		using ActionCallback = std::function<void()>;

		InputEvent MapKeyToEvent(int keyCode);
		bool HandleInput();

		void SetOnDirection(DirectionCallback callback);
		void SetOnPause(ActionCallback callback);

	private:
		static void Initialize();
		static void Cleanup();
		void ProcessEvent(InputEvent& event);
		DirectionCallback directionCallback;
		std::unordered_map<InputEvent, ActionCallback> callbacks;
	};
} 