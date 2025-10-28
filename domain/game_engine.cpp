#include "game_engine.hpp"
#include <iostream>

GameEngine::GameEngine(Snake&& initSnake, GameField&& initField) :
    field(std::move(initField)),
    snake(std::move(initSnake)),
    state(GameState::RUNNING),
    score(0)
{
    spawnApple();
    updateField();
}

bool GameEngine::update()
{
    if (state != GameState::RUNNING)
    {
        return false;
    }

    snake.move();

    if (checkWallCollision() || snake.checkSelfCollision())
    {
        state = GameState::GAME_OVER;
        return false;
    }

    if (checkAppleCollision())
    {
        snake.grow();
        score += GameConfig::APPLE_SCORE_VALUE;
        spawnApple();
    }

    updateField();
    return true;
}

void GameEngine::registerInputManager(InputManager::InputManager& inputManager)
{
    inputManager.onDirection([this](Vector2D direction) {
        this->handleDirectionEvent(direction);
    });

    inputManager.onPause([this]() {
        this->handlePause();
    });
}

bool GameEngine::spawnApple()
{
    Vector2D applePos = field.getRandomEmptyPosition();
    apple = std::make_unique<Apple>(applePos);
    return true;
}

bool GameEngine::updateField()
{
    field.clearField();

    if (apple)
    {
        field.placeObject(std::make_unique<Apple>(apple->getPosition()));
    }

    const auto& segments = snake.getSegments();
    for (const auto& segment : segments)
    {
        field.placeObject(std::make_unique<SnakeSegment>(segment->getPosition()));
    }

    return true;
}

bool GameEngine::checkWallCollision() const
{
    Vector2D head = snake.getHeadPosition();
    return !field.isPositionValid(head);
}

bool GameEngine::checkAppleCollision() const
{
    return snake.getHeadPosition() == apple->getPosition();
}

void GameEngine::handleDirectionEvent(const Vector2D& direction)
{
    if (direction == Direction::NONE)
        return;

    snake.setDirection(direction);
}

void GameEngine::handlePause()
{
    if (state == GameState::RUNNING)
    {
        state = GameState::PAUSE;
    }
    else if (state == GameState::PAUSE)
    {
        state = GameState::RUNNING;
    }
}