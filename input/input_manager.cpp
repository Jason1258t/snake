#include "input_manager.hpp"
#include <ncurses.h>

namespace InputManager
{
    InputEvent InputManager::mapKeyToEvent(int keyCode) {
        switch (keyCode) {
            case 'w': case 'W': 
            case KEY_UP:        return InputEvent::DIRECTION_UP;
            
            case 's': case 'S': 
            case KEY_DOWN:      return InputEvent::DIRECTION_DOWN;
            
            case 'a': case 'A': 
            case KEY_LEFT:      return InputEvent::DIRECTION_LEFT;
            
            case 'd': case 'D': 
            case KEY_RIGHT:     return InputEvent::DIRECTION_RIGHT;
            
            case 'p': case 'P': 
            case 27:            return InputEvent::PAUSE_TOGGLE; 
            case ' ':           return InputEvent::PAUSE_TOGGLE; 
        }
        
        return InputEvent::NONE;
    }

    void InputManager::onDirection(DirectionCallback callback) {
        directionCallback = callback;
    }
    
    void InputManager::onPause(ActionCallback callback) {
        callbacks[InputEvent::PAUSE_TOGGLE] = callback;
    }

    void InputManager::processEvent(InputEvent &event)
    {
        if (event >= InputEvent::DIRECTION_UP && event <= InputEvent::DIRECTION_RIGHT) {
            if (directionCallback) {
                Vector2D direction;
                switch (event) {
                    case InputEvent::DIRECTION_UP:    direction = Direction::UP; break;
                    case InputEvent::DIRECTION_DOWN:  direction = Direction::DOWN; break;
                    case InputEvent::DIRECTION_LEFT:  direction = Direction::LEFT; break;
                    case InputEvent::DIRECTION_RIGHT: direction = Direction::RIGHT; break;
                    default: return;
                }
                directionCallback(direction);
            }
        }
        else if (event != InputEvent::NONE && callbacks.count(event)) {
            callbacks[event]();
        }
    }

    bool InputManager::handleInput() {
        int keyCode = getch();
        if (keyCode == ERR) {
            return true;
        }

        InputEvent event = mapKeyToEvent(keyCode);
        processEvent(event);
        
        return true;
    }
    
    void InputManager::initialize() {
        initscr();              // Инициализация ncurses
        cbreak();               // Отключаем буферизацию строк
        noecho();               // Не отображаем вводимые символы
        keypad(stdscr, TRUE);   // Включаем специальные клавиши (стрелки и т.д.)
        curs_set(0);            // Скрываем курсор
        timeout(0);             // Неблокирующий ввод по умолчанию
    }

    void InputManager::cleanup() {
        endwin();
    }
}