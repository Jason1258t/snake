#pragma once

#include "config.hpp"
#include "field.hpp"
#include "game_object.hpp"
#include "../input/input_manager.hpp"
#include "snake.hpp"
#include <memory>

enum class GameState
{
    RUNNING,
    GAME_OVER,
    VICTORY,
    PAUSE
};

class GameEngine
{
private:
    GameField field;
    Snake snake;
    std::unique_ptr<Apple> apple;
    GameState state;
    int score;

    bool spawnApple();
    bool updateField();
    bool checkWallCollision() const;
    bool checkAppleCollision() const;
    void handleDirectionEvent(const Vector2D& direction);
    void handlePause();

public:
    GameEngine(Snake initSnake, GameField initField);
    bool update();
    void registerInputManager(InputManager::InputManager& inputManager);

    const GameField& getField() const { return field; }
    int getScore() const { return score; }
    GameState getState() const { return state; }
};