#include "GameEngine.hpp"
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

	snake = snake.move();

	if (CheckWallCollision() || snake.checkSelfCollision())
	{
		state = GameState::GAME_OVER;
		return false;
	}

	if (CheckAppleCollision())
	{
		snake = snake.grow();
		score += GameConfig::APPLE_SCORE_VALUE;
		SpawnApple();
	}

	UpdateField();
	return true;
}

void GameEngine::RegisterInputManager(InputManager::InputManager& inputManager)
{
	inputManager.SetOnDirection([this](Vector2D direction) {
		this->HandleDirectionEvent(direction);
	});

	inputManager.SetOnPause([this]() {
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
		field.placeObject(std::make_unique<Apple>(apple->GetPosition()));
	}

	const auto& segments = snake.getSegments();
	for (const auto& segment : segments)
	{
		field.placeObject(std::make_unique<SnakeSegment>(segment->GetPosition()));
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
	return snake.getHeadPosition() == apple->GetPosition();
}

void GameEngine::HandleDirectionEvent(const Vector2D& direction)
{
	if (direction == Direction::NONE)
		return;

	snake = snake.withDirection(direction);
}

void GameEngine::HandlePause()
{
	switch (state)
	{
	case GameState::RUNNING:
		state = GameState::PAUSE;
		break;
	case GameState::PAUSE:
		state = GameState::RUNNING;
		break;
	default:
		break;
	}
}