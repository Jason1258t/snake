#include "game.hpp"

class GameEngine
{
private:
	GameField field;
	Snake snake;
	Apple* apple;
	GameState state;
	InputManager input;
	int score;

public:
	GameEngine(const Snake& initSnake) :
		field(GameConfig::FIELD_WIDTH, GameConfig::FIELD_HEIGHT),
		snake(initSnake),
		state(GameState::RUNNING),
		score(0)
	{
		input.onDirection(handleDirectionEvent);
		input.onPause(handlePause);
		spawnApple();
		updateField();
	}

	void handleInput() 
	{
		input.handleInput();
	}

	bool update()
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
private:
	bool spawnApple()
	{
		Vector2D applePos = randomGen.generatePosition(field, snake.getBodyPositions());
		apple = std::make_unique<Apple>(applePos);
		return true;
	}

	bool updateField()
	{
		field.clearField();

		if (apple)
		{
			field.placeObject(apple.get());
		}

		const auto& segments = snake.getSegments();
		for (const auto& segment : segments)
		{
			field.placeObject(segment.get());
		}

		return true;
	}

	bool checkWallCollision() const
	{
		Vector2D head = snake.getHeadPosition();
		return !field.isPositionValid(head);
	}

	bool checkAppleCollision() const
	{
		if (!apple)
			return false;
		return snake.getHeadPosition() == apple->getPosition();
	}



	void handleDirectionEvent(const Vector2D& direction)
	{
		if (direction == Direction::NONE) return;

		snake.setDirection(direction);
	}

	void handlePause() 
	{
		if (state == GameState::RUNNING)
		{
			state = GameState::PAUSE;
		} else if (state == GameState::PAUSE)
		{
			state = GameState::RUNNING;
		}
	}
};
