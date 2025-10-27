#include "Snake.hpp"
#include "game_object.hpp"
#include "vector_2d.hpp"

bool Snake::setDirection(const Vector2D& newDirection)
{
	if (Direction::isOppositeDirection(currentDirection, newDirection))
		return false;
	if (!Direction::isDirectionValid(newDirection))
		return false;
	currentDirection = newDirection;
	return true;
}

bool Snake::move()
{
	Vector2D newHeadPos = segments.front()->getPosition() + currentDirection;
	segments.push_front(new SnakeSegment(newHeadPos));
	segments.pop_back();
	return true;
}

bool Snake::grow()
{
	Vector2D newHeadPos = segments.front()->getPosition() + currentDirection;
	segments.push_front(new SnakeSegment(newHeadPos));
	return true;
}

bool Snake::checkSelfCollision() const
{
	const Vector2D& head = segments.front()->getPosition();
	for (size_t i = 1; i < segments.size(); ++i)
	{
		if (head == segments[i]->getPosition())
			return true;
	}
	return false;
}