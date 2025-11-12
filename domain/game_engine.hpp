#pragma once

#include "../input/input_manager.hpp"
#include "../objects/field.hpp"
#include "../objects/game_object.hpp"
#include "../objects/snake.hpp"
#include "config.hpp"
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

	bool SpawnApple();
	bool UpdateField();
	bool CheckWallCollision() const;
	bool CheckAppleCollision() const;
	void HandleDirectionEvent(const Vector2D& direction);
	void HandlePause();

public:
	GameEngine(Snake&& initSnake, GameField&& initField);
	bool Update();
	void RegisterInputManager(InputManager::InputManager& inputManager);

	const GameField& GetField() const { return field; }
	int GetScore() const { return score; }
	GameState GetState() const { return state; }
};