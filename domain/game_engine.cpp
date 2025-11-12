#include "game_engine.hpp"
#include <iostream>

GameEngine::GameEngine(Snake&& initSnake, GameField&& initField) :
	field(std::move(initField)),
	snake(std::move(initSnake)),
	state(GameState::RUNNING),
	score(0)
{
	SpawnApple();
	UpdateField();
}

bool GameEngine::Update()
{
	if (state != GameState::RUNNING)
	{
		return false;
	}

	snake.move();

	if (CheckWallCollision() || snake.checkSelfCollision())
	{
		state = GameState::GAME_OVER;
		return false;
	}

	if (CheckAppleCollision())
	{
		snake.grow();
		score += GameConfig::APPLE_SCORE_VALUE;
		SpawnApple();
	}

	UpdateField();
	return true;
}

void GameEngine::RegisterInputManager(InputManager::InputManager& inputManager)
{
	inputManager.onDirection([this](Vector2D direction) {
		this->HandleDirectionEvent(direction);
	});

	inputManager.onPause([this]() {
		this->HandlePause();
	});
}

bool GameEngine::SpawnApple()
{
	Vector2D applePos = field.getRandomEmptyPosition();
	apple = std::make_unique<Apple>(applePos);
	return true;
}

bool GameEngine::UpdateField()
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

bool GameEngine::CheckWallCollision() const
{
	Vector2D head = snake.getHeadPosition();
	return !field.isPositionValid(head);
}

bool GameEngine::CheckAppleCollision() const
{
	return snake.getHeadPosition() == apple->getPosition();
}

void GameEngine::HandleDirectionEvent(const Vector2D& direction)
{
	if (direction == Direction::NONE)
		return;

	snake.setDirection(direction);
}

void GameEngine::HandlePause()
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