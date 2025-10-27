#include "game.hpp"
#include "config.hpp"
#include "field.hpp"
#include "game_object.hpp"
#include "snake.hpp"

class GameEngine
{
private:
	GameField field;
	Snake snake;
	Apple* apple;
	GameState state;
	int score;

public:
	GameEngine(const Snake& initSnake) :
		field(GameConfig::FIELD_WIDTH, GameConfig::FIELD_HEIGHT),
		snake(initSnake),
		state(GameState::RUNNING),
		score(0)
	{
		spawnApple();
		updateField();
	}

	bool processInput(char input)
	{
		Vector2D newDirection = getDirectionFromInput(input);
		if (newDirection != Direction::NONE)
		{
			return snake.setDirection(newDirection);
		}
		return true;
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

		// Размещаем яблоко
		if (apple)
		{
			field.placeObject(apple.get());
		}

		// Размещаем змейку
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
};
