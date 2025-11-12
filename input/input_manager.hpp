#pragma once
#include "../utils/vector_2d.hpp"
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
		InputManager() { initialize(); };

		~InputManager() { cleanup(); }

		using DirectionCallback = std::function<void(Vector2D)>;
		using ActionCallback = std::function<void()>;

		InputEvent mapKeyToEvent(int keyCode);
		bool handleInput();

		void onDirection(DirectionCallback callback);
		void onPause(ActionCallback callback);

	private:
		static void initialize();
		static void cleanup();
		void processEvent(InputEvent& event);
		DirectionCallback directionCallback;
		std::unordered_map<InputEvent, ActionCallback> callbacks;
	};
} 