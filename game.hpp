#pragma once

#include "config.hpp"
#include "field.hpp"
#include "game_object.hpp"
#include "snake.hpp"
#include "input_manager.hpp"

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
	InputManager input;

public:
	GameEngine(const Snake& initSnake) {};

	virtual void handleInput() = 0;

	virtual bool update() = 0;

	const GameField& getField() const
	{
		return field;
	}

	int getScore() const
	{
		return score;
	}

	GameState getState() const
	{
		return state;
	}
}