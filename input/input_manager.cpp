#include "input_manager.hpp"

namespace InputManager
{
    InputEvent InputManager::mapKeyToEvent(int keyCode) {
        switch (keyCode) {
            case 'w': case 'W': return InputEvent::DIRECTION_UP;
            case 's': case 'S': return InputEvent::DIRECTION_DOWN;
            case 'a': case 'A': return InputEvent::DIRECTION_LEFT;
            case 'd': case 'D': return InputEvent::DIRECTION_RIGHT;
            case 'p': case 'P': case 27: return InputEvent::PAUSE_TOGGLE; // ESC
        }
        
        return InputEvent::NONE;
    }

    void InputManager::onDirection(DirectionCallback callback) {
        directionCallback = callback;
    }
    
    void InputManager::onPause(ActionCallback callback) {
        callbacks[InputEvent::PAUSE_TOGGLE] = callback;
    }
    

    bool InputManager::handleInput() {
        if (!_kbhit())             
            return true;

        int keyCode = _getch();

        InputEvent event = mapKeyToEvent(keyCode);

        if (event >= InputEvent::DIRECTION_UP && event <= InputEvent::DIRECTION_RIGHT) {
            if (directionCallback) {
                Vector2D direction;
                switch (event) {
                    case InputEvent::DIRECTION_UP:    direction = Direction::UP; break;
                    case InputEvent::DIRECTION_DOWN:  direction = Direction::DOWN; break;
                    case InputEvent::DIRECTION_LEFT:  direction = Direction::LEFT; break;
                    case InputEvent::DIRECTION_RIGHT: direction = Direction::RIGHT; break;
                    default: return true;
                }
                directionCallback(direction);
            }
        }

        else if (event != InputEvent::NONE && callbacks.count(event)) {
            callbacks[event]();
        }
        
        return true;
    }
}