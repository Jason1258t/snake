#include "GameEngine.hpp"
#include <iostream>

Snake GameEngine::makeSnake()
{
	std::deque<std::unique_ptr<SnakeSegment>> segments;

	Vector2D startPos = Vector2D{ field.GetHeight() / 2, field.GetWidth() / 2 };

	segments.push_back(std::make_unique<SnakeSegment>(startPos));
	segments.push_back(std::make_unique<SnakeSegment>(startPos + Direction::LEFT));

	Snake snake = Snake(std::move(segments), Direction::RIGHT);
	return snake;
}

GameEngine::GameEngine(GameField&& initField) :
	field(std::move(initField)),
	snake(makeSnake()),
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

	snake = snake.Move();

	if (CheckWallCollision() || snake.CheckSelfCollision())
	{
		state = GameState::GAME_OVER;
		return false;
	}

	if (CheckAppleCollision())
	{
		snake = snake.Grow();
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
	Vector2D applePos = field.GetRandomEmptyPosition();
	apple = std::make_unique<Apple>(applePos);
	return true;
}

bool GameEngine::UpdateField()
{
	field.ClearField();

	if (apple)
	{
		field.PlaceObject(std::make_unique<Apple>(apple->GetPosition()));
	}

	const auto& segments = snake.GetSegments();
	for (const auto& segment : segments)
	{
		field.PlaceObject(std::make_unique<SnakeSegment>(segment->GetPosition()));
	}

	return true;
}

bool GameEngine::CheckWallCollision() const
{
	Vector2D head = snake.GetHeadPosition();
	return !field.IsPositionValid(head);
}

bool GameEngine::CheckAppleCollision() const
{
	return snake.GetHeadPosition() == apple->GetPosition();
}

void GameEngine::HandleDirectionEvent(const Vector2D& direction)
{
	if (direction == Direction::NONE)
		return;

	snake = snake.WithDirection(direction);
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