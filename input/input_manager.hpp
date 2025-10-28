#pragma once
#include "../utils/vector_2d.hpp"
#include <functional>
#include <unordered_map>
#include <conio.h>

namespace InputManager
{
    enum class InputEvent {
        DIRECTION_UP,
        DIRECTION_DOWN, 
        DIRECTION_LEFT,
        DIRECTION_RIGHT,
        PAUSE_TOGGLE,
        QUIT_GAME,
        NONE
    };

    using DirectionCallback = std::function<void(Vector2D)>;
    using ActionCallback = std::function<void()>;

    class InputManager {
    private:
        std::unordered_map<InputEvent, ActionCallback> callbacks;
        DirectionCallback directionCallback;

        InputEvent mapKeyToEvent(int keyCode);

    public:
        InputManager() = default;

        void onDirection(DirectionCallback callback);
        void onPause(ActionCallback callback);

        bool handleInput();
    };
}